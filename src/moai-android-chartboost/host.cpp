// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-chartboost/host.h>
#include <moai-android/JniUtils.h>
#include <host-modules/aku_modules_android_config.h>
#include <moai-android-chartboost/MOAIChartBoostAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidChartBoostAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidChartBoostAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidChartBoostContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIChartBoostAndroid );
}
