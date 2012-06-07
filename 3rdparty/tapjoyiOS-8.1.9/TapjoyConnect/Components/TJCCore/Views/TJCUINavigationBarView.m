// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCUINavigationBarView.h"
#import "TJCViewCommons.h"
#import "TJCUtil.h"

@implementation TJCUINavigationBarView

@synthesize navBarLeftBtn, navBarRightBtn, navBarTitle, navBar, titleLabel;


- (id)initWithFrame:(CGRect)frame
{	
	if((self= [super initWithFrame:frame]))
	{
		navBar = nil;
		navBarTitle = nil;
		navBarLeftBtn = nil;
		navBarRightBtn = nil;
		bgImage = nil;
		
	}
	return self;
}


- (id)initWithTitle:(NSString *)title withFrame:(CGRect)frame  AtY:(int)y
{
	CGRect navBarFrame = CGRectMake(0, y, frame.size.width, TJC_NAV_BAR_HEIGHT);
	
	if ((self = [self initWithFrame:navBarFrame]))//local constructor
	{
		navBarTitle = [[UINavigationItem alloc] initWithTitle:title];
		self.items = [NSArray arrayWithObject:navBarTitle];
		
		[self setTintColor: [[TJCViewCommons sharedTJCViewCommons] getUserDefinedColor]];
		[self setAutoresizingMask: UIViewAutoresizingFlexibleWidth];
		[self setAutoresizingMask: UIViewAutoresizingFlexibleWidth];
	}
	return self;
}


- (void)setCustomBackgroundImage:(UIImage*)image
{
	UIImageView *bgImageView = [[UIImageView alloc] initWithImage:image];
	
	[self addSubview:bgImageView];
	//[self sendSubviewToBack:bgImageView];
	
	[bgImageView release];
}


- (void)updateLeftNavBarBtnWithTitle:(NSString*) title
{
	[navBarLeftBtn setTitle:title];
}


- (void)updateRightNavBarBtnWithTitle:(NSString*) title
{
	[navBarRightBtn setTitle:title];
}


- (void)setNavBarLeftBtn:(UIBarButtonItem*) button
{
	[navBarTitle setLeftBarButtonItem:button];
}


- (void)setNavBarRightBtn:(UIBarButtonItem*) button
{
	[navBarTitle setRightBarButtonItem:button];
}


- (void)setNavBarLeftBtnWithTitle:(NSString*) title
{
	if(!navBarLeftBtn)
	{
		navBarLeftBtn = [[UIBarButtonItem alloc] initWithTitle:title style:UIBarButtonItemStyleBordered target:nil action:nil];
		[navBarTitle setLeftBarButtonItem:navBarLeftBtn];
	}
	else
	{
		[self updateLeftNavBarBtnWithTitle:title];
	}	
}


- (void)setNavBarRightBtnWithTitle:(NSString*) title
{
	if(!navBarRightBtn)
	{
		navBarRightBtn = [[UIBarButtonItem alloc] initWithTitle:title style:UIBarButtonItemStyleBordered target:nil action:nil];
		[navBarTitle setRightBarButtonItem:navBarRightBtn];
	}
	else
	{
		[self updateRightNavBarBtnWithTitle:title];
	}	
}


- (void)addLeftButtonTarget:(id)target action:(SEL)action forControlEvents:(UIControlEvents)controlEvents
{
	[navBarLeftBtn setAction:action];
	[navBarLeftBtn setTarget:target];
}


- (void)addRightButtonTarget:(id)target action:(SEL)action forControlEvents:(UIControlEvents)controlEvents
{
	[navBarRightBtn setAction:action];
	[navBarRightBtn setTarget:target];
}


- (void)dealloc
{
	[navBarTitle release];
	[navBarLeftBtn release];
	[navBarRightBtn release];
	[navBar release];
	[bgImage release];
	[titleLabel release];
	[super dealloc];
}

@end