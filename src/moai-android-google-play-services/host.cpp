// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-android-google-play-services/host.h>
#include <moai-android-google-play-services/MOAIGooglePlayServicesAndroid.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidGooglePlayServicesAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidGooglePlayServicesAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidGooglePlayServicesContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIGooglePlayServicesAndroid );
	
}