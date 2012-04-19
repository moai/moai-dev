// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <UIKit/UIKit.h>

@class MainViewController;

@interface TJ_Virtual_GoodsAppDelegate : NSObject <UIApplicationDelegate>
{
	UIWindow *window;
	UINavigationController *navCtrl_;
	IBOutlet MainViewController *mainCtrl_;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end
