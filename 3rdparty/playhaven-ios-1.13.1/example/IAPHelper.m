/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 IAPHelper.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 3/2/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "IAPHelper.h"
#import "PlayHavenSDK.h"

@interface NSObject (hash)
- (NSString *)hashString;
@end

@implementation NSObject (hash)
- (NSString *)hashString
{
    return [NSString stringWithFormat:@"%d", [self hash]];
}
@end

@interface SKProduct (LocalizedPrice)
@property (nonatomic, readonly) NSString *localizedPrice;
@end

@implementation SKProduct (LocalizedPrice)
- (NSString *)localizedPrice
{
    NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    [numberFormatter setLocale:self.priceLocale];

    NSString *formattedString = [numberFormatter stringFromNumber:self.price];

    [numberFormatter release];

    return formattedString;
}
@end

@interface IAPHelper (Private)
- (void)reportPurchase:(PHPurchase *)purchase withResolution:(PHPurchaseResolutionType)resolution receiptData:(NSData *)receiptData;
- (void)reportPurchase:(PHPurchase *)purchase withError:(NSError *)error receiptData:(NSData *)receiptData;
@end

@implementation IAPHelper

+ (IAPHelper *)sharedIAPHelper
{
    static dispatch_once_t once;
    static id sharedInstance;
    dispatch_once(&once, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (void)dealloc
{
    // Should never be called, but just here for clarity really.
    [_pendingPurchases release], _pendingPurchases = nil;
    [_pendingRequests release], _pendingRequests = nil;
    [super dealloc];
}

#pragma mark -
- (NSMutableDictionary *)pendingPurchases
{
    if (_pendingPurchases == nil) {
        _pendingPurchases = [[NSMutableDictionary alloc] init];
    }
    return _pendingPurchases;
}

- (NSMutableDictionary *)pendingRequests
{
    if (_pendingRequests == nil) {
        _pendingRequests = [[NSMutableDictionary alloc] init];
    }
    return _pendingRequests;
}

- (void)startPurchase:(PHPurchase *)purchase
{
    // The first step is requesting product information for this purchase.
    if (!!purchase) {
        NSSet *productIdentifiers  = [NSSet setWithObject:purchase.productIdentifier];
        SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];

        request.delegate = self;
        [request start];

        // Storing the purchase and the product request to retrieve later
        [self.pendingPurchases setValue:purchase forKey:[request hashString]];
        [self.pendingRequests setValue:request forKey:[request hashString]];

        [request release];
    }
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSString   *key      = [request hashString];
    PHPurchase *purchase = [self.pendingPurchases valueForKey:key];
    NSArray    *products = response.products;
    SKProduct  *product  = [products count] == 1 ? [products objectAtIndex:0] : nil;

    if ([purchase.productIdentifier isEqualToString:product.productIdentifier]) {
        // Ask the user to choose to purchase or not purchase an item
        NSString    *message = [NSString stringWithFormat:@"Do you want to buy %d %@ for %@?",
                                              purchase.quantity, product.localizedTitle, product.localizedPrice];

        UIAlertView *purchaseAlert = [[UIAlertView alloc] initWithTitle:@"In-Game Store"
                                                                message:message
                                                               delegate:self
                                                      cancelButtonTitle:@"Cancel"
                                                      otherButtonTitles:@"Buy", nil];

        [purchaseAlert show];
        [self.pendingPurchases setObject:purchase forKey:[purchaseAlert hashString]];

        [purchaseAlert release];
    } else {
        // Either the purchase or the product request is invalid, report as an error
        [self reportPurchase:purchase withResolution:PHPurchaseResolutionError receiptData:nil];
    }

    // Either way clean up the stored purchase and request
    [self.pendingPurchases removeObjectForKey:key];
    [self.pendingRequests removeObjectForKey:key];
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    NSString   *key      = [alertView hashString];
    PHPurchase *purchase = (PHPurchase *)[self.pendingPurchases objectForKey:key];

    if (buttonIndex == 0) {
        //the user has canceled the request
        [self reportPurchase:purchase withResolution:PHPurchaseResolutionCancel receiptData:nil];
    } else if (buttonIndex == 1) {
        //start an app store request
        SKPayment *payment = [SKPayment paymentWithProductIdentifier:purchase.productIdentifier];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
        [self.pendingRequests setValue:purchase forKey:[payment hashString]];
    }

    // Either way, clean up the stored alert view
    [self.pendingPurchases removeObjectForKey:key];
}

#pragma mark-
#pragma VGP Support Implementation

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    // Adding IAP reporting and VGP to transactions requires some modifications to the
    // payment queue observer. Send IAP Purchase tracking requests whenever a transaction
    // is purchased (SKTransactionStatePurchased), and send IAP Error tracking requests
    // whenever a transaction fails (SKTransactionStateFailed)
    for (SKPaymentTransaction *transaction in transactions) {
        NSString   *key      = [transaction.payment hashString];
        PHPurchase *purchase = [self.pendingRequests valueForKey:key];

        if (purchase == nil) {
            // In the case that a transaction is being restored, we need to
            // generate a new purchase object so that IAP transactions may
            // be reported accurately.
            purchase = [PHPurchase new];

            purchase.productIdentifier = transaction.payment.productIdentifier;
            purchase.quantity          = transaction.payment.quantity;

            [purchase autorelease];
        }

        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                // This would normally be the point where an in app purchase would
                // be delivered. Instead we're just doing the necessary reporting
                // to support IAP tracking and VGP content units.

                NSLog(@"IAPHelper: Purchased %@!", transaction.payment.productIdentifier);
                [self reportPurchase:purchase
                      withResolution:PHPurchaseResolutionBuy
                         receiptData:transaction.transactionReceipt];

                [self.pendingPurchases removeObjectForKey:key];
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                break;

            case SKPaymentTransactionStateFailed:
                // Reporting failed transactions and finalizing them.
                NSLog(@"IAPHelper: Failed to purchase %@!", transaction.payment.productIdentifier);
                [self reportPurchase:purchase
                           withError:transaction.error
                         receiptData:transaction.transactionReceipt];

                [self.pendingPurchases removeObjectForKey:key];
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                break;

            default:
                break;
        }
    }
}

