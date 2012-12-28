//
//  MobclixAdView.h
//  Mobclix iOS SDK
//
//  Copyright 2011 Mobclix. All rights reserved.
//

typedef enum {
	kMCAdsUnknownError	= 0,
	kMCAdsServerError	= -500,
	kMCAdsUnavailable	= -503,
	kMCAdsNotStarted	= -8888888,
	kMCAdsDisabled		= -9999999
} MCAdsError;

typedef enum {
	kMCAdsSuballocationOpen			= -1006,
	kMCAdsSuballocationIAd			= -275,
	kMCAdsSuballocationAdMob		= -750,
	kMCAdsSuballocationGoogle		= -10100,
	kMCAdsSuballocationMillennial	= -1375
} MCAdsSuballocationType;

@protocol MobclixAdViewDelegate;

/**
 * Do not initialize this class directly, please use the MobclixAdViewiPhone_* or MobclixAdViewiPad_* subclasses. 
 */
@interface MobclixAdView : UIView {
@private
	id _internal;
	BOOL _subsequent;
}

/**
 * Requests a new ad from the server
 * If ad refreshes are paused, this will also call resumeAdAutoRefresh
 *
 * This should only be called when you want to manually refresh an ad.
 */
- (void)getAd;

/**
 * Continues an Ad request.  This should be called when you (as the 
 * application developer) have chosen to implement  open allocation yourself and 
 * in your implementation you have not shown an ad.  Calling this method
 * indicates to the SDK that it should call the next OA candidate.
 * 
 * 
 */
- (void)continueRequest;



/**
 * Pauses the autorefresh of ads
 *
 * This should be called in viewWillDisappear
 */
- (void)pauseAdAutoRefresh;

/**
 *	Resumes the autorefresh of ads
 *
 * This should be called in viewDidAppear
 */
- (void)resumeAdAutoRefresh;

/**
 * Cancels any ads currently load and calls pauseAdRefresh
 *
 * This should be called in viewDidUnload/dealloc
 */
- (void)cancelAd;

/**
 * Receive callback information during the ad cycle.
 *
 * Note: Set delegate to nil when your delegate dealloc's.
 */
@property(nonatomic,assign) IBOutlet id<MobclixAdViewDelegate> delegate;

/**
 * Time interval between autorefreshes
 *
 * To disable autorefresh, set to -1
 * Default time is 30 seconds
 */
@property(nonatomic,assign) NSTimeInterval refreshTime;

/**
 * Override viewController for this ad view.
 * 
 * Note: We will automatically detect your view controller
 * Most users will not need to set this property.
 */
@property(nonatomic,assign) UIViewController* viewController;
@end


#pragma mark -
#pragma mark MobclixAdView iPhone Subclasses

@interface MobclixAdViewiPhone_320x50 : MobclixAdView {}
@end

@interface MobclixAdViewiPhone_300x250 : MobclixAdView {}
@end

#pragma mark -
#pragma mark MobclixAdView iPad Subclasses

@interface MobclixAdViewiPad_300x250 : MobclixAdView {}
@end

@interface MobclixAdViewiPad_728x90 : MobclixAdView {}
@end

@interface MobclixAdViewiPad_120x600 : MobclixAdView {}
@end

@interface MobclixAdViewiPad_468x60 : MobclixAdView {}
@end

#pragma mark -
#pragma mark MobclixAdViewDelegate Protocol

@protocol MobclixAdViewDelegate<NSObject>
@optional

// Advertisement Status Messages
- (void)adViewDidFinishLoad:(MobclixAdView*)adView;
- (void)adView:(MobclixAdView*)adView didFailLoadWithError:(NSError*)error;


// Overrides remote dashboard setting

/**
 * Return whether or not an ad with autoplay should be requested
 *
 * Autoplay ads are similar to interstitial ads and will automatically
 * open a website, video, or any other kind of modal action supported by the SDK.
 *
 * If this method isn't implemented, the settings provided on the Dashboard are used.
 */
- (BOOL)adViewCanAutoplay:(MobclixAdView*)adView;

/**
 * Rich Media features include access to the following:
 *	* LED Flash
 *	* Vibrate
 *	* Sound
 *	* Inline Video (iPad Only)
 *
 * To allow these features to work automatically, this callback should return NO.
 * 
 * Applications that vibrate or play sound, should return YES.
 *
 * If this method isn't implemented, the settings provided on the Dashboard are used.
 */
- (BOOL)richMediaRequiresUserInteraction:(MobclixAdView*)adView;


// Advertisement Suballocation Requests

/**
 *	Return YES if you want the Mobclix SDK to handle this request
 *
 *	The Mobclix SDK can currently support the following libraries:
 *		iAd
 *		GoogleAdMob
 *
 *	If you wish to integrate the networks yourself, return NO in this method.
 *	A subsequent delegate call to adView:didReceiveSuballocationRequest: will be made
 *	If you return NO.
 *
 *	If this method is not implemented, Mobclix SDK assumes YES.
 */
- (BOOL)adView:(MobclixAdView*)adView shouldHandleSuballocationRequest:(MCAdsSuballocationType)suballocationType;

/**
 *	When this method is called, you'll need to manage the sub allocation call yourself.
 *	This will be called if NO is returned in adView:shouldHandleSuballocationRequest:
 *	Or if the sub allocation request type is unsupported by the Mobclix SDK.
 *
 *	If you use auto refresh, it is recommended that you call pauseAdAutoRefresh in this method
 *	And then call resumeAdAutoRefresh when your sub allocation network is finished loading or fails
 */
- (void)adView:(MobclixAdView*)adView didReceiveSuballocationRequest:(MCAdsSuballocationType)suballocationType;

/**
 *	Return the publisher key specific to the suballocation type
 *
 *	This is only called if adView:shouldHandleSuballocationRequest: returns YES or is not implemented,
 *	and suballocationType is kMCAdsSuballocationAdMob.
 *
 *	iAd sub allocations do not require a publisher key, this method will not be called for kMCAdsSuballocationIAd.
 */
- (NSString*)adView:(MobclixAdView*)adView publisherKeyForSuballocationRequest:(MCAdsSuballocationType)suballocationType;


// Advertisement Touchthrough Messages
- (void)adViewWillTouchThrough:(MobclixAdView*)adView;
- (void)adViewDidFinishTouchThrough:(MobclixAdView*)adView;
- (void)adView:(MobclixAdView*)adView didTouchCustomAdWithString:(NSString*)string;

// Targeting Messages
- (NSString*)mcKeywords;//11/11/11  cward Apple began rejecting applications with the method "keywords" in our application. renaming
- (NSString*)query;

@end

extern NSString* const MCAdsErrorDomain;
