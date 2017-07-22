//
//  VungleSDK.h
//  Vungle iOS SDK
//  SDK Version: 5.1.0
//
//  Copyright (c) 2013-Present Vungle Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * VungleViewInfo is a container object for state passed
 * indicating how the play experience went
 */
@interface VungleViewInfo : NSObject <NSCopying>

/**
 * Represents a BOOL whether or not the video can be considered a full view.
 */
@property (nonatomic, readonly) NSNumber *completedView;

/**
 * The time in seconds that the user watched the video.
 */
@property (nonatomic, readonly) NSNumber *playTime;

/**
 * Represents a BOOL whether or not the user clicked the download button.
 */
@property (nonatomic, readonly) NSNumber *didDownload;

@end

@protocol VungleAssetLoader;

extern NSString *VungleSDKVersion;
extern NSString *VunglePlayAdOptionKeyIncentivizedAlertTitleText;
extern NSString *VunglePlayAdOptionKeyIncentivizedAlertBodyText;
extern NSString *VunglePlayAdOptionKeyIncentivizedAlertCloseButtonText;
extern NSString *VunglePlayAdOptionKeyIncentivizedAlertContinueButtonText;
extern NSString *VunglePlayAdOptionKeyOrientations;
extern NSString *VunglePlayAdOptionKeyUser;
extern NSString *VunglePlayAdOptionKeyExtraInfoDictionary;
extern NSString *VunglePlayAdOptionKeyExtra1;
extern NSString *VunglePlayAdOptionKeyExtra2;
extern NSString *VunglePlayAdOptionKeyExtra3;
extern NSString *VunglePlayAdOptionKeyExtra4;
extern NSString *VunglePlayAdOptionKeyExtra5;
extern NSString *VunglePlayAdOptionKeyExtra6;
extern NSString *VunglePlayAdOptionKeyExtra7;
extern NSString *VunglePlayAdOptionKeyExtra8;
extern NSString *VunglePlayAdOptionKeyLargeButtons;

typedef enum {
    VungleSDKErrorInvalidPlayAdOption = 1,
    VungleSDKErrorInvalidPlayAdExtraKey,
    VungleSDKErrorCannotPlayAd,
    VungleSDKErrorCannotPlayAdAlreadyPlaying,
    VungleSDKErrorCannotPlayAdWaiting,
    VungleSDKErrorNoAppID,
    InvalidPlacementsArray,
    VungleSDKErrorInvalidiOSVersion,
    VungleSDKErrorTopMostViewControllerMismatch,
    VungleSDKErrorUnknownPlacementID,
    VungleSDKErrorSDKNotInitialized
} VungleSDKErrorCode;

@protocol VungleSDKLogger <NSObject>
- (void)vungleSDKLog:(NSString *)message;
@end

@class VungleSDK;

@protocol VungleSDKDelegate <NSObject>
@optional

/**
 * If implemented, this will get called when the SDK has an ad ready to be displayed. Also it will
 * get called with an argument `NO` for `isAdPlayable` when for some reason, there is
 * no ad available, for instance there is a corrupt ad or the OS wiped the cache.
 * Please note that receiving a `NO` here does not mean that you can't play an Ad: if you haven't
 * opted-out of our Exchange, you might be able to get a streaming ad if you call `play`.
 * @param isAdPlayable A boolean indicating if an ad is currently in a playable state
 * @param placementID The ID of a placement which is ready to be played
 */
- (void)vungleAdPlayabilityUpdate:(BOOL)isAdPlayable placementID:(nullable NSString *)placementID;
/**
 * If implemented, this will get called when the SDK is about to show an ad. This point
 * might be a good time to pause your game, and turn off any sound you might be playing.
 * @param placementID The placement which is about to be shown.
 */
- (void)vungleWillShowAdForPlacementID:(nullable NSString *)placementID;

/**
 * If implemented, this method gets called when a Vungle Ad Unit is about to be completely dismissed.
 * At this point, it's recommended to resume your Game or App.
 */
- (void)vungleWillCloseAdWithViewInfo:(nonnull VungleViewInfo *)info placementID:(nonnull NSString *)placementID;