- (void)reportPurchase:(PHPurchase *)purchase withResolution:(PHPurchaseResolutionType)resolution receiptData:(NSData *)receiptData
{
    // PHPurchase objects are generated from VGP content units. It is important to preserve
    // these instances throughout the IAP process. This way, these purchase instances may be
    // used to report purchases to PlayHaven, as well as back to the originating content unit.
    if (!!purchase) {
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSString       *token    = [defaults valueForKey:@"ExampleToken"];
        NSString       *secret   = [defaults valueForKey:@"ExampleSecret"];

        // Reporting to the Tracking API
        PHPublisherIAPTrackingRequest *request =
                  [PHPublisherIAPTrackingRequest requestForApp:token
                                                        secret:secret
                                                       product:purchase.productIdentifier
                                                      quantity:purchase.quantity
                                                    resolution:resolution
                                                   receiptData:receiptData];
        [request send];

        // Reporting back to the content unit.
        [purchase reportResolution:resolution];
    }
}

- (void)reportPurchase:(PHPurchase *)purchase withError:(NSError *)error receiptData:(NSData *)receiptData
{
    // To get a more complete picture of your IAP implementation, report any errors, user
    // cancellations, or other incomplete transactions to PlayHaven. It is also important
    // to inform the originating content unit (for VGP-driven purchases) of the error.
    if (!!purchase) {
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSString       *token    = [defaults valueForKey:@"ExampleToken"];
        NSString       *secret   = [defaults valueForKey:@"ExampleSecret"];

        // Reporting to the Tracking API
        PHPublisherIAPTrackingRequest *request =
                  [PHPublisherIAPTrackingRequest requestForApp:token
                                                        secret:secret
                                                       product:purchase.productIdentifier
                                                      quantity:purchase.quantity
                                                         error:error
                                                   receiptData:receiptData];
        [request send];

        // Reporting back to the content unit
        [purchase reportResolution:request.resolution];
    }
}

- (void)restorePurchases
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}
@end
