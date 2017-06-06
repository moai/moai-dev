// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-tenjin/host.h>
#include <moai-ios-tenjin/MOAITenjinIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosTenjinAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosTenjinAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosTenjinContextInitialize () {

	REGISTER_LUA_CLASS ( MOAITenjinIOS );
}
