// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-wildtangent-ads/host.h>
#include <moai-android/JniUtils.h>
#include <host-modules/aku_modules_android_config.h>
#include <moai-android-wildtangent-ads/MOAIWildTangentAdsAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidWildTangentAdsAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidWildTangentAdsAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidWildTangentAdsContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIWildTangentAdsAndroid );
	
}