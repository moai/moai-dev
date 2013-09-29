/*
 Copyright (C) 2012, Vungle, Inc. All Rights Reserved
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of the author nor the names of its contributors may be used
 to endorse or promote products derived from this software without specific
 prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// VGVunglePub.h -- vungle ad publisher api
// by allen brunson  january 2 2012

#ifndef VUNGLE_VGVUNGLEPUB_H
#define VUNGLE_VGVUNGLEPUB_H

#import "VGAdType.h"
#import "VGPlayData.h"
#import "VGStatusData.h"
#import "VGUserData.h"


/******************************************************************************/
/*                                                                            */
/***  VGVunglePubDelegate protocol                                          ***/
/*                                                                            */
/******************************************************************************/

@protocol VGVunglePubDelegate
@optional
-(void)vungleMoviePlayed:(VGPlayData*)playData;
-(void)vungleStatusUpdate:(VGStatusData*)statusData;
-(void)vungleViewDidDisappear:(UIViewController*)viewController;
-(void)vungleViewWillAppear:(UIViewController*)viewController;
-(void)vungleAppStoreViewDidDisappear;
@end

typedef  id<VGVunglePubDelegate>  VGVungleDelegate;


/******************************************************************************/
/*                                                                            */
/***  VGVunglePub class                                                     ***/
/*                                                                            */
/******************************************************************************/

@interface VGVunglePub: NSObject
{
}

// start and stop the vungle pub sdk

+(void)startWithPubAppID:(NSString*)pubAppID;
+(void)startWithPubAppID:(NSString*)pubAppID userData:(VGUserData*)userData;
+(void)stop;



+(NSString *)getOpenUDID;

// information about the vungle pub sdk library

+(VGStatusData*)currentStatusData;  // current status data
+(BOOL)running;                     // true if library is running
+(void)showCacheFiles;              // print cache file info to the terminal
+(void)showDeviceSettings;          // print device settings to the terminal
+(NSString*)versionString;          // library version

// get and set vungle delegate

+(VGVungleDelegate)delegate;
+(void)setDelegate:(VGVungleDelegate)delegate;

// returns true if an ad is available and this device is internet-connected

+(BOOL)adIsAvailable;

// present an ad modally over 'controller'

+(void)playModalAd:(UIViewController*)controller animated:(BOOL)animated;
+(void)playModalAd:(UIViewController*)controller animated:(BOOL)animated showClose:(BOOL)value;
+(void)playIncentivizedAd:(UIViewController*)controller animated:(BOOL)animated showClose:(BOOL)flag userTag:(NSString *)user;

//alert box config if close during incentivized play
+(void)alertBoxWithTitle:(NSString*)title Body:(NSString*)body leftButtonTitle:(NSString*)button1 rightButtonTitle:(NSString*)button2;
+(BOOL)setCustomCountDownText:(NSString*)text;
+(void)muteIfMusicPlaying:(BOOL)state;
+(void)setSoundEnabled:(BOOL)enabled;

// disk space allowed for caching ads, in megabytes

+(NSInteger)cacheSizeGet;
+(void)cacheSizeSet:(NSInteger)megabytes;

// log settings

+(void)logToStdout:(BOOL)state;
+(void)logTarget:(id)target action:(SEL)action;

// debug settings. for vungle internal use only. no effect in release builds.

+(void)alwaysShowClose:(BOOL)state;
+(BOOL)debugEnabled;
+(void)forceOldPlayer:(BOOL)state;
+(void)memoryReport:(BOOL)state;
+(void)muted:(BOOL)state;
+(void)play:(NSString*)URL type:(VGAdType)type view:(UIViewController*)view;
+(void)purgeTest:(BOOL)state;
+(void)expiryTest:(BOOL)state;
+(void)removeCache;
+(void)removeCache:(BOOL)state;
+(void)resumeTest:(BOOL)state;
+(void)showJSON:(BOOL)state;
+(void)skipMarkViewed:(BOOL)state;

/**
 * this method allows the developer to set autorotate the ads 
 * by setting | allow | to TRUE and FALSE to use the default orientation 
 *
 */
+ (void)allowAutoRotate:(BOOL)allow;

/**
 * Returns TRUE if an ad is in the process to be displayed on screen, otherwise returns FALSE.
 */
+ (BOOL)loadingOfAdViewControllerInProgress;

/**
 * To set loadingOfAdViewControllerInProgres true or false
 */
+ (void)setLoadingOfAdViewControllerInProgres:(BOOL)inProgress;

/**
 * To load previous url
 */
+ (void)loadBaseUrl;

/**
 * To update BaseUrl value according to value |url| send from outside of SDK.
 */

+(void)setBaseUrl:(VGConfigBaseUrl)URL;

@end


/******************************************************************************/
/*                                                                            */
/***  VGVunglePub class                                                     ***/
/*                                                                            */
/******************************************************************************

overview
--------

main class for the vungle ad publisher sdk. everything you can do with the
library is described in this one header file.

*/

#endif  // VUNGLE_VGVUNGLEPUB_H
