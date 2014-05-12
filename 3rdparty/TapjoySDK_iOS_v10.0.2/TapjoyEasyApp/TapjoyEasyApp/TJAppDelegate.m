// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJAppDelegate.h"
#import <Tapjoy/Tapjoy.h>

@implementation TJAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Tapjoy Connect Notifications
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(tjcConnectSuccess:)
                                                 name:TJC_CONNECT_SUCCESS
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(tjcConnectFail:)
                                                 name:TJC_CONNECT_FAILED
                                               object:nil];
    
	// NOTE: This is the only step required if you're an advertiser.
	// NOTE: This must be replaced by your App ID. It is retrieved from the Tapjoy website, in your account.
	[Tapjoy requestTapjoyConnect:@"13b0ae6a-8516-4405-9dcf-fe4e526486b2"
					   secretKey:@"XHdOwPa8de7p4aseeYP0"
						 options:@{ TJC_OPTION_ENABLE_LOGGING : @(YES) }
		// If you are not using Tapjoy Managed currency, you would set your own user ID here.
        // TJC_OPTION_USER_ID : @"A_UNIQUE_USER_ID"
   
		// You can also set your event segmentation parameters here.
		// Example segmentationParams object -- NSDictionary *segmentationParams = @{@"iap" : @(YES)};
		// TJC_OPTION_SEGMENTATION_PARAMS : segmentationParams
     ];
	
    return YES;
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Add an observer for when a user has successfully earned currency.
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(showEarnedCurrencyAlert:)
												 name:TJC_TAPPOINTS_EARNED_NOTIFICATION
											   object:nil];
	
	// Best Practice: We recommend calling getTapPoints as often as possible so the user’s balance is always up-to-date.
	[Tapjoy getTapPoints];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Remove this to prevent the possibility of multiple redundant notifications.
	[[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_TAPPOINTS_EARNED_NOTIFICATION object:nil];
}

-(void)tjcConnectSuccess:(NSNotification*)notifyObj
{
	NSLog(@"Tapjoy connect Succeeded");
}


- (void)tjcConnectFail:(NSNotification*)notifyObj
{
	NSLog(@"Tapjoy connect Failed");
}

- (void)showEarnedCurrencyAlert:(NSNotification*)notifyObj
{
	NSNumber *tapPointsEarned = notifyObj.object;
	int earnedNum = [tapPointsEarned intValue];
	
	NSLog(@"Currency earned: %d", earnedNum);
	
	// Pops up a UIAlert notifying the user that they have successfully earned some currency.
	// This is the default alert, so you may place a custom alert here if you choose to do so.
	[Tapjoy showDefaultEarnedCurrencyAlert];
}

@end
