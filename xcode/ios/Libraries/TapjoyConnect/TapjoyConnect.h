//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license



/*! \mainpage Tapjoy iOS SDK
 *
 * The Tapjoy iOS SDK.
 */


#import <Foundation/Foundation.h>


#define TJC_LIBRARY_VERSION_NUMBER			@"8.1.1"					/*!< The SDK version number. */

#define TJC_SERVICE_URL							@"https://ws.tapjoyads.com/"
#define TJC_SERVICE_URL_ALTERNATE			@"https://ws1.tapjoyads.com/"
#define TJC_TAPJOY_HOST_NAME					@"ws.tapjoyads.com"
#define TJC_TAPJOY_ALT_HOST_NAME				@"ws1.tapjoyads.com"
#define TJC_LINKSHARE_HOST_NAME				@"click.linksynergy.com"

#define TJC_UDID									@"udid"					/*!< The unique device identifier. Deprecated in iOS 5. */
#define TJC_UNIQUE_MAC_ID						@"mac_address"			/*!< The unique ID retrieved by taking the hash of mac address. */
#define TJC_DEVICE_NAME							@"device_name"			/*!< This is the specific device name ("iPhone1,1", "iPod1,1"...) */
#define TJC_DEVICE_TYPE_NAME					@"device_type"			/*!< The model name of the device. This is less descriptive than the device name. */
#define TJC_DEVICE_OS_VERSION_NAME			@"os_version"			/*!< The device system version. */
#define TJC_DEVICE_COUNTRY_CODE				@"country_code"		/*!< The country code is retrieved from the locale object, from user data (not device). */
#define TJC_DEVICE_LANGUAGE					@"language_code"		/*!< The language is retrieved from the locale object, from user data (not device). */
#define TJC_DEVICE_LAD							@"lad"					/*!< Little Alien Dude. */
#define TJC_APP_ID_NAME							@"app_id"				/*!< The application id is set by the developer, and is a unique id provided by Tapjoy. */
#define TJC_APP_VERSION_NAME					@"app_version"			/*!< The application version is retrieved from the application plist file, from the bundle version. */
#define TJC_CONNECT_LIBRARY_VERSION_NAME	@"library_version"	/*!< The library version is the SDK version number. */	
#define TJC_VERIFIER								@"verifier"				/*!< A hashed value that is verified on the server to confirm a valid connect. */
#define TJC_TIMESTAMP							@"timestamp"			/*!< The time in seconds when a connect call is made. */
#define TJC_GUID									@"guid"					/*!< Used as part of the verifier. */
#define TJC_CARRIER_NAME						@"carrier_name"		/*!< The name of the user’s home cellular service provider. */
#define TJC_ALLOWS_VOIP							@"allows_voip"			/*!< Indicates if the carrier allows VoIP calls to be made on its network. */
#define TJC_CARRIER_COUNTRY_CODE				@"carrier_country_code"	/*!< The ISO country code for the user’s cellular service provider. */
#define TJC_MOBILE_COUNTRY_CODE				@"mobile_country_code"	/*!< The mobile country code (MCC) for the user’s cellular service provider. */
#define TJC_MOBILE_NETWORK_CODE				@"mobile_network_code"	/*!< The mobile network code (MNC) for the user’s cellular service provider. */
#define TJC_PLATFORM								@"platform"				/*!< The name of the platform. */
#define TJC_PLATFORM_IOS						@"iOS"
// NOTE: This doesn't actually affect currency earned, just the value displayed on the offer wall.
#define TJC_URL_PARAM_CURRENCY_MULTIPLIER	@"display_multiplier"	/*!< Currency multiplier value. */
#define TJC_CONNECTION_TYPE_NAME				@"connection_type"	/*!< The type of data connection that is being used. */


#define TJC_CONNECT_API							@"connect"						/*!< API for Tapjoy connect. */
#define TJC_SET_USER_ID_API					@"set_publisher_user_id"	/*!< API for setting publisher user id. */


