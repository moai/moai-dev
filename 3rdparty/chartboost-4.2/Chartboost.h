//
//  Chartboost.h
//  Chartboost
//  4.2
//
//  Created by Kenneth Ballenegger on 8/1/11.
//  Copyright 2011 Chartboost. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef enum {
    CBLoadErrorInternal,
    CBLoadErrorInternetUnavailable,
    CBLoadErrorTooManyConnections, /**< Too many requests are pending for that location  */
    CBLoadErrorWrongOrientation,    /**< Interstitial loaded with wrong orientation */
    CBLoadErrorFirstSessionInterstitialsDisabled, /**< Interstitial disabled, first session */
    CBLoadErrorNetworkFailure,  /**< Network request failed */
    CBLoadErrorNoAdFound,  /**<  No ad received */
    CBLoadErrorSessionNotStarted, /**< Session not started, use startSession method */
    CBLoadErrorAgeGateFailure,  /**< User failed to pass the Age Gate */
} CBLoadError;

typedef enum : unsigned long long {
    CBLocationStartup = 1,
    CBLocationHomeScreen,
    CBLocationMainMenu,
    CBLocationGameScreen,
    CBLocationAchievements,
    CBLocationQuests,
    CBLocationPause,
    CBLocationLevelStart,
    CBLocationLevelComplete,
    CBLocationTurnComplete,
    CBLocationIAPStore,
    CBLocationItemStore,
    CBLocationGameOver,
    CBLocationLeaderBoard,
    CBLocationSettings,
    CBLocationQuit,
} CBLocation;

typedef void (^CBAgeGateConfirmation)(BOOL confirm);


@class CBAnalytics, CBStore, CBPostInstallAnalyticsTracker;


@protocol ChartboostDelegate;

@interface Chartboost : NSObject

@property (retain) NSString *appId;
@property (retain) NSString *appSignature;
@property (retain) NSString *appPublicKey;

@property (retain) UIView   *rootView;

@property (assign) id <ChartboostDelegate> delegate;

@property (assign) BOOL scansBundleForCachedImages;

// Extra configuration settings
// Timeout for requests (minimum is 10s, default is 30s)
@property NSUInteger timeout;
// Override the orientation (otherwise automatically detected from status bar)
@property UIInterfaceOrientation orientation;

/// Get the singleton
+ (Chartboost *)sharedChartboost;

/// Start the Chartboost session
- (void)startSession;

/// Cache an interstitial
- (void)cacheInterstitial;

/// Cache an interstitial taking a location argument
- (void)cacheInterstitial:(CBLocation)location;

/// Show an interstitial
- (void)showInterstitial;

/// Show an interstitial taking location and/or a view argument
- (void)showInterstitial:(CBLocation)location;

/// Implement this to check if an interstitial is stored in cache for the default location
- (BOOL)hasCachedInterstitial;

/// Implement this to check if an interstitial is stored in cache for a specific location
- (BOOL)hasCachedInterstitial:(CBLocation)location;

/// Cache the More Apps page
- (void)cacheMoreApps;

/// Show the More Apps page
- (void)showMoreApps;

/// Returns the device identifier for internal testing purposes
- (NSString *)deviceIdentifier;

/// Implement this to check if the more apps page is stored in the cache
- (BOOL)hasCachedMoreApps;

/// Dismiss any Chartboost view programatically
- (void)dismissChartboostView;

@end


@protocol ChartboostDelegate <NSObject>
@optional

// All of the delegate methods below are optional.
// Implement them only when you need to more finely control Chartboost's behavior.



/// Called before requesting an interestitial from the back-end
- (BOOL)shouldRequestInterstitial:(CBLocation)location;

/// Called when an interstitial has been received, before it is presented on screen
/// Return NO if showing an interstitial is currently innapropriate, for example if the user has entered the main game mode.
- (BOOL)shouldDisplayInterstitial:(CBLocation)location;

/// Called when an interstitial has been received and cached.
- (void)didCacheInterstitial:(CBLocation)location;

/// Called when an interstitial has failed to come back from the server
- (void)didFailToLoadInterstitial:(CBLocation)location  withError:(CBLoadError)error;

/// Called when the user dismisses the interstitial
/// If you are displaying the add yourself, dismiss it now.
- (void)didDismissInterstitial:(CBLocation)location;

/// Same as above, but only called when dismissed for a close
- (void)didCloseInterstitial:(CBLocation)location;

/// Same as above, but only called when dismissed for a click
- (void)didClickInterstitial:(CBLocation)location;

/// Called when the App Store sheet is dismissed, when displaying the embedded app sheet.
- (void)didCompleteAppStoreSheetFlow;


/// Called before requesting the more apps view from the back-end
/// Return NO if when showing the loading view is not the desired user experience.
- (BOOL)shouldDisplayLoadingViewForMoreApps;

/// Called when an more apps page has been received, before it is presented on screen
/// Return NO if showing the more apps page is currently innapropriate
- (BOOL)shouldDisplayMoreApps;

/// Called when the More Apps page has been received and cached
- (void)didCacheMoreApps;

/// Called when a more apps page has failed to come back from the server
- (void)didFailToLoadMoreApps:(CBLoadError)error;

/// Called when the user dismisses the more apps view
/// If you are displaying the add yourself, dismiss it now.
- (void)didDismissMoreApps;

/// Same as above, but only called when dismissed for a close
- (void)didCloseMoreApps;

/// Same as above, but only called when dismissed for a click
- (void)didClickMoreApps;


/// Whether Chartboost should show ads in the first session
/// Defaults to YES
- (BOOL)shouldRequestInterstitialsInFirstSession;

/// Implement this method to control the blocking for an age gate.
/// If the method returns NO, the callback should not be used
- (BOOL)shouldPauseClickForConfirmation:(CBAgeGateConfirmation)callback;

@end


