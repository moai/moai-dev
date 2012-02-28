// //
//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement can be found here: https://www.tapjoy.com/sdk/license


package com.tapjoy;

public class TapjoyConstants
{
	// SDK Version.
	public static final String TJC_LIBRARY_VERSION_NUMBER							= "8.1.2";
	
	//--------------------------------------------------------------------------------
	// URL parameter names.
	//--------------------------------------------------------------------------------
	public static final String TJC_ANDROID_ID										= "android_id";									// ANDROID_ID of the device.
	public static final String TJC_DEVICE_ID_NAME									= "udid";										// Unique ID of the device (IMEI or MEID).  Uses serial ID if not present.
	public static final String TJC_DEVICE_NAME										= "device_name";								// This is the specific device name (DroidX, Droid, etc)
	public static final String TJC_DEVICE_MANUFACTURER								= "device_manufacturer";						// This is the manufacturer of the device.
	public static final String TJC_DEVICE_TYPE_NAME									= "device_type";								// Platform type (Android, iPhone, iPad).
	public static final String TJC_DEVICE_OS_VERSION_NAME							= "os_version";									// Version of Android running.
	public static final String TJC_DEVICE_COUNTRY_CODE								= "country_code";								// Country code.
	public static final String TJC_DEVICE_LANGUAGE									= "language_code";								// Language code.
	public static final String TJC_APP_ID_NAME										= "app_id";										// Tapjoy App ID.
	public static final String TJC_APP_VERSION_NAME									= "app_version";								// App version.
	public static final String TJC_CONNECT_LIBRARY_VERSION_NAME						= "library_version";							// Tapjoy Connect version.
	public static final String TJC_USER_ID											= "publisher_user_id";							// User ID for this device/account.
	public static final String TJC_DEVICE_SCREEN_DENSITY							= "screen_density";								// Device screen density.
	public static final String TJC_DEVICE_SCREEN_LAYOUT_SIZE						= "screen_layout_size";							// Device screen layout size.
	
	public static final String TJC_CARRIER_NAME										= "carrier_name";								// Carrier Name.
	public static final String TJC_CARRIER_COUNTRY_CODE								= "carrier_country_code";						// Carrier Country Code (ISO)
	public static final String TJC_MOBILE_COUNTRY_CODE								= "mobile_country_code";						// Carrier Mobile Country Code (MCC)
	public static final String TJC_CONNECTION_TYPE									= "connection_type";							// Connection type (mobile or WIFI)
	public static final String TJC_PLATFORM											= "platform";									// Software platform running on the device ("android")
	
	// Virtual currency display multiplier.
	public static final String TJC_CURRENCY_MULTIPLIER								= "display_multiplier";							// Multiplier for virtual currency.
	
	// Banner Ads
	public static final String TJC_DISPLAY_AD_SIZE									= "size";										// Dimensions of the banner ad.
	
	// Spend tap points.
	public static final String TJC_TAP_POINTS										= "tap_points";									// Amount of tap points to spend/award.
	
	// Multiple currency related.
	public static final String TJC_CURRENCY_ID										= "currency_id";								// Tapjoy multiple currency ID.
	public static final String TJC_MULTIPLE_CURRENCY_SELECTOR_FLAG					= "currency_selector";							// Tapjoy multiple currency selector flag.
	
	// Video/Offerwall related
	public static final String TJC_VIDEO_OFFER_IDS									= "video_offer_ids";							// Video IDs passed to the offer wall.
	
	// Verifier related.
	public static final String TJC_TIMESTAMP										= "timestamp";									// Timestamp
	public static final String TJC_VERIFIER											= "verifier";									// Verifier
	public static final String TJC_GUID												= "guid";										// Unique identifier for each transaction
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	
	public static final String TJC_DEVICE_PLATFORM_TYPE								= "android";									// Devivce platform type (device_type parameter).
	
	//--------------------------------------------------------------------------------
	// URL paths.
	//--------------------------------------------------------------------------------
	public static final String TJC_SERVICE_URL 										= "https://ws.tapjoyads.com/";					// Base URL for all server requests.
	public static final String TJC_BASE_REDIRECT_DOMAIN 							= "ws.tapjoyads.com";							// URL to look for when redirecting.
	public static final String TJC_CONNECT_URL_PATH									= "connect?";									// Connect.
	public static final String TJC_USER_ID_URL_PATH									= "set_publisher_user_id?";						// Set publisher_user_id for Tapjoy Games.
	
