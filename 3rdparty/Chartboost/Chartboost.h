//
//  Chartboost.h
//  Chartboost
//  3.1.1
//
//  Created by Kenneth Ballenegger on 8/1/11.
//  Copyright 2011 Chartboost. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>



@protocol ChartboostDelegate;


@interface Chartboost : NSObject

@property (retain) NSString *appId;
@property (retain) NSString *appSignature;

@property (assign) id <ChartboostDelegate> delegate;


// Extra configuration settings
// Timeout for requests (minimum is 10s, default is 30s)
@property NSUInteger timeout;
// Override the orientation (otherwise automatically detected from status bar)
@property UIInterfaceOrientation orientation;


// Get the singleton
+ (Chartboost *)sharedChartboost;

// Start the Chartboost session
- (void)startSession;

// Cache an interstitial, optionally takes a location argument
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

@end


@protocol ChartboostDelegate <NSObject>
@optional

// All of the delegate methods below are optional.
// Implement them only when you need to more finely control Chartboost's behavior.



// Called before requesting an interestitial from the back-end
- (BOOL)shouldRequestInterstitial:(NSString *)location;

// Called when an interstitial has been received, before it is presented on screen
// Return NO if showing an interstitial is currently innapropriate, for example if the user has entered the main game mode.
- (BOOL)shouldDisplayInterstitial:(NSString *)location;

// Called when an interstitial has been received and cached.
- (void)didCacheInterstitial:(NSString *)location;

// Called when an interstitial has failed to come back from the server
- (void)didFailToLoadInterstitial:(NSString *)location;

// Called when the user dismisses the interstitial
// If you are displaying the add yourself, dismiss it now.
- (void)didDismissInterstitial:(NSString *)location;

// Same as above, but only called when dismissed for a close
- (void)didCloseInterstitial:(NSString *)location;

// Same as above, but only called when dismissed for a click
- (void)didClickInterstitial:(NSString *)location;


// Called before requesting the more apps view from the back-end
// Return NO if when showing the loading view is not the desired user experience.
- (BOOL)shouldDisplayLoadingViewForMoreApps;

// Called when an more apps page has been received, before it is presented on screen
// Return NO if showing the more apps page is currently innapropriate
- (BOOL)shouldDisplayMoreApps;

// Called when the More Apps page has been received and cached
- (void)didCacheMoreApps;

// Called when a more apps page has failed to come back from the server
- (void)didFailToLoadMoreApps;

// Called when the user dismisses the more apps view
// If you are displaying the add yourself, dismiss it now.
- (void)didDismissMoreApps;

// Same as above, but only called when dismissed for a close
- (void)didCloseMoreApps;

// Same as above, but only called when dismissed for a click
- (void)didClickMoreApps;



// Whether Chartboost should show ads in the first session
// Defaults to YES
- (BOOL)shouldRequestInterstitialsInFirstSession;


@end


