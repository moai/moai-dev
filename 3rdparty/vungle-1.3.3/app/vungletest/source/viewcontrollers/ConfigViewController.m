// ConfigViewController.m -- preferences view
// by allen brunson  february 3 2012

#import "ConfigGlue.h"
#import "ConfigViewController.h"

static NSString* sDebugSettings   = @"Debug Settings";
static NSString* sFakeAdDisplay   = @"Fake Ad Display";
static NSString* sRemoveCache     = @"Remove Cache";
static NSString* sResetToDefaults = @"Reset to Defaults";

static NSString* adTypeToString(VGAdType type)
{
    switch (type)
    {
        case VGAdTypeReal:               return @"Real";
        case VGAdTypeFakePreBundleHTML:  return @"PreBundle HTML";
        case VGAdTypeFakePreBundleZip:   return @"PreBundle ZIP";
        case VGAdTypeFakePostBundleHTML: return @"PostBundle HTML";
        case VGAdTypeFakePostBundleZip:  return @"PostBundle ZIP";
        case VGAdTypeFakeMovie:          return @"Movie";
        case VGAdTypeUnknown:            break;
    }
    
    return @"unknown";
}

@implementation ConfigViewController

+(ConfigViewController*)controller
{
    return [[[self alloc] initWithStyle:UITableViewStyleGrouped] autorelease];
}

-(void)dealloc
{
    [super dealloc];
}

-(VGConfigGroup*)groupControls
{
    VGConfigGroup*  cgrp = [VGConfigGroup configGroup];
    VGConfigItem*   item = nil;
    
    cgrp.label = @"Controls";
    
    item = [VGConfigItem itemWithType:VGConfButtonPlain];
    item.label = @"Restart Vungle";
    item.bttnTarget = ConfigGlue.configGlue;
    item.bttnAction = @selector(bttnRestart:);
    [cgrp itemAdd:item];
    
    if ([VGVunglePub debugEnabled])
    {
        item = [VGConfigItem itemWithType:VGConfButtonPlain];
        item.label = sRemoveCache;
        item.bttnTarget = ConfigGlue.configGlue;
        item.bttnAction = @selector(bttnRemoveCache:);
        [cgrp itemAdd:item];
        
        item = [VGConfigItem itemWithType:VGConfButtonPlain];
        item.label = @"Stop";
        item.bttnTarget = ConfigGlue.configGlue;
        item.bttnAction = @selector(bttnStop:);
        [cgrp itemAdd:item];
        
        item = [VGConfigItem itemWithType:VGConfButtonPlain];
        item.label = @"Start";
        item.bttnTarget = ConfigGlue.configGlue;
        item.bttnAction = @selector(bttnStart:);
        [cgrp itemAdd:item];
    }
    
    item = [VGConfigItem itemWithType:VGConfButtonPlain];
    item.label = @"Show Cache Files";
    item.bttnTarget = ConfigGlue.configGlue;
    item.bttnAction = @selector(bttnShowCacheFiles:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfButtonPlain];
    item.label = @"Show Device Settings";
    item.bttnTarget = ConfigGlue.configGlue;
    item.bttnAction = @selector(bttnShowDeviceSettings:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfButtonPlain];
    item.label = @"Clear Terminal";
    item.bttnTarget = ConfigGlue.configGlue;
    item.bttnAction = @selector(bttnTerminalClear:);
    [cgrp itemAdd:item];
    
    [cgrp itemAdd:self.itemBaseURL];
        
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Hide Status Bar";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataHideStatusBarLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataHideStatusBarSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Hide Navigation Bar";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataHideNavigationBarLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataHideNavigationBarSave:);
    [cgrp itemAdd:item];
    
    return cgrp;
}

-(VGConfigGroup*)groupDebug
{
    VGConfigGroup*  cgrp = [VGConfigGroup configGroup];
    VGConfigItem*   item = nil;
    
    cgrp.label = @"Debug";
    
    item = [VGConfigItem itemWithType:VGConfButtonToGroup];
    item.label = sFakeAdDisplay;
    item.group = self.groupDebugFakeAd;
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfButtonToGroup];
    item.label = sDebugSettings;
    item.group = self.groupDebugSettings;
    [cgrp itemAdd:item];
    
    return cgrp;
}

-(VGConfigGroup*)groupDebugFakeAd
{
    VGConfigGroup*  cgrp = [VGConfigGroup configGroup];
    VGConfigItem*   item = nil;
    
    cgrp.label = sFakeAdDisplay;
    
    [cgrp itemAdd:self.itemAdType];
    
    item = [VGConfigItem itemWithType:VGConfURL];
    item.label = @"URL";
    item.editWidth = 0.7;
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataFakeURLLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataFakeURLSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfButtonPlain];
    item.label = @"Start Ad";
    item.bttnTarget = ConfigGlue.configGlue;
    item.bttnAction = @selector(bttnFakeAd:);
    [cgrp itemAdd:item];
    
    return cgrp;
}

