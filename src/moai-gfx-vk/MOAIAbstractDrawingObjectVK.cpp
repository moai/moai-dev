// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDrawingObjectVK.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractDrawingObjectVK::_loadShaderUniformVK ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingObjectVK, "U" )
	
	ZLIndex globalID		= state.GetValue < u32 >( 3, 0 );
	ZLIndex uniformID		= state.GetValue < u32 >( 3, 0 );
	ZLIndex index			= state.GetValue < u32 >( 3, 0 );
	
	self->LoadShaderUniformVK ( globalID, uniformID, index );
	return 0;
}

//================================================================//
// MOAIAbstractDrawingObjectVK
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDrawingObjectVK::MOAIAbstractDrawingObjectVK () {

	RTTI_BEGIN ( MOAIAbstractDrawingObjectVK )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractDrawingObjectVK >)
		RTTI_EXTEND ( MOAIAbstractDrawingObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDrawingObjectVK::~MOAIAbstractDrawingObjectVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingObjectVK::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingObjectVK::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "loadShaderUniformVK",		_loadShaderUniformVK },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
