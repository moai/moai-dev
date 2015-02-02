// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-playhaven/host.h>
#include <moai-ios-playhaven/MOAIPlayhavenIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosPlayhavenAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosPlayhavenAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosPlayhavenContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIPlayhavenIOS );
}
