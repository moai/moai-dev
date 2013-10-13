// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

//The Tapjoy iOS SDK.


#import <UIKit/UIKit.h>
#import "TapjoyConnectConstants.h"
#import "TJEvent.h"


#define TJC_DEPRECATION_WARNING __attribute__((deprecated("Go to knowledge.tapjoy.com for instructions on how to fix this warning")))
#define TJ_DEPRECATED_CLASS     __attribute__((deprecated("TapjoyConnect Class is deprecated, use Tapjoy Class")))

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
@class TJCPushManager;
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
@property (nonatomic, strong) TJCPushManager *pushManager;
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
+ (NSString*)getAppID;

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
+ (NSString*)getUserID;

/**
 * Retrieves the secret key.
 *
 * @return The Tapjoy secret key for this application.
 */
+ (NSString*)getSecretKey;

/**
 * Sets the plugin.
 *
 * The plugin defaults to "native".
 *
 * @param thePlugin The name of the plugin.
 * @return n/a
 */
+ (void)setPlugin:(NSString*)thePlugin;

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

/**
 * The Tapjoy Video Ad Delegate Protocol.
 */
@protocol TJCVideoAdDelegate <NSObject>

@optional

/**
 * Called when the video ad begins playing.
 *
 * @return n/a
 */
- (void)videoAdBegan;

/**
 * Called when the video ad is closed.
 *
 * @return n/a
 */
- (void)videoAdClosed;

/**
 * Called when an error occurs during video loading or playback.
 *
 * @param errorMsg A string containing a description of the fail error
 * @return n/a
 */
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
 * Attempts to load a new Ad.
 *
 * @warning This method is deprecated.
 * @return n/a
 */
+ (void) refreshDisplayAd TJC_DEPRECATION_WARNING;

/**
 * Attempts to load a new Ad.
 *
 * @warning This method is deprecated.
 * @param currencyID The currency ID specifies which currency to advertise in the display ad.
 * @return n/a
 */
+ (void) refreshDisplayAdWithCurrencyID:(NSString*)currencyID TJC_DEPRECATION_WARNING;

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

+ (void)sendIAPEventWithName:(NSString*)name price:(float)price quantity:(int)quantity currencyCode:(NSString*)currencyCode TJC_DEPRECATION_WARNING;

@end


@interface Tapjoy (TJCFullScreenAdManager)

/**
 * Requests for Featured App are notified via the TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION notification.
 *
 * @warning This method is deprecated.
 * @return n/a
 */
+ (void)getFullScreenAd;

/**
 * Requests for Featured App are notified via the TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION notification.
 *
 * @warning This method is deprecated.
 * @param currencyID The GUID for the currency earned when completing this full screen ad.
 * @return n/a
 */
+ (void)getFullScreenAdWithCurrencyID:(NSString*)currencyID;

/**
 * Wrapper method for displaying the Full Screen Featured App Ad.
 *
 * Places the Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 * 
 * @warning This method is deprecated.
 * @return The TJCUIWebPageAdView object.
 */
+ (UIView*)showFullScreenAd;

/**
 * Wrapper method for displaying the Full Screen Featured App Ad.
 *
 * Places the Full Screen Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *
 * @warning This method is deprecated.
 * @param vController Reference to the application's view Controller. (Note: It must not be nil or an invalid object).
 * @return n/a
 */
+ (void)showFullScreenAdWithViewController:(UIViewController*)vController;

/**
 * Sets the delay count for full screen ad.
 *
 * @warning This method is deprecated.
 * @param delayCount The number of times that the app must run (since initial app install) before a full screen ad can be displayed.
 * @return n/a
 */
+ (void)setFullScreenAdDelayCount:(int)delayCount;

