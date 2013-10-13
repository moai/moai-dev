// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#define TJC_CONNECT_SUCCESS					@"TJC_Connect_Success"
#define TJC_CONNECT_FAILED					@"TJC_Connect_Failed"

// This notification is fired after getTapPoints has been called, and indicates that user currency amount has been received from the server.
#define TJC_TAP_POINTS_RESPONSE_NOTIFICATION				@"TJC_TAP_POINTS_RESPONSE_NOTIFICATION"
// This notification is fired after spendTapPoints has been called, and indicates that the user has successfully spent currency.
#define TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION			@"TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION"
// This notification is fired after awardTapPoints has been called, and indicates that the user has successfully been awarded currency.
#define TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION			@"TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION"

// Error notification for getTapPoints.
#define TJC_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR			@"TJC_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR"
// Error notification for spendTapPoints
#define TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR	@"TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR"
// Error notification for awardTapPoints
#define TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR	@"TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR"

// Notification that is fired after an event has been logged.
#define TJC_EVENT_TRACKING_RESPONSE_NOTIFICATION			@"TJC_EVENT_TRACKING_RESPONSE_NOTIFICATION"
// Error notification for Event Tracking.
#define TJC_EVENT_TRACKING_RESPONSE_NOTIFICATION_ERROR		@"TJC_EVENT_TRACKING_RESPONSE_NOTIFICATION_ERROR"

// Full Screen Ad notification is fired after full screen ad data is received from the server.
#define TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION			@"TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION"
#define TJC_FEATURED_APP_RESPONSE_NOTIFICATION				@"TJC_FEATURED_APP_RESPONSE_NOTIFICATION"	// Deprecated
// Error notification for Full Screen Ad.
#define TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION_ERROR		@"TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION_ERROR"

// Daily reward notification is fired after the ad data is received from the server.
#define TJC_DAILY_REWARD_RESPONSE_NOTIFICATION				@"TJC_DAILY_REWARD_RESPONSE_NOTIFICATION"
// Error notification for daily reward ad.
#define TJC_DAILY_REWARD_RESPONSE_NOTIFICATION_ERROR		@"TJC_DAILY_REWARD_RESPONSE_NOTIFICATION_ERROR"

// Offers notification is fired after the ad data is loaded.
#define TJC_OFFERS_RESPONSE_NOTIFICATION			@"TJC_OFFERS_RESPONSE_NOTIFICATION"
// Error notification for offers.
#define TJC_OFFERS_RESPONSE_NOTIFICATION_ERROR		@"TJC_OFFERS_RESPONSE_NOTIFICATION_ERROR"

// Notification that a user has just successfully completed an offer and earned currency. This only fires on init/resume.
#define TJC_TAPPOINTS_EARNED_NOTIFICATION					@"TJC_TAPPOINTS_EARNED_NOTIFICATION"

// Fired when any Tapjoy view is closed.
#define TJC_VIEW_CLOSED_NOTIFICATION						@"TJC_VIEW_CLOSED_NOTIFICATION"

// The keys for the options dictionary when calling requestTapjoyConnect.
#define TJC_OPTION_DISABLE_VIDEOS				@"TJC_OPTION_DISABLE_VIDEOS"
#define TJC_OPTION_VIDEO_CACHE_COUNT			@"TJC_OPTION_VIDEO_CACHE_COUNT"
#define TJC_OPTION_FULL_SCREEN_AD_DELAY_COUNT	@"TJC_OPTION_FULL_SCREEN_AD_DELAY_COUNT"
#define TJC_OPTION_ENABLE_LOGGING				@"TJC_OPTION_ENABLE_LOGGING"
#define TJC_OPTION_USER_ID						@"TJC_OPTION_USER_ID"
#define TJC_OPTION_TRANSITION_EFFECT			@"TJC_OPTION_TRANSITION_EFFECT"
#define TJC_OPTION_CLEAR_SHARED_URL_CACHE		@"TJC_OPTION_CLEAR_SHARED_URL_CACHE"
#define TJC_OPTION_COLLECT_MAC_ADDRESS			@"TJC_OPTION_COLLECT_MAC_ADDRESS"


#define TJC_DISPLAY_AD_SIZE_320X50	@"320x50"
#define TJC_DISPLAY_AD_SIZE_640X100	@"640x100"
#define TJC_DISPLAY_AD_SIZE_768X90	@"768x90"

// Deprecated
#define TJC_AD_BANNERSIZE_320X50	@"320x50"
#define TJC_AD_BANNERSIZE_640X100	@"640x100"
#define TJC_AD_BANNERSIZE_768X90	@"768x90"


/*!	\enum TJCTransitionEnum
 *	\brief Enumeration of the different ways the View can animate. Use this with the TJC_OPTION_TRANSITION_EFFECT option.
 */
typedef enum TJCTransitionEnum
{
	TJCTransitionBottomToTop = 0,		/*!< View animates from the bottom to the top of the screen. */
	TJCTransitionTopToBottom,			/*!< View animates from the top to the bottom of the screen. */
	TJCTransitionLeftToRight,			/*!< View animates from the left to the right of the screen. */
	TJCTransitionRightToLeft,			/*!< View animates from the right to the left of the screen. */
	TJCTransitionFadeEffect,			/*!< View fades into visibility. */
	TJCTransitionNormalToBottom,		/*!< View animates off screen downwards. */
	TJCTransitionNormalToTop,			/*!< View animates off screen upwards. */
	TJCTransitionNormalToLeft,			/*!< View animates off screen to the left. */
	TJCTransitionNormalToRight,			/*!< View animates off screen to the right. */
	TJCTransitionFadeEffectReverse,		/*!< View fades out of visibility. */
	TJCTransitionExpand,				/*!< View starts in the middle with zero size and expands to fit the screen. */
	TJCTransitionShrink,				/*!< View shrinks to the middle of the screen until no visible. */
	TJCTransitionFlip,
	TJCTransitionFlipReverse,
	TJCTransitionPageCurl,
	TJCTransitionPageCurlReverse,
	TJCTransitionNoEffect = -1			/*!< No animation effect. */
} TJCTransitionEnum;


/*!	\enum TJCViewTypeEnum
 *	\brief Enumeration of the different views that are passed back in the TJCViewDelegate protocol methods.
 */
typedef enum TJCViewTypeEnum
{
	TJC_EVENT_CONTENT_VIEW,		/*!< The content view for an event. */
	TJC_OTHER_AD_VIEW,			/*!< Any other type of Tapjoy view. */
	TJC_AD_VIEW_MAX,
} TJCViewTypeEnum;


/*!  \enum TJCMacAddressOption
 *  \brief Settings for collection of MAC address. Use this with the TJC_OPTION_COLLECT_MAC_ADDRESS option.
 */
typedef enum TJCMacAddressOption
{
	TJCMacAddressOptionOn = 0,        /*!< Enables collection of MAC address. */
	TJCMacAddressOptionOffWithVersionCheck,  /*!< Disables collection of MAC address for iOS versions 6.0 and above. */
	TJCMacAddressOptionOff          /*!< Completely disables collection of MAC address. THIS WILL EFFECTIVELY DISABLE THE SDK FOR IOS VERSIONS BELOW 6.0! */
} TJCMacAddressOption;