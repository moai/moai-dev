/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 PHConstants.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/14/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <UIKit/UIImage.h>

/** @file */

// Constants
#define PH_SDK_VERSION @"1.13.1"

#ifndef PH_BASE_URL
#define PH_BASE_URL @"http://api2.playhaven.com"
#endif

#ifndef PH_CONTENT_ADDRESS
#define PH_CONTENT_ADDRESS @"http://media.playhaven.com"
#endif


/**
 * PHContentView notification that a callback is ready for processing
 **/
// TODO: Can we safely rename this to put a space between the PH and CONTENTVIEW words?
#define PHCONTENTVIEW_CALLBACK_NOTIFICATION  @"PHContentViewPHCallbackNotification"
#define PH_PREFETCH_CALLBACK_NOTIFICATION   @"PHPrecacherPHCallbackNotification"


/**
 * Defines characteristics of the requests that get sent from content units to
 * native code. See content-templates:src/js/playhaven.js for template impl.
 *
 * * \b 1: GET request with dispatch parameter keys and values in query string
 * * \b 2: GET request with dispatch parameters encoded as a single JSON string
 *      in query string. Rewards support requires this setting.
 * * \b 3: Unknown dispatches are ignored instead of throwing an error
 * * \b 4: ph://launch dispatches no longer create native spinner views
 * * \b 5: ph://launch dispatches support in-app app store launches on iOS 6 <em>(Current Version)</em>
 **/
#define PH_DISPATCH_PROTOCOL_VERSION 5

/**
 * Defines the maximum amount of time that an API request will wait for a
 * response from the server.
 **/
#define PH_REQUEST_TIMEOUT 10

/**
 * Defines the maximum amount of time that an API request will wait for a
 * response from the server.
 **/
#ifdef DEBUG
#define PH_PREFETCHING_TIMEOUT 30
#else
#define PH_PREFETCHING_TIMEOUT 10
#endif

/**
 * Recycles content view instances to reduce the number of allocations.
 * Behavior of the SDK without this define has not been tested.
 **/
#define PH_USE_CONTENT_VIEW_RECYCLING

/**
 * By default, content requests are dismissed when the app is backgrounded.
 * Set \c PH_DONT_DISMISS_WHEN_BACKGROUNDED as a preprocessor macro to disable this behavior.
 **/
#ifndef PH_DONT_DISMISS_WHEN_BACKGROUNDED
#define PH_DISMISS_WHEN_BACKGROUNDED
#endif

/**
 * By default, PlayHaven will require the StoreKit framework. Builds that don't need
 * IAP tracking features may define \c PH_USE_STOREKIT to be 0.
 **/
#ifndef PH_USE_STOREKIT
#define PH_USE_STOREKIT 1
#endif


/**
 * By default, PlayHaven will require the AdSupport framework. Projects using a version of
 * Xcode older than 4.5 may define \c PH_USE_AD_SUPPORT to be 0.
 *
 * @note By disabling the AdSupport framework, the SDK will not be able to collect the IFA
 **/
#ifndef PH_USE_AD_SUPPORT
#define PH_USE_AD_SUPPORT 1
#endif


/**
 * @name Namespacing
 * The Unity3D plugin requires namespaced classes so that they may co-exist
 * alongside libraries that use the same classes. However, source builds that are
 * also using these classes should be able to use the existing implementations.
 **/
/*@{*/
#ifdef PH_NAMESPACE_LIBS
#define PH_NAMESPACE_SBJSON     1
#define PH_NAMESPACE_SDURLCACHE 1
#endif

/**
 * The Unity3D plugin requires namespaced SBJSON classes so that they may co-exist
 * alongside libraries that use SBJSON. However, source builds that are also using
 * SBJSON should be able to use the SBJSON classes already in their project. This
 * allows the SDK to accommodate both.
 **/
#ifndef PH_NAMESPACE_SBJSON
#define PH_SBJSONBASE_CLASS        SBJsonBase
#define PH_SBJSONPARSER_CLASS      SBJsonParser
#define PH_SBJSONWRITER_CLASS      SBJsonWriter
#define PH_SBJSONERRORDOMAIN_CONST SBJSONErrorDomain
#else
#define PH_SBJSONBASE_CLASS        SBJsonBasePH
#define PH_SBJSONPARSER_CLASS      SBJsonParserPH
#define PH_SBJSONWRITER_CLASS      SBJsonWriterPH
#define PH_SBJSONERRORDOMAIN_CONST SBJSONErrorDomainPH
#endif

