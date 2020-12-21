// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawAPIGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIDrawAPIGL::_loadShaderUniformGL ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPIGL )
	
	ZLIndex globalID		= state.GetValue < u32 >( 2, 0 );
	ZLIndex uniformID		= state.GetValue < u32 >( 3, 0 );
	ZLIndex index			= state.GetValue < u32 >( 4, 0 );
	
	self->LoadShaderUniformGL ( globalID, uniformID, index );
	return 0;
}

//================================================================//
// MOAIDrawAPIGL
//================================================================//

//----------------------------------------------------------------//
MOAIDrawAPIGL& MOAIDrawAPIGL::LoadShaderUniformGL ( ZLIndex globalID, ZLIndex uniformID, ZLIndex index ) {

	MOAIDrawingParamGL::LoadShaderUniform param;
	param.mPipelineGlobalID = globalID;
	param.mTargetUniformID = uniformID;
	param.mTargetUniformIndex = index;
	this->SubmitCommand < MOAIDrawingParamGL::LoadShaderUniform >( MOAIDrawAPIGL::LOAD_SHADER_UNIFORM_GL, param );
	
	return *this;
}

//----------------------------------------------------------------//
MOAIDrawAPIGL::MOAIDrawAPIGL () {
}

//----------------------------------------------------------------//
MOAIDrawAPIGL::~MOAIDrawAPIGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawAPIGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDrawAPIGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDrawAPIGL::MOAIAbstractCmdAPI_RegisterLuaAPI ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "loadShaderUniformGL",		_loadShaderUniformGL },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
