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


static const float FULLSCREEN_AD_BACK_BUTTON_SIZE = 30.0f;
static const float FULLSCREEN_AD_BORDER_SIZE = 3.0f;


/*!	\interface TJCFeaturedAppView
 *	\brief The Tapjoy Connect Featured App View class.
 *
 */
@interface TJCFeaturedAppView : TJCUIWebPageView 
{
	NSString *currencyID_;				/*!< The last currency ID passed in. Used to show the proper currency if the offer wall is opened from the featured view. */
	NSString *publisherUserID_;		/*!< The publisher ID. */
}


@property (copy) NSString *currencyID_;
@property (copy) NSString *publisherUserID_;

+ (TJCFeaturedAppView*) sharedTJCFeaturedAppView;

/*!	\fn refreshWithFrame:(CGRect)frame
 *	\brief Refreshes the #TJCFeaturedAppView with the given GCRect frame.
 *
 *	\param frame The frame properties are used to define the location and boundaries to initialize the #TJCFeaturedAppView in.
 *	\return n/a
 */
- (void)refreshWithFrame:(CGRect)frame;

/*!	\fn loadViewWithURL:(NSString*)theURL
 *	\brief Loads the web view with the given URL.
 *
 *	\param theURL The URL to load the web view with.
 *	\return n/a
 */
- (void) loadViewWithURL:(NSString*)theURL;

/*!	\fn setUpFeaturedAdURLWithServiceURL:(NSString*)serviceURL
 *	\brief Sets up the featured ad URL with the given service portion of the URL.
 *
 * The generic parameters and the user id is appended to the given URL and the result is returned.
 *	\param serviceURL The service portion of the URL.
 *	\return The result URL.
 */
- (NSString*)setUpFeaturedAdURLWithServiceURL:(NSString*)serviceURL;

/*!	\fn initializeWebViewUI
 *	\brief Initializes the web page view.
 *
 *	\param n/a
 *	\return n/a
 */
- (void) initializeWebViewUI;

- (void) backtoGameAction:(id) sender;

- (void) updateViewWithOrientation:(UIInterfaceOrientation)fromInterfaceOrientation;

@end
