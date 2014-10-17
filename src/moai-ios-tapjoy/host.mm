// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-tapjoy/host.h>
#include <moai-ios-tapjoy/MOAITapjoyIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosTapjoyAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosTapjoyAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosTapjoyContextInitialize () {

	REGISTER_LUA_CLASS ( MOAITapjoyIOS );
}
