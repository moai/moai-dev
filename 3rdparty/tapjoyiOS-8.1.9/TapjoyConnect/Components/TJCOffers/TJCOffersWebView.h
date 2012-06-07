// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import "TJCUIWebPageView.h"
#import "TapjoyConnect.h"


@class TJCUINavigationBarView;

/*!	\interface TJCOffersWebView
 *	\brief The Tapjoy Connect Offers Web View class.
 *
 */
@interface TJCOffersWebView : TJCUIWebPageView
{
	TJCUINavigationBarView *navBar_;		/*!< The navigation bar for the web UIView. */
	NSString *publisherUserID_;			/*!< The publisher ID. */
	UIViewController *parentVController_;	/*!< The parent UIViewController. */
	BOOL enableNavBar;						/*!< Indicates whether to make the navigation bar visible or not. */
	BOOL flagOrientationManaged;			/*!< Indicates whether the orientation is self managed. It will otherwise be set to whatever orientation the #parentVController_'s navigationController is set to. */
	NSString *currencyID_;					/*!< The name of the currency to load in the offer wall. */
	NSString *isSelectorVisible_;			/*!< Specifies whether the currency selector will be visible in the offer wall for multiple currencies. */
}


@property (nonatomic,retain) NSString *publisherUserID_;
@property (nonatomic,assign) UIViewController *parentVController_;//delegate so just a reference
@property (nonatomic, retain) NSString *currencyID_;
@property (nonatomic, retain) NSString *isSelectorVisible_;
@property (nonatomic, retain) TJCUINavigationBarView *navBar;


+ (TJCOffersWebView*) sharedTJCOffersWebView;

/*!	\fn initWithFrame:enableNavBar:(CGRect frame, BOOL enableNavigationBar)
 *	\brief Initializes a #TJCOffersWebView with the given GCRect frame.
 *
 *	\param frame The frame properties are used to define the location and boundaries to initialize the #TJCOffersWebView in.
 *	\param frame enableNavigationBar Specifies whether to enable the navigation bar.
 *	\return The #TJCOffersWebView UIView object.
 */
- (id) initWithFrame:(CGRect)frame enableNavBar:(BOOL)enableNavigationBar;

/*!	\fn refreshWithFrame:enableNavBar:(CGRect frame, BOOL enableNavigationBar)
 *	\brief Refreshes the #TJCOffersWebView with the given GCRect frame.
 *
 *	\param frame The frame properties are used to define the location and boundaries to initialize the #TJCOffersWebView in.
 *	\param frame enableNavigationBar Specifies whether to enable the navigation bar.
 *	\return n/a
 */
- (void)refreshWithFrame:(CGRect)frame enableNavBar:(BOOL)enableNavigationBar;

/*!	\fn setCustomNavBarImage:(UIImage*)image
 *	\brief Sets the navigation bar background image.
 *  
 *	\param image The UIImage to set the offer wall's navigation bar's background image to.
 *  \return n/a
 */
- (void)setCustomNavBarImage:(UIImage*)image;

/*!	\fn loadView
 *	\brief Initializes the properties of the #TJCOffersWebView with the current service URL.
 *
 *	\param n/a
 *	\return n/a
 */
- (void) loadView;

/*!	\fn setUpOffersURLWithServiceURL:(NSString*)serviceURL
 *	\brief Sets up the offers URL with the given service portion of the URL.
 *
 * The generic parameters and the user id is appended to the given URL and the result is returned.
 *	\param serviceURL The service portion of the URL.
 *	\return The result URL.
 */
- (NSString*) setUpOffersURLWithServiceURL:(NSString*)serviceURL;

/*!	\fn refreshWebView
 *	\brief Refreshes the offers web view.
 *
 *	\param n/a
 *	\return n/a
 */
- (void) refreshWebView;


@end