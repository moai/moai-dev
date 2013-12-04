/*
 *  Kontagent.h
 *  
 *  Created by macdrive on 6/10/11.
 *  Copyright (c) 2011 Kontagent. All rights reserved.
 *
 *  Umbrella header for Kontagent framework.
 *
 */

#ifndef KT_KONTAGENT_H
#define KT_KONTAGENT_H

#import <Foundation/Foundation.h>

extern NSString * const KT_PRODUCTION_API_URL_PREFIX;
extern NSString * const KT_TEST_API_URL_PREFIX;
extern NSString * const KT_UNIT_TEST_URL_PREFIX;

extern NSString * const KT_FUNCTION_APPLICATION_ADDED;
extern NSString * const KT_FUNCTION_CUSTOM_EVENT;
extern NSString * const KT_FUNCTION_GOAL_COUNT;
extern NSString * const KT_FUNCTION_INVITE_SENT;
extern NSString * const KT_FUNCTION_INVITE_RESPONSE;
extern NSString * const KT_FUNCTION_MESSAGE_SENT;
extern NSString * const KT_FUNCTION_MESSAGE_RECEIVED;
extern NSString * const KT_FUNCTION_PAGE_REQUEST;
extern NSString * const KT_FUNCTION_REVENUE_TRACKING;
extern NSString * const KT_FUNCTION_STREAM_POST;
extern NSString * const KT_FUNCTION_STREAM_POST_RESPONSE;
extern NSString * const KT_FUNCTION_UNDIRECTED_COMMUNICATION_CLICK;
extern NSString * const KT_FUNCTION_USER_INFORMATION;
extern NSString * const KT_FUNCTION_NOTIFICATION_EMAIL_SENT;
extern NSString * const KT_FUNCTION_NOTIFICATION_EMAIL_RESPONSE;
extern NSString * const KT_FUNCTION_FEATURE_INSTALLATION_TRACK;

extern NSString * const KT_FUNCTION_FEEDPUB;

extern NSString * const KT_FUNCTION_FB_LIB;

extern NSString * const KT_PREFERENCE_NAME;
extern NSString * const KT_PREFERENCE_SENDER_ID_KEY;
extern NSString * const KT_PREFERENCE_MODE_KEY;
extern NSString * const KT_PREFERENCE_TIMESTAMP_PARAMETER_SUPPPORTED;

extern NSString * const KT_PASTEBOARD_SUID_PROPERTY_KEY;

extern NSString * const KT_SESSION_ENABLE_ACQUISITION_TRACKING;

// CONFIGURATION
extern NSString * const KT_SESSION_API_URL_CONFIG_KEY;
extern NSString * const KT_SESSION_API_KEY_CONFIG_KEY;
extern NSString * const KT_SESSION_SENDER_ID_CONFIG_KEY;
extern NSString * const KT_SESSION_MODE_CONFIG_KEY;
extern NSString * const KT_SESSION_DEBUG_CONFIG_KEY;
extern NSString * const KT_SESSION_APA_CONFIG_KEY;
extern NSString * const KT_SESSION_CUSTOM_ID_CONFIG_KEY;
extern NSString * const KT_SESSION_FB_APP_ID_CONFIG_KEY;
extern NSString * const KT_SESSION_FB_EXPTAL_SERV_URL_CONFIG_KEY;
extern NSString * const KT_SESSION_APP_VMAJ_KEY; 

