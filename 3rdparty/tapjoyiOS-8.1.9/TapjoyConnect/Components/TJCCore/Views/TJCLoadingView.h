// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>



@interface TJCLoadingView : NSObject 
{
	IBOutlet UIView *loadingBG;
	IBOutlet UIActivityIndicatorView *activityIndicator;
	IBOutlet UILabel *loadingLabel;
	IBOutlet UIView *containerView;
	IBOutlet UIView *mainView;
}

@property (nonatomic, retain) UIView *mainView;


- (id)initWithFrame:(CGRect)frame;
- (void)disable;
- (void)enable;
- (void)fadeIn;
- (void)fadeOut;

@end
