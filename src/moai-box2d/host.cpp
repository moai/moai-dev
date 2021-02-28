// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-box2d/host.h>
#include <moai-box2d/headers.h>

//================================================================//
// aku-box2d
//================================================================//

//----------------------------------------------------------------//
void AKUBox2DAppFinalize () {
}

//----------------------------------------------------------------//
void AKUBox2DAppInitialize () {
}

//----------------------------------------------------------------//
void AKUBox2DContextInitialize ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	REGISTER_LUA_CLASS ( context, MOAIBox2DArbiter )
	REGISTER_LUA_CLASS ( context, MOAIBox2DBody )
	REGISTER_LUA_CLASS ( context, MOAIBox2DDistanceJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DFixture )
	REGISTER_LUA_CLASS ( context, MOAIBox2DFrictionJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DGearJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DMouseJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DPrismaticJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DPulleyJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DRopeJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DRevoluteJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DWeldJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DWheelJoint )
	REGISTER_LUA_CLASS ( context, MOAIBox2DWorld )
}