#if defined(__cplusplus)
extern "C" {
#endif
    
enum _KTReturn
{
    /**
     * No error, the function call was successful.
     */
    kKTReturnSuccess = 0,
    
    /**
     * Unknown cause of failure.
     */
    kKTReturnUnknownError = -1,
    
    kKTReturnError = kKTReturnUnknownError,
    
    /**
     * This functionality is not supported.
     */
    kKTReturnNotSupported = -2,
    
    /**
     * One or more function parameters passed were not acceptable.
     */
    kKTReturnBadParameter = -3,
    
    /**
     * API Key validation error codes
     */
    
    //Indicates that API key should be 32 characters long
    kKTReturnApiKeyIsNot32CharsInLength = -4,
    
    //Indicates that API key must contain only lower case letters
    kKTReturnApiKeyContainsNotLowerCaseChars = -5,
    
    //Indicates that API key must contain only hexadecimal chars
    kKTReturnApiKeyContainsNonHexadecimalChars = -6,
    
    /**
     * Custom ID validation error codes
     */
    
    //Indicates that sanitized (after all non-hexadecimal chars are stripped) Custom ID's length is less than MIN (6 chars by default)
    kKTReturnSanitizedCustomIDMinLengthIsInvalid = -7,
    
    //Indicates that KTConfiguration failed to verify either Required or Optional config keys
    kKTReturnConfigurationGenericError = -8,
    
    //Indicates that FB appId is required to start a session if Customer app is linked against libKontagent_FB.a library
    kKTReturnFBAppIdMissingError = -9
    
};
typedef int32_t KTReturn;
    
#if defined(__cplusplus)
}
#endif


@interface KTParamMap : NSObject {
@private
    NSMutableDictionary* parameters;
}

@property(nonatomic, retain) NSMutableDictionary* parameters;

- (void)put:(NSString*)aKey value:(NSString*)aValue;

- (void)appendWithComma:(NSString*)aKey value:(NSString*)aValue;

- (void)append:(NSString *)aKey value:(NSString*)aValue;

- (NSString*) urlParamString;

+ (KTParamMap*) cloneParamMap: (KTParamMap*)aMap andSkipKeys:(NSArray*)keysToSkip;

@end


/*! \mainpage Kontagent SDK
 *
 * In order to begin usage of SDK, you should perform the next pre-requisite steps:
 *  -# Ensure that you have an API key.
 *  -# Add Kontagent.framework to the project and exclude it from target membership.<br>
 *         It will be linked via -force_load option.  
 *  -# Add the following options to OTHER_LDFLAGS in Xcode:<br>
 *         -framework SystemConfiguration -framework CoreData <br> 
 *         -weak_framework CoreTelephony -weak_framework IOKit <br> 
 *         -force_load "$(PATH_TO_KONTAGENT)/Kontagent.framework/Kontagent"
 *      
 *
 * Before sending any requests to the server, you must create and start a server session:
 * @code
 *      [Kontagent startSession:ApiKey senderId:@"SenderId" mode:kKontagentSDKMode_TEST];
 * @endcode
 * 
 * Parameter "mode" function defines what server sdk will be using: test or production.
 * Parameter "senderId" function can be nil.
 * You can find all necessary constants in <Kontagent/KontagentLibTypes.h>, 
 * <Kontagent/KontagentLib.h> and <Kontagent/KontagentLibReturn.h>. 
 *  
 * When "start" is called for the first time, an "Application added" request will be sent to server.
 *
 * To stop session you need to call:
 * @code
 *      [Kontagent stopSession];
 * @endcode
 * Do not forget to call this "stopSession" on application's exit or when server session is no more needed.
 * "stopSession" writes all unsaved requests to database on the disk.
 *
 * After that, you can instrument your code by calling API methods like this:
 * @code
 *      [Kontagent messageSent:@"1543" trackingId:[Kontagent generateUniqueTrackingTag] optionalParams:nil];
 *      [Kontagent pageRequest:nil];
 * @endcode
 * If you would like to enable/disable SDK logging then you must enable/disable debugging mode:
 * @code
 *      [Kontagent enableDebug];
 *      [Kontagent disableDebug];
 * @endcode
 */

