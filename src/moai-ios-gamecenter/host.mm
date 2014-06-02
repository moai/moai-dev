// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-gamecenter/host.h>
#include <moai-ios-gamecenter/MOAIGameCenterIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosGameCenterAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosGameCenterAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosGameCenterContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIGameCenterIOS );
}
