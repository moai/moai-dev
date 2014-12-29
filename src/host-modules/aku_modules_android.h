// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_IOS_H
#define	AKU_MODULES_IOS_H

#include <host-modules/aku_modules_android_config.h>
#include <host-modules/aku_modules.h>

#if AKU_WITH_ANDROID_ADCOLONY
	#include <moai-android-adcolony/host.h>
#endif

#if AKU_WITH_ANDROID_CHARTBOOST
	#include <moai-android-chartboost/host.h>
#endif

#if AKU_WITH_ANDROID_CRITTERCISM
  #include <moai-android-crittercism/host.h>
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

#if AKU_WITH_ANDROID_TAPJOY
	#include <moai-android-tapjoy/host.h>
#endif

#if AKU_WITH_ANDROID_TWITTER
	#include <moai-android-twitter/host.h>
#endif

#if AKU_WITH_ANDROID_VUNGLE
	#include <moai-android-vungle/host.h>
#endif
/*
#ifndef AKU_WITH_ANDROID_AMAZON_BILLING
	#define AKU_WITH_ANDROID_AMAZON_BILLING 1
#endif

#ifndef AKU_WITH_ANDROID_CHARTBOOST
	#define AKU_WITH_ANDROID_CHARTBOOST 0
#endif

#ifndef AKU_WITH_ANDROID_CRITTERCISM
	#define AKU_WITH_ANDROID_CRITTERCISM 0
#endif

#ifndef AKU_WITH_ANDROID_FACEBOOK
	#define AKU_WITH_ANDROID_FACEBOOK 0
#endif

#ifndef AKU_WITH_ANDROID_FLURRY
	#define AKU_WITH_ANDROID_FLURRY 0
#endif

#ifndef AKU_WITH_ANDROID_FORTUMO
	#define AKU_WITH_ANDROID_FORTUMO 0
#endif

#ifndef AKU_WITH_ANDROID_GOOGLE_BILLING
	#define AKU_WITH_ANDROID_GOOGLE_BILLING 0
#endif

#ifndef AKU_WITH_ANDROID_GOOGLE_PUSH
	#define AKU_WITH_ANDROID_GOOGLE_PUSH 0
#endif

#ifndef AKU_WITH_ANDROID_GOOGLE_PLAY_SERVICES
	#define AKU_WITH_ANDROID_GOOGLE_PLAY_SERVICES 0
#endif

#ifndef AKU_WITH_ANDROID_TAPJOY
	#define AKU_WITH_ANDROID_TAPJOY 0
#endif

#ifndef AKU_WITH_ANDROID_TSTORE
	#define AKU_WITH_ANDROID_TSTORE 0
#endif
*/

/*
#ifndef AKU_WITH_ANDROID_VUNGLE
	#define AKU_WITH_ANDROID_VUNGLE 0
#endif
*/

//----------------------------------------------------------------//
void		AKUModulesAndroidAppFinalize				();
void		AKUModulesAndroidAppInitialize				();
void		AKUModulesAndroidContextInitialize			();
void		AKUModulesAndroidPause						( bool pause );
void		AKUModulesAndroidUpdate						();

#endif
