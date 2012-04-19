// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import "TJCCallsWrapper.h"
#import "TapjoyConnect.h"


/*!	\interface TJCFeaturedAppViewHandler
 *	\brief The Tapjoy Connect Featured App View Handler class.
 *
 */
@interface TJCFeaturedAppViewHandler : UIView 
{
	
}

+ (TJCFeaturedAppViewHandler*)sharedTJCFeaturedAppViewHandler;

+ (UIView*)showFullScreenAdWithURL:(NSString*)adURL;

+ (UIView*)showFullScreenAdWithURL:(NSString*)adURL withFrame:(CGRect)frame;

+ (void)showFullScreenAdWithURL:(NSString*)adURL withViewController:(UIViewController*)vController;

@end


@interface TapjoyConnect (TJCFeaturedAppViewHandler)

/*!	\fn getFeaturedApp
 *	\brief Requests for Featured App are notified via the TJC_FEATURED_APP_RESPONSE_NOTIFICATION notification.
 *
 *	\param n/a
 *	\return n/a
 */
+ (void)getFeaturedApp;

/*!	\fn getFeaturedAppWithCurrencyID:(NSString*)currencyID
 *	\brief Requests for Featured App are notified via the TJC_FEATURED_APP_RESPONSE_NOTIFICATION notification.
 *
 *	\param currencyID The GUID for the currency earned when completing this featured ad.
 *	\return n/a
 */
+ (void)getFeaturedAppWithCurrencyID:(NSString*)currencyID;

/*!	\fn showFeaturedAppFullScreenAd
 *	\brief Wrapper method for displaying the Full Screen Featured App Ad.
 *
 * Places the Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *	\param n/a
 *	\return The #TJCUIWebPageAdView object.
 */
+ (UIView*)showFeaturedAppFullScreenAd;

/*!	\fn showFeaturedAppFullScreenAdWithFrame:(CGRect)frame
 *	\brief Wrapper method for displaying the Full Screen Featured App Ad.
 *
 * Places the Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 * This method is usually used to make life easier for landscape only GL-based apps.
 *	\param frame The frame of the featured app view.
 *	\return The #TJCUIWebPageAdView object.
 */
+ (UIView*)showFeaturedAppFullScreenAdWithFrame:(CGRect)frame;

/*!	\fn showFeaturedAppFullScreenAdWithViewController:(UIViewController*)vController
 *	\brief Wrapper method for displaying the Full Screen Featured App Ad.
 *
 * Places the Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *	\param vController Reference to the application's view Controller. (Note: It must not be nil or an invalid object).
 *	\return n/a
 */
+ (void)showFeaturedAppFullScreenAdWithViewController:(UIViewController*)vController;

/*!	\fn setFeaturedAppDisplayCount:(int) displayCount
 *	\brief Sets the display count for featured app.
 *
 *	\param displayCount The number of times that the featured app should be displayed.
 *	\return n/a
 */
+ (void)setFeaturedAppDisplayCount:(int) displayCount;

@end


@interface TJCCallsWrapper (TJCFeaturedAppViewHandler)

/*!	\fn showFeaturedAppFullScreenAdWithURL:(NSString*)adURL
 *	\brief Places the Featured App Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *
 *	\param adURL The URL to load in the Ad Web View.
 *	\return n/a
 */
- (UIView*)showFeaturedAppFullScreenAdWithURL:(NSString*)adURL;

/*!	\fn showFeaturedAppFullScreenAd:withFrame:(NSString* adURL, CGRect frame)
 *	\brief Places the Featured App Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *
 *	\param adURL The URL to load in the Ad Web View.
 *	\param frame The frame of the featured app view.
 *	\return n/a
 */
- (UIView*)showFeaturedAppFullScreenAd:(NSString*)adURL withFrame:(CGRect)frame;

/*!	\fn showFeaturedAppFullScreenAd:withViewController:(NSString* adURL, UIViewController* vController)
 *	\brief Wrapper method for displaying the Full Screen Featured App Ad.
 *
 * Places the Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *	\param adURL The URL to load in the Ad Web View.
 *	\param vController Reference to the application's view Controller. (Note: It must not be nil or an invalid object).
 *	\return n/a
 */
- (void)showFeaturedAppFullScreenAd:(NSString*)adURL withViewController:(UIViewController*)vController;

@end