// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetArrayVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
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

//----------------------------------------------------------------//
MOAIShaderVK::MOAIShaderVK () {

	RTTI_BEGIN ( MOAIShaderVK )
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
	MOAIPipelineLayoutVK* pipelineLayout = program ? program->GetPipelineLayout () : NULL;

	if ( pipelineLayout ) {
		this->mDescriptorSetArray = new MOAIDescriptorSetArrayVK ();
		this->mDescriptorSetArray->SetPipelineLayout ( *pipelineLayout );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIShaderVK::MOAIAbstractHasGfxScript_GetGfxScript () {

	MOAIAbstractGfxScript* gfxScript = this->MOAIHasGfxScript::MOAIAbstractHasGfxScript_GetGfxScript ();
	return gfxScript ? gfxScript : ( this->mProgram ? this->mProgram->GetGfxScript () : NULL );
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
