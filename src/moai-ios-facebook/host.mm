// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-facebook/host.h>
#include <moai-ios-facebook/MOAIFacebookIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosFacebookAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosFacebookAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosFacebookContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIFacebookIOS );
}
