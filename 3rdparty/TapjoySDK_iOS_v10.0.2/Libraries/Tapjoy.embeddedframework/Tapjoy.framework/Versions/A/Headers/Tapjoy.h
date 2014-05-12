// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

//The Tapjoy iOS SDK.

#ifndef _TAPJOY_H
#define _TAPJOY_H

#import <UIKit/UIKit.h>
#import "TapjoyConnectConstants.h"
#import "TJEvent.h"


#define TJC_DEPRECATION_WARNING(VERSION) __attribute__((deprecated("Go to tech.tapjoy.com for instructions on how to fix this warning")))
#define TJ_DEPRECATED_CLASS     __attribute__((deprecated("TapjoyConnect Class is deprecated, use Tapjoy Class")))
#define TJC_HIGHEST_UNSUPPORTED_SYSTEM_VERISON	@"4.3.5"

typedef void (^tapPointsCompletion)(NSDictionary *parameters, NSError *error);

@interface TJCAdView : UIView <UIWebViewDelegate>
@end

@class TJCAdManager;
@class TJCEventTrackingManager;
@class TJCFullScreenAdManager;
@class TJCOffersManager;
@class TJCUserAccountManager;
@class TJCVideoManager;
@class TJCViewHandler;
@class TJCNetReachability;
@class TJCUtil;
@class TJCLog;
@class TJEvent;

/**	
 * The Tapjoy Connect Main class. This class provides all publicly available methods for developers to integrate Tapjoy into their applications. 
 */
@interface Tapjoy :  NSObject

/** The application ID unique to this app. */
@property (nonatomic, copy) NSString *appID;

/** The Tapjoy secret key for this applicaiton. */
@property (nonatomic, copy) NSString *secretKey;

/** The user ID, a custom ID set by the developer of an app to keep track of its unique users. */
@property (nonatomic, copy) NSString *userID;

/** The name of the plugin used. If no plugin is used, this value is set to "native" by default. */
@property (nonatomic, copy) NSString *plugin;

/** The currency multiplier value, used to adjust currency earned. */
@property (nonatomic, assign) float currencyMultiplier;	


@property (nonatomic, strong) TJCAdManager *displayAdManager;
@property (nonatomic, strong) TJCEventTrackingManager *eventTrackingManager;
@property (nonatomic, strong) TJCFullScreenAdManager *fullScreenAdManager;
@property (nonatomic, strong) TJCOffersManager *offersManager;
@property (nonatomic, strong) TJCUserAccountManager *userAccountManager;
@property (nonatomic, strong) TJCVideoManager *videoManager;
@property (nonatomic, strong) TJCViewHandler *viewHandler;
@property (nonatomic, strong) TJCUtil *util;
@property (nonatomic, strong) TJCLog *log;


/** 
 * This method is called to initialize the Tapjoy system and notify the server that this device is running your application.
 *
 * This method should be called upon app delegate initialization in the applicationDidFinishLaunching method.
 *
 * @param appID The application ID. Retrieved from the app dashboard in your Tapjoy account.
 * @param secretKey The application secret key. Retrieved from the app dashboard in your Tapjoy account.
 * @return n/a
 */
+ (void)requestTapjoyConnect:(NSString*)appID secretKey:(NSString*)secretKey;

/**
 * This method is called to initialize the Tapjoy system and notify the server that this device is running your application.
 *
 * This method should be called upon app delegate initialization in the applicationDidFinishLaunching method.
 *
 * @param appID The application ID. Retrieved from the app dashboard in your Tapjoy account.
 * @param secretKey The application secret key. Retrieved from the app dashboard in your Tapjoy account.
 * @param optionsDict NSDictionary of special flags to enable non-standard settings
 * @return n/a
 */
+ (void)requestTapjoyConnect:(NSString*)appID secretKey:(NSString*)secretKey options:(NSDictionary*)optionsDict;


/**
 * Informs the Tapjoy server that the specified Pay-Per-Action was completed. Should be called whenever a user completes an in-game action.
 *
 * @param actionID The action ID of the completed action
 * @return n/a
 */
+ (void)actionComplete:(NSString*)actionID;

/**	
 * Retrieves the globally accessible Tapjoy singleton object.
 *
 * @return The globally accessible Tapjoy singleton object.
 */
+ (id)sharedTapjoyConnect;

/**
 * Retrieves the Tapjoy app ID.
 * 
 * @return The Tapjoy app ID passed into requestTapjoyConnect
 */
+ (NSString*)getAppID TJC_DEPRECATION_WARNING(10.0);

