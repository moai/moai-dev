// Prefs.h -- application preferences
// by allen brunson  february 3 2012


/******************************************************************************/
/*                                                                            */
/***  Prefs class                                                           ***/
/*                                                                            */
/******************************************************************************/

@interface Prefs: NSObject
{
}

// class methods

+(void)resetDefaultsDebug;
+(void)resetDefaultsSettings;

// prefs methods

+(VGAdOrientation)adOrientationGet;
+(void)adOrientationSet:(VGAdOrientation)orient;

+(VGAdType)adTypeGet;
+(void)adTypeSet:(VGAdType)adType;

+(BOOL)alwaysShowCloseGet;
+(void)alwaysShowCloseSet:(BOOL)state;

+(NSString*)fakeURLGet;
+(void)fakeURLSet:(NSString*)URL;

+(BOOL)forceOldPlayerGet;
+(void)forceOldPlayerSet:(BOOL)state;

+(BOOL)locationEnabledGet;
+(void)locationEnabledSet:(BOOL)state;

+(BOOL)memoryReportGet;
+(void)memoryReportSet:(BOOL)state;

+(BOOL)mutedGet;
+(void)mutedSet:(BOOL)state;

+(NSString*)pubAppIDGet;
+(void)pubAppIDSet:(NSString*)pubAppID;

+(BOOL)purgeTestGet;
+(void)purgeTestSet:(BOOL)state;

+(BOOL)expiryTestGet;
+(void)expiryTestSet:(BOOL)state;

+(BOOL)removeCacheGet;
+(void)removeCacheSet:(BOOL)state;

+(BOOL)resumeTestGet;
+(void)resumeTestSet:(BOOL)state;

+(BOOL)showJSONGet;
+(void)showJSONSet:(BOOL)state;

+(BOOL)skipMarkViewedGet;
+(void)skipMarkViewedSet:(BOOL)state;

/**
 * To get old baseUrl value form preferences
 */
+ (VGConfigBaseUrl)adBaseUrlGet;

/**
 * To set new baseUrl value in preference. It also notify VungleSDK
 * about this url change by calling SDK method. 
 */
+ (void)adBaseUrlSet:(VGConfigBaseUrl)url;

/**
 * To get old Hide Status Bar value form preferences
 */
+ (BOOL)hideStatusBarGet;

/**
 * To set new Hide Status Bar value in preference.
 */
+ (void)hideStatusBarSet:(BOOL)state;

/**
 * To get old Hide Navigation Bar value form preferences
 */
+ (BOOL)hideNavigationBarGet;

/**
 * To set new Hide Navigation Bar value in preference.
 */
+ (void)hideNavigationBarSet:(BOOL)state;

+(void)setIncentivized:(BOOL)state;

+(BOOL)getIncentivized;

@end


/******************************************************************************/
/*                                                                            */
/***  Prefs class                                                           ***/
/*                                                                            */
/******************************************************************************

overview
--------

saved preferences

*/
