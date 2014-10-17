/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC ``AS IS'' AND ANY EXPRESS OR
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

#import <UIKit/UIKit.h>

typedef enum _UALogLevel {
    UALogLevelUndefined = -1,
    UALogLevelNone = 0,
    UALogLevelError = 1,
    UALogLevelWarn = 2,
    UALogLevelInfo = 3,
    UALogLevelDebug = 4,
    UALogLevelTrace = 5
} UALogLevel;


#define UA_LEVEL_LOG_THREAD(level, levelString, fmt, ...) \
    do { \
        if (uaLoggingEnabled && uaLogLevel >= level) { \
            NSString *thread = ([[NSThread currentThread] isMainThread]) ? @"M" : @"B"; \
            NSLog((@"[%@] [%@] => %s [Line %d] " fmt), levelString, thread, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

#define UA_LEVEL_LOG_NO_THREAD(level, levelString, fmt, ...) \
    do { \
        if (uaLoggingEnabled && uaLogLevel >= level) { \
            NSLog((@"[%@] %s [Line %d] " fmt), levelString, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

//only log thread if #UA_LOG_THREAD is defined
#ifdef UA_LOG_THREAD
#define UA_LEVEL_LOG UA_LEVEL_LOG_THREAD
#else
#define UA_LEVEL_LOG UA_LEVEL_LOG_NO_THREAD
#endif

extern BOOL uaLoggingEnabled; // Default is true
extern UALogLevel uaLogLevel; // Default is UALogLevelError

#define UA_LTRACE(fmt, ...) UA_LEVEL_LOG(UALogLevelTrace, @"T", fmt, ##__VA_ARGS__)
#define UA_LDEBUG(fmt, ...) UA_LEVEL_LOG(UALogLevelDebug, @"D", fmt, ##__VA_ARGS__)
#define UA_LINFO(fmt, ...) UA_LEVEL_LOG(UALogLevelInfo, @"I", fmt, ##__VA_ARGS__)
#define UA_LWARN(fmt, ...) UA_LEVEL_LOG(UALogLevelWarn, @"W", fmt, ##__VA_ARGS__)
#define UA_LERR(fmt, ...) UA_LEVEL_LOG(UALogLevelError, @"E", fmt, ##__VA_ARGS__)

#define UALOG UA_LDEBUG

// constants
#define kAirshipProductionServer @"https://device-api.urbanairship.com"
#define kAnalyticsProductionServer @"https://combine.urbanairship.com";

#ifdef _UA_VERSION
#define UA_VERSION @ _UA_VERSION
#else
#define UA_VERSION @ "1.1.2"
#endif

#define UA_VERSION_INTERFACE(CLASSNAME) \
@interface CLASSNAME : NSObject         \
+ (NSString *)get;                      \
@end


#define UA_VERSION_IMPLEMENTATION(CLASSNAME, VERSION_STR)   \
@implementation CLASSNAME                                   \
+ (NSString *)get {                                         \
return VERSION_STR;                                         \
}                                                           \
@end


#define SINGLETON_INTERFACE(CLASSNAME)                                                      \
+ (CLASSNAME*)shared;                                                                       \

#define SINGLETON_IMPLEMENTATION(CLASSNAME)                                                 \
                                                                                            \
static CLASSNAME* g_shared##CLASSNAME = nil;                                                \
\
+ (CLASSNAME*)shared                                                                        \
{                                                                                           \
static dispatch_once_t sharedOncePredicate##CLASSNAME;                                                 \
\
dispatch_once(&sharedOncePredicate##CLASSNAME, ^{                                                      \
g_shared##CLASSNAME = [[self alloc] init];                                                  \
});                                                                                         \
return g_shared##CLASSNAME;                                                                 \
}                                                                                           \
\
+ (id)allocWithZone:(NSZone*)zone                                                           \
{                                                                                           \
static dispatch_once_t allocOncePredicate##CLASSNAME;                                                  \
dispatch_once(&allocOncePredicate##CLASSNAME, ^{                                                       \
if (g_shared##CLASSNAME == nil) {                                                           \
g_shared##CLASSNAME = [super allocWithZone:zone];                                           \
}                                                                                           \
});                                                                                         \
return g_shared##CLASSNAME;                                                                 \
}                                                                                           \
\
- (id)copyWithZone:(NSZone*)zone                                                            \
{                                                                                           \
return self;                                                                                \
}                                                                                           \

// TODO: Remove this when its actually available
#ifndef kCFCoreFoundationVersionNumber_iOS_7_0
#define kCFCoreFoundationVersionNumber_iOS_7_0 847.0
#endif

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 70000
#define IF_IOS7_OR_GREATER(...) \
    if (kCFCoreFoundationVersionNumber >= kCFCoreFoundationVersionNumber_iOS_7_0) \
    { \
        __VA_ARGS__ \
    }
#else
#define IF_IOS_7_OR_GREATER(...)
#endif

#define UA_SUPPRESS_PERFORM_SELECTOR_LEAK_WARNING(THE_CODE) \
do { \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Warc-performSelector-leaks\"") \
THE_CODE; \
_Pragma("clang diagnostic pop") \
} while (0)

