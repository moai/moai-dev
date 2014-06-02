// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJViewController.h"
#import <Tapjoy/Tapjoy.h>

@interface TJViewController () <TJCAdDelegate, TJCViewDelegate, TJEventDelegate>

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UIView *bannerView;
@property (weak, nonatomic) IBOutlet UILabel *statusLabel;
@property (weak, nonatomic) IBOutlet UILabel *sdkVersionLabel;

// Button references
@property (weak, nonatomic) IBOutlet UIButton *showOfferwallButton;
@property (weak, nonatomic) IBOutlet UIButton *getDirectPlayVideoAdButton;
@property (weak, nonatomic) IBOutlet UIButton *sendEventButton;
@property (weak, nonatomic) IBOutlet UIButton *getDisplayAdButton;
@property (weak, nonatomic) IBOutlet UIButton *getTapPointsButton;
@property (weak, nonatomic) IBOutlet UIButton *spendTapPointsButton;
@property (weak, nonatomic) IBOutlet UIButton *awardTapPointsButton;

@end

@implementation TJViewController


- (void)viewDidLoad
{
    [super viewDidLoad];

	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(showDirectPlayVideoAd:)
												 name:TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION
											   object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(showDirectPlayVideoAdError:)
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
	[self setGetDirectPlayVideoAdButton:nil];
	[self setSendEventButton:nil];
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
	[self enableButton:_showOfferwallButton enable:NO];
	[Tapjoy showOffersWithViewController:self];
}


- (void)showOffersResponse:(NSNotification*)notification
{
	[_statusLabel setText:@"showOffers success"];
	[self enableButton:_showOfferwallButton enable:YES];
}


- (void)showOffersResponseError:(NSNotification*)notification
{
	[_statusLabel setText:@"showOffers failure"];
	[self enableButton:_showOfferwallButton enable:YES];
	[[[UIAlertView alloc] initWithTitle:@"Error"
								message:@"An error occurred while loading the Offerwall"
							   delegate:self
					  cancelButtonTitle:@"OK"
					  otherButtonTitles:nil] show];
}


- (IBAction)getDirectPlayVideoAdAction:(id)sender
{
	[self enableButton:_getDirectPlayVideoAdButton enable:NO];
    // Shows a direct play video
    // The getFullScreenAd call has been deprecated and should only be used for getting direct play videos.
    // To show a full screen ad please use the Events framework -- see sendEventAction: below
	[Tapjoy getFullScreenAd];
}


- (void)showDirectPlayVideoAd:(NSNotification*)notification
{
	[_statusLabel setText:@"getFullScreenAd success"];
	[self enableButton:_getDirectPlayVideoAdButton enable:YES];
	[Tapjoy showFullScreenAdWithViewController:self];
}


- (void)showDirectPlayVideoAdError:(NSNotification*)notification
{
	[_statusLabel setText:@"getFullScreenAd failure"];
	[self enableButton:_getDirectPlayVideoAdButton enable:YES];
	[[[UIAlertView alloc] initWithTitle:@"Error"
							   message:@"No Full Screen Ad available"
							  delegate:self
					 cancelButtonTitle:@"OK"
					  otherButtonTitles:nil] show];
}


- (IBAction)getBannerAdAction:(id)sender
{
	[self enableButton:_getDisplayAdButton enable:NO];
	[Tapjoy getDisplayAdWithDelegate:self];
}


-(void)logTapPointsError
{
    // Print out the updated points value.
    [_statusLabel setText:@"Error occurred, be sure to validate currency errors and not allow purchases if there is an error"];
}