+ (void)getFeaturedApp TJC_DEPRECATION_WARNING;
+ (void)getFeaturedAppWithCurrencyID:(NSString*)currencyID TJC_DEPRECATION_WARNING;
+ (UIView*)showFeaturedAppFullScreenAd TJC_DEPRECATION_WARNING;
+ (void)showFeaturedAppFullScreenAdWithViewController:(UIViewController*)vController TJC_DEPRECATION_WARNING;
+ (void)setFeaturedAppDisplayCount:(int) displayCount TJC_DEPRECATION_WARNING;
+ (void)setFeaturedAppDelayCount:(int)delayCount TJC_DEPRECATION_WARNING;

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

/**
 * Allocates and initializes a TJCOffersWebView.
 *
 * @warning This method is deprecated.
 * @param vController The UIViewController to set as TJCOffersWebView's parentVController_.
 * @param visible TRUE makes the navigation bar visible, FALSE makes it not visible.
 * @return n/a
 */
+ (void)showOffersWithViewController:(UIViewController*)vController withInternalNavBar:(BOOL)visible TJC_DEPRECATION_WARNING;

/** 
 * Allocates and initializes a TJCOffersWebView. This is only used when multiple currencies are enabled.
 *
 * @warning This method is deprecated.
 * @param vController The UIViewController to set as TJCOffersWebView's parentVController_.
 * @param visible TRUE makes the navigation bar visible, FALSE makes it not visible.
 * @param currencyID The id of the currency to show in the offer wall.
 * @param isSelectorVisible Specifies whether to display the currency selector in the offer wall.
 * @return n/a
 */
+ (void)showOffersWithCurrencyID:(NSString*)currencyID
withViewController:(UIViewController*)vController
withInternalNavBar:(BOOL)visible
withCurrencySelector:(BOOL)isSelectorVisible TJC_DEPRECATION_WARNING;

/**
 * Sets the navigation bar background image.
 *
 * @warning This method is deprecated.
 * @param image The UIImage to set the offer wall's navigation bar's background image to.
 * @return n/a
 */
+ (void)setOffersNavBarImage:(UIImage*)image TJC_DEPRECATION_WARNING;

@end


@interface Tapjoy (TJCDailyRewardManager)

/**
 * Requests for daily reward are notified via the TJC_DAILY_REWARD_RESPONSE_NOTIFICATION notification.
 *
 * @warning This method is deprecated.
 * @return n/a
 */
+ (void)getDailyRewardAd TJC_DEPRECATION_WARNING;

/**
 * Requests for Daily reward are notified via the TJC_DAILY_REWARD_RESPONSE_NOTIFICATION notification.
 *
 * @warning This method is deprecated.
 * @param currencyID The GUID for the currency earned when completing this full screen ad.
 * @return n/a
 */
+ (void)getDailyRewardAdWithCurrencyID:(NSString*)currencyID TJC_DEPRECATION_WARNING;

/**
 * Wrapper method for displaying the Daily reward.
 *
 * Places the Daily reward UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 *
 * @warning This method is deprecated.
 * @return The TJCUIWebPageAdView object.
 */
+ (UIView*)showDailyRewardAd TJC_DEPRECATION_WARNING;

/**
 * Wrapper method for displaying the daily reward Ad.
 *
 * Places the daily reward Ad UIView on the top of the view stack and animates it into view if an animation transition effect is set.
 * @warning This method is deprecated.
 * @param vController Reference to the application's view Controller. (Note: It must not be nil or an invalid object).
 * @return n/a
 */
+ (void)showDailyRewardAdWithViewController:(UIViewController*)vController TJC_DEPRECATION_WARNING;

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
 * Initializes video ads.
 *
 * @warning This method is deprecated.
 * @param delegate The class that implements the TJCVideoAdDelegate protocol.
 * @return n/a
 */
+ (void)initVideoAdWithDelegate:(id<TJCVideoAdDelegate>)delegate TJC_DEPRECATION_WARNING;

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
+ (void)cacheVideosWithDelegate:(id<TJCVideoAdDelegate>)delegate;

/**
 * Sets the cache count for video ads.
 *
 * @param count The number of videos to cache to device locally.
 * @return n/a
 */
