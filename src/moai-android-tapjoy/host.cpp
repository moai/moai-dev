// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-tapjoy/host.h>
#include <moai-android-tapjoy/MOAITapjoyAndroid.h>

#if AKU_WITH_ANDROID_TAPJOY

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

#endif //AKU_WITH_ANDROID_TAPJOY
