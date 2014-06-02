// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIShader.h>
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
bool MOAIShader::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	attrID = ( attrID & MOAIAttrOp::ATTR_ID_MASK ) - 1;

	if ( attrID >= this->mUniformBuffers.Size ()) return false;

	if ( op == MOAIAttrOp::CHECK ) {
		attrOp.SetFlags ( MOAIAttrOp::ATTR_WRITE );
		return true;
	}

	if ( op == MOAIAttrOp::SET ) {
		this->mUniformBuffers [ attrID ].SetValue ( attrOp, false );
		return true;
	}

	if ( op == MOAIAttrOp::ADD ) {
		this->mUniformBuffers [ attrID ].AddValue ( attrOp );
		return true;
	}

	return false;
}

//----------------------------------------------------------------//
void MOAIShader::BindUniforms () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIShaderProgram* program = this->mProgram;

	if ( program ) {
		
		u32 nUniforms = program->mUniforms.Size ();
		
		for ( u32 i = 0; i < nUniforms; ++i ) {
			MOAIShaderUniform& uniform = program->mUniforms [ i ];
			if ( uniform.SetValue ( this->mUniformBuffers [ i ], true )) {
				gfxDevice.Flush ();
				uniform.Bind ();
			}
		}
	}
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
		
		u32 nUniforms = program->mUniforms.Size ();
		this->mUniformBuffers.Init ( nUniforms );
		
		for ( u32 i = 0; i < nUniforms; ++i ) {
		
			u32 type = program->mUniforms [ i ].GetType ();
			MOAIShaderUniformBuffer& uniformBuffer = this->mUniformBuffers [ i ];
			uniformBuffer.SetType ( program->mUniforms [ i ].GetType ());
			
			switch ( type ) {
				case MOAIShaderUniformBuffer::UNIFORM_INDEX: {
					uniformBuffer.SetValue ( 1 );
					break;
				}
				case MOAIShaderUniformBuffer::UNIFORM_MATRIX_F3: {
					ZLMatrix3x3 mtx;
					mtx.Ident ();
					uniformBuffer.SetValue ( mtx, false );
					break;
				}
				case MOAIShaderUniformBuffer::UNIFORM_MATRIX_F4: {
					ZLMatrix4x4 mtx;
					mtx.Ident ();
					uniformBuffer.SetValue ( mtx, false );
					break;
				}
			}
		}
	}
}
