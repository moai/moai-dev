// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-adcolony/host.h>
#include <moai-ios-adcolony/MOAIAdColonyIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosAdColonyAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosAdColonyAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosAdColonyContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIAdColonyIOS );
}