// ConfigGlue.h -- configuration target methods
// by allen brunson  february 3 2012


/******************************************************************************/
/*                                                                            */
/***  ConfigGlue class                                                      ***/
/*                                                                            */
/******************************************************************************/

@interface ConfigGlue: NSObject
{
    @private
}

// public methods

+(ConfigGlue*)configGlue;

+(void)vungleRestart;
+(void)vungleRestartIfNeeded;

// construction and destruction

-(void)dealloc;
-(id)init;

// button items

-(void)bttnFakeAd:(VGConfigItem*)item;
-(void)bttnRemoveCache:(VGConfigItem*)item;
-(void)bttnResetDefaultsDebug:(VGConfigItem*)item;
-(void)bttnResetDefaultsSettings:(VGConfigItem*)item;
-(void)bttnRestart:(VGConfigItem*)item;
-(void)bttnStart:(VGConfigItem*)item;
-(void)bttnStop:(VGConfigItem*)item;
-(void)bttnShowCacheFiles:(VGConfigItem*)item;
-(void)bttnShowDeviceSettings:(VGConfigItem*)item;
-(void)bttnTerminalClear:(VGConfigItem*)item;

// load/save items

-(void)dataAdOrientationLoad:(VGConfigItem*)item;
-(void)dataAdOrientationSave:(VGConfigItem*)item;

-(void)dataAdTypeLoad:(VGConfigItem*)item;
-(void)dataAdTypeSave:(VGConfigItem*)item;

-(void)dataAlwaysShowCloseLoad:(VGConfigItem*)item;
-(void)dataAlwaysShowCloseSave:(VGConfigItem*)item;

-(void)dataFakeURLLoad:(VGConfigItem*)item;
-(void)dataFakeURLSave:(VGConfigItem*)item;

-(void)dataForceOldPlayerLoad:(VGConfigItem*)item;
-(void)dataForceOldPlayerSave:(VGConfigItem*)item;

-(void)dataLocationEnabledLoad:(VGConfigItem*)item;
-(void)dataLocationEnabledSave:(VGConfigItem*)item;

-(void)dataMemoryReportLoad:(VGConfigItem*)item;
-(void)dataMemoryReportSave:(VGConfigItem*)item;

-(void)dataMutedLoad:(VGConfigItem*)item;
-(void)dataMutedSave:(VGConfigItem*)item;

-(void)dataPubAppIDLoad:(VGConfigItem*)item;
-(void)dataPubAppIDSave:(VGConfigItem*)item;

-(void)dataPurgeTestLoad:(VGConfigItem*)item;
-(void)dataPurgeTestSave:(VGConfigItem*)item;

-(void)dataExpiryTestLoad:(VGConfigItem*)item;
-(void)dataExpiryTestSave:(VGConfigItem*)item;

-(void)dataRemoveCacheLoad:(VGConfigItem*)item;
-(void)dataRemoveCacheSave:(VGConfigItem*)item;

-(void)dataResumeTestLoad:(VGConfigItem*)item;
-(void)dataResumeTestSave:(VGConfigItem*)item;

-(void)dataShowJSONLoad:(VGConfigItem*)item;
-(void)dataShowJSONSave:(VGConfigItem*)item;

-(void)dataSkipMarkViewedLoad:(VGConfigItem*)item;
-(void)dataSkipMarkViewedSave:(VGConfigItem*)item;

-(void)getIncentivized:(VGConfigItem*)item;
-(void)setIncentivized:(VGConfigItem*)item;

/**
 * To get old baseUrl preferences
 */
- (void)buttonConfigBaseUrl:(VGConfigItem*)item;

/**
 * To save new baseUrl preferences
 */
- (void)dataCofigBaseUrlSave:(VGConfigItem*)item;

/**
 * To get old Hide Status Bar value
 */
- (void)dataHideStatusBarLoad:(VGConfigItem*)item;

/**
 * To save new Hide Status Bar value
 */
- (void)dataHideStatusBarSave:(VGConfigItem*)item;

/**
 * To get old Hide Navigation Bar value
 */
- (void)dataHideNavigationBarLoad:(VGConfigItem*)item;

/**
 * To save new Hide Navigation Bar value
 */
- (void)dataHideNavigationBarSave:(VGConfigItem*)item;


@end


/******************************************************************************/
/*                                                                            */
/***  ConfigGlue class                                                      ***/
/*                                                                            */
/******************************************************************************

overview
--------

target methods for wiring up the prefs view


maintenance notes
-----------------

changing these items should trigger a reboot:

pubAppID
adOrientation
location

changing pubAppID also triggers a cache purge.

all other settings can be applied without shutting down and restarting.

*/
