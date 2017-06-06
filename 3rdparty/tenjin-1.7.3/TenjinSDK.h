//
//  TenjinSDK.h
//  TenjinSDK
//
//  Copyright (c) 2016 Tenjin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface TenjinSDK : NSObject

//required to initialize the Tenjin SDK
+ (TenjinSDK *)sharedInstanceWithToken:(NSString *)apiToken;

//requierd to initialize the Tenjin SDK by caputuring a Facebook deeplink
+ (TenjinSDK *)sharedInstanceWithToken:(NSString *)apiToken andDeferredDeeplink:(NSURL *) url;

//returns the shared Tenjin SDK instance
+ (TenjinSDK *)sharedInstance;

//use sendEventWithName for custom event names
+ (void)sendEventWithName:(NSString *)eventName;

//This method checks to make sure integers are passed as values.
+ (void)sendEventWithName:(NSString *)eventName andEventValue:(NSString *)eventValue;

//This method is deprecated in favor of [transaction: andReceipt:], so Tenjin can verify your transactions
+ (void)transaction:(SKPaymentTransaction *)transaction __attribute__((deprecated));

//Use this method to submit a transaction to Tenjin, we will also attempt to verify it for our records
+ (void)transaction:(SKPaymentTransaction *)transaction andReceipt:(NSData *)receipt;

//use transactionWithProductName... when you don't use Apple's SKPaymentTransaction and need to pass revenue directly
+ (void)transactionWithProductName:(NSString *)productName andCurrencyCode:(NSString *)currencyCode andQuantity:(NSInteger)quantity andUnitPrice:(NSDecimalNumber *)price;

//use transactionWithProductName...when you don't use Apple's SKPaymentTransaction and need to pass revenue directly with a NSData binary receipt
+ (void)transactionWithProductName:(NSString *)productName andCurrencyCode:(NSString *)currencyCode andQuantity:(NSInteger)quantity andUnitPrice:(NSDecimalNumber *)price andTransactionId:(NSString *)transactionId andReceipt:(NSData *)receipt;

//use this method when you want to pass in a base64 receipt instead of a NSData receipt
+ (void)transactionWithProductName:(NSString *)productName andCurrencyCode:(NSString *)currencyCode andQuantity:(NSInteger) quantity andUnitPrice:(NSDecimalNumber *)price andTransactionId:(NSString *)transactionId andBase64Receipt:(NSString *)receipt;

//use this method to register the attribution callback
- (void)registerDeepLinkHandler:(void (^)(NSDictionary * params, NSError * error))deeplinkHandler;

@end
