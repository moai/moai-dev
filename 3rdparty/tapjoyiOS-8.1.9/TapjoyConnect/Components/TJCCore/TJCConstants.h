// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#ifndef TJC_CONSTANTS_DEC
#define TJC_CONSTANTS_DEC

#define TJC_CURRENCY_KEY_NAME				@"TJC_CURRENCY_KEY_NAME"
#define TJC_POINTS_KEY_NAME				@"TJC_POINTS_KEY_NAME"
#define TJC_POINTS_DELTA_KEY_NAME		@"TJC_POINTS_DELTA_KEY_NAME"
#define TJC_POINT_ID_KEY_NAME				@"TJC_POINT_ID_KEY_NAME"

// NSUserDefaults cache for video object dictionary.
#define TJC_CACHED_VIDEO_DICT				@"TJC_CACHED_VIDEO_DICT"
// Video objects dictionary that contains all video data retrieved from the server.
#define TJC_ALL_VIDEOS_DICT				@"TJC_ALL_VIDEOS_DICT"

// Set User ID API.
#define TJC_URL_PARAM_SET_USER_ID		@"set_user_id"

// Multiple currency parameters.
#define TJC_URL_PARAM_CURRENCY_ID		@"currency_id"
#define TJC_URL_PARAM_SELECTOR_ID		@"currency_selector"

// Tap Points APIs.
#define TJC_URL_PARAM_SPEND_TAP_POINTS	@"points/spend"
#define TJC_URL_PARAM_AWARD_TAP_POINTS	@"points/award"
#define TJC_URL_PARAM_VG_ITEMS_USER		@"get_vg_store_items/user_account"

// Tap Points URL parameter, used for Tap Point operations.
#define TJC_URL_PARAM_TAP_POINTS			@"tap_points"

// Virtual Good APIs.
#define TJC_URL_PARAM_PURCHASE_VG			@"points/purchase_vg"
#define TJC_URL_PARAM_VG_ID					@"virtual_good_id"
#define TJC_URL_PARAM_VG_ITEMS_ALL			@"get_vg_store_items/all"
#define TJC_URL_PARAM_VG_ITEMS_PURCHASED	@"get_vg_store_items/purchased"

// Virtual Good URL index parameters.
#define TJC_URL_PARAM_VG_ITEMS_START		@"start"
#define TJC_URL_PARAM_VG_ITEMS_MAX			@"max"

// Featured App APIs.
#define TJC_FEATURED_FULLSCREEN_URL_NAME	@"get_offers/fullscreenad"
#define TJC_FEATURED_URL_NAME					@"get_offers/featured"

// Offers APIs.
#define TJC_WEB_OFFERS_URL_NAME				@"get_offers/webpage"

// JC: TODO: Used for video ads, check that these are correct.
#define TJC_GET_OFFERS_API						@"get_offers"
#define TJC_URL_PARAM_OFFER_ID				@"offer_id"
#define TJC_URL_PARAM_CLICK_ID				@"click_id"

// Display Ad API.
#define TJC_BANNER_URL_NAME					@"display_ad"

// Video Ad API
#define TJC_VIDEO_AD_GET_API					@"videos"			// Retrieves video data.
#define TJC_VIDEO_AD_VALIDATE_API			@"validate_video"	// Checks that video is still valid.
#define TJC_VIDEO_AD_CLICK_API				@"click/video"		// Video begins playing.
#define TJC_VIDEO_AD_COMPLETE_API			@"offer_complete"	// Video is finished playing.
#define TJC_URL_PARAM_VIDEO_IDS				@"video_offer_ids"// List of video ids.
#define TJC_URL_PARAM_WIFI_VIDEOS			@"all_videos=true"// Indicates that video streaming is allowed and all available videos will be displayed on the offer wall.
#define TJC_VIDEO_CACHE_COUNT					5	// The default number of videos to cache on the device.
#define TJC_VIDEO_CLICK_PROTOCOL				@"tjvideo"
#define TJC_VIDEO_CLICK_PROTOCOL_COMPLETE	@"tjvideo://"
#define TJC_VIDEO_CLICK_ID						@"video_id="
#define TJC_VIDEO_CLICK_URL					@"click_url="
#define TJC_VIDEO_CLICK_CURRENCY_AMOUNT	@"amount="
#define TJC_VIDEO_CLICK_CURRENCY_NAME		@"currency_name="
#define TJC_VIDEO_LOGO_IMAGE_URL				@"https://s3.amazonaws.com/tapjoy/videos/assets/watermark.png"


// Display Ad URL size parameter.
#define TJC_URL_PARAM_DISPLAY_AD_SIZE		@"size"

#define TJC_REQUEST_TIME_OUT 35

#define TJC_URL_PARAM_VG_MAX_INDEX		999
#define TJC_URL_PARAM_VG_PAGE_INDEX		999

// Used for featured app.
#define TJC_FEATURED_APP_DEFAULT_MAX_DISPLAY_COUNT	4
#define TJC_FEATURED_COUNT_INF		-1


//Custom HEXColor function
#define TJC_HEXCOLOR(c) [UIColor colorWithRed:((c>>24)&0xFF)/255.0f \
green:((c>>16)&0xFF)/255.0f \
blue:((c>>8)&0xFF)/255.0f \
alpha:((c)&0xFF)/255.0f]

#define TJC_DEFAULT_TRANSITION_DELAY (0.4f)

#define TJC_SHOW_BOX_CLOSE_NOTIFICATION @"TJC_SHOW_BOX_NOTIFICATION"
#define TJC_DEFAULT_COLOR   0x0E2C4CFF	//928844

// For the loading view.
#define TJC_LOADING_RECT_CORNER_RADIUS	5

#define TJC_GENERIC_TAG_ID	12345

#define TJC_GENERIC_CONNECTION_ERROR_MESSAGE	@"Service is unreachable.\nDo you want to try again?"


#endif
