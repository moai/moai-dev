// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAIShaderGL.h>
#include <moai-gfx/MOAIShaderProgramGL.h>
#include <moai-gfx/MOAIShaderMgrGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderGL::_getAttributeID ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderGL, "UN" )

	ZLIndex uniformID		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	ZLIndex index			= state.GetValue < MOAILuaIndex >( 3, ZLIndexOp::ZERO );
	
	if ( self->mProgram ) {
		state.Push ( self->mProgram->GetAttributeID ( uniformID, index ));
	}
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderGL::_setProgram ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderGL, "U" )
	
	self->SetProgram ( state.GetLuaObject < MOAIShaderProgramGL >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
int MOAIShaderGL::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderGL, "U" )

	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );

	MOAIShaderProgramGL* program = MOAICast < MOAIShaderProgramGL >( self->mProgram );

	if ( program ) {
		program->SetUniform ( L, 3, self->mPendingUniformBuffer.GetBuffer (), uniformID, ZLIndexOp::ZERO );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIShaderGL::_setUniformArrayItem ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderGL, "U" )

	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 3, ZLIndexOp::ZERO );

	MOAIShaderProgramGL* program = MOAICast < MOAIShaderProgramGL >( self->mProgram );

	if ( program ) {
		program->SetUniform ( L, 4, self->mPendingUniformBuffer.GetBuffer (), uniformID, index );
	}
	return 0;
}

//================================================================//
// MOAIShaderGL
//================================================================//

//----------------------------------------------------------------//
MOAIShaderGL::MOAIShaderGL () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
		RTTI_EXTEND ( ZLShaderGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderGL::~MOAIShaderGL () {

	this->SetProgram ( 0 );
}

//----------------------------------------------------------------//
void MOAIShaderGL::RegisterLuaClass ( MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIShaderGL::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getAttributeID",				_getAttributeID },
		{ "setProgram",					_setProgram },
		{ "setUniform",					_setUniform },
		{ "setUniformArrayItem",		_setUniformArrayItem },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIShaderGL::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	MOAIShaderProgramGL* program = MOAICast < MOAIShaderProgramGL >( this->mProgram );
	if ( program ) {
		return program->ApplyAttrOp ( this->mPendingUniformBuffer.GetBuffer (), attrID, attr, op );
	}
	return false;
}
