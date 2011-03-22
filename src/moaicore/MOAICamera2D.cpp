// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICamera2D.h>

//================================================================//
// MOAICamera2D
//================================================================//

//----------------------------------------------------------------//
MOAICamera2D::MOAICamera2D () {
	
	RTTI_SINGLE ( MOAITransform2D )
}

//----------------------------------------------------------------//
MOAICamera2D::~MOAICamera2D () {
}

//----------------------------------------------------------------//
void MOAICamera2D::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAICamera2D::RegisterLuaFuncs ( USLuaState& state ) {

	MOAITransform2D::RegisterLuaFuncs ( state );

	//LuaReg regTable [] = {
	//	{ "setLoc",			_setLoc },
	//	{ "setScale",		_setScale },
	//	{ NULL, NULL }
	//};

	//luaL_register ( state, 0, regTable );
}


