// ConfigGlue.m -- configuration target methods
// by allen brunson  february 3 2012

#import "AppDelegate.h"
#import "ConfigGlue.h"
#import "Prefs.h"

static BOOL sRestartNeeded      = FALSE;
static BOOL sRestartRemoveCache = FALSE;

@implementation ConfigGlue

-(void)bttnFakeAd:(VGConfigItem*)item
{
    UIViewController*  cntl = [[AppDelegate appDelegate] mainViewController];
    NSString*          text = [Prefs fakeURLGet];
    const VGAdType     type = [Prefs adTypeGet];
    
    if (VGStringIsEmpty(text))
    {
        [AppDelegate alertWithMessage:@"You must supply a URL."];
        return;
    }
  
    [VGVunglePub play:text type:type view:cntl];
    [AppDelegate popToRootViewController];
}

-(void)bttnRemoveCache:(VGConfigItem*)item
{
    sRestartRemoveCache = TRUE;
    [[self class] vungleRestart];
    [AppDelegate popToRootViewController];
}

-(void)bttnResetDefaultsDebug:(VGConfigItem*)item
{
    [Prefs resetDefaultsDebug];
}

-(void)bttnResetDefaultsSettings:(VGConfigItem*)item
{
    sRestartNeeded      = TRUE;
    sRestartRemoveCache = TRUE;
    [Prefs resetDefaultsSettings];
}

-(void)bttnRestart:(VGConfigItem*)item
{
    [[self class] vungleRestart];
    [AppDelegate popToRootViewController];
}

-(void)bttnStop:(VGConfigItem*)item
{
    [[self class] vungleStop];
    [AppDelegate popToRootViewController];
}

-(void)bttnStart:(VGConfigItem*)item
{
    [[self class] vungleStart];
    [AppDelegate popToRootViewController];
}

-(void)bttnShowCacheFiles:(VGConfigItem*)item
{
    [VGVunglePub showCacheFiles];
    [AppDelegate popToRootViewController];
}

-(void)bttnShowDeviceSettings:(VGConfigItem*)item
{
    [VGVunglePub showDeviceSettings];
    [AppDelegate popToRootViewController];
}

-(void)bttnTerminalClear:(VGConfigItem*)item
{
    [[[AppDelegate appDelegate] mainViewController] terminalClear];
    [AppDelegate popToRootViewController];
}


+(ConfigGlue*)configGlue
{
    static ConfigGlue*  glue = nil;
    
    if (!VGIsKindOf(glue, [self class]))
    {
        glue = [[self alloc] init];
    }
    
    return glue;
}

-(void)dataAdOrientationLoad:(VGConfigItem*)item
{
    item.valueInteger = [Prefs adOrientationGet];
}

-(void)dataAdOrientationSave:(VGConfigItem*)item
{
    sRestartNeeded = TRUE;
    [Prefs adOrientationSet:item.valueInteger];
}

-(void)dataAdTypeLoad:(VGConfigItem*)item
{
    item.valueInteger = [Prefs adTypeGet];
}

-(void)dataAdTypeSave:(VGConfigItem*)item
{
    [Prefs adTypeSet:item.valueInteger];
}

-(void)dataAlwaysShowCloseLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs alwaysShowCloseGet];
}

-(void)dataAlwaysShowCloseSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs alwaysShowCloseSet:rval];
    [VGVunglePub alwaysShowClose:rval];
}

-(void)dataFakeURLLoad:(VGConfigItem*)item
{
    item.valueString = [Prefs fakeURLGet];
}

-(void)dataFakeURLSave:(VGConfigItem*)item
{
    [Prefs fakeURLSet:item.valueString];
}

-(void)dataForceOldPlayerLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs forceOldPlayerGet];
}

-(void)dataForceOldPlayerSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs forceOldPlayerSet:rval];
    [VGVunglePub forceOldPlayer:rval];
}

-(void)dataLocationEnabledLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs locationEnabledGet];
}

-(void)dataLocationEnabledSave:(VGConfigItem*)item
{
    sRestartNeeded = TRUE;
    [Prefs locationEnabledSet:item.valueBool];
}

