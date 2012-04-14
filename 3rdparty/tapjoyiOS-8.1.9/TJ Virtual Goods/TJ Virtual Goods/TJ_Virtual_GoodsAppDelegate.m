// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJ_Virtual_GoodsAppDelegate.h"
#import "MainViewController.h"
#import "TapjoyConnect.h"


@implementation TJ_Virtual_GoodsAppDelegate

@synthesize window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	[TJCLog setLogThreshold:LOG_DEBUG];
	
	// Connect Relevent Calls
	// Connect Notifications 
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(tjcConnectSuccess:) name:TJC_CONNECT_SUCCESS object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(tjcConnectFail:) name:TJC_CONNECT_FAILED object:nil];
	
	// NOTE: This must be replaced by your App Id. It is Retrieved from the Tapjoy website, in your account.
	[TapjoyConnect requestTapjoyConnect:@"93e78102-cbd7-4ebf-85cc-315ba83ef2d5" secretKey:@"JWxgS26URM0XotaghqGn"];
	
	// If you are not using Tapjoy Managed currency, you would set your own user ID here.
	//[TapjoyConnect setUserID:@"A_UNIQUE_USER_ID"];
	
	// View Specific Calls
	[TapjoyConnect setTransitionEffect:TJCTransitionExpand];
	[TapjoyConnect setUserdefinedColorWithIntValue:0x808080];
	//OR
	//[TapjoyConnect setUserDefinedColorWithRed:208 WithGreen:119 WithBlue:0];
	
	// Get Featured App Call
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getFeaturedApp:) name:TJC_FEATURED_APP_RESPONSE_NOTIFICATION object:nil];
	[TapjoyConnect getFeaturedApp];
	// This will set the display count to infinity effectively always showing the featured app.
	[TapjoyConnect setFeaturedAppDisplayCount:TJC_FEATURED_COUNT_INF];
	
	// Notifications for Tap Points related callbacks.
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(getUpdatedPoints:) 
																name:TJC_TAP_POINTS_RESPONSE_NOTIFICATION 
															 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(getUpdatedPoints:) 
																name:TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION 
															 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(getUpdatedPoints:) 
																name:TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION 
															 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(getUpdatedPointsError:) 
																name:TJC_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR 
															 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(getUpdatedPointsError:) 
																name:TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR 
															 object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(getUpdatedPointsError:) 
																name:TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR 
															 object:nil];
	[TapjoyConnect getTapPoints];
	
	// Watch for virtual good notification indicating that items are ready to go.
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(virtualGoodsUpdated:) 
																name:TJC_PURCHASED_ITEMS_RESPONSE_NOTIFICATION 
															 object:nil];
	
	// This both initiates a virtual goods download and an alert if the goods have been updated on this device.
	[TapjoyConnect showDownloadVGItemsPrompt];
	
	// Get Tapjoy Display Ads Call.
	[TapjoyConnect getDisplayAdWithDelegate:mainCtrl_];
	[mainCtrl_.view addSubview:[TapjoyConnect getDisplayAdView]];
	
	navCtrl_ = [[UINavigationController alloc]initWithRootViewController:mainCtrl_];
	navCtrl_.navigationBar.barStyle = UIBarStyleDefault;
	navCtrl_.navigationBarHidden = YES;
	
	[window addSubview:navCtrl_.view];
	
	[window makeKeyAndVisible];
	
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Remove this to prevent the possibility of multiple redundant notifications.
	[[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_TAPPOINTS_EARNED_NOTIFICATION object:nil];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	/*
	 Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
	 If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
	 */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	/*
	 Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
	 */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Add an observer for when a user has successfully earned currency.
	[[NSNotificationCenter defaultCenter] addObserver:self
														  selector:@selector(showEarnedCurrencyAlert:) 
																name:TJC_TAPPOINTS_EARNED_NOTIFICATION 
															 object:nil];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	/*
	 Called when the application is about to terminate.
	 Save data if appropriate.
	 See also applicationDidEnterBackground:.
	 */
}

- (void)dealloc
{
	[window release];
	[navCtrl_ release];
	[mainCtrl_ release];
	[super dealloc];
}