/**
 * Assigns a user ID for this user/device. This is used to identify the user in your application
 *
 * @param theUserID The user ID you wish to assign to this device.
 * @return n/a
 */
+ (void)setUserID:(NSString*)theUserID;

/**
 * Gets the user ID assigned to this device.
 *
 * @return The Tapjoy user ID.
 */
+ (NSString*)getUserID TJC_DEPRECATION_WARNING(10.0);

/**
 * Retrieves the secret key.
 *
 * @return The Tapjoy secret key for this application.
 */
+ (NSString*)getSecretKey TJC_DEPRECATION_WARNING(10.0);

/**
 * Sets the currency multiplier for virtual currency to be earned. The default is 1.0.
 *
 * Only used for non-managed (by Tapjoy) currency.
 * 
 * @param mult The currency multiplier.
 * @return n/a
 */
+ (void)setCurrencyMultiplier:(float)mult;

/**
 * Gets the currency multiplier for virtual currency to be earned.
 *
 * @return The currency multiplier value.
 */
+ (float)getCurrencyMultiplier;

/**
 * Toggle logging to the console.
 *
 * @param enable YES to enable logging, NO otherwise.
 * @return n/a
 */
+ (void)enableLogging:(BOOL)enable;

/**
 * Returns the SDK version.
 *
 * @return The Tapjoy SDK version.
 */
+ (NSString*)getVersion;

/**
 * Dismisses both offer wall and fullscreen ads.
 *
 * @return n/a
 */
+ (void)dismissContent;

/**
 * Sets a dictionary of segmentation parameters for this user
 *
 * @param A dictionary of parameters. See http://tech.tapjoy.com for information about valid segment names to use
 * @return n/a
 */
+ (void)sendSegmentationParams:(NSDictionary*)params;

@end



/**
 * The Tapjoy Ad Delegate Protocol.
 */
@protocol TJCAdDelegate <NSObject>

@required

/**
 * This method is invoked when data for a Tapjoy Ad has successfully been retrieved.
 *
 * Once this method has been invoked, the Ad view can safely be set visible and on top of the view stack.
 * @param adView The Tapjoy Ad view.
 * @return n/a
 */
- (void) didReceiveAd:(TJCAdView*)adView;

/**
 * When this is invoked, it most likely means that this app does not have any Tapjoy Ads associated with it.
 *
 * @param msg A string containing a description of the fail error.
 * @return n/a
 */
- (void) didFailWithMessage:(NSString*)msg;

/**
 * The request for the Ad requires a content size parameter, which needs to be provided here.
 *
 * @return Must return one of TJC_DISPLAY_AD_SIZE_320X50 or TJC_DISPLAY_AD_SIZE_640X100 or TJC_DISPLAY_AD_SIZE_768x90.
 */
- (NSString*) adContentSize;

/**
 * This method will indicate whether to refresh the Ad periodically. When this method returns YES, Ads are automatically refreshed every 15 seconds.
 *
 * @return YES will indicate that Ads should be automatically refreshed every 15 seconds, NO for no refreshing.
 */
- (BOOL) shouldRefreshAd;

@end

@protocol TJCVideoAdDelegate <NSObject>

@optional

- (void)videoAdBegan;

- (void)videoAdClosed;

- (void)videoAdCompleted;

- (void)videoAdError:(NSString*)errorMsg;

@end


/**
 * The Tapjoy View Delegate Protocol.
 */
@protocol TJCViewDelegate <NSObject>

@optional

/**
 * Called when a Tapjoy view will appear.
 *
 * @param viewType The type of view that will appear. Refer to TJCViewTypeEnum for view types.
 * @return n/a
 */
- (void)viewWillAppearWithType:(int)viewType;

/**
 * Called when a Tapjoy view did appear.
 *
 * @param viewType The type of view that did appear. Refer to TJCViewTypeEnum for view types.
 * @return n/a
 */
- (void)viewDidAppearWithType:(int)viewType;

/**
 * Called when a Tapjoy view will disappear.
 *
 * @param viewType The type of view that will disappear. Refer to TJCViewTypeEnum for view types.
 * @return n/a
 */
- (void)viewWillDisappearWithType:(int)viewType;

/**
 * Called when a Tapjoy view did disappear.
 *
 * @param viewType The type of view that did disappear. Refer to TJCViewTypeEnum for view types.
 * @return n/a
 */
- (void)viewDidDisappearWithType:(int)viewType;

@end


@interface Tapjoy (TJCAdManager)

/**
 * Initiates a URL request to get the Tapjoy Ad data.
 *
 * @param deleg The class instance that implements the TJCAdDelegate protocol.
 * @return The TJCAdView object.
 */