-(void)dataMemoryReportLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs memoryReportGet];
}

-(void)dataMemoryReportSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs memoryReportSet:rval];
    [VGVunglePub memoryReport:rval];
}

-(void)dataMutedLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs mutedGet];
}

-(void)dataMutedSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs mutedSet:rval];
    [VGVunglePub muted:rval];
}

-(void)dataPubAppIDLoad:(VGConfigItem*)item
{
    item.valueString = [Prefs pubAppIDGet];
}

-(void)dataPubAppIDSave:(VGConfigItem*)item
{
    sRestartNeeded      = TRUE;
    sRestartRemoveCache = TRUE;
    [Prefs pubAppIDSet:item.valueString];
}

-(void)dataPurgeTestLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs purgeTestGet];
}

-(void)dataPurgeTestSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs purgeTestSet:rval];
    [VGVunglePub purgeTest:rval];
}

-(void)dataExpiryTestLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs expiryTestGet];
}

-(void)dataExpiryTestSave:(VGConfigItem *)item
{
    const BOOL rval = item.valueBool;
    
    [Prefs expiryTestSet:rval];
    [VGVunglePub expiryTest:rval];
}

-(void)dataRemoveCacheLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs removeCacheGet];
}

-(void)dataRemoveCacheSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs removeCacheSet:rval];
    [VGVunglePub removeCache:rval];
}

-(void)dataResumeTestLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs resumeTestGet];
}

-(void)dataResumeTestSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs resumeTestSet:rval];
    [VGVunglePub resumeTest:rval];
}

-(void)dataShowJSONLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs showJSONGet];
}

-(void)dataShowJSONSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs showJSONSet:rval];
    [VGVunglePub showJSON:rval];
}

-(void)dataSkipMarkViewedLoad:(VGConfigItem*)item
{
    item.valueBool = [Prefs skipMarkViewedGet];
}

-(void)dataSkipMarkViewedSave:(VGConfigItem*)item
{
    const BOOL  rval = item.valueBool;
    
    [Prefs skipMarkViewedSet:rval];
    [VGVunglePub skipMarkViewed:rval];
}

-(void)getIncentivized:(VGConfigItem*)item
{
    item.valueBool = [Prefs getIncentivized];
}
-(void)setIncentivized:(VGConfigItem*)item
{
    
    const BOOL rval = item.valueBool;
    
    [Prefs setIncentivized:rval];
}

-(void)dealloc
{
    [super dealloc];
}

-(id)init
{
    self = [super init];
    if (!self) return nil;
    
    return self;
}

+(void)vungleStop
{
    [[AppDelegate appDelegate] vungleStop];
}

+(void)vungleStart
{
    [[AppDelegate appDelegate] vungleStart];
}

+(void)vungleRestart
{
    if (sRestartRemoveCache)
    {
        [VGVunglePub removeCache];
    }
    
    [[AppDelegate appDelegate] vungleStop];
    [[AppDelegate appDelegate] vungleStart];
    
    sRestartNeeded      = FALSE;
    sRestartRemoveCache = FALSE;
}

+(void)vungleRestartIfNeeded
{
    if (sRestartNeeded)
    {
        [self vungleRestart];
    }
}


- (void)buttonConfigBaseUrl:(VGConfigItem*)item{
    item.valueInteger = [Prefs adBaseUrlGet];
}

- (void)dataCofigBaseUrlSave:(VGConfigItem*)item{
    sRestartNeeded = TRUE;
    [Prefs adBaseUrlSet:item.valueInteger];

}

- (void)dataHideStatusBarLoad:(VGConfigItem*)item{
    item.valueBool = [Prefs hideStatusBarGet];
}

- (void)dataHideStatusBarSave:(VGConfigItem*)item{
    const BOOL  rval = item.valueBool;
    [Prefs hideStatusBarSet:rval];
}

- (void)dataHideNavigationBarLoad:(VGConfigItem*)item{
    item.valueBool = [Prefs hideNavigationBarGet];
}

- (void)dataHideNavigationBarSave:(VGConfigItem*)item{
    const BOOL  rval = item.valueBool;
    [Prefs hideNavigationBarSet:rval];
}


@end
