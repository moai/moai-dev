// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDrawingAPIObjectVK.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractDrawingAPIObjectVK::_loadShaderUniformVK ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingAPIObjectVK, "U" )
	
	ZLIndex globalID		= state.GetValue < ZLIndex >( 3, 0 );
	ZLIndex uniformID		= state.GetValue < ZLIndex >( 3, 0 );
	ZLIndex index			= state.GetValue < ZLIndex >( 3, 0 );
	
	self->LoadShaderUniformVK ( globalID, uniformID, index );
	return 0;
}

//================================================================//
// MOAIAbstractDrawingAPIObjectVK
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIObjectVK::MOAIAbstractDrawingAPIObjectVK () {

	RTTI_BEGIN ( MOAIAbstractDrawingAPIObjectVK )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractDrawingAPIObjectVK >)
		RTTI_EXTEND ( MOAIAbstractDrawingAPIObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIObjectVK::~MOAIAbstractDrawingAPIObjectVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIObjectVK::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIObjectVK::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "loadShaderUniformVK",		_loadShaderUniformVK },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
