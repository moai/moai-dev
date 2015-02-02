// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-twitter/host.h>
#include <moai-ios-twitter/MOAITwitterIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosTwitterAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosTwitterAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosTwitterContextInitialize () {

	REGISTER_LUA_CLASS ( MOAITwitterIOS );
}
