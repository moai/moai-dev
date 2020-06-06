// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDrawingLuaAPIVK.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPIVK::_loadShaderUniformVK ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPIVK, "U" )
	
	ZLIndex globalID		= state.GetValue < u32 >( 3, 0 );
	ZLIndex uniformID		= state.GetValue < u32 >( 3, 0 );
	ZLIndex index			= state.GetValue < u32 >( 3, 0 );
	
	self->LoadShaderUniformVK ( globalID, uniformID, index );
	return 0;
}

//================================================================//
// MOAIAbstractDrawingLuaAPIVK
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDrawingLuaAPIVK::MOAIAbstractDrawingLuaAPIVK () {

	RTTI_BEGIN ( MOAIAbstractDrawingLuaAPIVK )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractDrawingLuaAPIVK >)
		RTTI_EXTEND ( MOAIAbstractDrawingLuaAPI )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDrawingLuaAPIVK::~MOAIAbstractDrawingLuaAPIVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingLuaAPIVK::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingLuaAPIVK::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "loadShaderUniformVK",		_loadShaderUniformVK },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
