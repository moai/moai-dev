// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCVGItemButton.h"
#import "TJCVGItemButtonView.h"
#import "TJCVGItemDetail.h"
#import "TJCViewCommons.h"
#import "TJCVGViewHandler.h"
#import "TJCVGView.h"


@implementation TJCVGItemButton

@synthesize itemButton, storeItemRef;

- (id) init
{
	if ((self = [super init]))
	{
		itemButton = [[[NSBundle mainBundle] loadNibNamed:@"TJCVGItemButton" owner:self options:nil] objectAtIndex:0];
	}
	
	return self;
}


- (IBAction)itemSelected
{
	BOOL shouldEnableBuyButton = YES;
	
	if ([[TJCVGView sharedTJCVGView] getCurrentActiveView] == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		shouldEnableBuyButton = NO;
	}
	
	itemDetail = [[TJCVGItemDetail alloc] initWithItem:self 
														  withFrame:[TJCVGView sharedTJCVGView].mainView.bounds
												withButtonEnabled:shouldEnableBuyButton];
	
	// Add the details view to the calls wrapper view.
	//[[TJCCallsWrapper sharedTJCCallsWrapper].view addSubview:itemDetail.mainView];
	[[TJCVGView sharedTJCVGView].mainView addSubview:itemDetail.mainView];
	
	[TJCViewCommons animateTJCView:itemDetail.mainView withTJCTransition:TJCTransitionBottomToTop withDelay:0.5f];
}


- (void)dealloc 
{
	[itemDetail release];
	[itemButton release];
	[storeItemRef release];
	[super dealloc];
}


@end
