// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_IOS_H
#define	AKU_MODULES_IOS_H

#include <host-modules/aku_modules_ios_config.h>
#include <host-modules/aku_modules.h>

#if AKU_WITH_IOS
	#include <moai-ios/host.h>
#endif

#if AKU_WITH_IOS_MOTION
	#include <moai-ios-motion/host.h>
#endif

#if AKU_WITH_IOS_MOVIE
	#include <moai-ios-movie/host.h>
#endif

//----------------------------------------------------------------//
void		AKUModulesIosAppFinalize				();
void		AKUModulesIosAppInitialize				();
void		AKUModulesIosContextInitialize			();
void		AKUModulesIosPause						( bool pause );
void		AKUModulesIosUpdate						();

//----------------------------------------------------------------//
BOOL        AKUModulesIosApplicationDidFinishLaunchingWithOptions  ( UIApplication* application, NSDictionary* launchOptions );
BOOL        AKUModulesIosApplicationOpenURL                        ( UIApplication* application,  NSURL* url, NSString* sourceApplication, id annotation );

#endif