	public static final String TJC_USERDATA_URL_PATH								= "get_vg_store_items/user_account?";			// Get tap points.
	public static final String TJC_SPEND_POINTS_URL_PATH							= "points/spend?";								// Spend tap points.
	public static final String TJC_AWARD_POINTS_URL_PATH							= "points/award?";								// Award tap points.
	
	public static final String TJC_SHOW_OFFERS_URL_PATH								= "get_offers/webpage?";						// Offer wall.
	public static final String TJC_FEATURED_APP_URL_PATH 							= "get_offers/featured?";						// Featured App.
	public static final String TJC_DISPLAY_AD_URL_PATH 								= "display_ad?";								// Display Ad.
	
	// Videos
	public static final String TJC_GET_VIDEOS_URL_PATH 								= "videos?";									// Get all valid videos.
	
	// Virtual Goods
	public static final String TJC_VG_GET_ALL_URL_PATH 								= "get_vg_store_items/all?";					// Get all available virtual goods.
	public static final String TJC_VG_GET_PURCHASED_URL_PATH 						= "get_vg_store_items/purchased?";				// Get purchased virtual goods.
	public static final String TJC_VG_PURCHASE_WITH_CURRENCY_URL_PATH				= "points/purchase_vg?";						// BUY a virtual good.
	
	public static final String TJC_GAMESTATE_LOAD_URL_PATH							= "game_state/load?";							// Load gamestate.
	public static final String TJC_GAMESTATE_SAVE_URL_PATH							= "game_state/save?";							// Save gamestate.
	
	
	//--------------------------------------------------------------------------------
	// Preferences related.
	//--------------------------------------------------------------------------------
	public static final String TJC_PREFERENCE 										= "tjcPrefrences";								// Prefs file.
	public static final String PREF_EMULATOR_DEVICE_ID 								= "emulatorDeviceId";
	public static final String PREF_REFERRAL_URL 									= "InstallReferral";
	public static final String PREF_CONTAINS_EXTERNAL_DATA							= "containsExternalData";
	public static final String PREF_ELAPSED_TIME 									= "tapjoy_elapsed_time";						// Used for paid app installs.
	public static final String PREF_REFERRER_DEBUG 									= "referrer_debug";
	
	public static final String TJC_VIDEOS_PREFERENCE								= "TapjoyVideoPrefs";
	
	// For Featured App Store IDs and display limits.
	public static final String TJC_FEATURED_APP_PREFERENCE							= "TapjoyFeaturedAppPrefs";
	
	// For Virtual Goods.
	public static final String PREF_PRIMARY_COLOR									= "tapjoyPrimaryColor";
	
	// Earned Tap Points Notification
	public static final String PREF_LAST_TAP_POINTS									= "last_tap_points";
	
	
	//--------------------------------------------------------------------------------
	// Extras used to send between intents.
	//--------------------------------------------------------------------------------
	public static final String EXTRA_URL_BASE										= "URL_BASE";
	public static final String EXTRA_URL_PARAMS										= "URL_PARAMS";
	public static final String EXTRA_USER_ID										= "USER_ID";
	public static final String EXTRA_FEATURED_APP_FULLSCREEN_AD_URL 				= "FULLSCREEN_AD_URL";
	public static final String EXTRA_DISPLAY_AD_URL					 				= "DISPLAY_AD_URL";
	public static final String EXTRA_VIDEO_PATH						 				= "VIDEO_PATH";
	
	//--------------------------------------------------------------------------------
	// PAID APP constants
	//--------------------------------------------------------------------------------
	public static final long TIMER_INCREMENT 										= 10000;										// Intervals between timer tasks.
	public static final long PAID_APP_TIME 											= 1000*60*15;									// Total amount of time to wait for a paid app.  15 minutes.
	
	public static final long THROTTLE_GET_TAP_POINTS_INTERVAL						= 60000;										// Throttle time between getTapPoints.
	
	
	//--------------------------------------------------------------------------------
	// DATABASE virtual goods related
	//--------------------------------------------------------------------------------
	public static final String DATABASE_NAME 										= "TapjoyLocalDB.sql";
	public static final int DATABASE_VERSION 										= 720;											// *** UPDATE THIS WHENEVER THE VIRTUAL GOODS DATABASE TABLE CHANGES ***
																																	// Updated from 1 to 720:	Add DataFileHash column to database.
}
