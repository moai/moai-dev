// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-facebook/host.h>
#include <moai-android-facebook/MOAIFacebookAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidFacebookAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidFacebookAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidFacebookContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIFacebookAndroid );
	
}