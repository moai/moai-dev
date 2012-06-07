// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "MainViewController.h"
#import "TapjoyConnect.h"

@implementation MainViewController

@synthesize showFeaturedBtn, tapPointsLabel;


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void) viewDidLoad 
{
	[super viewDidLoad];
	
	[showOffersBtn setAutoresizingMask:
	 UIViewAutoresizingFlexibleTopMargin|
	 UIViewAutoresizingFlexibleBottomMargin];
	
	[refreshAdBtn setAutoresizingMask:
	 UIViewAutoresizingFlexibleTopMargin|
	 UIViewAutoresizingFlexibleBottomMargin];
	
	[showFeaturedBtn setAutoresizingMask:
	 UIViewAutoresizingFlexibleTopMargin|
	 UIViewAutoresizingFlexibleBottomMargin];
	
	[getTapPointsBtn setAutoresizingMask:
	 UIViewAutoresizingFlexibleTopMargin|
	 UIViewAutoresizingFlexibleBottomMargin];
	
	[spendTapPointsBtn setAutoresizingMask:
	 UIViewAutoresizingFlexibleTopMargin|
	 UIViewAutoresizingFlexibleBottomMargin];
	
	[showVGBtn setAutoresizingMask:
	 UIViewAutoresizingFlexibleTopMargin|
	 UIViewAutoresizingFlexibleBottomMargin];
	
	[tapPointsLabel setAutoresizingMask:
	 UIViewAutoresizingFlexibleTopMargin|
	 UIViewAutoresizingFlexibleBottomMargin];
	
	[awardTapPointsBtn setAutoresizingMask:
	 UIViewAutoresizingFlexibleTopMargin|
	 UIViewAutoresizingFlexibleBottomMargin];
	
	[showFeaturedBtn setEnabled:NO];
	[showFeaturedBtn setAlpha:0.5f];
}


- (IBAction)showOffersBtnWithNavBarAction:(id)sender
{
	
	[self.navigationController setNavigationBarHidden:YES];
	
	[TapjoyConnect showOffersWithViewController:self];
	
	//OR
	//UIView *v = [TapjoyConnect showOffers];
	//[self.view addSubview:v];
}


- (IBAction)showFeaturedAdAction:(id)sender
{
	[TapjoyConnect showFeaturedAppFullScreenAdWithViewController:self];
}


- (IBAction)refreshAdAction:(id)sender
{
	[TapjoyConnect refreshDisplayAd];
}


- (IBAction)getTapPointsAction:(id)sender
{
	[tapPointsLabel setText:@"Updating..."];
	[TapjoyConnect getTapPoints];
}


- (IBAction)spendTapPointsAction:(id)sender
{
	[tapPointsLabel setText:@"Updating..."];
	[TapjoyConnect spendTapPoints:5];
}


- (IBAction)awardTapPointsAction:(id)sender
{
	[tapPointsLabel setText:@"Updating..."];
	[TapjoyConnect awardTapPoints:5];
}


- (IBAction)showVirtualGoodsBtnAction:(id)sender
{
	[TapjoyConnect showVirtualGoodsWithViewController:self];
}


- (void) viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	[self.navigationController setNavigationBarHidden:YES];
}


// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Return YES for supported orientations
	return YES;
}


- (void)didReceiveMemoryWarning 
{
	// Releases the view if it doesn't have a superview.
	[super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload 
{
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc 
{
	[showOffersBtn release];
	[refreshAdBtn release];
	[showFeaturedBtn release];
	[getTapPointsBtn release];
	[spendTapPointsBtn release];
	[showVGBtn release];
	[tapPointsLabel release];
	
	[super dealloc];
}



#pragma mark Tapjoy Display Ads Delegate Methods

- (void)didReceiveAd:(TJCAdView*)adView
{
	adView.frame = CGRectMake(0, 0, 320, 50);
}


- (void)didFailWithMessage:(NSString*)msg
{
	NSLog(@"No Tapjoy Display Ads available");
}


- (NSString*)adContentSize
{
	return TJC_AD_BANNERSIZE_320X50;
}


- (BOOL)shouldRefreshAd
{
	return NO;
}


@end
