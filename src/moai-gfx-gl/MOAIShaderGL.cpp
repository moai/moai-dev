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
int MOAIShaderGL::_setProgram ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderGL, "U" )
	
	self->SetProgram ( state.GetLuaObject < MOAIShaderProgramGL >( 2, true ));

	return 0;
}

//================================================================//
// MOAIShaderGL
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderGL::ApplyUniforms () {

	if ( this->mProgram ) {
		this->mProgram->ApplyUniforms ( this->mUniforms );
	}
}

//----------------------------------------------------------------//
void MOAIShaderGL::BindUniforms () {
	
	if ( this->mProgram ) {
		this->mProgram->BindUniforms ();
	}
}

//----------------------------------------------------------------//
void MOAIShaderGL::ExecuteSetUniformGL ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParamGL::LoadShaderUniform& param ) {
	
	MOAIUniformHandle uniform = this->GetUniformHandle ( param.mTargetUniformID, param.mTargetUniformIndex );
	if ( !uniform.IsValid ()) return;

	if ( param.mPipelineGlobalID < MOAIGfxMgr::TOTAL_MATRICES ) {

		uniform.SetValue ( gfxMgr.GetMtx ( param.mPipelineGlobalID ));
	}
	else {

		switch (( ZLSize )param.mPipelineGlobalID ) {

			case MOAIGfxMgr::PEN_COLOR:

				uniform.SetValue ( gfxMgr.GetFinalColor ());
				break;

			case MOAIGfxMgr::VIEW_HALF_HEIGHT:

				uniform.SetValue ( gfxMgr.GetViewRect ().Height () * 0.5f );
				break;

			case MOAIGfxMgr::VIEW_HALF_WIDTH: {

				uniform.SetValue ( gfxMgr.GetViewRect ().Width () * 0.5f );
				break;
			}
			case MOAIGfxMgr::VIEW_HEIGHT:

				uniform.SetValue ( gfxMgr.GetViewRect ().Height ());
				break;

			case MOAIGfxMgr::VIEW_WIDTH:

				uniform.SetValue ( gfxMgr.GetViewRect ().Width ());
				break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderGL::GatherUniforms () {

	MOAIGfxScript* gfxScript = this->GetGfxScript ();
	gfxScript = gfxScript ? gfxScript : ( this->mProgram ? this->mProgram->GetGfxScript () : NULL );
	
	if ( gfxScript ) {
		gfxScript->ExecuteBytecode ( this );
	}
}

//----------------------------------------------------------------//
MOAIUniformHandle MOAIShaderGL::GetUniformHandle ( ZLIndex uniformID, ZLIndex index ) {

	return this->mProgram->GetUniformHandle ( this->mUniforms.GetBuffer (), uniformID, index );
}

//----------------------------------------------------------------//
bool MOAIShaderGL::HasDirtyUniforms () {

	return !this->mProgram->mUniformBuffer.IsIdentical ( this->mUniforms );
}

//----------------------------------------------------------------//
MOAIShaderGL::MOAIShaderGL () {

	RTTI_BEGIN ( MOAIShaderGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIShaderGL >)
		RTTI_EXTEND ( MOAIShader )
		RTTI_EXTEND ( MOAIAbstractUniformBuffer )
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
		program->InitUniformBuffer ( this->mUniforms );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIShaderGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setProgram",					_setProgram },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIShaderGL::MOAIAbstractCmdStreamFilter_FilterCommand ( u32 cmd, const void* param, ZLSize paramSize ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
	
		case MOAIDrawAPIGL::LOAD_SHADER_UNIFORM_GL: {
			this->ExecuteSetUniformGL ( gfxMgr, *( const MOAIDrawingParamGL::LoadShaderUniform* )param );
			break;
		}
	}
	return true;
}

//----------------------------------------------------------------//
const MOAIUniformSchema* MOAIShaderGL::MOAIAbstractUniformBuffer_GetSchema () const {

	return this->mProgram;
}

//----------------------------------------------------------------//
bool MOAIShaderGL::MOAIShader_IsReadyForUse () const {

	return ( this->mProgram && this->mProgram->IsReadyForUse ());
}
