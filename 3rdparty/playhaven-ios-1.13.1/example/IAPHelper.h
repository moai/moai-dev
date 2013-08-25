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

 IAPHelper.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 3/2/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

@class PHPurchase;
@interface IAPHelper : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver> {
    NSMutableDictionary *_pendingPurchases;
    NSMutableDictionary *_pendingRequests;
}
@property(nonatomic, readonly) NSMutableDictionary *pendingPurchases;
@property(nonatomic, readonly) NSMutableDictionary *pendingRequests;

+ (IAPHelper *)sharedIAPHelper;

- (void)startPurchase:(PHPurchase *)purchase;
- (void)restorePurchases;
@end
