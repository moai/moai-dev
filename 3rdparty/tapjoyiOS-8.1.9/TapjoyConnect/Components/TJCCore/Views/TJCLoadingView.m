// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCLoadingView.h"
#import "TJCConstants.h"


@implementation TJCLoadingView

@synthesize mainView;

- (id)initWithFrame:(CGRect)frame
{
	self = [super init];
	
	if (self)
	{
		mainView = [[[NSBundle mainBundle] loadNibNamed:@"TJCLoadingView" owner:self options:nil] objectAtIndex:0];
		[loadingBG.layer setCornerRadius:5];
	}
	
	return self;
}


- (void)disable
{
	[mainView setAlpha:0];
	[activityIndicator stopAnimating];
}


- (void)enable
{
	[mainView setAlpha:1];
	[activityIndicator startAnimating];
}


- (void)fadeIn
{
	// Don't fade in if it's already visible.
	if (mainView.alpha == 1)
	{
		return;
	}
	
	[mainView setAlpha:0];
	[activityIndicator startAnimating];
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJC_DEFAULT_TRANSITION_DELAY];
	
	[mainView setAlpha:1];
	
	[UIView commitAnimations];
}


- (void)fadeOut
{
	// Don't fade out if it's not visible.
	if (mainView.alpha == 0)
	{
		return;
	}
	
	[mainView setAlpha:1];
	[activityIndicator stopAnimating];
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJC_DEFAULT_TRANSITION_DELAY];
	
	[mainView setAlpha:0];
	
	[UIView commitAnimations];
}


- (void)dealloc
{
	[loadingBG release];
	[activityIndicator release];
	[loadingLabel release];
	[containerView release];
	[mainView release];
	[super dealloc];
}

@end
