// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-tapjoy/host.h>
#include <moai-android-tapjoy/MOAITapjoyAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidTapjoyAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidTapjoyAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidTapjoyContextInitialize () {

	REGISTER_LUA_CLASS ( MOAITapjoyAndroid );
	
}