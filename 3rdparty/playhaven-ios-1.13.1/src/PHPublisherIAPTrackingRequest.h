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

 PHPublisherIAPTrackingRequest.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 1/13/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if PH_USE_STOREKIT != 0
#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "PHAPIRequest.h"
#import "PHPurchase.h"

/**
 * @internal
 *
 * @brief Request for reporting IAP transaction information to PlayHaven, used for
 * user segmentation and targeting by total in-app purchase spend.
 * See PHPurchase.h for more information about purchase resolution types
 **/
@interface PHPublisherIAPTrackingRequest : PHAPIRequest <SKProductsRequestDelegate> {
    NSString *_product;
    NSInteger _quantity;
    PHPurchaseResolutionType _resolution;
    SKProductsRequest       *_request;
    NSError *_error;
    NSData  *_receiptData;
}

/**
 * @name Conversion cookies
 **/
/*@{*/
/**
 * Conversion cookie getter/setter
 * Conversion cookies are set by the SDK when a content unit initiates a
 * purchase through the purchase dispatch. (i.e: VGP content units), they are
 * used to track a potential IAP purchase and uniquely tag them as being a
 * VGP-driven conversion
 **/
+ (void)setConversionCookie:(NSString *)cookie forProduct:(NSString *)product;
+ (NSString *)getConversionCookieForProduct:(NSString *)product;
/*@}*/

/**
 * Returns a request to report a user buying or canceling an IAP product with
 * id \c product, for successful requests, also send iTunes receipt data so
 * that the API can independently verify the transaction
 *
 * @param token
 *   The token
 *
 * @param secret
 *   The secret
 *
 * @param product
 *   The product
 *
 * @param quantity
 *   The quantity
 *
 * @param resolution
 *   The resolution
 *
 * @param receiptData
 *   The receiptData
 *
 * @return
 *   A request
 **/
+ (id)requestForApp:(NSString *)token secret:(NSString *)secret product:(NSString *)product quantity:(NSInteger)quantity
         resolution:(PHPurchaseResolutionType)resolution receiptData:(NSData *)receiptData;

/**
 * Returns a request to report an IAP transaction that encountered an error
 *
 * @param token
 *   The token
 *
 * @param secret
 *   The secret
 *
 * @param product
 *   The product
 *
 * @param quantity
 *   The quantity
 *
 * @param error
 *   The error
 *
 * @param receiptData
 *   The receiptData
 *
 * @return
 *   A request
 **/
+ (id)requestForApp:(NSString *)token secret:(NSString *)secret product:(NSString *)product quantity:(NSInteger)quantity
              error:(NSError *)error receiptData:(NSData *)receiptData;

/**
 * @deprecated.
 * Returns a request to report a user buying or canceling an IAP product.
 *
 * @param token
 *   The token
 *
 * @param secret
 *   The secret
 *
 * @param product
 *   The product
 *
 * @param quantity
 *   The quantity
 *
 * @param resolution
 *   The resolution
 *
 * @return
 *   A request
 **/
+ (id)requestForApp:(NSString *)token secret:(NSString *)secret product:(NSString *)product quantity:(NSInteger)quantity
         resolution:(PHPurchaseResolutionType)resolution DEPRECATED_ATTRIBUTE;

/**
 * @deprecated.
 * Returns a request to report an IAP transaction that encountered an error.
 *
 * @param token
 *   The token
 *
 * @param secret
 *   The secret
 *
 * @param product
 *   The product
 *
 * @param quantity
 *   The quantity
 *
 * @param error
 *   The error
 *
 * @return
 *   A request
 **/
+ (id)requestForApp:(NSString *)token secret:(NSString *)secret product:(NSString *)product quantity:(NSInteger)quantity
              error:(NSError *)error DEPRECATED_ATTRIBUTE;

@property (nonatomic, copy)   NSString *product;     /**< IAP bundle identifier string registered with Apple */
@property (nonatomic, assign) NSInteger quantity;    /**< The total quantity purchased for this IAP transaction */
@property (nonatomic, retain) NSError  *error;       /**< The error encountered by this request, if applicable */
@property (nonatomic, retain) NSData   *receiptData; /**< iTunes transaction receipt data for this transaction */
@property (nonatomic, assign) PHPurchaseResolutionType resolution; /**< The resolution of this transaction (buy, cancel, error) */
@end
#endif
