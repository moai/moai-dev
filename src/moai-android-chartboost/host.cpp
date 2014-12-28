// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-chartboost/host.h>
#include <moai-android-chartboost/MOAIChartBoostAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidChartboostAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidChartboostAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidChartboostContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIChartBoostAndroid );
	
}