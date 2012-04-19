// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>

#import "TJCVGUtil.h"
#import "TJCVGConstants.h"

#define TJC_VG_TAP_BTN_HIGHLIGHT_IMG_NAME	@"standard-black-button.png"
#define TJC_VG_TAP_BTN_NORMAL_IMG_NAME		@"standard-black-button-1.png"

@interface TJCVGGenericButtons : UIButton 
{
	id customParams;
	UISegmentedControl * segmentButton;
	UIColor * customColor;
}

@property(nonatomic,retain) id customParams;


- (id)initBlackStandarBtnWithFrame:(CGRect)frame;

//custom initfunction which takes only x and y size is hard coded 
- (id)initMenuTickButtonAtX:(int)x andY:(int)y;

//custom button for Tabs
- (id)initTabViewButtonAtx:(int)x andY:(int)y;

//Standard OK Button and relevent buttons
- (id)initStandardCommandButton:(int)x andY:(int)y;

//returns a generic button with NormImageWIdth and Height
- (id)initButtonWithX:(int)x andY:(int)y withNormImg:(UIImage*)normImg withHighlightImg:(UIImage*)highlightImg;

- (void)setStandardTintColorWithTitle:(NSString*)title withParentView:(UIView*)view;

- (void)setCustomButtonTintColor:(UIColor*)color withTitle:(NSString*)title withParentView:(UIView*)view;

- (void)setBtnTitle:(NSString*)title;

- (void)setFont:(UIFont*) aFont;

@end
