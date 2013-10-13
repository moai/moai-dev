// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-vungle/host.h>
#include <moai-ios-vungle/MOAIVungleIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosVungleAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosVungleAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosVungleContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIVungleIOS );
}
