// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-flurry/host.h>
#include <moai-ios-flurry/MOAIFlurryIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosFlurryAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosFlurryAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosFlurryContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIFlurryIOS );
}
