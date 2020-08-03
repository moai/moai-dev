// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIAbstractDrawingObjectGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractDrawingObjectGL::_loadShaderUniformGL ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingObjectGL, "U" )
	
	ZLIndex globalID		= state.GetValue < u32 >( 2, 0 );
	ZLIndex uniformID		= state.GetValue < u32 >( 3, 0 );
	ZLIndex index			= state.GetValue < u32 >( 4, 0 );
	
	self->LoadShaderUniformGL ( globalID, uniformID, index );
	return 0;
}

//================================================================//
// MOAIAbstractDrawingObjectGL
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDrawingObjectGL::MOAIAbstractDrawingObjectGL () {

	RTTI_BEGIN ( MOAIAbstractDrawingObjectGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractDrawingObjectGL >)
		RTTI_EXTEND ( MOAIAbstractDrawingObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDrawingObjectGL::~MOAIAbstractDrawingObjectGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingObjectGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingObjectGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "loadShaderUniformGL",		_loadShaderUniformGL },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
