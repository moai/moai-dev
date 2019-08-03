// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIShaderProgramGL.h>

//================================================================//
// lua
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
void MOAIShaderGL::ApplyUniforms () {

	if ( this->mProgram ) {
		this->mProgram->ApplyUniforms ( this->mPendingUniformBuffer );
	}
}

//----------------------------------------------------------------//
void MOAIShaderGL::BindUniforms () {
	
	if ( this->mProgram ) {
		this->mProgram->BindUniforms ();
	}
}

//----------------------------------------------------------------//
bool MOAIShaderGL::HasDirtyUniforms () {

	return !this->mProgram->mUniformBuffer.IsIdentical ( this->mPendingUniformBuffer );
}

//----------------------------------------------------------------//
MOAIShaderGL::MOAIShaderGL () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIShader )
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderGL::~MOAIShaderGL () {

	this->SetProgram ( 0 );
}

//----------------------------------------------------------------//
void MOAIShaderGL::SetProgram ( MOAIShaderProgramGL* program ) {
	
	this->mProgram = program;
	
	if ( program ) {
		program->InitUniformBuffer ( this->mPendingUniformBuffer );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIShaderGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );

	luaL_Reg regTable [] = {
		{ "getAttributeID",				_getAttributeID },
		{ "setProgram",					_setProgram },
		{ "setUniform",					_setUniform },
		{ "setUniformArrayItem",		_setUniformArrayItem },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIShaderGL::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	MOAIShaderProgramGL* program = MOAICast < MOAIShaderProgramGL >( this->mProgram );
	if ( program ) {
		return program->ApplyAttrOp ( this->mPendingUniformBuffer.GetBuffer (), attrID, attr, op );
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIShaderGL::MOAIShader_IsReady () const {

	return ( this->mProgram && this->mProgram->IsReady ());
}

//----------------------------------------------------------------//
void MOAIShaderGL::MOAIShader_SelectTextures () {

	if ( this->mProgram ) {
		this->mProgram->SelectTextures ();
	}
}

//----------------------------------------------------------------//
void MOAIShaderGL::MOAIShader_UpdateUniforms () {

	if ( this->mProgram ) {
		this->mProgram->UpdateUniforms ( this->mPendingUniformBuffer );
	}
}
