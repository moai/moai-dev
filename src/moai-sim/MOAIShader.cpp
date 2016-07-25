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

//----------------------------------------------------------------//
int MOAIShader::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )

	u32 uniform = state.GetValue ( 2, 1 ) - 1;
	
	if ( uniform < self->mUniformBuffers.Size ()) {

		int size = state.GetTop () - 2;
		
		if ( size < 4 ) {
		
			ZLVec4D vec = state.GetValue < ZLVec4D >( 2, ZLVec4D::ZERO );
			
			self->mUniformBuffers [ uniform ].SetValue ( vec );
		}
		else {

			ZLMatrix4x4 mtx = ZLMatrix4x4::IDENT;

			switch ( size ) {
			
				case 6:
					self->mUniformBuffers [ uniform ].SetValue ( state.GetValue < ZLAffine2D >( 3, ZLAffine2D::IDENT ));
					break;
				
				case 9:
					self->mUniformBuffers [ uniform ].SetValue ( state.GetValue < ZLMatrix3x3 >( 3, ZLMatrix3x3::IDENT ));
					break;
				
				case 12:
					self->mUniformBuffers [ uniform ].SetValue ( state.GetValue < ZLAffine3D >( 3, ZLAffine3D::IDENT ));
					break;
				
				case 16:
					self->mUniformBuffers [ uniform ].SetValue ( state.GetValue < ZLMatrix4x4 >( 3, ZLMatrix4x4::IDENT ));
					break;
				
				default:
					MOAILogF ( L, ZLLog::LOG_ERROR, "Error setting matrix. Did not recognize number of paramaters (6, 9, 12 or 16)." );
			}
		}
	}
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
bool MOAIShader::ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	attrID = ( attrID & MOAIAttribute::ATTR_ID_MASK ) - 1;

	if ( attrID >= this->mUniformBuffers.Size ()) return false;

	switch ( op ) {

		case MOAIAttribute::CHECK:
			attr.SetFlags ( MOAIAttribute::ATTR_WRITE );
			return true;

		case MOAIAttribute::SET:
			this->mUniformBuffers [ attrID ].SetValue ( attr );
			return true;

		case MOAIAttribute::ADD:
			this->mUniformBuffers [ attrID ].AddValue ( attr );
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
		{ "setUniform",					_setUniform },
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

			MOAIShaderUniform& uniform = program->mUniforms [ i ];
			MOAIShaderUniformBuffer& uniformBuffer = this->mUniformBuffers [ i ];
			
			uniformBuffer.Init ( uniform.mType, uniform.mWidth );
			uniformBuffer.Default ();
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
