// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-crittercism/host.h>
#include <moai-ios-crittercism/MOAICrittercismIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosCrittercismAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosCrittercismAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosCrittercismContextInitialize () {

	REGISTER_LUA_CLASS ( MOAICrittercismIOS );
}