- (void)vungleSDKwillCloseAdWithViewInfo:(NSDictionary *)viewInfo
                 willPresentProductSheet:(BOOL)willPresentProductSheet __attribute__((deprecated("Use vungleSDKWillCloseAdWithViewInfo: instead.")));

- (void)vungleSDKwillCloseProductSheet:(id)productSheet __attribute__((deprecated("Use vungleSDKWillCloseAdWithViewInfo: instead.")));

/**
 * If implemented, this will get called when VungleSDK has finished initialization.
 * It's only at this point that one can call `playAd:options:placementID:error`
 * and `loadPlacementWithID:` without getting initialization errors
 */
- (void)vungleSDKDidInitialize;

/**
 * If implemented, this will get called if the VungleSDK fails to initialize.
 * The included NSError object should give some information as to the failure reason.
 * @note If initialization fails, you will need to restart the VungleSDK.
 */
- (void)vungleSDKFailedToInitializeWithError:(NSError *)error;

@end

@interface VungleSDK : NSObject
@property (strong) NSDictionary *userData;
@property (strong) id<VungleSDKDelegate> delegate;
@property (strong) id<VungleAssetLoader> assetLoader;
@property (assign) BOOL muted;
@property (atomic, readonly, getter=isInitialized) BOOL initialized;

/**
 * Returns the singleton instance.
 */
+ (VungleSDK *)sharedSDK;

/**
 * Setup the SDK with an asset loader. This must be called before any call to sharedSDK in order
 * to properly set the asset loader.
 */
+ (VungleSDK *)setupSDKWithAssetLoader:(id<VungleAssetLoader>)loader;

/**
 * Initializes the SDK. You can get your app id on Vungle's dashboard: https://v.vungle.com
 * @param appID the unique identifier for your app
 * @param placements An array of strings representing placements defined in the dashboard.
 * @param error An error object containing information about why initialization failed
 * @return YES if the SDK has started, NO otherwise
 */
- (BOOL)startWithAppId:(nonnull NSString *)appID placements:(nonnull NSArray<NSString *> *)placements error:(NSError **)error;

/**
 * Will play Ad Unit presenting it over the `controller` parameter
 * @param controller A subclass of UIViewController. Should correspond to the ViewControler at the top of the ViewController hierarchy
 * @param options A reference to an instance of NSDictionary with customized ad playback options
 * @param placementID The placement defined on the Vungle dashboard
 * @param error An optional double reference to an NSError. In case this method returns `NO` it will be non-nil
 * @return YES/NO in case of success/error while presenting an AdUnit
 * @warning Should be called from the main-thread.
 */
- (BOOL)playAd:(UIViewController *)controller options:(nullable NSDictionary *)options placementID:(nullable NSString *)placementID error:( NSError *__autoreleasing _Nullable *_Nullable)error;

/**
 * Returns `YES` when there is certainty that an ad will be able to play for a given placementID.
 * Returning `NO`, you can still try to play and get a streaming Ad.
 * @param placementID the specific ID of the placement you are trying to present
 */
- (BOOL)isAdCachedForPlacementID:(nonnull NSString *)placementID;

/**
 * Prepares a placement when you know that you will want
 * to show an ad experience tied to a specific placementID.
 * @param placementID the specific ID of the placement you would like to present at some point soon
 * @return NO if something goes immediately wrong with loading, YES otherwise
 */
- (BOOL)loadPlacementWithID:(NSString *)placementID error:(NSError **)error;

/**
 * Returns debug info.
 */
- (NSDictionary *)debugInfo;

/**
 * By default, logging is off.
 */
- (void)setLoggingEnabled:(BOOL)enable;

/**
 * Log a new message. The message will be sent to all loggers.
 */
- (void)log:(NSString *)message, ...NS_FORMAT_FUNCTION(1, 2);

/**
 * Attach a new logger. It will get called on every log generated by Vungle (internally and externally).
 */
- (void)attachLogger:(id<VungleSDKLogger>)logger;

/**
 * Detaches a logger. Make sure to do this, otherwise you might leak memory.
 */
- (void)detachLogger:(id<VungleSDKLogger>)logger;

/**
 * This only works on the simulator
 */
- (void)clearSleep;

@end

NS_ASSUME_NONNULL_END
