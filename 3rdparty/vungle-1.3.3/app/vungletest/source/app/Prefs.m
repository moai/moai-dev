// Prefs.m -- application preferences
// by allen brunson  february 3 2012

#import "Prefs.h"

static NSString* sKeyAdOrientation   = @"adOrientation";
static NSString* sKeyAdType          = @"adType";
static NSString* sKeyAlwaysShowClose = @"alwaysShowClose";
static NSString* sKeyFakeURL         = @"fakeURL";
static NSString* sKeyForceOldPlayer  = @"forceOldPlayer";
static NSString* sKeyLocationEnabled = @"locationEnabled";
static NSString* sKeyMemoryReport    = @"memoryReport";
static NSString* sKeyMuted           = @"muted";
static NSString* sKeyPubAppID        = @"pubAppID";
static NSString* sKeyPurgeTest       = @"purgeTest";
static NSString* sKeyExpiryTest      = @"expiryTest";
static NSString* sKeyRemoveCache     = @"removeCache";
static NSString* sKeyResumeTest      = @"resumeTest";
static NSString* sKeyShowJSON        = @"showJSON";
static NSString* sKeySkipMarkViewed  = @"skipMarkViewed";
static NSString* sKeyBaseUrl         = @"baseUrl";        //Key for Getting/Setting base url value
static NSString* sKeyHideStatusBar   = @"hideStatusBar";  //Key for Getting/Setting Hide Status Bar value
static NSString* sKeyHideNavigationBar = @"hideNavigationBar";  //Key for Getting/Setting Hide Navigation Bar value


static id prefGetObject(NSString* key, id defaultObject, Class class)
{
    NSUserDefaults*  ndef = [NSUserDefaults standardUserDefaults];
    id               nobj = [ndef objectForKey:key];
    
    if (VGIsKindOf(nobj, class))
    {
        return nobj;
    }
    else
    {
        return defaultObject;
    }    
}

static BOOL prefGetBool(NSString* key, BOOL defaultValue)
{
    NSNumber*  ndef = [NSNumber numberWithBool:defaultValue];
    NSNumber*  nnum = prefGetObject(key, ndef, [NSNumber class]);
    
    return [nnum boolValue];
}

static NSInteger prefGetInteger(NSString* key, NSInteger defaultValue)
{
    NSNumber*  ndef = [NSNumber numberWithBool:defaultValue];
    NSNumber*  nnum = prefGetObject(key, ndef, [NSNumber class]);
    
    return [nnum integerValue];
}

static NSString* prefGetString(NSString* key, NSString* defaultValue)
{
    NSString*  ndef = VGSanitizeString(defaultValue);
    NSString*  text = prefGetObject(key, ndef, [NSString class]);
    
    return text;
}

static void prefRemove(NSString* key)
{
    NSUserDefaults*  ndef = [NSUserDefaults standardUserDefaults];
    
    [ndef removeObjectForKey:key];
    [ndef synchronize];
}

static void prefSetObject(NSString* key, id object)
{
    NSUserDefaults*  ndef = [NSUserDefaults standardUserDefaults];
    
    if (VGIsKindOf(object, [NSObject class]))
    {
        [ndef setObject:object forKey:key];
        [ndef synchronize];
    }    
}

static void prefSetBool(NSString* key, BOOL boolValue)
{
    prefSetObject(key, [NSNumber numberWithBool:boolValue]);
}

static void prefSetInteger(NSString* key, NSInteger intValue)
{
    prefSetObject(key, [NSNumber numberWithInteger:intValue]);
}

static void prefSetString(NSString* key, NSString* stringValue)
{
    prefSetObject(key, VGSanitizeString(stringValue));
}

@implementation Prefs

+(VGAdOrientation)adOrientationGet
{
    const VGAdOrientation vdef = VGAdOrientationDefault;
    return VGAdOrientationForceValid(prefGetInteger(sKeyAdOrientation, vdef));
}

+(void)adOrientationSet:(VGAdOrientation)orient
{
    prefSetInteger(sKeyAdOrientation, VGAdOrientationForceValid(orient));
}

+(VGAdType)adTypeGet
{
    const NSInteger ival = prefGetInteger(sKeyAdType, VGAdTypeFakeFirst);
    return VGAdTypeForceValid(ival, TRUE);
}

+(void)adTypeSet:(VGAdType)adType
{
    prefSetInteger(sKeyAdType, VGAdTypeForceValid(adType, TRUE));
}

+(BOOL)alwaysShowCloseGet
{
    return prefGetBool(sKeyAlwaysShowClose, FALSE);
}

+(void)alwaysShowCloseSet:(BOOL)state
{
    prefSetBool(sKeyAlwaysShowClose, state);
}