- (IBAction)getTapPointsAction:(id)sender
{
	[self enableButton:_getTapPointsButton enable:NO];
	[Tapjoy getTapPointsWithCompletion:^(NSDictionary *parameters, NSError *error) {
		if (error)
			[self logTapPointsError];
		else
		{
			[_statusLabel setText:[NSString stringWithFormat:@"getTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
		}
		[self enableButton:_getTapPointsButton enable:YES];
	}];
}


- (IBAction)spendTapPointsAction:(id)sender
{
	[self enableButton:_spendTapPointsButton enable:NO];
	[Tapjoy spendTapPoints:10 completion:^(NSDictionary *parameters, NSError *error) {
		if (error)
			[self logTapPointsError];
		else
			[_statusLabel setText:[NSString stringWithFormat:@"spendTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
		[self enableButton:_spendTapPointsButton enable:YES];
	}];
}



- (IBAction)awardTapPointsAction:(id)sender
{
	[self enableButton:_awardTapPointsButton enable:NO];
	[Tapjoy awardTapPoints:10 completion:^(NSDictionary *parameters, NSError *error) {
		if (error)
			[self logTapPointsError];
		else
		{
			[_statusLabel setText:[NSString stringWithFormat:@"awardTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
		}

		[self enableButton:_awardTapPointsButton enable:YES];
	}];	
}

- (IBAction)sendEventAction:(id)sender
{
	[self enableButton:_sendEventButton enable:NO];
	TJEvent *evt = [TJEvent eventWithName:@"test_unit" delegate:self];
	
	// If presentAutomatically is set to YES, set the ViewController used to display content by calling:
	// [evt setPresentationViewController:self];
	
	// By default, ad content will be shown automatically on a successful send. For finer control of when content should be shown, call:
	[evt setPresentAutomatically:NO];
	
	[evt send];
	[_statusLabel setText:@"Sending event..."];
}

#pragma mark - Tapjoy Display Ad Delegate methods
	 
	 
	 
- (void)didReceiveAd:(TJCAdView*)adView
{
	[_statusLabel setText:@"getDisplayAd success"];
	[self enableButton:_getDisplayAdButton enable:YES];
	[_bannerView addSubview:adView];
}

- (void)didFailWithMessage:(NSString*)msg
{
	[_statusLabel setText:[NSString stringWithFormat:@"Banner ad error:%@", msg]];
	[self enableButton:_getDisplayAdButton enable:YES];
}

- (NSString*)adContentSize
{
	return TJC_DISPLAY_AD_SIZE_320X50;
}

- (BOOL)shouldRefreshAd
{
	return NO;
}

#pragma mark Tapjoy View Delegate Methods

- (void)viewWillAppearWithType:(int)viewType
{
    NSLog(@"Tapjoy view is about to be shown.");
}

- (void)viewDidAppearWithType:(int)viewType
{
	NSLog(@"Tapjoy view has been shown.");
	// Best Practice: We recommend calling getTapPoints as often as possible so the user’s balance is always up-to-date.
	[Tapjoy getTapPoints];
}

- (void)viewWillDisappearWithType:(int)viewType
{
    NSLog(@"Tapjoy view is about to go away.");
}

- (void)viewDidDisappearWithType:(int)viewType
{
	NSLog(@"Tapjoy view has been closed.");
	// Best Practice: We recommend calling getTapPoints as often as possible so the user’s balance is always up-to-date.
	[Tapjoy getTapPoints];
}

#pragma mark TJEventDelegate methods

- (void)sendEventComplete:(TJEvent*)event withContent:(BOOL)contentIsAvailable
{
	[_statusLabel setText:[NSString stringWithFormat:@"Tapjoy send event complete, contentIsAvailable:%d", contentIsAvailable]];
	// Make sure we recieved content from the event call
	if (contentIsAvailable) {
		// If you do not have presentAutomatically set to YES for the event we need to present the event's content ourselves
		[event presentContentWithViewController:self];
	}
	[self enableButton:_sendEventButton enable:YES];
}


- (void)sendEventFail:(TJEvent*)event error:(NSError*)error
{
	[_statusLabel setText:[NSString stringWithFormat:@"Tapjoy send event failed with error: %@", [error localizedDescription]]];
	[self enableButton:_sendEventButton enable:YES];
}


- (void)contentWillAppear:(TJEvent*)event
{
	if (!event.eventValue)
		NSLog(@"Content will appear for %@ event", [event eventName]);
	else
		NSLog(@"Content will appear for %@ event with value: %@", [event eventName], [event eventValue]);
}


- (void)contentDidAppear:(TJEvent*)event
{
	if (![event eventValue])
		NSLog(@"Content did appear for %@ event", [event eventName]);
	else
		NSLog(@"Content did appear for %@ event with value: %@", [event eventName], [event eventValue]);
}


- (void)contentWillDisappear:(TJEvent*)event
{
	if (![event eventValue])
		NSLog(@"Content will disappear for %@ event", [event eventName]);
	else
		NSLog(@"Content will disappear for %@ event with value: %@", [event eventName], [event eventValue]);
}


- (void)contentDidDisappear:(TJEvent*)event
{
	if (![event eventValue])
		NSLog(@"Content did disappear for %@ event", [event eventName]);
	else
		NSLog(@"Content did disappear for %@ event with value: %@", [event eventName], [event eventValue]);
}

- (void)event:(TJEvent*)event didRequestAction:(TJEventRequest*)request {
	NSString *message = [NSString stringWithFormat: @"Type: %d, Identifier: %@, Quantity: %d", request.type, request.identifier, request.quantity];
	[[[UIAlertView alloc] initWithTitle: @"Got Action Callback" message: message delegate: nil cancelButtonTitle: @"OK" otherButtonTitles: nil] show];

	/*
	// Your app should perform an action based on the value of the request.type property
	switch(request.type) {
		case TJEventRequestInAppPurchase:
			// Your app should initiate an in-app purchase of the product identified by request.identifier
			break;
		case TJEventRequestVirtualGood:
			// Your app should award the user the item specified by request.identifier with the amount specified by request.quantity
			break;
		case TJEventRequestCurrency:
			// The user has been awarded the currency specified with request.identifier, with the amount specified by request.quantity
			break;
		case TJEventRequestNavigation:
			// Your app should attempt to navigate to the location specified by request.identifier
			break;
	}
	*/
	
	// Your app must call either completed or cancelled to complete the lifecycle of the request
	[request completed];
}

@end
