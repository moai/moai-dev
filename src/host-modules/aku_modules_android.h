// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_ANDROID_H
#define	AKU_MODULES_ANDROID_H

#include <host-modules/aku_modules_android_config.h>
#include <host-modules/aku_modules.h>

#if AKU_WITH_ANDROID
  #include <moai-android/host.h>
#endif

#if AKU_WITH_ANDROID_NOTIFICATIONS
	#include <moai-android/host.h>
#endif

//----------------------------------------------------------------//
void		AKUModulesAndroidAppFinalize				();
void		AKUModulesAndroidAppInitialize				();
void		AKUModulesAndroidContextInitialize			();
void		AKUModulesAndroidPause						( bool pause );
void		AKUModulesAndroidUpdate						();

#endif
