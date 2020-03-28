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
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIShaderGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setProgram",					_setProgram },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIShaderGL::MOAIAbstractHasGfxScript_GetGfxScript	() {

	MOAIAbstractGfxScript* gfxScript = this->MOAIHasGfxScript::MOAIAbstractHasGfxScript_GetGfxScript ();
	return gfxScript ? gfxScript : ( this->mProgram ? this->mProgram->GetGfxScript () : NULL );
}

//----------------------------------------------------------------//
const MOAIUniformSchema* MOAIShaderGL::MOAIAbstractUniformBuffer_GetSchema () const {

	return this->mProgram;
}

//----------------------------------------------------------------//
bool MOAIShaderGL::MOAIShader_IsReadyForUse () const {

	return ( this->mProgram && this->mProgram->IsReadyForUse ());
}
