// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


#define TJC_NAV_BTN_HEIGHT 30
#define TJC_NAV_BAR_HEIGHT 44
#define TJC_NAV_BTN_LEFT_RIGHT_MARGIN 5


@class TJCUtil;


/*!	\interface TJCUINavigationBarView
 *	\brief The Tapjoy Connect UI Navigation Bar class.
 */
@interface TJCUINavigationBarView : UINavigationBar
{
	UINavigationBar *navBar;			/*!< The navigation bar. */
	UINavigationItem *navBarTitle;		/*!< The navigation bar title. */
	UIBarButtonItem *navBarLeftBtn;		/*!< The left navigation bar button. */
	UIBarButtonItem *navBarRightBtn;	/*!< The right navigation bar button. */
    UIImage *bgImage;
    UILabel *titleLabel;
}


@property (nonatomic, readonly) UINavigationBar *navBar;
@property (nonatomic, readonly) UINavigationItem *navBarTitle;
@property (nonatomic, readonly) UIBarButtonItem *navBarLeftBtn;
@property (nonatomic, readonly) UIBarButtonItem *navBarRightBtn;
@property (nonatomic, readonly) UILabel *titleLabel;

/*!	\fn initWithTitle:withFrame:AtY:(NSString* title, CGRect frame, int y)
 *	\brief Initializes the navigation bar.
 *  
 *	\param title The title for the navigation bar.
 *	\param frame The frame to fit the navigation bar in.
 *	\param y The y position of the navigation bar.
 *  \return The #TJCUINavigationBarView object.
 */
- (id)initWithTitle:(NSString*)title withFrame:(CGRect)frame  AtY:(int)y;

/*!	\fn setNavBarLeftBtnWithTitle:(NSString*) title
 *	\brief Initializes and sets the title for #navBarLeftBtn.
 *  
 * A new navigation bar button item is allocated if it does not already exist.
 *	\param title The title for the navigation bar button item.
 *  \return n/a
 */
- (void)setNavBarLeftBtnWithTitle:(NSString*)title;

/*!	\fn setNavBarRightBtnWithTitle:(NSString*) title
 *	\brief Initializes and sets the title for #navBarRightBtn.
 *  
 * A new navigation bar button item is allocated if it does not already exist.
 *	\param title The title for the navigation bar button item.
 *  \return n/a
 */
- (void)setNavBarRightBtnWithTitle:(NSString*)title;

/*!	\fn updateLeftNavBarBtnWithTitle:(NSString*) title
 *	\brief Sets the title for #navBarLeftBtn.
 *  
 *	\param title The title for the navigation bar button item.
 *  \return n/a
 */
- (void)updateLeftNavBarBtnWithTitle:(NSString*)title;

/*!	\fn updateRightNavBarBtnWithTitle:(NSString*) title
 *	\brief Sets the title for #navBarRightBtn.
 *  
 *	\param title The title for the navigation bar button item.
 *  \return n/a
 */
- (void)updateRightNavBarBtnWithTitle:(NSString*)title;

/*!	\fn addLeftButtonTarget:action:forControlEvents:(id target, SEL action, UIControlEvents controlEvents)
 *	\brief Sets the button action for the left bar button.
 *  
 *	\param target The object that receives an action when the button is selected.
 *	\param action The action to perform when the button is selected.
  *	\param controlEvents The event that must occur for the action to be executed.
 *  \return n/a
 */
- (void)addLeftButtonTarget:(id)target action:(SEL)action forControlEvents:(UIControlEvents)controlEvents;

/*!	\fn addRightButtonTarget:action:forControlEvents:(id target, SEL action, UIControlEvents controlEvents)
 *	\brief Sets the button action for the right bar button.
 *  
 *	\param target The object that receives an action when the button is selected.
 *	\param action The action to perform when the button is selected.
 *	\param controlEvents The event that must occur for the action to be executed.
 *  \return n/a
 */
- (void)addRightButtonTarget:(id)target action:(SEL)action forControlEvents:(UIControlEvents)controlEvents;

/*!	\fn setNavBarLeftBtn:(UIBarButtonItem*) button
 *	\brief Sets the left bar button item for #navBarTitle.
 *  
 *	\param button The UIBarButtonItem to set in the UINavigationItem.
 *  \return n/a
 */
- (void)setNavBarLeftBtn:(UIBarButtonItem*)button;

/*!	\fn setNavBarRightBtn:(UIBarButtonItem*) button
 *	\brief Sets the right bar button item for #navBarTitle.
 *  
 *	\param button The UIBarButtonItem to set in the UINavigationItem.
 *  \return n/a
 */
- (void)setNavBarRightBtn:(UIBarButtonItem*)button;

/*!	\fn setCustomBackgroundImage:(UIImage*)image;
 *	\brief Sets the navigation bar background image.
 *  
 *	\param image The UIImage to set this navigation bar's background image to.
 *  \return n/a
 */
- (void)setCustomBackgroundImage:(UIImage*)image;

@end