#if defined(__cplusplus)
extern "C" {
#endif
    
/**
 *  SDK Modes 
 */
enum 
{
    kKontagentSDKMode_PRODUCTION  = 0,
    kKontagentSDKMode_TEST        = 1
};        
    
@class KTSession;    
@interface Kontagent : NSObject {
}
/**
 * \brief This method detects if the device is jailbroken or not
 *  @return TRUE - if the device is jailBroken, otherwise - FALSE
 */

+(BOOL) isDeviceJailbroken;

+ (id)instance;

//----------------------------------------------------------
// Versions
//----------------------------------------------------------

/**
 * \brief   Changes the offline message queue size for specific session to a new value.
 *          Beware that this method must be called only after the session is actually started, otherwise it will have no effect
 *  @param[in] newQueueSize   New queue length value
 *  @param[in] apiKey Your desired session API key
 */

+ (void) changeMaxQueueSize:(NSUInteger)newQueueSize forSessionApiKey:(NSString*)apiKey;

/**
 *  \brief  Retrieves current message queue size for given session.
 *   @return Returns MAX integer in case if apiKey is not found, otherwise returns current queue size.
 *  @param[in] apiKey Your desired session API key
 */
+ (NSUInteger) currentMaxQueueSizeForSessionApiKey:(NSString*)apiKey;

+ (NSString*) libraryVersion;

/**
 * \brief   Set SDK mode
 * 
 * @param[in] aMode     PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST).
 */
+ (void) setMode:(NSUInteger)aMode;

/**
 * \brief   First method that should be called initialize SDK 
 * 
 * @param[in] anApiKey  This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aSenderId Unique long UInt64 value in the form of NSString. It can be nil. In such case UID will be generated within SDK.
 *                      anApiKey is used as unique session id. Sender Id is stored to the internal Hash<anApiKey,aSenderId> for further re-use.
 * @param[in] aMode     PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST).
 */
+ (OSStatus) startSession:(NSString*)anApiKey senderId:(NSString*)aSenderId mode:(NSUInteger)aMode;

/**
 * \brief   First method that should be called initialize SDK 
 * 
 * @param[in] anApiKey  This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aMode     PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST).
 */
+ (OSStatus) startSession:(NSString*)anApiKey mode:(NSUInteger)aMode;

/**
 * \brief   First method that should be called initialize SDK 
 * 
 * @param[in] anApiKey  This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aSenderId Unique long UInt64 value in the form of NSString. It can be nil. In such case UID will be generated within SDK.
 *                      anApiKey is used as unique session id. Sender Id is stored to the internal Hash<anApiKey,aSenderId> for further re-use.
 * @param[in] aMode     PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST).
 * @param[in] aFlag     When TRUE - "applicationAdded" request will be sent automatically from "startSession" method.
 */
+ (OSStatus) startSession:(NSString*)anApiKey senderId:(NSString*)aSenderId mode:(NSUInteger)aMode shouldSendApplicationAddedAutomatically:(BOOL)aFlag;

/**
 * \brief   First method that should be called initialize SDK 
 * 
 * @param[in] anApiKey  This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aMode     PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST).
 * @param[in] aFlag     When TRUE - "applicationAdded" request will be sent automatically from "startSession" method.
 */
+ (OSStatus) startSession:(NSString*)anApiKey mode:(NSUInteger)aMode shouldSendApplicationAddedAutomatically:(BOOL)aFlag;

/**
 * \brief   First method that should be called initialize SDK 
 * 
 * @param[in] anApiKey  This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aSenderId Unique long UInt64 value in the form of NSString. It can be nil. In such case UID will be generated within SDK.
 *                      anApiKey is used as unique session id. Sender Id is stored to the internal Hash<anApiKey,aSenderId> for further re-use.
 * @param[in] aMode     PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST).
 * @param[in] aFlag     When TRUE - "applicationAdded" request will be sent automatically from "startSession" method.
 * @param[in] aCustomID customer specified ID which will be a part of su-tag for "UCC" and "APA" messages
 */
+ (OSStatus) startSession:(NSString*)anApiKey senderId:(NSString*)aSenderId mode:(NSUInteger)aMode shouldSendApplicationAddedAutomatically:(BOOL)aFlag customID:(NSString*)aCustomID;

/**
 * \brief   First method that should be called initialize SDK 
 * 
 * @param[in] anApiKey  This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aSenderId Unique long UInt64 value in the form of NSString. It can be nil. In such case UID will be generated within SDK.
 *                      anApiKey is used as unique session id. Sender Id is stored to the internal Hash<anApiKey,aSenderId> for further re-use.
 * @param[in] aMode     PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST).
 * @param[in] aFlag     When TRUE - "applicationAdded" request will be sent automatically from "startSession" method.
 * @param[in] aCustomID customer specified ID which will be a part of su-tag for "UCC" and "APA" messages
 * @param[in] appId     this parameter is required if libKontagent_FB.a is present in runtime. Otherwise can be NULL.
 */

+ (OSStatus) startSession:(NSString*)anApiKey
                 senderId:(NSString*)aSenderId
                     mode:(NSUInteger)aMode
shouldSendApplicationAddedAutomatically:(BOOL)aFlag
                 customID:(NSString*)aCustomID
                  FBAppId:(NSString*)appId;

/**
 * \brief   First method that should be called initialize SDK 
 * 
 * @param[in] aConfiguration  This is dictionary with configuration parameters:
 * <TABLE>
 * <TR>
 *      <TD>Key</TD> 
 *      <TD>Type</TD> <TD>Status</TD>
 *      <TD>Comments</TD>
 * </TR>
 * <TR>
 *      <TD>KT_SESSION_API_KEY_CONFIG_KEY</TD> 
 *      <TD>NSString</TD> <TD>REQUIRED</TD>
 *      <TD>This is autogenerated by the Kontagent system when you setup an application.</TD>
 * </TR>
 * <TR>
 *      <TD>KT_SESSION_CUSTOM_ID_KEY_CONFIG_KEY</TD> 
 *      <TD>NSString</TD> <TD>OPTIONAL</TD>
 *      <TD>Provide Custom ID if you want one to be included into "su" param of "UCC" and "APA" messages.
 *          Please be aware that Custom ID is processed by internal hashing algorithm and will be different to it's initial value.
 *          Custom ID should contain only hexadecimal characters and it's MIN length should be greater than 6.</TD>
 * </TR>

 * <TR>
 *      <TD>KT_SESSION_API_URL_CONFIG_KEY</TD> 
 *      <TD>NSString</TD><TD>OPTIONAL</TD>
 *      <TD>This is an URL to Kontagent system backend. This key overloads built-in URL prefix for PRODUCTION and TEST modes.</TD>
 * </TR>
 * <TR>
 *      <TD>KT_SESSION_SENDER_ID_CONFIG_KEY</TD>
 *      <TD>NSString</TD> <TD>OPTIONAL</TD>    
 *      <TD>Unique long UInt64 value in the form of NSString. By default UID will be generated within SDK.</TD>
 * </TR><TR>
 *      <TD>KT_SESSION_MODE_CONFIG_KEY</TD>     
 *      <TD>NSNumber</TD> <TD>OPTIONAL</TD>    
 *      <TD>PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST). 
 *          Default value is equal to mode that was set with [Kontagent setMode:] function.
 *          kKontagentSDKMode_PRODUCTION is set on SDK init.</TD>
 * </TR><TR>
 *      <TD>KT_SESSION_DEBUG_CONFIG_KEY</TD>         
 *      <TD>NSNumber</TD> <TD>OPTIONAL</TD>    
 *      <TD>This enables certain SDK methods to send messages to the console log.
 *          Debug is disabled by default and should be disabled in production applications.
 *          Default value is 0. To enable DEBUG set value to 1.</TD>
 * </TR><TR>
 *      <TD>KT_SESSION_APA_CONFIG_KEY</TD>           
 *      <TD>NSNumber</TD> <TD>OPTIONAL</TD>    
 *      <TD>This enables SDK to send "applicationAdded" request automatically from "startSession" method. Default value is 1.</TD>
 * </TR>
 * </TR><TR>
 *      <TD>KT_SESSION_APP_VMAJ_KEY</TD>
 *      <TD>NSString</TD> <TD>OPTIONAL</TD>
 *      <TD>Major version number. This represents your version number scheme, and is limited to 50 characters. Valid characters are a-z, A-Z, 0-9, -, and _.</TD>
 * </TR>
 * </TABLE>
 * @return  kKTReturnSuccess on success, otherwise - kKTReturnError.
 */
+ (OSStatus) startSession:(NSDictionary*)aConfiguration;

+ (OSStatus) startSession:(NSString*)anApiKey
                 senderId:(NSString*)aSenderId
                     mode:(NSUInteger)aMode
shouldSendApplicationAddedAutomatically:(BOOL)aFlag
                 customID:(NSString*)aCustomID
        apiKeyForTimezone:(NSString*)apiKeyForTimezone
     apiKeyTimezoneOffset:(NSString*)apiKeyTimezoneOffset;

/**
 * \brief   First method that should be called initialize SDK
 *
 * @param[in] anApiKey  This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aMode     PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST).
 * @param[in] aEnableAcquisitionTracking

 */
+ (OSStatus) startSession:(NSString*)anApiKey mode:(NSUInteger)aMode enableAcquisitionTracking:(BOOL)aEnableAcquisitionTracking;



/**
 * \brief   This method should be called to finalize SDK 
 * 
 */
+ (void) stopSession;


/**
 * \brief   This method detects if it's a first SDK start for current session
 *  @return TRUE - if this is first SDK run, otherwise - FALSE
 */
+ (BOOL) isFirstRun;

/**
 * \brief   This message is sent first time the Kontagent SDK is launched on a device.
 *  
 * See http://www.kontagent.com/docs/technical-leads/api-sdk-spec/ios/applicationadded/
 * 
 * @param[in]   anOptionalParams    Optional Parameters.
 */
+ (void) applicationAdded:(KTParamMap*)anOptionalParams;



/**
 * \brief   This message is sent when an user triggers an application-defined action. 
 *          For more information on how to correctly structure an event, see Understanding Events, Subtypes, and Tags.
 *  
 * See http://www.kontagent.com/docs/technical-leads/api-sdk-spec/ios/customevent/
 * 
 * @param[in]   aName               A name, up to 32 characters long, describing the event 
 *                                  (should only contain characters a-z, A-Z, 0-9, _ ).
 *
 * @param[in]   anOptionalParams    Optional Parameters.
 */
+ (void) customEvent:(NSString*)aName optionalParams:(KTParamMap*)anOptionalParams;

/**
 * \brief   This message provides information for tracking revenue and monetization transactions by users.
 *
 * See http://www.kontagent.com/docs/technical-leads/api-sdk-spec/ios/revenuetracking/
 *
 * @param[in] aValue            The revenue generated for the user in cents. All values must be passed in
 * 
 * @param[in] aValue            The revenue generated for the user in cents. All values must be passed in 
 *                              cents, and not dollars. Example: $1.25 should be passed as 125.
 * @param[in] anOptionalParams  Optional Parameters.
 */
+ (void) revenueTracking:(NSInteger)aValue optionalParams:(KTParamMap*)anOptionalParams;

// Convenience methods that wrap the API.
/**
 * \brief   This method gathers manufacturer, model, os version, and carrier on launch.
 *
 */
+ (void) sendDeviceInformation:(KTParamMap*)anOptionalParams;

/**
 * \brief   This method sends a ucc message.
 *
 */
+ (void) sendUndirectedCommClick:(KTParamMap*)anOptionalParams;


/**
 * \brief   This method gets the api key.
 *
 */
+ (NSString*) getApiKey;

/**
 * \brief   Helper method to generate a 16 char tracking tag.
 * 
 * @return A 16-digit unique hexadecimal string (0-9, A-F) tracking tag.
 */
+ (NSString*) generateUniqueTrackingTag;

/**
 * \brief   This enables certain SDK methods to send messages to the console log.  
 *          Debug is disabled by default and should be disabled in production applications.
 * 
 */
+ (void) enableDebug;

/**
 * \brief   Disables debugging features.
 */
+ (void) disableDebug;

/**
 * \brief   Determines whether the application is in debug mode.
 * @return  TRUE if debug is enabled, otherwise FALSE.  Default is FALSE.
 */
+ (BOOL) debugEnabled;

/**
 * \brief   Return senderId which corresponds to ApiKey
 * 
 * @param[in] anApiKey  This is autogenerated by the Kontagent system when you setup an application.
 */

+ (NSString*) getSenderId:(NSString*)anApiKey;

/**
 * \brief   This method allows you to change sender ID value for current session.
 * Beware, that calling this method with "null" param will fallback sender ID to the value specified in
 * start/create session call or to SDK generated value if senderID wasn't initially specified.
 * @param[in] theSenderId New sender ID value
 * @param[in] apiKey desired session API key
 */
+ (void) setSenderId:(NSString*)theSenderId forApiKey:(NSString*)apiKey;

/**
 * \brief   This method should be called to create SDK session
 * 
 * @param[in] anApiKey    This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aSenderId   
 */
+ (KTSession*) createSession:(NSString*)anApiKey senderId:(NSString*)aSenderId;

/**
 * \brief   This method should be called to create SDK session
 * 
 * @param[in] anApiKey    This is autogenerated by the Kontagent system when you setup an application.
 * @param[in] aSenderId
 * @param[in] aFlag       When TRUE - "applicationAdded" request will be sent automatically from "start" method of KTSession object.
 */

+ (KTSession*) createSession:(NSString*)anApiKey senderId:(NSString*)aSenderId shouldSendApplicationAddedAutomatically:(BOOL)aFlag;

/**
 * \brief   This method should be called to create SDK session
 * 
 * @param[in] aConfiguration  This is dictionary with configuration parameters:
 * <TABLE>
 * <TR>
 *      <TD>Key</TD> 
 *      <TD>Type</TD> <TD>Status</TD>
 *      <TD>Comments</TD>
 * </TR>
 * <TR>
 *      <TD>KT_SESSION_API_KEY_CONFIG_KEY</TD> 
 *      <TD>NSString</TD> <TD>REQUIRED</TD>
 *      <TD>This is autogenerated by the Kontagent system when you setup an application.</TD>
 * </TR>
 * <TR>
 *      <TD>KT_SESSION_API_URL_CONFIG_KEY</TD> 
 *      <TD>NSString</TD><TD>OPTIONAL</TD>
 *      <TD>This is an URL to Kontagent system backend. This key overloads built-in URL prefix for PRODUCTION and TEST modes.</TD>
 * </TR>
 * <TR>
 *      <TD>KT_SESSION_SENDER_ID_CONFIG_KEY</TD>
 *      <TD>NSString</TD> <TD>OPTIONAL</TD>    
 *      <TD>Unique long UInt64 value in the form of NSString. By default UID will be generated within SDK.</TD>
 * </TR><TR>
 *      <TD>KT_SESSION_MODE_CONFIG_KEY</TD>     
 *      <TD>NSNumber</TD> <TD>OPTIONAL</TD>    
 *      <TD>PRODUCTION or TEST (kKontagentSDKMode_PRODUCTION/kKontagentSDKMode_TEST). 
 *          Default value is equal to mode that was set with [Kontagent setMode:] function.
 *          kKontagentSDKMode_PRODUCTION is set on SDK init.</TD>
 * </TR><TR>
 *      <TD>KT_SESSION_DEBUG_CONFIG_KEY</TD>         
 *      <TD>NSNumber</TD> <TD>OPTIONAL</TD>    
 *      <TD>This enables certain SDK methods to send messages to the console log.
 *          Debug is disabled by default and should be disabled in production applications.
 *          Default value is 0. To enable DEBUG set value to 1.</TD>
 * </TR><TR>
 *      <TD>KT_SESSION_APA_CONFIG_KEY</TD>           
 *      <TD>NSNumber</TD> <TD>OPTIONAL</TD>    
 *      <TD>This enables SDK to send "applicationAdded" request automatically from "startSession" method. Default value is 1.</TD>
 * </TR>
 * </TABLE>
 * @return  KTSession object reference if creation was successful, otherwise nil.
 */
+ (KTSession*) createSession:(NSDictionary*)aConfiguration;


@end
    
#if defined(__cplusplus)
}
#endif


