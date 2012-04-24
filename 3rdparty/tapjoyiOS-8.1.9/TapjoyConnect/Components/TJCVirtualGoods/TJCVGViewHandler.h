// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import "TJCVGConstants.h"
#import "TJCCallsWrapper.h"
#import "TapjoyConnect.h"


@class TJCVGView;
@class TJCFileDownloadHandler;

@interface TJCVGViewHandler : NSObject 
{
	TJCFileDownloadHandler *fileDownloadHandler_;
	int scrollToRow_;
}

@property (nonatomic,readonly) TJCFileDownloadHandler *fileDownloadHandler;
@property (nonatomic) int scrollToRow;


+ (TJCVGViewHandler*)sharedTJCVGViewHandler;

- (UIView*)showVirtualGoods;

- (UIView*)showVirtualGoodsWithFrame:(CGRect)frame;

- (void)showVirtualGoodsWithViewController:(UIViewController*)viewController;

- (TJCVGView*)getVirtualGoodsView;

- (UIView*)getVirtualGoodsWithTab:(int)tabIndex;

- (UIView*)getVirtualGoodsWithTab:(int)tabIndex withScrollToRow:(int)row;

- (NSMutableArray*)getAvailableVGStoreItems;

- (NSMutableArray*)getPurchasedVGStoreItems;

/*!	\fn getAllVGStoreItems
 *	\brief Returns The StoreItem objects array. This is the same as getPurchasedVGStoreItemsArray but is guaranteed to contain all virtual goods if game state is loaded first.
 *
 *	\param n/a
 *	\return The array of purchased StoreItem objects.
 */
- (NSMutableArray*)getAllLocalVGStoreItems;

- (void)initiateRequestForAllStoreItems;

- (void)initiateRequestForPurchasedVGStoreItems;

- (void)getPurchasedVGStoreItemsWithPrompt;

// This is used only in conjunction with game state and effectively downloads all virtual goods to the client.
- (void)updateAllLocalVGStoreItems;

- (void)deleteOldDB;

- (void)initializeDBPathVariables;

+ (NSString*)getGlobalDBSavePath;

- (void)updateViewWithOrientation:(UIInterfaceOrientation)fromInterfaceOrientation;

@end





@interface TapjoyConnect (TJCVGViewHandler)

/*!	\fn showVirtualGoodsView
 *	\brief Shows the Virtual Goods Main View.
 *	\deprecated Updated for version 7.0.0. Use showVirtualGoods.
 *
 *	\param n/a
 *	\return The Virtual Goods Main View.
 */
+ (UIView*)showVirtualGoodsView;

/*!	\fn showVirtualGoods
 *	\brief Shows the Virtual Goods Main View.
 *
 *	\param n/a
 *	\return The Virtual Goods UIView.
 */
+ (UIView*)showVirtualGoods;

/*!	\fn showVirtualGoodsWithViewController:(UIViewController*)viewController
 *	\brief Shows the virtual goods view within the given view controller.
 *
 *	\param viewController The view controller to at the virtual goods view to.
 *	\return n/a
 */
+ (void)showVirtualGoodsWithViewController:(UIViewController*)viewController;

/*!	\fn getVirtualGoodsView
 *	\brief Returns the Virtual Goods view and initiates the UI animation
 *
 *	\param n/a
 *	\return The Virtual Goods Main View.
 */
+ (UIView*)getVirtualGoodsView;

/*!	\fn getPurchasedVGStoreItems
 *	\brief Returns The StoreItem objects array for downloaded purchased goods.
 *
 *	\param n/a
 *	\return The array of purchased StoreItem objects.
 */
+ (NSMutableArray*)getPurchasedVGStoreItems;

/*!	\fn getAllVGStoreItems
 *	\brief Returns The StoreItem objects array. This is the same as getPurchasedVGStoreItemsArray but is guaranteed to contain all virtual goods if game state is loaded first.
 *
 *	\param n/a
 *	\return The array of purchased StoreItem objects.
 */
+ (NSMutableArray*)getAllLocalVGStoreItems;

/*!	\fn getAvailableItems
 *	\brief Initiates a request to retrieve the purchased Virtual Goods.
 *
 *	\param n/a
 *	\return n/a
 */
+ (void)getAvailableItems;

/*!	\fn showDownloadVGItemsPrompt
 *	\brief Informs the TJC Library to show the alert view prompt to download purchased items.
 *
 *	It is recommended that this should be called after connect.
 *	\param n/a
 *	\return n/a
 */
+ (void)showDownloadVGItemsPrompt;

@end






@interface TJCCallsWrapper (TJCVGViewHandler)

/*!	\fn showVirtualGoods
 *	\brief Places the virtual goods UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *
 *	\param n/a
 *	\return The virtual goods UIView.
 */
- (UIView*)showVirtualGoods;

/*!	\fn showVirtualGoodsWithViewController:(UIViewController*)viewController
 *	\brief Shows the virtual goods view within the given view controller.
 *
 *	\param viewController The view controller to at the virtual goods view to.
 *	\return n/a
 */
- (void)showVirtualGoodsWithViewController:(UIViewController*)viewController;

/*!	\fn showDownloadVGItemsPrompt
 *	\brief Pops up a UIAlert if the user has virtual goods that have been purchased but not yet downloaded on the device.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)showDownloadVGItemsPrompt;

@end