+(NSString*)fakeURLGet
{
    return prefGetString(sKeyFakeURL, @"");
}

+(void)fakeURLSet:(NSString*)URL
{
    prefSetString(sKeyFakeURL, URL);
}

+(BOOL)forceOldPlayerGet
{
    return prefGetBool(sKeyForceOldPlayer, FALSE);
}

+(void)forceOldPlayerSet:(BOOL)state
{
    prefSetBool(sKeyForceOldPlayer, state);
}

+(BOOL)locationEnabledGet
{
    return prefGetBool(sKeyLocationEnabled, FALSE);
}

+(void)locationEnabledSet:(BOOL)state
{
    prefSetBool(sKeyLocationEnabled, state);
}

+(BOOL)memoryReportGet
{
    return prefGetBool(sKeyMemoryReport, FALSE);
}

+(void)memoryReportSet:(BOOL)state
{
    prefSetBool(sKeyMemoryReport, state);
}

+(BOOL)mutedGet
{
    return prefGetBool(sKeyMuted, FALSE);
}

+(void)mutedSet:(BOOL)state
{
    prefSetBool(sKeyMuted, state);
}

+(NSString*)pubAppIDGet
{
    return prefGetString(sKeyPubAppID, @"vungleTest");
}

+(void)pubAppIDSet:(NSString*)pubAppID
{
    prefSetString(sKeyPubAppID, pubAppID);
}

+(BOOL)purgeTestGet
{
    return prefGetBool(sKeyPurgeTest, FALSE);
}

+(void)purgeTestSet:(BOOL)state
{
    prefSetBool(sKeyPurgeTest, state);
}

+(BOOL)expiryTestGet
{
    return prefGetBool(sKeyExpiryTest, FALSE);
}

+(void) expiryTestSet:(BOOL)state
{
    prefSetBool(sKeyExpiryTest, state);
}

+(BOOL)removeCacheGet
{
    return prefGetBool(sKeyRemoveCache, FALSE);
}

+(void)removeCacheSet:(BOOL)state
{
    prefSetBool(sKeyRemoveCache, state);
}

+(void)resetDefaultsDebug
{
    prefRemove(sKeyAlwaysShowClose);
    prefRemove(sKeyForceOldPlayer);
    prefRemove(sKeyMemoryReport);
    prefRemove(sKeyMuted);
    prefRemove(sKeyPurgeTest);
    prefRemove(sKeyRemoveCache);
    prefRemove(sKeyResumeTest);
    prefRemove(sKeyShowJSON);
    prefRemove(sKeySkipMarkViewed);
}

+(void)resetDefaultsSettings
{
    prefRemove(sKeyAdOrientation);
    prefRemove(sKeyLocationEnabled);
    prefRemove(sKeyPubAppID);
}

+(BOOL)resumeTestGet
{
    return prefGetBool(sKeyResumeTest, FALSE);
}

+(void)resumeTestSet:(BOOL)state
{
    prefSetBool(sKeyResumeTest, state);
}

+(BOOL)showJSONGet
{
    return prefGetBool(sKeyShowJSON, FALSE);
}

+(void)showJSONSet:(BOOL)state
{
    prefSetBool(sKeyShowJSON, state);
}

+(BOOL)skipMarkViewedGet
{
    return prefGetBool(sKeySkipMarkViewed, FALSE);
}

+(void)skipMarkViewedSet:(BOOL)state
{
    prefSetBool(sKeySkipMarkViewed, state);
}

+ (VGConfigBaseUrl)adBaseUrlGet{
    const VGConfigBaseUrl vdef = VGBaseUrlProd;
    
    #if VG_DEBUG == 0
    return 0;
    #endif
    
    return prefGetInteger(sKeyBaseUrl, vdef);
}
+ (void)adBaseUrlSet:(VGConfigBaseUrl)url{
    prefSetInteger(sKeyBaseUrl, url);
    [VGVunglePub setBaseUrl:url];
}

+ (BOOL)hideStatusBarGet{
    return prefGetBool(sKeyHideStatusBar, FALSE);
}

+ (void)hideStatusBarSet:(BOOL)state{
    prefSetBool(sKeyHideStatusBar, state);
}

+ (BOOL)hideNavigationBarGet{
    return prefGetBool(sKeyHideNavigationBar, FALSE);
}

+ (void)hideNavigationBarSet:(BOOL)state{
    prefSetBool(sKeyHideNavigationBar, state);
}

+(BOOL)getIncentivized {
    return prefGetBool(@"incentivized", FALSE);
}
+(void)setIncentivized:(BOOL)state {
    prefSetBool(@"incentivized", state);
}

@end
