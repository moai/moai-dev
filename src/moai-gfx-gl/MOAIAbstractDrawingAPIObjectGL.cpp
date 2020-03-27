// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIAbstractDrawingAPIObjectGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractDrawingAPIObjectGL::_loadShaderUniformGL ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingAPIObjectGL, "U" )
	
	ZLIndex globalID		= state.GetValue < ZLIndex >( 3, 0 );
	ZLIndex uniformID		= state.GetValue < ZLIndex >( 3, 0 );
	ZLIndex index			= state.GetValue < ZLIndex >( 3, 0 );
	
	self->LoadShaderUniformGL ( globalID, uniformID, index );
	return 0;
}

//================================================================//
// MOAIAbstractDrawingAPIObjectGL
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIObjectGL::MOAIAbstractDrawingAPIObjectGL () {

	RTTI_BEGIN ( MOAIAbstractDrawingAPIObjectGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractDrawingAPIObjectGL >)
		RTTI_EXTEND ( MOAIAbstractDrawingAPIObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIObjectGL::~MOAIAbstractDrawingAPIObjectGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIObjectGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIObjectGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "loadShaderUniformGL",		_loadShaderUniformGL },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
