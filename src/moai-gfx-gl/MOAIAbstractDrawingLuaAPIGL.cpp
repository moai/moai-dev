// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIAbstractDrawingLuaAPIGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPIGL::_loadShaderUniformGL ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPIGL, "U" )
	
	ZLIndex globalID		= state.GetValue < ZLIndex >( 3, 0 );
	ZLIndex uniformID		= state.GetValue < ZLIndex >( 3, 0 );
	ZLIndex index			= state.GetValue < ZLIndex >( 3, 0 );
	
	self->LoadShaderUniformGL ( globalID, uniformID, index );
	return 0;
}

//================================================================//
// MOAIAbstractDrawingLuaAPIGL
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDrawingLuaAPIGL::MOAIAbstractDrawingLuaAPIGL () {

	RTTI_BEGIN ( MOAIAbstractDrawingLuaAPIGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractDrawingLuaAPIGL >)
		RTTI_EXTEND ( MOAIAbstractDrawingLuaAPI )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDrawingLuaAPIGL::~MOAIAbstractDrawingLuaAPIGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingLuaAPIGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingLuaAPIGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "loadShaderUniformGL",		_loadShaderUniformGL },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
