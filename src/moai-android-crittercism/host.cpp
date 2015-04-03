// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-crittercism/host.h>
#include <moai-android-crittercism/MOAICrittercismAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidCrittercismAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidCrittercismAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidCrittercismContextInitialize () {

	REGISTER_LUA_CLASS ( MOAICrittercismAndroid );
	
}