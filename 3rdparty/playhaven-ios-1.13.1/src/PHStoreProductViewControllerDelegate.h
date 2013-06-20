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

 PHStoreProductViewControllerDelegate.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 9/18/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#if PH_USE_STOREKIT != 0

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@class SKStoreProductViewController;
@protocol SKStoreProductViewControllerDelegate;

/**
 * @internal
 *
 * @brief Singleton class that manages an overlay view controller, inserts it into the
 * application's UIWindow subviews, and uses it to display an
 * SKStoreProductViewController for a given iTunes product id.
 **/
@interface PHStoreProductViewControllerDelegate : NSObject <SKStoreProductViewControllerDelegate> {
    UIViewController *_visibleViewController;
}

/**
 * Singleton accessor
 **/
+ (PHStoreProductViewControllerDelegate *)getDelegate;

/**
 * Present an SKStoreProductViewController for the iTunes product with id \c productId
 *
 * @param productId
 *   The product ID
 *
 * @return
 *   Some kind of BOOL
 **/
- (BOOL)showProductId:(NSString *)productId;
@end
#endif
