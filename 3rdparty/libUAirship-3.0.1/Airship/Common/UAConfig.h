/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 EVENT SHALL URBAN AIRSHIP INC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Foundation/Foundation.h>

#import "UAGlobal.h"

/**
 * The UAConfig object provides an interface for passing common configurable values to [UAirship takeOff].
 * The simplest way to use this class is to add an AirshipConfig.plist file in your app's bundle and set
 * the desired options. The plist keys use the same names as the properties on this class. Older, all-caps
 * keys are still supported, but you should migrate your properties file to make use of a number of new options.
 */
@interface UAConfig : NSObject

///---------------------------------------------------------------------------------------
/// @name Resolved Values
///---------------------------------------------------------------------------------------

/**
 * The current app key (resolved using the inProduction flag).
 */
@property (weak, nonatomic, readonly) NSString *appKey;

/**
 * The current app secret (resolved using the inProduction flag).
 */
@property (weak, nonatomic, readonly) NSString *appSecret;

/**
 * The current log level for the library's UA_L<level> macros (resolved using the inProduction flag).
 */
@property (nonatomic, readonly) UALogLevel logLevel;

/**
 * The production status of this application. This may be set directly, or it may be determined
 * automatically if the detectProvisioningMode flag is set to `YES`.
 */
@property (nonatomic, assign) BOOL inProduction;

/**
 * Toggles Urban Airship analytics. Defaults to `YES`. If set to `NO`, many UA features will not be
 * available to this application.
 */
@property (nonatomic, assign) BOOL analyticsEnabled;

///---------------------------------------------------------------------------------------
/// @name Configuration Values
///---------------------------------------------------------------------------------------

/**
 * The development app key. This should match the application on go.urbanairship.com that is
 * configured with your development push certificate.
 */
@property (nonatomic, copy) NSString *developmentAppKey;

/**
 * The development app secret. This should match the application on go.urbanairship.com that is
 * configured with your development push certificate.
 */
@property (nonatomic, copy) NSString *developmentAppSecret;

/**
 * The production app key. This should match the application on go.urbanairship.com that is
 * configured with your production push certificate. This is used for App Store, Ad-Hoc and Enterprise
 * app configurations.
 */
@property (nonatomic, copy) NSString *productionAppKey;

/**
 * The production app secret. This should match the application on go.urbanairship.com that is
 * configured with your production push certificate. This is used for App Store, Ad-Hoc and Enterprise
 * app configurations.
 */
@property (nonatomic, copy) NSString *productionAppSecret;

/**
 * The log level used for development apps. Defaults to `UALogLevelDebug` (4).
 */
@property (nonatomic, assign) UALogLevel developmentLogLevel;

/**
 * The log level used for production apps. Defaults to `UALogLevelError` (1).
 */
@property (nonatomic, assign) UALogLevel productionLogLevel;


/**
 * The size in MB for the UA Disk Cache.  Defaults to 100.
 *
 * Only items that are small enough (1/20th of the size) of the cache will be 
 * cached.
 * 
 * Any size greater than 0 will cause the UA Disk Cache to become active. 
 * UAURLProtocol will be registered as a NSURLProtocol.  Only requests whose
 * mainDocumentURL or URL that have been added as a cachable URL will be considered
 * for caching.  By defualt it includes all of the Rich Application Page URLs.
 *
 */
@property (nonatomic, assign) NSUInteger cacheDiskSizeInMB;

/**
 * If enabled, the UA library automatically registers for remote notifications when push is enabled
 * and intercepts incoming notifications in both the foreground and upon launch.
 *
 * Defaults to YES. If this is disabled, you will need to register for remote notifications
 * in application:didFinishLaunchingWithOptions: and forward all notification-related app delegate
 * calls to UAPush and UAInbox.
 */
@property (nonatomic, assign) BOOL automaticSetupEnabled;

///---------------------------------------------------------------------------------------
/// @name Advanced Configuration Options
///---------------------------------------------------------------------------------------


/**
 * Apps may be set to self-configure based on the APS-environment set in the embedded.mobileprovision file.
 * If `YES`, the inProduction value will be determined at runtime by reading the provisioning profile. If
 * `NO`, the inProduction flag may be set directly or using the AirshipConfig.plist file. Defaults to
 * `YES` for safety so that the production keys will always be used if the profile cannot be read in a released app.
 * Simulator builds do not include profile, so this flag does not have any effect in cases where there is not
 * a profile present. It will fall back to the inProduction property as set in code or a .plist file.
 */
@property (nonatomic, assign) BOOL detectProvisioningMode;

/**
 * If set to `YES`, the app will clear all keychain user information every time the app starts.
 * This is designed for development mode only.
 */
@property (nonatomic, assign) BOOL clearKeychain;


/**
 * The Urban Airship device API url. This option is reserved for internal debugging.
 */
@property (nonatomic, copy) NSString *deviceAPIURL;

/**
 * The Urban Airship analytics API url. This option is reserved for internal debugging.
 */
@property (nonatomic, copy) NSString *analyticsURL;


///---------------------------------------------------------------------------------------
/// @name Factory Methods
///---------------------------------------------------------------------------------------

/**
 * Creates an instance using the values set in the `AirshipConfig.plist` file.
 */
+ (UAConfig *)defaultConfig;

/**
 * Creates an instance using the values found in the specified `.plist` file.
 * @param path The path of the specified file.
 */
+ (UAConfig *)configWithContentsOfFile:(NSString *)path;

/**
 * Creates an instance with empty values.
 */
+ (UAConfig *)config;

///---------------------------------------------------------------------------------------
/// @name Utilities, Helpers
///---------------------------------------------------------------------------------------


/**
 * Validates the current configuration. In addition to performing a strict validation, this method
 * will log warnings and common configuration errors.
 * @return `YES` if the current configuration is valid, otherwise `NO`.
 */
- (BOOL)validate;

@end
