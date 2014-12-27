// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-twitter/host.h>
#include <moai-android-twitter/MOAITwitterAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidTwitterAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidTwitterAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidTwitterContextInitialize () {

	REGISTER_LUA_CLASS ( MOAITwitterAndroid );
	
}