/**
 * @page session_api Session API of Kontagent SDK
 *
 * 
 * Before sending any requests to the server, you must create and start a server session:
 * @code
 *      [Kontagent setMode:kKontagentSDKMode_TEST];
 *      KTSession* theSession = [Kontagent createSession:@"ApiKey" senderId:@"SenderId"];
 *      [theSession start];
 * @endcode
 * 
 * Parameter of "setMode" function defines what server sdk will be using: test or production. 
 * You can find all necessary constants in <Kontagent/KontagentLibTypes.h>, 
 * <Kontagent/KontagentLib.h> and <Kontagent/KontagentLibReturn.h>. 
 *  
 * When "start" is called for the first time, an "Application added" request will be sent to server.
 *
 * To stop session you need to call:
 * @code
 *      [theSession stop] 
 * @endcode
 * Do not forget to call this "stop" on application's exit or when server session is no more needed.
 * "stop" writes all unsaved requests to database on the disk.
 *
 * After that, you can instrument your code by calling API methods like this:
 * @code
 *      [theSession messageSent:@"1543" trackingId:[Kontagent generateUniqueTrackingTag] optionalParams:nil];
 *      [theSession pageRequest:nil];
 * @endcode
 * If you would like to delete particular session object simply call:
 * @code
 *      [theSession release];
 * @endcode
 */