/**
 * The \c PH_NAMESPACE_SDURLCACHE Macros
 **/
#ifndef PH_NAMESPACE_SDURLCACHE
#define PH_SDURLCACHE_CLASS          SDURLCache
#define PH_SDCACHEDURLRESPONSE_CLASS SDCachedURLResponse
#else
#define PH_SDURLCACHE_CLASS          SDURLCachePH
#define PH_SDCACHEDURLRESPONSE_CLASS SDCachedURLResponsePH
#endif

/**
 * Sends UDID with each request. To disable, set to 0.
 **/
#ifndef PH_USE_UNIQUE_IDENTIFIER
#define PH_USE_UNIQUE_IDENTIFIER 0
#endif

/**
 * Sends the device's MAC address with each request. To disable, set to 0.
 **/
#ifndef PH_USE_MAC_ADDRESS
#define PH_USE_MAC_ADDRESS 1
#endif

// Macros
#define PH_URL(PATH) [PH_BASE_URL stringByAppendingString:@#PATH]
#define PH_URL_FMT(PATH,FMT) [PH_BASE_URL stringByAppendingFormat:@#PATH, FMT]

#ifndef PH_LOG
#define PH_LOG(COMMENT,...) NSLog(@"[PlayHaven-%@] %@", PH_SDK_VERSION, [NSString stringWithFormat:COMMENT, __VA_ARGS__])
#endif

#ifndef PH_NOTE
#define PH_NOTE(COMMENT) NSLog(@"[PlayHaven-%@] %@", PH_SDK_VERSION, COMMENT)
#endif

#ifndef PH_DEBUG_LOG
#define PH_DEBUG_LOG 0
#endif

#if PH_DEBUG_LOG == 1
#define PH_DEBUG(fmt, ...) NSLog((@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define PH_DEBUG(...)
#endif

#define PH_MULTITASKING_SUPPORTED [[UIDevice currentDevice] respondsToSelector:@selector(isMultitaskingSupported)] && [[UIDevice currentDevice] isMultitaskingSupported]

NSString *PHGID(void);
void PHClearGID(void);

// Errors
typedef enum {
    PHAPIResponseErrorType,
    PHRequestResponseErrorType,
    PHOrientationErrorType,
    PHLoadContextErrorType,
    PHWindowErrorType,
    PHIAPTrackingSimulatorErrorType,
    PHRequestDigestErrorType,
} PHErrorType;

NSError *PHCreateError(PHErrorType errorType);


/**
 * Determines the status of the device's connectivity.
 *
 * @returns
 * * 0: No connection
 * * 1: Cellular data, 3G/EDGE
 * * 2: WiFi
 **/
int PHNetworkStatus(void);

NSString *PHAgnosticStringValue(id object);

// Caching constant definitions

#define PH_PREFETCH_URL_PLIST @"prefetchCache.plist"

#define PH_MAX_CONCURRENT_OPERATIONS    2

#define PH_MAX_SIZE_MEMORY_CACHE        1024 * 1024          // 1MB mem cache
#define PH_MAX_SIZE_FILESYSTEM_CACHE    1024 * 1024 * 10     // 10MB disk cache

/**
 * Play Haven default images
 **/
typedef struct {
    int width;
    int height;
    int length;
    char data[];

} playHavenImage;


/**
 * Play Haven default image helper functions
 **/
UIImage *convertByteDataToUIImage(playHavenImage *phImage);

/**
 * Return true if the device has a retina display, false otherwise. Use this to load @2x images
 **/
#define IS_RETINA_DISPLAY() [[UIScreen mainScreen] respondsToSelector:@selector(scale)] && [[UIScreen mainScreen] scale] == 2.0f

extern const playHavenImage badge_image;
extern const playHavenImage badge_2x_image;
extern const playHavenImage close_image;
extern const playHavenImage close_active_image;

NSString *PHGID();
void PHClearGID();