+ (id) getDisplayAdWithDelegate:(id <TJCAdDelegate>)deleg;

/**
 * Initiates a URL request to get the Tapjoy Ad data.
 *
 * @param deleg The class instance that implements the TJCAdDelegate protocol.
 * @param currencyID The currency ID specifies which currency to advertise in the display ad.
 * @return the TJCAdView object.
 */
+ (id) getDisplayAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)currencyID;

/**
 * Indicates whether ad data has been successfully loaded.
 *
 * @return YES to indicate that the ad data has been successfully loaded, NO otherwise.
 */
+ (BOOL) isDisplayAdLoaded;

/**
 * The Tapjoy Display Ad UIView returned by this method should be added to the current superview after an Ad has been successfully loaded.
 *
 * @return The Tapjoy Display Ad UIView.
 */
+ (TJCAdView*) getDisplayAdView;

@end


@interface Tapjoy (TJCEventTrackingManager)

/**
 * Initiates the request to POST the IAP data.
 *
 * @param name The name of the In-App-Purchase (IAP) item that this event should track.
 * @param price The amount that the item was sold for.
 * @param quantity The number of items for this purchase.
 * @param currencyCode The currency code, such as USD.
 * @return n/a
 */
+ (void)sendIAPEvent:(NSString*)name price:(float)price quantity:(int)quantity currencyCode:(NSString*)currencyCode;

@end


@interface Tapjoy (TJCFullScreenAdManager)

/**
 * Requests for Featured App are notified via the TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION notification.
 *
 * @warning This method is deprecated.
 * @return n/a
 */
+ (void)getFullScreenAd TJC_DEPRECATION_WARNING(10.0);

/**
 * Requests for Featured App are notified via the TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION notification.
 *
 * @warning This method is deprecated.
 * @param currencyID The GUID for the currency earned when completing this full screen ad.
 * @return n/a
 */
+ (void)getFullScreenAdWithCurrencyID:(NSString*)currencyID TJC_DEPRECATION_WARNING(10.0);

/**
 * Wrapper method for displaying the Full Screen Featured App Ad.
 *
 * Places the Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 * 
 * @warning This method is deprecated.
 * @return The TJCUIWebPageAdView object.
 */
+ (UIView*)showFullScreenAd TJC_DEPRECATION_WARNING(10.0);

/**
 * Wrapper method for displaying the Full Screen Featured App Ad.
 *
 * Places the Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *
 * @warning This method is deprecated.
 * @param vController Reference to the application's view Controller. (Note: It must not be nil or an invalid object).
 * @return n/a
 */
+ (void)showFullScreenAdWithViewController:(UIViewController*)vController TJC_DEPRECATION_WARNING(10.0);

/**
 * Sets the delay count for full screen ad.
 *
 * @warning This method is deprecated.
 * @param delayCount The number of times that the app must run (since initial app install) before a full screen ad can be displayed.
 * @return n/a
 */
+ (void)setFullScreenAdDelayCount:(int)delayCount TJC_DEPRECATION_WARNING(10.0);

@end


@interface Tapjoy (TJCOffersManager)

/**
 * Allocates and initializes a TJCOffersWebView.
 *
 * @return The TJCOffersWebView UIView object.
 */
+ (UIView*)showOffers;

/**
 * Allocates and initializes a TJCOffersWebView.
 *
 * @param vController The UIViewController to set as TJCOffersWebView's parentVController_.
 * @return n/a
 */
+ (void)showOffersWithViewController:(UIViewController*)vController;

/**
 * Allocates and initializes a TJCOffersWebView. This is only used when multiple currencies are enabled.
 *
 * @param currencyID The id of the currency to show in the offer wall.
 * @param isSelectorVisible Specifies whether to display the currency selector in the offer wall.
 * @return The TJCOffersWebView UIView object.
 */
+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible;

/**
 * Allocates and initializes a TJCOffersWebView. This is only used when multiple currencies are enabled.
 *
 * @param vController The UIViewController to set as TJCOffersWebView's parentVController_.
 * @param currencyID The id of the currency to show in the offer wall.
 * @param isSelectorVisible Specifies whether to display the currency selector in the offer wall.
 * @return n/a
 */
+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withCurrencySelector:(BOOL)isSelectorVisible;

@end


@interface Tapjoy (TJCUserAccountManager)

/**
 * Requests for Tap Points (Virtual Currency) notify via TJC_TAP_POINTS_RESPONSE_NOTIFICATION notification.
 *
 * @return n/a
 */
