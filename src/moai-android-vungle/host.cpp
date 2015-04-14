// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-vungle/host.h>
#include <moai-android-vungle/MOAIVungleAndroid.h>

#if AKU_WITH_ANDROID_VUNGLE

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidVungleAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidVungleAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidVungleContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIVungleAndroid );
	
}

#endif //AKU_WITH_ANDROID_VUNGLE