/*!	\interface TapjoyConnect
 *	\brief The Tapjoy Connect Main class.
 *
 */
@interface TapjoyConnect :  NSObject
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
<NSXMLParserDelegate>
#endif
{
@private
	NSString *appID_;						/*!< The application ID unique to this app. */
	NSString *secretKey_;				/*!< The Tapjoy secret key for this applicaiton. */
	NSString *userID_;					/*!< The user ID, used to display ads. This is the UDID by default. */
	float currencyMultiplier_;			/*!< The currency multiplier value, used to adjust currency earned. */
	NSData *data_;							/*!< Holds data for any data that comes back from a URL request. */
	NSURLConnection *connection_;		/*!< Used to provide support to perform the loading of a URL request. Delegate methods are defined to handle when a response is receive with associated data. This is used for asynchronous requests only. */
	NSString *currentXMLElement_;		/*!< Contains @"Success when a connection is successfully made, nil otherwise. */
	int connectAttempts_;				/*!< The connect attempts is used to determine whether the alternate URL will be used. */
	BOOL isInitialConnect_;				/*!< Used to keep track of an initial connect call to prevent multiple repeated calls. */
}

@property (nonatomic,copy) NSString* appID;
@property (nonatomic,copy) NSString* secretKey;
@property (nonatomic,copy) NSString* userID;
@property (nonatomic) BOOL isInitialConnect;


/*!	\fn requestTapjoyConnect:secretKey:(NSString *appID, NSString *secretKey)
 *	\brief This method is called to initialize the TapjoyConnect system.
 *
 * This method should be called upon app delegate initialization in the applicationDidFinishLaunching method.
 *	\param appID The application ID. Retrieved from the app dashboard in your Tapjoy account.
 *  \param secretKey The application secret key. Retrieved from the app dashboard in your Tapjoy account.
 *	\return The globally accessible #TapjoyConnect object.
 */
+ (TapjoyConnect*)requestTapjoyConnect:(NSString*)appID secretKey:(NSString*)secretKey;

/*!	\fn actionComplete:(NSString*)actionID
 *	\brief This is called when an action is completed.
 *
 * Actions are much like connects, except that this method is only called when a user completes an in-game action.
 *	\param actionID The action ID.
 *	\return The globally accessible #TapjoyConnect object.
 */
+ (TapjoyConnect*)actionComplete:(NSString*)actionID;

/*!	\fn sharedTapjoyConnect
 *	\brief Retrieves the globally accessible #TapjoyConnect singleton object.
 *
 *	\param n/a
 *	\return The globally accessible #TapjoyConnect singleton object.
 */
+ (TapjoyConnect*)sharedTapjoyConnect;

/*!	\fn deviceNotificationReceived
 *	\brief This is called whenever the application returns to the foreground.
 *
 *	\param n/a
 *	\return n/a
 */
+ (void)deviceNotificationReceived;

/*!	\fn TJCSHA256CommonParamsWithTimeStamp:(NSString*)timeStamp
 *	\brief Generates a SHA-256 hash value with the given time stamp.
 *
 * The following are sent as common parameters: appID, UDID, and timestamp.
 *	\param timeStamp The time stamp to generate the hash with.
 *	\return The SHA-256 hash value.
 */
+ (NSString*)TJCSHA256CommonParamsWithTimeStamp:(NSString*)timeStamp;

/*!	\fn TJCSHA256CommonParamsWithTimeStamp:tapPointsAmount:guid:(NSString* timeStamp, int points, NSString* guid)
 *	\brief Generates a SHA-256 hash value with the time stamp and some common parameters.
 *
 * The following are sent as common parameters: appID, UDID, and timestamp.
 *	\param timeStamp The time stamp to generate the hash with.
 *	\param points The amount of tap points to award to the user.
 *	\param guid A generated GUID for this particular URL request.
 *	\return The SHA-256 hash value.
 */
+ (NSString*)TJCSHA256CommonParamsWithTimeStamp:(NSString*)timeStamp tapPointsAmount:(int)points guid:(NSString*)guid;

