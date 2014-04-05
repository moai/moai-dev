// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-http-server/host.h>
#include <moai-http-server/headers.h>

//================================================================//
// aku-util
//================================================================//

//----------------------------------------------------------------//
void AKUHttpServerAppFinalize () {
}

//----------------------------------------------------------------//
void AKUHttpServerAppInitialize () {
}

//----------------------------------------------------------------//
void AKUHttpServerContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIHttpServer ) 
}
