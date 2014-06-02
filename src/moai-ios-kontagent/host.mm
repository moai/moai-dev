// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-kontagent/host.h>
#include <moai-ios-kontagent/MOAIKontagentIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosKontagentAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosKontagentAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosKontagentContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIKontagentIOS );
}