+ (void)getTapPoints;

/**
 * Requests for Tap Points (Virtual Currency).
 *
 * @param completion The completion block that is invoked after a response is received from the server.
 * @return n/a
 */
+ (void)getTapPointsWithCompletion:(tapPointsCompletion)completion;

/**
 * Updates the virtual currency for the user with the given spent amount of currency.
 *
 * If the spent amount exceeds the current amount of currency the user has, nothing will happen.
 * @param points The amount of currency to subtract from the current total amount of currency the user has.
 * @return n/a
 */
+ (void)spendTapPoints:(int)points;

/**
 * Updates the virtual currency for the user with the given spent amount of currency.
 *
 * If the spent amount exceeds the current amount of currency the user has, nothing will happen.
 * @param points The amount of currency to subtract from the current total amount of currency the user has.
 * @param completion The completion block that is invoked after a response is received from the server.
 * @return n/a
 */
+ (void)spendTapPoints:(int)points completion:(tapPointsCompletion)completion;

/**
 * Updates the virtual currency for the user with the given awarded amount of currency.
 *
 * @param points The amount of currency to add to the current total amount of currency the user has.
 * @return n/a
 */
+ (void)awardTapPoints:(int)points;

/**
 * Updates the virtual currency for the user with the given awarded amount of currency.
 *
 * @param points The amount of currency to add to the current total amount of currency the user has.
 * @param completion The completion block that is invoked after a response is received from the server.
 * @return n/a
 */
+ (void)awardTapPoints:(int)points completion:(tapPointsCompletion)completion;

/**
 * Shows a UIAlert that tells the user how much currency they just earned.
 *
 * @return n/a
 */
+ (void)showDefaultEarnedCurrencyAlert;

@end


@interface Tapjoy (TJCVideoViewHandler)

/**
 * Sets the class that implements the TJCVideoAdDelegate protocol.
 *
 * @param delegate The class that implements the TJCVideoAdDelegate protocol.
 * @return n/a
 */
+ (void)setVideoAdDelegate:(id<TJCVideoAdDelegate>)delegate;

/**
 * Begins the caching process if auto caching is disabled.
 *
 * @param delegate The class that implements the TJCVideoAdDelegate protocol.
 * @return n/a
 */
+ (void)cacheVideosWithDelegate:(id<TJCVideoAdDelegate>)delegate TJC_DEPRECATION_WARNING(10.0);

/**
 * Sets the cache count for video ads.
 *
 * @param count The number of videos to cache to device locally.
 * @return n/a
 */
+ (void)setVideoCacheCount:(int)count TJC_DEPRECATION_WARNING(10.0);

/**
 * Set the visibility of the Tapjoy logo during video playback.
 *
 * @param display Boolean that indicates whether the Tapjoy logo should be displayed or not during video playback.
 * @return n/a
 */
+ (void)shouldDisplayVideoLogo:(BOOL)display TJC_DEPRECATION_WARNING(10.0);

/**
 * Whether the videos should be disabled on this device during runtime. If set to true, no videos will be
 * cached nor displayed on the offerwall for streaming (wi-fi only)
 *
 * @param shouldDisable Boolean that indicates whether the Tapjoy videos should be disabled on this device.
 * @return n/a
 */
+ (void)disableVideo:(BOOL)shouldDisable TJC_DEPRECATION_WARNING(10.0);

@end


@interface Tapjoy (TJCViewHandler)

/**
 * Sets a transition effect for the Tapjoy UIViews.
 *
 * @param transitionEffect The transition effect to set, enumerated by TJCTransitionEnum.
 * @return n/a
 */
+ (void)setTransitionEffect:(TJCTransitionEnum)transitionEffect TJC_DEPRECATION_WARNING(10.0);

/**
 * Gets the transition delay.
 *
 * @return The transition delay.
 */
+ (float)getTransitionDelay TJC_DEPRECATION_WARNING(10.0);

/**
 * Gets the currently set transition effect enumerated value TJCTransitionEnum.
 *
 * @return The currently set transition effect.
 */
+ (TJCTransitionEnum)getCurrentTransitionEffect TJC_DEPRECATION_WARNING(10.0);

/**
 * Gets the currently set reverse transition effect enumerated value TJCTransitionEnum.
 *
 * @return The currently set reverse transition effect.
 */
+ (TJCTransitionEnum)getReverseTransitionEffect TJC_DEPRECATION_WARNING(10.0);

/**
 * Sets the delegate that conforms to the TJCViewDelegate protocol to receive Tapjoy view action callbacks.
 *
 * @param delegate The TJCViewDelegate object.
 * @return n/a
 */
