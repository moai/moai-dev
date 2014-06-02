// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-urban-airship/host.h>
#include <moai-ios-urban-airship/MOAIUrbanAirshipIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosUrbanAirshipAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosUrbanAirshipAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosUrbanAirshipContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIUrbanAirshipIOS );
}
