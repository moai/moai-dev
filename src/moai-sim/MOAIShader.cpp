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

	u32 uniformID	= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 index		= state.GetValue < u32 >( 3, 1 ) - 1;

	MOAIShaderUniform* uniform = self->GetUniform ( uniformID );
	
	if ( uniform ) {

		int size = state.GetTop () - 3;
		
		if ( size < 4 ) {
		
			ZLVec4D vec = state.GetValue < ZLVec4D >( 3, ZLVec4D::ZERO );
			
			uniform->SetValue ( self->mUniformBuffer, index, vec );
		}
		else {

			ZLMatrix4x4 mtx = ZLMatrix4x4::IDENT;

			switch ( size ) {
			
				case 6:
					uniform->SetValue ( self->mUniformBuffer, index, state.GetValue < ZLAffine2D >( 3, ZLAffine2D::IDENT ));
					break;
				
				case 9:
					uniform->SetValue ( self->mUniformBuffer, index, state.GetValue < ZLMatrix3x3 >( 3, ZLMatrix3x3::IDENT ));
					break;
				
				case 12:
					uniform->SetValue ( self->mUniformBuffer, index, state.GetValue < ZLAffine3D >( 3, ZLAffine3D::IDENT ));
					break;
				
				case 16:
					uniform->SetValue ( self->mUniformBuffer, index, state.GetValue < ZLMatrix4x4 >( 3, ZLMatrix4x4::IDENT ));
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

	if ( this->mProgram ) {

		u32 index = 0;
		MOAIShaderUniform* uniform = this->mProgram->GetUniformForAttributeID (( attrID & MOAIAttribute::ATTR_ID_MASK ) - 1, index );
		
		if ( uniform ) {

			switch ( op ) {

				case MOAIAttribute::CHECK:
					attr.SetFlags ( MOAIAttribute::ATTR_WRITE );
					return true;

				case MOAIAttribute::SET:
					uniform->SetValue ( this->mUniformBuffer, index, attr );
					return true;

				case MOAIAttribute::ADD:
					uniform->AddValue ( this->mUniformBuffer, index, attr );
					return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------//
MOAIShaderUniform* MOAIShader::GetUniform ( u32 uniformID ) {

	return this->mProgram ? this->mProgram->GetUniform ( uniformID ) : 0;
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

	this->mUniformBuffer.Clear ();
	this->mProgram.Set ( *this, program );
	
	if ( program ) {
		
		this->mUniformBuffer.Init ( program->mUniformBuffer.Size ());
		
		size_t nUniforms = program->mUniforms.Size ();
		for ( size_t i = 0; i < nUniforms; ++i ) {
			program->mUniforms [ i ].Default ( this->mUniformBuffer );
		}
	}
}

//----------------------------------------------------------------//
void MOAIShader::UpdateAndBindUniforms () {

	MOAIShaderProgram* program = this->mProgram;

	if ( program ) {
	
		program->ApplyGlobals ();
	
		size_t nUniforms = program->mUniforms.Size ();
		
		for ( size_t i = 0; i < nUniforms; ++i ) {
		
			MOAIShaderUniform& uniform = program->mUniforms [ i ];
			
			for ( u32 j = 0; j < uniform.mCount; ++j ) {
			
				u8& flag = program->mUniformFlags [ uniform.mFlagsBase + j ];
			
				if (( flag & MOAIShaderUniform::UNIFORM_FLAG_GLOBAL ) == 0 ) {
					flag |= uniform.CopyValue ( program->mUniformBuffer, this->mUniformBuffer, j );
				}
				flag &= ~MOAIShaderUniform::UNIFORM_FLAG_GLOBAL;
			}
		}
		program->BindUniforms ();
	}
}
