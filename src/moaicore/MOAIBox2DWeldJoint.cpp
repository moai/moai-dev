// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DWeldJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

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

#endif
