// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_ANDROID_H
#define	AKU_MODULES_ANDROID_H

#include <host-modules/aku_modules_android_config.h>
#include <host-modules/aku_modules.h>

#if AKU_WITH_ANDROID_ADCOLONY
	#include <moai-android-adcolony/MOAIAdColonyAndroid.h>
#endif

#if AKU_WITH_ANDROID_BILLING
	#include <moai-android/MOAIBillingAndroid.h>
#endif

#if AKU_WITH_ANDROID_CHARTBOOST
	#include <moai-android-chartboost/host.h>
#endif

#if AKU_WITH_ANDROID_CRITTERCISM
	#include <moai-android-crittercism/MOAICrittercismAndroid.h>
#endif

#if AKU_WITH_ANDROID_FACEBOOK
	#include <moai-android-facebook/MOAIFacebookAndroid.h>
#endif

#if AKU_WITH_ANDROID_FLURRY
	#include <moai-android-flurry/MOAIFlurryAndroid.h>
#endif

#if AKU_WITH_ANDROID_NOTIFICATIONS
	#include <moai-android/MOAINotificationsAndroid.h>
#endif

#if AKU_WITH_ANDROID_TAPJOY
	#include <moai-android-tapjoy/MOAITapjoyAndroid.h>
#endif

#if AKU_WITH_ANDROID_TSTOREWALL
	#include <moai-android-tstore/MOAITstoreWallAndroid.h>;
#endif

#if AKU_WITH_ANDROID_TSTOREGAMECENTER
	#include <moai-android-tstore/MOAITstoreGamecenterAndroid.h>;
#endif

#if AKU_WITH_ANDROID_PLAYSERVICES
	#include <moai-android-google-play-services/MOAIGooglePlayServicesAndroid.h>;
#endif

#if AKU_WITH_ANDROID_VUNGLE
	#include <moai-android-vungle/MOAIVungleAndroid.h>;
#endif

//----------------------------------------------------------------//
void		AKUModulesAndroidAppFinalize			();
void		AKUModulesAndroidAppInitialize			();
void		AKUModulesAndroidContextInitialize		();
void		AKUModulesAndroidPause					( bool pause );
void		AKUModulesAndroidUpdate					();

#endif