-(VGConfigGroup*)groupDebugSettings
{
    VGConfigGroup*  cgrp = [VGConfigGroup configGroup];
    VGConfigItem*   item = nil;
    
    cgrp.label = sDebugSettings;
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Always Show Close";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataAlwaysShowCloseLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataAlwaysShowCloseSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Force Old Player";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataForceOldPlayerLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataForceOldPlayerSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Use incentivized views";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(getIncentivized:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(setIncentivized:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Memory Report";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataMemoryReportLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataMemoryReportSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Muted";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataMutedLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataMutedSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Purge Test";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataPurgeTestLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataPurgeTestSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Expiry Test";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataExpiryTestLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataExpiryTestSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = sRemoveCache;
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataRemoveCacheLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataRemoveCacheSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Resume Test";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataResumeTestLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataResumeTestSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Show JSON";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataShowJSONLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataShowJSONSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Skip Mark Viewed";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataSkipMarkViewedLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataSkipMarkViewedSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfButtonPlain];
    item.label = sResetToDefaults;
    item.reload = TRUE;
    item.bttnTarget = ConfigGlue.configGlue;
    item.bttnAction = @selector(bttnResetDefaultsDebug:);
    [cgrp itemAdd:item];
    
    return cgrp;
}

-(VGConfigGroup*)groupSettings
{
    VGConfigGroup*  cgrp = [VGConfigGroup configGroup];
    VGConfigItem*   item = nil;
    
    cgrp.label = @"Settings";
    
    item = [VGConfigItem itemWithType:VGConfText];
    item.label = @"Pub App ID";
    item.editWidth = 0.55;
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataPubAppIDLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataPubAppIDSave:);
    [cgrp itemAdd:item];
    
    [cgrp itemAdd:self.itemAdOrientation];
    
    item = [VGConfigItem itemWithType:VGConfBoolean];
    item.label = @"Location Enabled";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataLocationEnabledLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataLocationEnabledSave:);
    [cgrp itemAdd:item];
    
    item = [VGConfigItem itemWithType:VGConfButtonPlain];
    item.label = sResetToDefaults;
    item.reload = TRUE;
    item.bttnTarget = ConfigGlue.configGlue;
    item.bttnAction = @selector(bttnResetDefaultsSettings:);
    [cgrp itemAdd:item];
    
    return cgrp;
}

-(id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (!self) return nil;
    
    self.title = @"Preferences";
    
    return self;
}

-(VGConfigItem*)itemAdOrientation
{
    VGConfigItem*  isub = nil;
    VGConfigItem*  item = nil;
    
    item = [VGConfigItem itemWithType:VGConfChooser];
    item.group = [VGConfigGroup configGroup];
    item.label = @"Ad Orientation";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataAdOrientationLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataAdOrientationSave:);
    
    for (VGAdOrientation orient = VGAdOrientationFirst; orient <= VGAdOrientationLast; orient++)
    {
        isub = [VGConfigItem itemWithType:VGConfButtonChooserItem];
        isub.label = [VGUserData adOrientationString:orient];
        [isub setValueInteger:orient];
        [item.group itemAdd:isub];
    }
    
    return item;
}

-(VGConfigItem*)itemAdType
{
    VGConfigItem*  isub = nil;
    VGConfigItem*  item = nil;
    
    item = [VGConfigItem itemWithType:VGConfChooser];
    item.group = [VGConfigGroup configGroup];
    item.label = @"Ad Type";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(dataAdTypeLoad:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataAdTypeSave:);
    
    for (VGAdType type = VGAdTypeFakeFirst; type <= VGAdTypeFakeLast; type++)
    {
        isub = [VGConfigItem itemWithType:VGConfButtonChooserItem];
        isub.label = adTypeToString(type);
        [isub setValueInteger:type];
        [item.group itemAdd:isub];
    }
    
    return item;
}

-(VGConfigItem*)itemBaseURL
{
    VGConfigItem*  isub = nil;
    VGConfigItem*  item = nil;
    
    item = [VGConfigItem itemWithType:VGConfChooser];
    item.group = [VGConfigGroup configGroup];
    item.label = @"Config Base URL";
    item.loadTarget = ConfigGlue.configGlue;
    item.loadAction = @selector(buttonConfigBaseUrl:);
    item.saveTarget = ConfigGlue.configGlue;
    item.saveAction = @selector(dataCofigBaseUrlSave:);
    
    for (VGConfigBaseUrl url = VGBaseUrlFirst; url <= VGBaseUrlLast; url++)
    {
        isub = [VGConfigItem itemWithType:VGConfButtonChooserItem];
        isub.label = [VGUserData baseUrlString:url];
        [isub setValueInteger:url];
        [item.group itemAdd:isub];
    }
    
    return item;
}

-(NSArray*)loadGroups
{
    NSMutableArray*  list = [NSMutableArray array];
    
    [list addObject:self.groupSettings];
    [list addObject:self.groupControls];
    
    if ([VGVunglePub debugEnabled])
    {
        [list addObject:self.groupDebug];
    } 
    
    return list;
}

-(void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    [ConfigGlue vungleRestartIfNeeded];
}

@end