#if defined(__cplusplus)
extern "C" {
#endif
    
@interface KTSession : NSObject {
@private
    NSString*  apiKey;
    NSString*  senderId;
    NSUInteger sdkMode;
}

/**
 * \brief   Changes the offline message queue size to a new value.
 *          Beware that this method must be called only after the session is actually started, otherwise it will have no effect
 *  @param[in] newQueueSize   New queue length value
 */
- (void) changeMaxQueueSize: (NSUInteger)newQueueSize;

/**
 *  \brief  Retrieves current maximum message queue size for given session.
 *   @return Returns MAX integer in case if apiKey is not found, otherwise returns current queue size.
 */

- (NSUInteger) currentMaxQueueSize;


/**
 * \brief   Start SDK session 
 * 
 */
- (OSStatus) start;

/**
 * \brief   Stop SDK session 
 * 
 */
- (void) stop;

/**
 * \brief   This message is sent first time the Kontagent SDK is launched on a device.
 *  
 * See http://www.kontagent.com/docs/api-libraries/rest-api-advanced/application-added/
 * 
 * @param[in]   anOptionalParams    Optional Parameters.
 */
- (void) applicationAdded:(KTParamMap*)anOptionalParams;

/**
 * \brief   This message is sent when an user triggers an application-defined action. 
 *          For more information on how to correctly structure an event, see Understanding Events, Subtypes, and Tags.
 *  
 * See http://www.kontagent.com/docs/api-libraries/rest-api-advanced/custom-event/
 * 
 * @param[in]   aName               A name, up to 32 characters long, describing the event 
 *                                  (should only contain characters a-z, A-Z, 0-9, _ ).
 *
 * @param[in]   anOptionalParams    Optional Parameters.
 */
- (void) customEvent:(NSString*)aName optionalParams:(KTParamMap*)anOptionalParams;

/**
 * \brief   This message provides information for tracking revenue and monetization transactions by users.
 *
 * See http://www.kontagent.com/docs/api-libraries/rest-api-advanced/revenue-tracking/
 *
 * @param[in] aValue            The revenue generated for the user in cents. All values must be passed in
 *                              cents, and not dollars. Example: $1.25 should be passed as 125.
 * @param[in] anOptionalParams  Optional Parameters.
 */
- (void) revenueTracking:(NSInteger)aValue optionalParams:(KTParamMap*)anOptionalParams;

// Convenience methods that wrap the API.
/**
 * \brief   This method gathers manufacturer, model, os version, and carrier on launch.
 *
 */
- (void) sendDeviceInformation:(KTParamMap*)anOptionalParams;

/**
 * \brief   This method sends a ucc message.
 *
 */
- (void) sendUndirectedCommClick:(KTParamMap*)anOptionalParams;


/**
 * \brief   This gets the api key.
 *
 */
- (NSString*) getApiKey;

@end    
    
#if defined(__cplusplus)
}
#endif

#endif // KT_KONTAGENT_H
