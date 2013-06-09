// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-box2d/host.h>
#include <moai-box2d/headers.h>

//================================================================//
// aku-util
//================================================================//

static bool sIsInitialized = false;

//----------------------------------------------------------------//
void AKUFinalizeBox2D () {
	
	if ( !sIsInitialized ) return;
	sIsInitialized = false;
}

//----------------------------------------------------------------//
void AKUInitializeBox2D () {

	if ( !sIsInitialized ) {
		sIsInitialized = true;
	}

    REGISTER_LUA_CLASS ( MOAIBox2DArbiter )
	REGISTER_LUA_CLASS ( MOAIBox2DBody )
	REGISTER_LUA_CLASS ( MOAIBox2DDistanceJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DFixture )
	REGISTER_LUA_CLASS ( MOAIBox2DFrictionJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DGearJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DMouseJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DPrismaticJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DPulleyJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DRopeJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DRevoluteJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DWeldJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DWheelJoint )
	REGISTER_LUA_CLASS ( MOAIBox2DWorld )
}
