// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-flurry/host.h>
#include <moai-android-flurry/MOAIFlurryAndroid.h>

#if AKU_WITH_ANDROID_FLURRY

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidFlurryAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidFlurryAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidFlurryContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIFlurryAndroid );
	
}

#endif //AKU_WITH_ANDROID_FLURRY
