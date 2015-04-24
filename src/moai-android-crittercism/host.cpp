// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// From moai-box2d's pch.h
#include <moai_config.h>
#include <zl-util/headers.h>
#include <moai-core/headers.h>
#include <moai-sim/headers.h>
#include <moai-util/headers.h>

// We need to know what's configured as well?
#include <host-modules/aku_modules_android_config.h>

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