// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_ANDROID_H
#define	AKU_MODULES_ANDROID_H

#include <host-modules/aku_modules_android_config.h>
#include <host-modules/aku_modules.h>

#if AKU_WITH_ANDROID_ADCOLONY
	#include <moai-android-adcolony/host.h>
#endif

#if AKU_WITH_ANDROID
  #include <moai-android/host.h>
#endif

#if AKU_WITH_ANDROID_BILLING
	#include <moai-android/host.h>
#endif

#if AKU_WITH_ANDROID_CHARTBOOST
	#include <moai-android-chartboost/host.h>
#endif

#if AKU_WITH_ANDROID_CRITTERCISM
	#include <moai-android-crittercism/host.h>
#endif

#if AKU_WITH_ANDROID_DELTADNA
	#include <moai-android-deltadna/host.h>
#endif

#if AKU_WITH_ANDROID_FACEBOOK
	#include <moai-android-facebook/host.h>
#endif

#if AKU_WITH_ANDROID_FLURRY
	#include <moai-android-flurry/host.h>
#endif

#if AKU_WITH_ANDROID_GOOGLE_PLAY_SERVICES
	#include <moai-android-google-play-services/host.h>
#endif

#if AKU_WITH_ANDROID_NOTIFICATIONS
	#include <moai-android/host.h>
#endif

#if AKU_WITH_ANDROID_TAPJOY
	#include <moai-android-tapjoy/host.h>
#endif

#if AKU_WITH_ANDROID_TSTOREWALL
	#include <moai-android-tstore/host.h>;
#endif

#if AKU_WITH_ANDROID_TSTOREGAMECENTER
	#include <moai-android-tstore/host.h>;
#endif

#if AKU_WITH_ANDROID_TWITTER
	#include <moai-android-twitter/host.h>
#endif

#if AKU_WITH_ANDROID_VUNGLE
	#include <moai-android-vungle/host.h>
#endif



//----------------------------------------------------------------//
void		AKUModulesAndroidAppFinalize				();
void		AKUModulesAndroidAppInitialize				();
void		AKUModulesAndroidContextInitialize			();
void		AKUModulesAndroidPause						( bool pause );
void		AKUModulesAndroidUpdate						();

#endif
