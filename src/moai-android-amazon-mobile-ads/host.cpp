// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-amazon-mobile-ads/host.h>
#include <moai-android/JniUtils.h>
#include <host-modules/aku_modules_android_config.h>
#include <moai-android-amazon-mobile-ads/MOAIAmazonMobileAdsAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidAmazonMobileAdsAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidAmazonMobileAdsAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidAmazonMobileAdsContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIAmazonMobileAdsAndroid );
}
