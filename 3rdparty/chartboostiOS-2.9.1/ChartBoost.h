//
//  ChartBoost.h
//  ChartBoost
//  2.9.1
//
//  Created by Kenneth Ballenegger on 8/1/11.
//  Copyright 2011 ChartBoost. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>



@protocol ChartBoostDelegate;


@interface ChartBoost : NSObject

@property (retain) NSString *appId;
@property (retain) NSString *appSignature;

@property (assign) id <ChartBoostDelegate> delegate;


// Extra configuration settings
// Timeout for requests (minimum is 10s, default is 30s)
@property NSUInteger timeout;


// Get the singleton
+ (ChartBoost *)sharedChartBoost;

// Start the Chartboost session
- (void)startSession;

// Cache an interstitial
- (void)cacheInterstitial;
- (void)cacheInterstitial:(NSString *)location;

// Show an interstitial, optionally takes a location argument
- (void)showInterstitial;
- (void)showInterstitial:(NSString *)location;

// Implement this to check if an interstitial is stored in cache for the default location  	
- (BOOL)hasCachedInterstitial;
// Implement this to check if an interstitial is stored in cache for a specific location
- (BOOL)hasCachedInterstitial:(NSString *)location;

// Cache the More Apps page
- (void)cacheMoreApps;

// Show the More Apps page
- (void)showMoreApps;

// Show / Hide the user's identity (default = FALSE)
- (void)setIdentityHidden:(BOOL)hidden;
// Returns whether or not this identity is hidden
- (BOOL)isIdentityHidden;

@end


@protocol ChartBoostDelegate <NSObject>
@optional

// All of the delegate methods below are optional.
// Implement them only when you need to more finely control ChartBoost's behavior.



// Called before requesting an interestitial from the back-end
// Only useful to prevent an ad from being implicitly displayed on fast app switching
- (BOOL)shouldRequestInterstitial;

// Called when an interstitial has been received, before it is presented on screen
// Return NO if showing an interstitial is currently innapropriate, for example if the user has entered the main game mode.
// This is also the method you want to use if you're going to display the interestitial yourself.
- (BOOL)shouldDisplayInterstitial:(UIView *)interstitialView;

// Called when an interstitial has failed to come back from the server
- (void)didFailToLoadInterstitial;

// Called when the user dismisses the interstitial
// If you are displaying the add yourself, dismiss it now.
- (void)didDismissInterstitial:(UIView *)interstitialView;

// Same as above, but only called when dismissed for a close
- (void)didCloseInterstitial:(UIView *)interstitialView;

// Same as above, but only called when dismissed for a click
- (void)didClickInterstitial:(UIView *)interstitialView;


// Called before requesting the more apps view from the back-end
// Return NO if when showing the loading view is not the desired user experience.
// You'll want to use this if you're caching the more app view.
- (BOOL)shouldDisplayLoadingViewForMoreApps;

// Called when an more apps page has been received, before it is presented on screen
// Return NO if showing an interstitial is currently innapropriate, for example if the user has entered the main game mode.
// This is also the method you want to use if you're going to display the interestitial yourself.
- (BOOL)shouldDisplayMoreApps:(UIView *)moreAppsView;

// Called when a more apps page has failed to come back from the server
- (void)didFailToLoadMoreApps;

// Called when the user dismisses the more apps view
// If you are displaying the add yourself, dismiss it now.
- (void)didDismissMoreApps:(UIView *)moreAppsView;

// Same as above, but only called when dismissed for a close
- (void)didCloseMoreApps:(UIView *)moreAppsView;

// Same as above, but only called when dismissed for a click
- (void)didClickMoreApps:(UIView *)moreAppsView;


@end
