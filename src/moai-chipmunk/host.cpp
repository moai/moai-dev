// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-chipmunk/host.h>
#include <moai-chipmunk/headers.h>

//================================================================//
// aku-util
//================================================================//

static bool sIsInitialized = false;

//----------------------------------------------------------------//
void AKUFinalizeChipmunk () {
	
	if ( !sIsInitialized ) return;
	sIsInitialized = false;
}

//----------------------------------------------------------------//
void AKUInitializeChipmunk () {

	if ( !sIsInitialized ) {
		cpInitChipmunk ();
		sIsInitialized = true;
	}

    REGISTER_LUA_CLASS ( MOAICp )
	REGISTER_LUA_CLASS ( MOAICpArbiter )
	REGISTER_LUA_CLASS ( MOAICpBody )
	REGISTER_LUA_CLASS ( MOAICpConstraint )
	REGISTER_LUA_CLASS ( MOAICpShape )
	REGISTER_LUA_CLASS ( MOAICpSpace )
}
