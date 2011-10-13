//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCLoadingViewController.h"
#import "TJCConstants.h"


@implementation TJCLoadingViewController

- (id)initWithFrame:(CGRect)frame
{
	self = [super init];
	
	if (self)
	{
		self.view = [[[NSBundle mainBundle] loadNibNamed:@"TJCLoadingViewController" owner:self options:nil] objectAtIndex:0];
		[loadingBG.layer setCornerRadius:5];
	}
	
	return self;
}


- (void)disable
{
	[self.view setAlpha:0];
	[activityIndicator stopAnimating];
}


- (void)enable
{
	[self.view setAlpha:1];
	[activityIndicator startAnimating];
}


- (void)fadeIn
{
	// Don't fade in if it's already visible.
	if (self.view.alpha == 1)
	{
		return;
	}
	
	[self.view setAlpha:0];
	[activityIndicator startAnimating];

	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJC_DEFAULT_TRANSITION_DELAY];
	
	[self.view setAlpha:1];
	
	[UIView commitAnimations];
}


- (void)fadeOut
{
	// Don't fade out if it's not visible.
	if (self.view.alpha == 0)
	{
		return;
	}
	
	[self.view setAlpha:1];
	[activityIndicator stopAnimating];
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJC_DEFAULT_TRANSITION_DELAY];
	
	[self.view setAlpha:0];
	
	[UIView commitAnimations];
}


- (void)dealloc
{
    [super dealloc];
}

@end
