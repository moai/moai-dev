//
//  Crittercism.h
//  Crittercism iOS Library
//
//  Copyright 2010-2013 Crittercism, Inc. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "CrittercismDelegate.h"
#import "CRFilter.h"

// Operating System Support
//
// The Crittercism iOS library supports iOS v5.0+

// Additional Requirements:
//
// Crittercism requires that you link to the SystemConfiguration framework.

// Basic Integration
//
// In your Application Delegate's application:didFinishLaunchingWithOptions:
// method, add a call to +[Crittercism enableWithAppID:], supplying the app ID
// of an app you created via the Crittercism Web Portal.
//
// Example:
//
// [Crittercism enableWithAppID:@"YOURAPPIDGOESHERE"];

@class CLLocation;

@interface Crittercism : NSObject

// Enabling Crittercism
//
// !! You must call one of these methods before using any other Crittercism
// functionality. !!
//
// You can pass in a delegate that conforms to the CrittercismDelegate protocol
// if you wish to be notified that your app crashed on the previous load.
// This protocol currently has only one method:
//
//   - (void)crittercismDidCrashOnLastLoad;
//
// Alternatively, you can check the didCrashOnLastLoad BOOL property.
//
// The filters parameter can be used to prevent sensitive URLs from being
// captured by the network instrumentation. To use this, pass in an
// NSArray of CRFilter objects that will be matched against URLs captured by the
// library.
//
// (Note - the filtering will take place off of your application's main thread.)

+ (void)enableWithAppID:(NSString *)appId;

+ (void)enableWithAppID:(NSString *)appId
            andDelegate:(id <CrittercismDelegate>)critterDelegate;

+ (void)enableWithAppID:(NSString *)appId
            andDelegate:(id <CrittercismDelegate>)critterDelegate
          andURLFilters:(NSArray *)filters;

+ (void)enableWithAppID:(NSString *)appId
          andURLFilters:(NSArray *)filters;

// Designated "initializer"
+ (void)enableWithAppID:(NSString *)appId
            andDelegate:(id <CrittercismDelegate>)critterDelegate
          andURLFilters:(NSArray *)filters
 disableInstrumentation:(BOOL)disableInstrumentation;

// Adds an additional filter for network instrumentation.
// See CRFilter.h for additional details.
+ (void)addFilter:(CRFilter *)filter;

// Breadcrumbs provide the ability to track activity within your app.
//
// A breadcrumb is a free form string you supply, which will be timestamped,
// and stored in case a crash occurs. Crash reports will contain the breadcrumb
// trail from the run of your app that crashed, as well as that of the prior
// run.
//
// Breadcrumbs are limited to 140 characters, and only the most recent 100 are
// kept. Crittercism will automatically insert a breadcrumb marked
//   "session_start"
// on each initial launch, or foreground of your app.
//
// Note - Breadcrumbs are an Enterprise level feature.

+ (void)leaveBreadcrumb:(NSString *)breadcrumb;

// By default, breadcrumbs are flushed to disk immediately when written.
// This is by design - in order to provide an accurate record of everything
// that happened up until the point your app crashed.
//
// If you are concerned about the performance costs of writing the file, you can
// instruct the library to perform all breadcrumb writes on a background thread.

+ (void)setAsyncBreadcrumbMode:(BOOL)writeAsync;

// Inform Crittercism of the device's most recent location for use with
// performance monitoring.
+ (void)updateLocation:(CLLocation *)location;

// Handled exceptions are a way of reporting exceptions your app intentionally
// caught. If the passed in NSException object was @thrown, the stack trace
// of the thread that threw the exception will be displayed on the Crittercism
// web portal.

// Reporting of handled exceptions is throttled to once per minute. During
// that minute period, up to 5 handled exceptions will be buffered.

// Note - Handled exceptions are a Premium level feature.

+ (BOOL)logHandledException:(NSException *)exception;

// If you wish to offer your users the ability to opt out of Crittercism
// crash reporting, you can set the OptOutStatus to YES. If you do so, any
// pending crash reports will be purged.

+ (void)setOptOutStatus:(BOOL)status;

// Retrieve current opt out status.

+ (BOOL)getOptOutStatus;

// Set the maximum number of crash reports that will be stored on
// the local device if the device does not have internet connectivity. If
// more than |maxOfflineCrashReports| crashes occur, the oldest crash will be
// overwritten. Decreasing the value of this setting will not delete
// any offline crash reports. Unsent crash reports will be kept until they are
// successfully transmitted to Crittercism, hence there may be more than
// |maxOfflineCrashReports| stored on the device for a short period of time.
//
// The default value is 3, and there is an upper bound of 10.

+ (void)setMaxOfflineCrashReports:(NSUInteger)max;

// Get the maximum number of Crittercism crash reports that will be stored on
// the local device if the device does not have internet connectivity.

+ (NSUInteger)maxOfflineCrashReports;

// Get the Crittercism generated unique identifier for this device.
// !! This is NOT the device's UDID.
//
// If called before enabling the library, this will return an empty string.
//
// All Crittercism enabled apps on a device will share the UUID created by the
// first installed Crittercism enabled app.
//
// If all Crittercism enabled applications are removed from a device, a new
// UUID will be generated when the next one is installed.

+ (NSString *)getUserUUID;

// Associate a username string with the device's Crittercism UUID. This will
// send the association to Crittercism's back end.

+ (void)setUsername:(NSString *)username;

// Associate an arbitrary key/value pair with the device's Crittercism UUID.

+ (void)setValue:(NSString *)value forKey:(NSString *)key;

// Crittercism delegate property

+ (id <CrittercismDelegate>)delegate;

+ (void)setDelegate:(id <CrittercismDelegate>)delegate;

// Did the application crash on the previous load?

+ (BOOL)didCrashOnLastLoad;

@end