+ (void)setVideoCacheCount:(int)count;

/**
 * Set the visibility of the Tapjoy logo during video playback.
 *
 * @param display Boolean that indicates whether the Tapjoy logo should be displayed or not during video playback.
 * @return n/a
 */
+ (void)shouldDisplayVideoLogo:(BOOL)display;

/**
 * Whether the videos should be disabled on this device during runtime. If set to true, no videos will be
 * cached nor displayed on the offerwall for streaming (wi-fi only)
 *
 * @param shouldDisable Boolean that indicates whether the Tapjoy videos should be disabled on this device.
 * @return n/a
 */
+ (void)disableVideo:(BOOL)shouldDisable;

@end


@interface Tapjoy (TJCViewHandler)

/**
 * Sets a transition effect for the Tapjoy UIViews.
 *
 * @param transitionEffect The transition effect to set, enumerated by TJCTransitionEnum.
 * @return n/a
 */
+ (void)setTransitionEffect:(TJCTransitionEnum)transitionEffect;

/**
 * Gets the transition delay.
 *
 * @return The transition delay.
 */
+ (float)getTransitionDelay;

/**
 * Gets the currently set transition effect enumerated value TJCTransitionEnum.
 *
 * @return The currently set transition effect.
 */
+ (TJCTransitionEnum)getCurrentTransitionEffect;

/**
 * Gets the currently set reverse transition effect enumerated value TJCTransitionEnum.
 *
 * @return The currently set reverse transition effect.
 */
+ (TJCTransitionEnum)getReverseTransitionEffect;

/**
 * Sets the delegate that conforms to the TJCViewDelegate protocol to receive Tapjoy view action callbacks.
 *
 * @param delegate The TJCViewDelegate object.
 * @return n/a
 */
+ (void)setViewDelegate:(id<TJCViewDelegate>)delegate;

@end


@interface Tapjoy (TJCPushManager)

+ (NSString*)pushToken;

+ (void)registerForRemoteNotifications;

+ (void)registerForRemoteNotificationsWithAlias:(NSString*)pushAlias;

+ (void)registerForRemoteNotificationsWithAlias:(NSString*)pushAlias tags:(NSArray*)tags;

+ (void)didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)token;

+ (void)didFailToRegisterForRemoteNotificationsWithError:(NSError*)error;

+ (void)didReceiveRemoteNotification:(NSDictionary *)notification;

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
+ (void)setPlugin:(NSString*)thePlugin TJ_DEPRECATED_CLASS;
+ (void)setCurrencyMultiplier:(float)mult TJ_DEPRECATED_CLASS;
+ (float)getCurrencyMultiplier TJ_DEPRECATED_CLASS;
+ (void)enableLogging:(BOOL)enable TJ_DEPRECATED_CLASS;

