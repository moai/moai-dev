// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-chipmunk/host.h>
#include <moai-chipmunk/headers.h>

//================================================================//
// aku-util
//================================================================//

//----------------------------------------------------------------//
void AKUChipmunkAppFinalize () {
}

//----------------------------------------------------------------//
void AKUChipmunkAppInitialize () {

	cpInitChipmunk ();
}

//----------------------------------------------------------------//
void AKUChipmunkContextInitialize () {

	REGISTER_LUA_CLASS ( MOAICp )
	REGISTER_LUA_CLASS ( MOAICpArbiter )
	REGISTER_LUA_CLASS ( MOAICpBody )
	REGISTER_LUA_CLASS ( MOAICpConstraint )
	REGISTER_LUA_CLASS ( MOAICpShape )
	REGISTER_LUA_CLASS ( MOAICpSpace )
}
