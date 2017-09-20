// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_setProgram ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )
	
	self->SetProgram ( state.GetLuaObject < MOAIShaderProgram >( 2, true ));

	return 0;
}

//================================================================//
// MOAIShader
//================================================================//

//----------------------------------------------------------------//
MOAIShader* MOAIShader::AffirmShader ( MOAILuaState& state, int idx ) {

	MOAIShader* shader = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = MOAIShaderMgr::Get ().GetShader ( state.GetValue < u32 >( idx, MOAIShaderMgr::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShader >( idx, true );
	}
	return shader;
}

//----------------------------------------------------------------//
bool MOAIShader::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	attrID = ( attrID & MOAIAttrOp::ATTR_ID_MASK ) - 1;

	if ( attrID >= this->mUniformBuffers.Size ()) return false;

	switch ( op ) {

		case MOAIAttrOp::CHECK:
			this->mUniformBuffers [ attrID ].GetFlags ( attrOp );
			return true;

		case MOAIAttrOp::SET:
			this->mUniformBuffers [ attrID ].SetValue ( attrOp );
			return true;

		case MOAIAttrOp::ADD:
			this->mUniformBuffers [ attrID ].AddValue ( attrOp );
			return true;
		
		case MOAIAttrOp::GET:
			this->mUniformBuffers [ attrID ].GetValue ( attrOp );
			return true;
	}

	return false;
}

//----------------------------------------------------------------//
MOAIShader::MOAIShader () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShader::~MOAIShader () {

	this->SetProgram ( 0 );
}

//----------------------------------------------------------------//
void MOAIShader::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIShader::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setProgram",					_setProgram },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShader::SetProgram ( MOAIShaderProgram* program ) {

	this->mUniformBuffers.Clear ();
	this->mProgram.Set ( *this, program );
	
	if ( program ) {
		
		size_t nUniforms = program->mUniforms.Size ();
		this->mUniformBuffers.Init ( nUniforms );
		
		for ( size_t i = 0; i < nUniforms; ++i ) {

			MOAIShaderUniformBuffer& uniformBuffer = this->mUniformBuffers [ i ];
			uniformBuffer.SetType ( program->mUniforms [ i ].GetType ());
			uniformBuffer.SetValue ( program->mDefaults [ i ]);
		}
	}
}

//----------------------------------------------------------------//
void MOAIShader::UpdateAndBindUniforms () {

	MOAIShaderProgram* program = this->mProgram;

	if ( program ) {
		
		size_t nUniforms = program->mUniforms.Size ();
		
		for ( size_t i = 0; i < nUniforms; ++i ) {
			MOAIShaderUniform& uniform = program->mUniforms [ i ];
			if (( uniform.mFlags & MOAIShaderUniform::UNIFORM_FLAG_GLOBAL ) == 0 ) {
				uniform.mFlags |= uniform.SetValue ( this->mUniformBuffers [ i ]);
			}
		}
		
		program->ApplyGlobals ();
		program->BindUniforms ();
	}
}
