// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIShaderVK::_getAttributeID ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderVK, "UN" )
//
//	ZLIndex uniformID		= state.GetValue < MOAILuaIndex >( 2, 0 );
//	ZLIndex index			= state.GetValue < MOAILuaIndex >( 3, 0 );
//
//	if ( self->mProgram ) {
//		state.Push ( self->mProgram->GetAttributeID ( uniformID, index ));
//	}
//	return 1;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIShaderVK::_setProgram ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderVK, "U" )
//
//	self->SetProgram ( state.GetLuaObject < MOAIShaderProgramVK >( 2, true ));
//
//	return 0;
//}
//
////----------------------------------------------------------------//
//int MOAIShaderVK::_setUniform ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderVK, "U" )
//
//	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 2, 0 );
//
//	MOAIShaderProgramVK* program = MOAICast < MOAIShaderProgramVK >( self->mProgram );
//
//	if ( program ) {
//		program->SetUniform ( L, 3, self->mPendingUniformBuffer.GetBuffer (), uniformID, 0 );
//	}
//	return 0;
//}
//
////----------------------------------------------------------------//
//int MOAIShaderVK::_setUniformArrayItem ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderVK, "U" )
//
//	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 2, 0 );
//	ZLIndex index		= state.GetValue < MOAILuaIndex >( 3, 0 );
//
//	MOAIShaderProgramVK* program = MOAICast < MOAIShaderProgramVK >( self->mProgram );
//
//	if ( program ) {
//		program->SetUniform ( L, 4, self->mPendingUniformBuffer.GetBuffer (), uniformID, index );
//	}
//	return 0;
//}

//================================================================//
// MOAIShaderVK
//================================================================//

////----------------------------------------------------------------//
//void MOAIShaderVK::ApplyUniforms () {
//
//	if ( this->mProgram ) {
//		this->mProgram->ApplyUniforms ( this->mPendingUniformBuffer );
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIShaderVK::BindUniforms () {
//
//	if ( this->mProgram ) {
//		this->mProgram->BindUniforms ();
//	}
//}

//----------------------------------------------------------------//
MOAIGfxComposerVK& MOAIShaderVK::GetGfxComposer () {

	assert ( this->mProgram );
	return this->mProgram->GetGfxComposer ();
}

////----------------------------------------------------------------//
//bool MOAIShaderVK::HasDirtyUniforms () {
//
//	return !this->mProgram->mUniformBuffer.IsIdentical ( this->mPendingUniformBuffer );
//}

//----------------------------------------------------------------//
MOAIShaderVK::MOAIShaderVK () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIShader )
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderVK::~MOAIShaderVK () {

//	this->SetProgram ( 0 );
}

////----------------------------------------------------------------//
//void MOAIShaderVK::ScheduleTextures () {
//
//	if ( this->mProgram ) {
//		this->mProgram->ScheduleTextures ();
//	}
//}

//----------------------------------------------------------------//
void MOAIShaderVK::SetProgram ( MOAIShaderProgramVK* program ) {

	this->mProgram = program;

//	if ( program ) {
//		program->InitUniformBuffer ( this->mPendingUniformBuffer );
//	}
}

////----------------------------------------------------------------//
//void MOAIShaderVK::UpdateUniforms () {
//
//	if ( this->mProgram ) {
//		this->mProgram->UpdateUniforms ( this->mPendingUniformBuffer );
//	}
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIShaderVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );

//	luaL_Reg regTable [] = {
//		{ "getAttributeID",				_getAttributeID },
//		{ "setProgram",					_setProgram },
//		{ "setUniform",					_setUniform },
//		{ "setUniformArrayItem",		_setUniformArrayItem },
//		{ NULL, NULL }
//	};
//	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIShaderVK::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

//	MOAIShaderProgramVK* program = MOAICast < MOAIShaderProgramVK >( this->mProgram );
//	if ( program ) {
//		return program->ApplyAttrOp ( this->mPendingUniformBuffer.GetBuffer (), attrID, attr, op );
//	}
//	return false;
	return true;
}

//----------------------------------------------------------------//
void MOAIShaderVK::MOAIShader_ComposeUniforms () {
}

//----------------------------------------------------------------//
bool MOAIShaderVK::MOAIShader_IsReadyForUse () const {

	return true;
}

//----------------------------------------------------------------//
void MOAIShaderVK::MOAIShader_SelectTextures () {
}