+ (void)setViewDelegate:(id<TJCViewDelegate>)delegate;

@end

@interface TapjoyConnect : NSObject

+ (void)requestTapjoyConnect:(NSString*)appID secretKey:(NSString*)secretKey TJ_DEPRECATED_CLASS;
+ (void)requestTapjoyConnect:(NSString*)appID secretKey:(NSString*)secretKey options:(NSDictionary*)optionsDict TJ_DEPRECATED_CLASS;
+ (void)actionComplete:(NSString*)actionID TJ_DEPRECATED_CLASS;
+ (id)sharedTapjoyConnect TJ_DEPRECATED_CLASS;
+ (NSString*)getAppID TJ_DEPRECATED_CLASS;
+ (void)setUserID:(NSString*)theUserID TJ_DEPRECATED_CLASS;
+ (NSString*)getUserID TJ_DEPRECATED_CLASS;
+ (NSString*)getSecretKey TJ_DEPRECATED_CLASS;
+ (void)setCurrencyMultiplier:(float)mult TJ_DEPRECATED_CLASS;
+ (float)getCurrencyMultiplier TJ_DEPRECATED_CLASS;
+ (void)enableLogging:(BOOL)enable TJ_DEPRECATED_CLASS;

+ (id) getDisplayAdWithDelegate:(id <TJCAdDelegate>)deleg TJ_DEPRECATED_CLASS;
+ (id) getDisplayAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)currencyID TJ_DEPRECATED_CLASS;
+ (BOOL) isDisplayAdLoaded TJ_DEPRECATED_CLASS;
+ (TJCAdView*) getDisplayAdView TJ_DEPRECATED_CLASS;
#pragma mark Event Methods
+ (void)sendIAPEvent:(NSString*)name price:(float)price quantity:(int)quantity currencyCode:(NSString*)currencyCode TJ_DEPRECATED_CLASS;
#pragma mark Full Screen Ad Methods
+ (void)getFullScreenAd TJ_DEPRECATED_CLASS;
+ (void)getFullScreenAdWithCurrencyID:(NSString*)currencyID TJ_DEPRECATED_CLASS;
+ (UIView*)showFullScreenAd TJ_DEPRECATED_CLASS;
+ (void)showFullScreenAdWithViewController:(UIViewController*)vController TJ_DEPRECATED_CLASS;
+ (void)setFullScreenAdDelayCount:(int)delayCount TJ_DEPRECATED_CLASS;
#pragma mark Offers Methods
+ (UIView*)showOffers TJ_DEPRECATED_CLASS;
+ (void)showOffersWithViewController:(UIViewController *)vController TJ_DEPRECATED_CLASS;
+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible TJ_DEPRECATED_CLASS;
+ (void)showOffersWithCurrencyID:(NSString*)currencyID
withViewController:(UIViewController*)vController
withCurrencySelector:(BOOL)isSelectorVisible TJ_DEPRECATED_CLASS;
#pragma mark User Account Methods
+ (void)getTapPoints TJ_DEPRECATED_CLASS;
+ (void)spendTapPoints:(int)points TJ_DEPRECATED_CLASS;
+ (void)awardTapPoints:(int)points TJ_DEPRECATED_CLASS;
+ (void)showDefaultEarnedCurrencyAlert TJ_DEPRECATED_CLASS;
#pragma mark Video Methods
+ (void)setVideoAdDelegate:(id<TJCVideoAdDelegate>)delegate TJ_DEPRECATED_CLASS;
+ (void)cacheVideosWithDelegate:(id<TJCVideoAdDelegate>)delegate TJ_DEPRECATED_CLASS;
+ (void)setVideoCacheCount:(int)count TJ_DEPRECATED_CLASS;
+ (void)shouldDisplayVideoLogo:(BOOL)display TJ_DEPRECATED_CLASS;
+ (void)disableVideo:(BOOL)shouldDisable TJ_DEPRECATED_CLASS;
#pragma mark View Methods
+ (void)setTransitionEffect:(TJCTransitionEnum) transitionEffect TJ_DEPRECATED_CLASS;
+ (float)getTransitionDelay TJ_DEPRECATED_CLASS;
+ (TJCTransitionEnum)getCurrentTransitionEffect TJ_DEPRECATED_CLASS;
+ (TJCTransitionEnum)getReverseTransitionEffect TJ_DEPRECATED_CLASS;
+ (void)setViewDelegate:(id<TJCViewDelegate>)delegate TJ_DEPRECATED_CLASS;

@end

#endif
