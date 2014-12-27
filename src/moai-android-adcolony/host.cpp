// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-adcolony/host.h>
#include <moai-android-adcolony/MOAIAdColonyAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidAdColonyAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidAdColonyAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidAdColonyContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIAdColonyAndroid );
	
}