#pragma mark TJC_NOTIFICATION_HANDLERS

-(void)getUpdatedPoints:(NSNotification*)notifyObj
{
	NSNumber *tapPoints = notifyObj.object;
	NSString *tapPointsStr = [NSString stringWithFormat:@"Tap Points: %d", [tapPoints intValue]];
	NSLog(@"%@", tapPointsStr);
	
	[mainCtrl_.tapPointsLabel setText:tapPointsStr];
}


- (void)showEarnedCurrencyAlert:(NSNotification*)notifyObj
{
	NSNumber *tapPointsEarned = notifyObj.object;
	int earnedNum = [tapPointsEarned intValue];
	
	NSLog(@"Currency earned: %d", earnedNum);
	
	// Pops up a UIAlert notifying the user that they have successfully earned some currency.
	// This is the default alert, so you may place a custom alert here if you choose to do so.
	[TapjoyConnect showDefaultEarnedCurrencyAlert];
	
	// This is a good place to remove this notification since it is undesirable to have a pop-up alert during gameplay.
	[[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_TAPPOINTS_EARNED_NOTIFICATION object:nil];
}


- (void)getUpdatedPointsError:(NSNotification*)notifyObj
{
	[mainCtrl_.tapPointsLabel setText:@"Loading..."];
}


-(void)getFeaturedApp:(NSNotification*)notifyObj
{
	//notify Object will be returned as Nil in case of internet error or unavailibity of featured App or its Max Number of count has exceeded its limit
	TJCFeaturedAppModel *featuredApp  = notifyObj.object;
	NSLog(@"Featured App Name: %@, Cost: %@, Description: %@", featuredApp.name, featuredApp.cost, featuredApp.description);
	NSLog(@"Featured App Image URL %@ ", featuredApp.iconURL);
	
	// Show the custom Tapjoy full screen featured app ad view.
	if (featuredApp)
	{
		[TapjoyConnect showFeaturedAppFullScreenAdWithViewController:mainCtrl_];
		[mainCtrl_.showFeaturedBtn setEnabled:YES];
		[mainCtrl_.showFeaturedBtn setAlpha:1.0f];
	}
}


- (void)virtualGoodsUpdated:(NSNotification*)notifyObj
{
	// Example on how to access the downloaded virtual goods.
	StoreItem * strItem;
	
	NSMutableArray* vgStoreItemArray = [TapjoyConnect getPurchasedVGStoreItems];
	
	for (int i = 0; i < [vgStoreItemArray count]; ++i) 
	{
		NSLog(@"VG Item %d Start================================================================================================", i);
		strItem = [vgStoreItemArray objectAtIndex:i];
		
		NSLog(@"Item Name:					%@", [strItem title]);
		NSLog(@"Item Type:					%@", [strItem storeItemType]);
		NSLog(@"Item Description:			%@", [strItem description]);
		NSLog(@"Item Price:					%d", [strItem price]);
		NSLog(@"Item Currency Name:			%@", [strItem currencyName]);
		NSLog(@"Item Data File Location:	%@", [strItem datafileLocation]);
		
		// Print out contents of data file.
		if(![[strItem datafileLocation] isEqualToString:@""])
		{
			NSError *error;
			NSArray *contentsArray = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:[strItem datafileLocation] error:&error];
			for (int j = 0; j < [contentsArray count]; ++j) 
			{
				NSLog(@"     %d Data File Contents: %@", j, [contentsArray objectAtIndex:j]);
			}
		}
		
		NSMutableDictionary *dict = [strItem attributeValues];
		
		for (id key in dict)
		{
			id value = [dict objectForKey:key];
			NSLog(@"     Attribute:%@ Value:%@", key, value);
		}
		NSLog(@"VG Item %d End==================================================================================================", i);
	}
	
	[vgStoreItemArray release];
}


-(void)tjcConnectSuccess:(NSNotification*)notifyObj
{
	NSLog(@"Tapjoy connect Succeeded");
}


-(void)tjcConnectFail:(NSNotification*)notifyObj
{
	NSLog(@"Tapjoy connect Failed");	
}


@end
