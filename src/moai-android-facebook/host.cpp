// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-facebook/host.h>
#include <moai-android-facebook/MOAIFacebookAndroid.h>

#if AKU_WITH_ANDROID_FACEBOOK

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

#endif //AKU_WITH_ANDROID_FACEBOOK
