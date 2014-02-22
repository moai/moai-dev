// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJViewController.h"
#import <Tapjoy/Tapjoy.h>

@interface TJViewController () <TJCAdDelegate, TJCVideoAdDelegate, TJCViewDelegate>

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UIView *bannerView;
@property (weak, nonatomic) IBOutlet UILabel *statusLabel;
@property (weak, nonatomic) IBOutlet UILabel *sdkVersionLabel;
@property (weak, nonatomic) IBOutlet UIButton *getFullScreenAdButton;

@end

@implementation TJViewController


- (void)viewDidLoad
{
    [super viewDidLoad];

	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(showFullScreenAd:)
												 name:TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION
											   object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(showFullScreenAdError:)
												 name:TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION_ERROR
											   object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(showOffersResponse:)
												 name:TJC_OFFERS_RESPONSE_NOTIFICATION
											   object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(showOffersResponseError:)
												 name:TJC_OFFERS_RESPONSE_NOTIFICATION_ERROR
											   object:nil];
	
	[_sdkVersionLabel setText:[NSString stringWithFormat:@"SDK version: %@", [Tapjoy getVersion]]];
	[_scrollView setContentSize:CGSizeMake(_scrollView.contentSize.width, _bannerView.frame.origin.y + _bannerView.frame.size.height)];
    [Tapjoy setViewDelegate:self];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewDidUnload {
	[self setScrollView:nil];
	[self setBannerView:nil];
	[self setStatusLabel:nil];
	[self setSdkVersionLabel:nil];
	[self setGetFullScreenAdButton:nil];
	[super viewDidUnload];
}

- (void)enableButton:(UIButton*)button enable:(BOOL)enable
{
	[button setUserInteractionEnabled:enable];
	[button setAlpha:enable ? 1.0 : 0.5];
}

#pragma mark - Tapjoy Related Methods

- (IBAction)showOfferwallAction:(id)sender
{
	[Tapjoy showOffersWithViewController:self];
}


- (void)showOffersResponse:(NSNotification*)notification
{
	[_statusLabel setText:@"showOffers success"];
}


- (void)showOffersResponseError:(NSNotification*)notification
{
	[_statusLabel setText:@"showOffers failure"];
	[[[UIAlertView alloc] initWithTitle:@"Error"
								message:@"An error occurred while loading the Offerwall"
							   delegate:self
					  cancelButtonTitle:@"OK"
					  otherButtonTitles:nil] show];
}


- (IBAction)getFullScreenAdAction:(id)sender
{
	[self enableButton:_getFullScreenAdButton enable:NO];
	[Tapjoy getFullScreenAd];
}


- (void)showFullScreenAd:(NSNotification*)notification
{
	[_statusLabel setText:@"getFullScreenAd success"];
	[self enableButton:_getFullScreenAdButton enable:YES];
	[Tapjoy showFullScreenAdWithViewController:self];
}


- (void)showFullScreenAdError:(NSNotification*)notification
{
	[_statusLabel setText:@"getFullScreenAd failure"];
	[self enableButton:_getFullScreenAdButton enable:YES];
	[[[UIAlertView alloc] initWithTitle:@"Error"
							   message:@"No Full Screen Ad available"
							  delegate:self
					 cancelButtonTitle:@"OK"
					  otherButtonTitles:nil] show];
}


- (IBAction)getBannerAdAction:(id)sender
{
	[Tapjoy getDisplayAdWithDelegate:self];
}


-(void)logTapPointsError
{
    // Print out the updated points value.
    [_statusLabel setText:@"Error occurred, be sure to validate currency errors and not allow purchases if there is an error"];
}


- (IBAction)getTapPointsAction:(id)sender
{
	[Tapjoy getTapPointsWithCompletion:^(NSDictionary *parameters, NSError *error) {
		if (error)
			[self logTapPointsError];
		else
		{
			[_statusLabel setText:[NSString stringWithFormat:@"getTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
		}
	}];
}


- (IBAction)spendTapPointsAction:(id)sender
{
	[Tapjoy spendTapPoints:10 completion:^(NSDictionary *parameters, NSError *error) {
		if (error)
			[self logTapPointsError];
		else
			[_statusLabel setText:[NSString stringWithFormat:@"spendTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
	}];
}



- (IBAction)awardTapPointsAction:(id)sender
{
	[Tapjoy awardTapPoints:10 completion:^(NSDictionary *parameters, NSError *error) {
		if (error)
			[self logTapPointsError];
		else
		{
			[_statusLabel setText:[NSString stringWithFormat:@"awardTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
		}
	}];
}





#pragma mark - Tapjoy Display Ad Delegate methods
	 
	 
	 
- (void)didReceiveAd:(TJCAdView*)adView
{
	[_statusLabel setText:@"getDisplayAd success"];
	[_bannerView addSubview:adView];
}

- (void)didFailWithMessage:(NSString*)msg
{
	[_statusLabel setText:[NSString stringWithFormat:@"Banner ad error:%@", msg]];
}

- (NSString*)adContentSize
{
	return TJC_AD_BANNERSIZE_320X50;
}

- (BOOL)shouldRefreshAd
{
	return NO;
}

#pragma mark - Tapjoy Video Ad Delegate methods

- (void)videoAdBegan
{
	// The beginning of a video ad - this is a good place to suspend app logic and mute sounds.
}

- (void)videoAdClosed
{
	// This is a good place to resume app logic, unmute sounds, and check currency value.
	[Tapjoy getTapPoints];
}

- (void)videoAdError:(NSString*)errorMsg
{
	[_statusLabel setText:[NSString stringWithFormat:@"Tapjoy video ad error:%@", errorMsg]];
}

#pragma mark Tapjoy View Delegate Methods

- (void)viewWillAppearWithType:(int)viewType
{
    NSLog(@"Tapjoy viewWillAppearWithType: %d", viewType);
}

- (void)viewDidAppearWithType:(int)viewType
{
    NSLog(@"Tapjoy viewDidAppearWithType: %d", viewType);
}

- (void)viewWillDisappearWithType:(int)viewType
{
    NSLog(@"Tapjoy viewWillDisappearWithType: %d", viewType);
}

- (void)viewDidDisappearWithType:(int)viewType
{
    NSLog(@"Tapjoy viewDidDisappearWithType: %d", viewType);
}


@end
