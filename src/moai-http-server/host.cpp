// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-http-server/host.h>
#include <moai-http-server/headers.h>

//================================================================//
// aku-util
//================================================================//

static bool sIsInitialized = false;

//----------------------------------------------------------------//
void AKUFinalizeHttpServer () {
	
	if ( !sIsInitialized ) return;
	sIsInitialized = false;
}

//----------------------------------------------------------------//
void AKUInitializeHttpServer () {

	if ( !sIsInitialized ) {
		sIsInitialized = true;
	}

	REGISTER_LUA_CLASS ( MOAIHttpServer ) 
}