/*!	\fn TJCSHA256WithString:(NSString*)dataStr
 *	\brief Generates a SHA-256 hash value with the given string.
 *
 *	\param dataStr The string from which the hash value will be generated from.
 *	\return The SHA-256 hash value.
 */
+ (NSString*)TJCSHA256WithString:(NSString*)dataStr;

/*!	\fn generateUUID
 *	\brief Generates a GUID.
 *
 *	\param n/a
 *	\return A GUID.
 */
+ (NSString*)generateUUID;

/*!	\fn getAppID
 *	\brief Retrieves the Tapjoy app ID.
 *
 *	\param n/a
 *	\return The Tapjoy app ID passed into requestTapjoyConnect.
 */
+ (NSString*)getAppID;

/*!	\fn setUserID:(NSString*)theUserID
 *	\brief Sets the user ID.
 *
 * The user ID defaults to the UDID. This is only changed when NOT using Tapjoy Managed Currency.
 *	\param theUserID The user ID.
 *	\return n/a
 */
+ (void)setUserID:(NSString*)theUserID;

/*!	\fn getUserID
 *	\brief Retrieves the user ID.
 *
 *	\param n/a
 *	\return The Tapjoy user ID. The user ID defaults to the UDID.
 */
+ (NSString*)getUserID;

/*!	\fn getSecretKey
 *	\brief Retrieves the secret.
 *
 *	\param n/a
 *	\return The Tapjoy secret key for this application.
 */
+ (NSString*)getSecretKey;

/*! \fn isJailBroken
 *	\brief Simple check to detect jail broken devices/apps.
 *
 * Note that this is NOT guaranteed to be accurate! There are very likely going to be ways to circumvent this check in the future.
 *	\param n/a
 *	\return YES for indicating that the device/app has been jailbroken, NO otherwise.
 */ 
- (BOOL)isJailBroken;

/*! \fn isJailBrokenStr
 *	\brief Simple check to detect jail broken devices/apps.
 *
 * Note that this is NOT guaranteed to be accurate! There are very likely going to be ways to circumvent this check in the future.
 *	\param n/a
 *	\return A string "YES" for indicating that the device/app has been jailbroken, "NO" otherwise.
 */ 
- (NSString*)isJailBrokenStr;

/*! \fn genericParameters
 *	\brief Retrieves the dictionary of generic parameters that are sent with every URL request.
 *
 *	\param n/a
 *	\return A dictionary of generic parameters, listed at the top of this file.
 */ 
- (NSMutableDictionary*)genericParameters;

/*! \fn createQueryStringFromDict:(NSDictionary*)paramDict
 *	\brief Takes the given dictionary and contructs a legal URL string from it.
 *
 *	\param n/a
 *	\return A legal URL string constructed from the given dicionary.
 */ 
- (NSString*)createQueryStringFromDict:(NSDictionary*)paramDict;
// Wrapper method.
+ (NSString*)createQueryStringFromDict:(NSDictionary*)paramDict;

/*!	\fn setCurrencyMultiplier:(float)mult
 *	\brief Sets the currency multiplier for virtual currency to be earned.
 *
 *	\param mult The currency multiplier.
 *	\return n/a
 */
- (void)setCurrencyMultiplier:(float)mult;
// Wrapper method.
+ (void)setCurrencyMultiplier:(float)mult;

/*!	\fn getCurrencyMultiplier
 *	\brief Gets the currency multiplier for virtual currency to be earned.
 *
 *	\param n/a
 *	\return The currency multiplier value.
 */
- (float)getCurrencyMultiplier;
// Wrapper method.
+ (float)getCurrencyMultiplier;


// Declared here to prevent warnings.
#pragma mark TapjoyConnect NSXMLParser Delegate Methods
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
- (void)startParsing:(NSData*) myData;
#endif


+ (NSString*)getMACAddress;

@end


#import "TapjoyConnectConstants.h"