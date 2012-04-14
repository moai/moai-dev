// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


/*!	\interface TJCCallsWrapper
 *	\brief The Tapjoy Connect Calls Wrapper class.
 *
 * This class is used to allow access to the offers view.
 */
@interface TJCCallsWrapper : UIViewController 
{
	UIInterfaceOrientation currentInterfaceOrientation;
}

@property (assign) UIInterfaceOrientation currentInterfaceOrientation;

/*!	\fn sharedTJCCallsWrapper
 *	\brief Returns a globally accessible #TJCCallsWrapper.
 *  
 *  \return A global #TJCCallsWrapper object.
 */
+ (TJCCallsWrapper*) sharedTJCCallsWrapper;

/*!	\fn updateViewsWithOrientation:(UIInterfaceOrientation)interfaceOrientation;
 *	\brief Some Tapjoy controlled views rely on the orientation being updated when the view is part of an external view controller.
 *
 *	\param orientation The new orientation of the view.
 *	\return n/a
 */
- (void) updateViewsWithOrientation:(UIInterfaceOrientation)interfaceOrientation;

/*!	\fn moveViewToFront
 *	\brief This moves the TJCCallsWrapper view to the front to ensure that any views to be displayed will be visisble.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)moveViewToFront;

- (void)showBoxCloseNotification:(NSNotification *)notifierObj;

@end
