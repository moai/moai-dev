// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DWeldJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIBox2DWeldJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DWeldJoint::MOAIBox2DWeldJoint () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DWeldJoint::~MOAIBox2DWeldJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DWeldJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DWeldJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );
}
