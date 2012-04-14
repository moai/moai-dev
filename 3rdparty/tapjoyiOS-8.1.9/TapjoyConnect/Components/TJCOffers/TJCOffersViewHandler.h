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

/*!	\interface TJCOffersViewHandler
 *	\brief The Tapjoy Connect Offers View Handler class.
 *
 */
@interface TJCOffersViewHandler : NSObject 

+ (UIView*)showOffersWithFrame:(CGRect)frame;

+ (UIView*)showOffers;

+ (void)showOffersWithViewController:(UIViewController*)vController;

+ (void)showOffersWithViewController:(UIViewController *)vController withInternalNavBar:(BOOL)visible;

+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withFrame:(CGRect)frame withCurrencySelector:(BOOL)isSelectorVisible;

+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible;

+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withCurrencySelector:(BOOL)isSelectorVisible;

+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withInternalNavBar:(BOOL)visible withCurrencySelector:(BOOL)isSelectorVisible;

@end



@interface TJCCallsWrapper (TJCOffersViewHandler)

- (UIView*)showOffersWithFrame:(CGRect)frame;

- (UIView*)showOffers;

- (void)showOffersWithViewController:(UIViewController*)vController;

- (void)showOffersWithViewController:(UIViewController *)vController withInternalNavBar:(BOOL)visible;

- (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withFrame:(CGRect)frame withCurrencySelector:(BOOL)isSelectorVisible;

- (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible;

- (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withCurrencySelector:(BOOL)isSelectorVisible;

- (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withInternalNavBar:(BOOL)visible withCurrencySelector:(BOOL)isSelectorVisible;

@end



@interface TapjoyConnect (TJCOffersViewHandler)

/*!	\fn showOffersWithFrame:(CGRect)frame
 *	\brief Allocates and initializes a #TJCOffersWebView.
 *
 *	\param frame The frame to load the offers view in.
 *	\return The #TJCOffersWebView UIView object.
 */
+ (UIView*)showOffersWithFrame:(CGRect)frame;

/*!	\fn showOffers
 *	\brief Allocates and initializes a #TJCOffersWebView.
 *
 *	\return The #TJCOffersWebView UIView object.
 */
+ (UIView*)showOffers;

/*!	\fn showOffersWithViewController:(UIViewController*)vController
 *	\brief Allocates and initializes a #TJCOffersWebView.
 *
 *	\param vController The UIViewController to set as #TJCOffersWebView's #parentVController_.
 *	\return n/a
 */
+ (void)showOffersWithViewController:(UIViewController*)vController;

/*!	\fn showOffersWithViewController:withInternalNavBar:(UIViewController* vController, BOOL visible)
 *	\brief Allocates and initializes a #TJCOffersWebView.
 *
 *	\param vController The UIViewController to set as #TJCOffersWebView's #parentVController_.
 *	\param visible TRUE makes the navigation bar visible, FALSE makes it not visible.
 *	\return n/a
 */
+ (void)showOffersWithViewController:(UIViewController *)vController withInternalNavBar:(BOOL)visible;

/*!	\fn showOffersWithCurrencyID:withFrame:withCurrencySelector:(NSString *currencyID, CGRect frame, BOOL isSelectorVisible)
 *	\brief Allocates and initializes a #TJCOffersWebView. This is only used when multiple currencies are enabled.
 *
 *	\param frame The frame to load the offers view in.
 *	\param currencyID The id of the currency to show in the offer wall.
 *	\param isSelectorVisible Specifies whether to display the currency selector in the offer wall.
 *	\return The #TJCOffersWebView UIView object.
 */
+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withFrame:(CGRect)frame withCurrencySelector:(BOOL)isSelectorVisible;

/*!	\fn showOffersWithCurrencyID:withCurrencySelector:(NSString *currencyID, BOOL isSelectorVisible)
 *	\brief Allocates and initializes a #TJCOffersWebView. This is only used when multiple currencies are enabled.
 *
 *	\param currencyID The id of the currency to show in the offer wall.
 *	\param isSelectorVisible Specifies whether to display the currency selector in the offer wall.
 *	\return The #TJCOffersWebView UIView object.
 */
+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible;

/*!	\fn showOffersWithCurrencyID:withViewController:withCurrencySelector:(NSString *currencyID, UIViewController *vController, BOOL isSelectorVisible)
 *	\brief Allocates and initializes a #TJCOffersWebView. This is only used when multiple currencies are enabled.
 *
 *	\param vController The UIViewController to set as #TJCOffersWebView's #parentVController_.
 *	\param currencyID The id of the currency to show in the offer wall.
 *	\param isSelectorVisible Specifies whether to display the currency selector in the offer wall.
 *	\return n/a
 */
+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withCurrencySelector:(BOOL)isSelectorVisible;

/*!	\fn showOffersWithCurrencyID:withViewController:withInternalNavBar:withCurrencySelector:(NSString *currencyID, UIViewController *vController, BOOL visible, BOOL isSelectorVisible)
 *	\brief Allocates and initializes a #TJCOffersWebView. This is only used when multiple currencies are enabled.
 *
 *	\param vController The UIViewController to set as #TJCOffersWebView's #parentVController_.
 *	\param visible TRUE makes the navigation bar visible, FALSE makes it not visible.
 *	\param currencyID The id of the currency to show in the offer wall.
 *	\param isSelectorVisible Specifies whether to display the currency selector in the offer wall.
 *	\return n/a
 */
+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withInternalNavBar:(BOOL)visible withCurrencySelector:(BOOL)isSelectorVisible;

/*!	\fn setOffersNavBarImage:(UIImage*)image
 *	\brief Sets the navigation bar background image.
 *  
 *	\param image The UIImage to set the offer wall's navigation bar's background image to.
 *  \return n/a
 */
+ (void)setOffersNavBarImage:(UIImage*)image;

@end