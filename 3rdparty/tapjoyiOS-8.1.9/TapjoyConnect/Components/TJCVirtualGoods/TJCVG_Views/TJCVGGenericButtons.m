// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVGGenericButtons.h"
#import "TJCVGUtil.h"
#import "TJCViewCommons.h"
#import "TJCLog.h"

@implementation TJCVGGenericButtons
@synthesize customParams;

- (id)initWithFrame:(CGRect)frame
{
	if((self=[super initWithFrame:CGRectMake(frame.origin.x, frame.origin.y+2, frame.size.width, frame.size.height)]))
	{
		
		//[self setFont:[UIFont fontWithName:@"Arial-BoldMT" size:12.5]];
		//[self setBackgroundImage:[TJCSettings getCommandBtnNormImg] forState:UIControlStateNormal];
	}
	return self;
}


- (void) setFont:(UIFont*) aFont
{
#ifndef __IPHONE_3_0
	[super setFont:aFont];
#else
	[super.titleLabel setFont:aFont];
#endif
}


- (id)initBlackStandarBtnWithFrame:(CGRect)frame
{
	if((self=[super initWithFrame:frame]))
	{
		[self setFont:[UIFont fontWithName:@"Arial-BoldMT" size:12.5f]];
		[self setBackgroundImage:[UIImage imageNamed:TJC_VG_TAP_BTN_NORMAL_IMG_NAME] forState:UIControlStateNormal];
		[self setBackgroundImage:[UIImage imageNamed:TJC_VG_TAP_BTN_HIGHLIGHT_IMG_NAME] forState:UIControlStateHighlighted];
	}
	return self;
}


- (id)initMenuTickButtonAtX:(int)x andY:(int)y 
{
	//if(self = [UIButton buttonWithType:UIButtonTypeCustom])
	//UIImage *bckImgForSize = [TJCSettings getMenuTickBtnNormalImg];
	if((self = [super initWithFrame:CGRectMake(x, y,124 ,33)]))
	{	
		UIFont * font=[UIFont fontWithName:@"Arial-BoldMT" size:12];
		[self setFont:font];
	}
	
	return self;
}


- (id)initTabViewButtonAtx:(int)x andY:(int)y 
{
	if((self = [super initWithFrame:CGRectMake(x, y,100 ,42)]))
	{	
		[self setFont:[UIFont fontWithName:@"Arial-BoldMT" size:12.5f]];
		[self setBackgroundImage:[UIImage imageNamed:TJC_VG_TAP_BTN_NORMAL_IMG_NAME] forState:UIControlStateNormal];
		[self setBackgroundImage:[UIImage imageNamed:TJC_VG_TAP_BTN_HIGHLIGHT_IMG_NAME] forState:UIControlStateHighlighted];
		[self setBackgroundImage:[UIImage imageNamed:TJC_VG_TAP_BTN_HIGHLIGHT_IMG_NAME] forState:UIControlStateSelected];
	}
	
	return self;
}


- (id)initStandardCommandButton:(int)x andY:(int)y
{
	if((self = [super initWithFrame:CGRectMake(x, y,60 ,30)]))
	{	
		[self setFont:[UIFont fontWithName:@"Arial-BoldMT" size:12.5f]];
	}
	
	return self;
}


- (id)initButtonWithX:(int)x andY:(int)y withNormImg:(UIImage*)normImg withHighlightImg:(UIImage*)highlightImg
{
	UIImage *bckImgForSize = normImg;
	if((self = [super initWithFrame:CGRectMake(x, y+4,bckImgForSize.size.width-80 ,bckImgForSize.size.height-40)]))
	{	
		[self setFont:[UIFont fontWithName:@"Arial-BoldMT" size:12.5f]];
		[self setBackgroundImage:normImg forState:UIControlStateNormal];
		[self setBackgroundImage:highlightImg forState:UIControlStateHighlighted];
	}
	
	return self;
}


- (void)setStandardTintColorWithTitle:(NSString*)title withParentView:(UIView*)view
{
	NSArray *buttonNames = [NSArray arrayWithObjects:title,nil];
	segmentButton = [[UISegmentedControl alloc] initWithItems:buttonNames];
	segmentButton.segmentedControlStyle = UISegmentedControlStyleBar;
	
	CGRect tempFrame = CGRectMake(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, self.frame.size.height);
	
	segmentButton.frame = tempFrame;
	
	[segmentButton setTintColor: [[TJCViewCommons sharedTJCViewCommons] getUserDefinedColor]];
	[view addSubview:segmentButton];
	customColor = [[TJCViewCommons sharedTJCViewCommons] getUserDefinedColor];
	
	[self addTarget:self action:@selector(buttonActionTouchDown:) forControlEvents:UIControlEventTouchDown];
	[self addTarget:self action:@selector(buttonActionTouchUp:) forControlEvents:UIControlEventTouchUpInside];
	[self addTarget:self action:@selector(buttonActionTouchUp:) forControlEvents:UIControlEventTouchDragOutside];
}


- (void)buttonActionTouchDown:(id)sender
{
	int white = -20;
	
	int oldColor = [[TJCViewCommons sharedTJCViewCommons] getUserDefinedColorCode];
	
	int highLightedColor =  [TJCVGUtil addWhiteWithColorInt:oldColor withWhiteInt:white];
	
	[segmentButton setTintColor:TJC_HEXCOLOR(highLightedColor)];
}


- (void)buttonActionTouchUp:(id)sender
{
	[segmentButton setTintColor: customColor];	
}


- (void)setCustomButtonTintColor:(UIColor*)color withTitle:(NSString*)title withParentView:(UIView*)view
{
	NSArray *buttonNames = [NSArray arrayWithObjects:title,nil];
	segmentButton = [[UISegmentedControl alloc] initWithItems:buttonNames];
	segmentButton.segmentedControlStyle = UISegmentedControlStyleBar;
	
	segmentButton.frame = self.frame;
	[segmentButton setTintColor: [[TJCViewCommons sharedTJCViewCommons] getUserDefinedColor]];	
	[view addSubview:segmentButton];
	customColor = color;
	
	[self addTarget:self action:@selector(buttonActionTouchDown:) forControlEvents:UIControlEventTouchDown];
	[self addTarget:self action:@selector(buttonActionTouchUp:) forControlEvents:UIControlEventTouchUpInside];
	[self addTarget:self action:@selector(buttonActionTouchUp:) forControlEvents:UIControlEventTouchDragOutside];
}


- (void)setBtnTitle:(NSString*)title
{
	[self setTitle:title forState:UIControlStateNormal];
	[self setTitle:title forState:UIControlStateHighlighted];
}


-(void) dealloc
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Releasing Generic Button"];
	if(customParams)
	{
		[customParams release];
		customParams = nil;
	}
	[super dealloc];
}

@end
