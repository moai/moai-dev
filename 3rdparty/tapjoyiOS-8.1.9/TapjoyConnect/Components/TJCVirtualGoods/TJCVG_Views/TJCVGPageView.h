// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <UIKit/UIKit.h>


@class TJCVGStoreItem;


@interface TJCVGPageView : NSObject
{
    int pageNumber;

	// Buttons are arranged left to right, then downwards.
	NSMutableArray *buttons;
	
	IBOutlet UIView *pageView;
}


@property (nonatomic, retain) IBOutlet UIView *pageView;
@property (nonatomic, retain) NSMutableArray *buttons;


// The page init method.
-(id)initWithPageNumber:(int)page withFrame:(CGRect)frame withData:(NSArray*)data;

// Initialize an individual button. This will all initiate a button fade-in animation.
-(void)initButtonWithID:(int)buttonID withDetail:(TJCVGStoreItem*)details;

// Fade the entire view in/out of visibility.
-(void)fadeOutViewWithDuration:(float)duration;
-(void)fadeInViewWithDuration:(float)duration;

// Fade all the buttons in/out of visibility.
-(void)fadeOutButtonsWithDuration:(float)duration;
-(void)fadeInButtonsWithDuration:(float)duration;

// Resizes and repositions the item buttons after a rotation.
- (void) reformatButtonsWithFrame:(CGRect)frame;

- (void)fadeInButtonWithIndex:(int)index;

@end