+ (id) getDisplayAdWithDelegate:(id <TJCAdDelegate>)deleg TJ_DEPRECATED_CLASS;
+ (id) getDisplayAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)currencyID TJ_DEPRECATED_CLASS;
+ (void) refreshDisplayAd TJ_DEPRECATED_CLASS;
+ (void) refreshDisplayAdWithCurrencyID:(NSString*)currencyID TJ_DEPRECATED_CLASS;
+ (BOOL) isDisplayAdLoaded TJ_DEPRECATED_CLASS;
+ (TJCAdView*) getDisplayAdView TJ_DEPRECATED_CLASS;
#pragma mark Event Methods
+ (void)sendIAPEvent:(NSString*)name price:(float)price quantity:(int)quantity currencyCode:(NSString*)currencyCode TJ_DEPRECATED_CLASS;
+ (void)sendIAPEventWithName:(NSString*)name price:(float)price quantity:(int)quantity currencyCode:(NSString*)currencyCode TJ_DEPRECATED_CLASS;
#pragma mark Full Screen Ad/Daily Reward Ad Methods
+ (void)getFullScreenAd TJ_DEPRECATED_CLASS;
+ (void)getFullScreenAdWithCurrencyID:(NSString*)currencyID TJ_DEPRECATED_CLASS;
+ (UIView*)showFullScreenAd TJ_DEPRECATED_CLASS;
+ (void)showFullScreenAdWithViewController:(UIViewController*)vController TJ_DEPRECATED_CLASS;
+ (void)setFullScreenAdDelayCount:(int)delayCount TJ_DEPRECATED_CLASS;
+ (void)getDailyRewardAd TJ_DEPRECATED_CLASS;
+ (void)getDailyRewardAdWithCurrencyID:(NSString*)currencyID TJ_DEPRECATED_CLASS;
+ (UIView*)showDailyRewardAd TJ_DEPRECATED_CLASS;
+ (void)showDailyRewardAdWithViewController:(UIViewController*)vController TJ_DEPRECATED_CLASS;
+ (void)getFeaturedApp TJ_DEPRECATED_CLASS;
+ (void)getFeaturedAppWithCurrencyID:(NSString*)currencyID TJ_DEPRECATED_CLASS;
+ (UIView*)showFeaturedAppFullScreenAd TJ_DEPRECATED_CLASS;
+ (void)showFeaturedAppFullScreenAdWithViewController:(UIViewController*)vController TJ_DEPRECATED_CLASS;
+ (void)setFeaturedAppDisplayCount:(int) displayCount TJ_DEPRECATED_CLASS;
+ (void)setFeaturedAppDelayCount:(int)delayCount TJ_DEPRECATED_CLASS;
#pragma mark Offers Methods
+ (UIView*)showOffers TJ_DEPRECATED_CLASS;
+ (void)showOffersWithViewController:(UIViewController *)vController TJ_DEPRECATED_CLASS;
+ (void)showOffersWithViewController:(UIViewController *)vController withInternalNavBar:(BOOL)visible TJ_DEPRECATED_CLASS;
+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible TJ_DEPRECATED_CLASS;
+ (void)showOffersWithCurrencyID:(NSString*)currencyID
withViewController:(UIViewController*)vController
withCurrencySelector:(BOOL)isSelectorVisible TJ_DEPRECATED_CLASS;
+ (void)showOffersWithCurrencyID:(NSString*)currencyID
withViewController:(UIViewController*)vController
withInternalNavBar:(BOOL)visible
withCurrencySelector:(BOOL)isSelectorVisible TJ_DEPRECATED_CLASS;
+ (void)setOffersNavBarImage:(UIImage*)image TJ_DEPRECATED_CLASS;
#pragma mark Push Methods
+ (NSString*)pushToken TJ_DEPRECATED_CLASS;
+ (void)registerForRemoteNotifications TJ_DEPRECATED_CLASS;
+ (void)registerForRemoteNotificationsWithAlias:(NSString*)alias TJ_DEPRECATED_CLASS;
+ (void)registerForRemoteNotificationsWithAlias:(NSString*)alias tags:(NSArray*)tags TJ_DEPRECATED_CLASS;
+ (void)didFailToRegisterForRemoteNotificationsWithError:(NSError*)error TJ_DEPRECATED_CLASS;
+ (void)didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)token TJ_DEPRECATED_CLASS;
+ (void)didReceiveRemoteNotification:(NSDictionary *)notification TJ_DEPRECATED_CLASS;
#pragma mark User Account Methods
+ (void)getTapPoints TJ_DEPRECATED_CLASS;
+ (void)spendTapPoints:(int)points TJ_DEPRECATED_CLASS;
+ (void)awardTapPoints:(int)points TJ_DEPRECATED_CLASS;
+ (void)showDefaultEarnedCurrencyAlert TJ_DEPRECATED_CLASS;
#pragma mark Video Methods
+ (void)initVideoAdWithDelegate:(id<TJCVideoAdDelegate>)delegate TJ_DEPRECATED_CLASS;
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

