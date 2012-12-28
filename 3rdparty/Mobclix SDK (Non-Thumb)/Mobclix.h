//
//  Mobclix.h
//  Mobclix iOS SDK
//
//  Copyright 2011 Mobclix. All rights reserved.
//

typedef enum {
    LOG_LEVEL_VERBOSE =	1 << 0,
	LOG_LEVEL_DEBUG =	1 << 1,
	LOG_LEVEL_INFO	=	1 << 2,
	LOG_LEVEL_WARN	=	1 << 3,
	LOG_LEVEL_ERROR	=	1 << 4
	
} MobclixLogLevel;

@class CLLocation;
@interface Mobclix : NSObject {
	
}

/**
 * Start up Mobclix using the applicationId provided in your application's Info.plist
 * This should be called in your AppDelegate's applicationDidFinishLaunching: method.
 */
+ (void)start;

/**
 * Start up Mobclix with a different applicationId
 * This should be called in your AppDelegate's applicationDidFinishLaunching: method.
 */
+ (void)startWithApplicationId:(NSString*)applicationId;

#pragma mark -
#pragma mark Location
/**
 * Location information should only be sent to Mobclix if your application uses locations services.
 * If your application does not make use of location services, and you track the users location
 * your application will be rejected by Apple.
 */

/**
 * Update location
 */
+ (void)updateLocation:(CLLocation*)location;

#pragma mark -
#pragma mark Piracy Detection

/**
 * Checks to see if the current running app is cracked.
 * Use with discretion.  As with any piracy detection system, there is a risk of
 * legitimate users being detected as pirated.
 */
+ (BOOL)isApplicationCracked;

/**
 * Version of this Mobclix iOS SDK.
 *
 */
+ (NSString*) version;

@end

