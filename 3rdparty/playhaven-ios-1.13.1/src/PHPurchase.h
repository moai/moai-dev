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

 PHPurchase.h
 playhaven-sdk-ios

 Created by Thomas DiZoglio on 1/12/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>

/** @file */
/**
 * This is used to enumerate the different resolutions a particular purchase might have
 **/
typedef enum {
    PHPurchaseResolutionBuy,    /**< ('buy'): the user was offered a purchasable good and purchased it successfully */
    PHPurchaseResolutionCancel, /**< ('cancel'): the user was offered a good but did not choose to purchase it */
    PHPurchaseResolutionError,  /**< ('error'): an error occurred during the purchase process */
    PHPurchaseResolutionFailure /**< ('failure'): the SDK was not able to confirm price and currency information for the good */
} PHPurchaseResolutionType;

/**
 * @internal
 *
 * @brief Contains information about a IAP purchase that is usually triggered by a
 * ph://purchase dispatch in a content unit. Also provides a means for the
 * content unit to be informed of the resolution of this purchase request
 **/
@interface PHPurchase : NSObject {

    NSString *_productIdentifier;
    NSString *_item;
    NSInteger _quantity;
    NSString *_receipt;
    NSString *_callback;
}

/**
 * Converts the enumerated resolution type to a string value for use in URL
 * parameter strings
 *
 * @param resolution
 *   The purchase resolution
 *
 * @return
 *   A string representing the resolution type
 **/
+ (NSString *)stringForResolution:(PHPurchaseResolutionType)resolution;

@property (nonatomic, copy)   NSString *productIdentifier; /**< iTunes product identifier */
@property (nonatomic, copy)   NSString *name;              /**< Product display name */
@property (nonatomic, assign) NSInteger quantity;          /**< Quantity of the item to purchase */
@property (nonatomic, copy)   NSString *receipt;           /**< Unique receipt value to use for validation */
@property (nonatomic, copy)   NSString *callback;          /**< Content unit callback identifier @note We're not passing a reference to the
                                                                originating content unit, this depends on callback ids being globally unique */

/**
 * Reports the resolution of the purchase (see PHPurchaseResolutionType above)
 * to the content unit that originated the purchase.
 *
 * @param resolution
 *   The resolution to report
 **/
- (void)reportResolution:(PHPurchaseResolutionType)resolution;
@end
