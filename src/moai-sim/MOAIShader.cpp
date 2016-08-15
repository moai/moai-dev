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
int MOAIShader::_getAttributeID ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "UN" )

	u32 uniformID		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 index			= state.GetValue < u32 >( 3, 1 ) - 1;
	
	state.Push ( self->GetAttributeID ( uniformID, index ));

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_reserveGlobals ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	return self->ReserveGlobals ( L, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_setGlobal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNNN" )
	
	return self->SetGlobal ( L, 2);
}

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

		MOAIShaderUniformInstance& instance = self->mUniformInstances [ uniformID ];

		int size = state.GetTop () - 3;
		
		if ( size < 4 ) {
		
			ZLVec4D vec = state.GetValue < ZLVec4D >( 3, ZLVec4D::ZERO );
			
			uniform->SetValue ( instance, index, vec );
		}
		else {

			ZLMatrix4x4 mtx = ZLMatrix4x4::IDENT;

			switch ( size ) {
			
				case 6:
					uniform->SetValue ( instance, index, state.GetValue < ZLAffine2D >( 3, ZLAffine2D::IDENT ));
					break;
				
				case 9:
					uniform->SetValue ( instance, index, state.GetValue < ZLMatrix3x3 >( 3, ZLMatrix3x3::IDENT ));
					break;
				
				case 12:
					uniform->SetValue ( instance, index, state.GetValue < ZLAffine3D >( 3, ZLAffine3D::IDENT ));
					break;
				
				case 16:
					uniform->SetValue ( instance, index, state.GetValue < ZLMatrix4x4 >( 3, ZLMatrix4x4::IDENT ));
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

		u32 uniformID = ( attrID & MOAIAttribute::ATTR_ID_MASK ) - 1;
		u32 index = 0;
		MOAIShaderUniform* uniform = this->GetUniformForAttributeID ( uniformID, index );
		
		if ( uniform ) {
		
			MOAIShaderUniformInstance& instance = this->mUniformInstances [ uniformID ];

			switch ( op ) {

				case MOAIAttribute::CHECK:
					attr.SetFlags ( MOAIAttribute::ATTR_WRITE );
					return true;

				case MOAIAttribute::SET:
					uniform->SetValue ( instance, index, attr );
					return true;

				case MOAIAttribute::ADD:
					uniform->AddValue ( instance, index, attr );
					return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIShader::ApplyGlobals () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	ZLRect viewRect = gfxMgr.mGfxState.GetViewRect ();

	// NOTE: matrices are submitted *column major*; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	for ( u32 i = 0; i < this->mGlobals.Size (); ++i ) {
	
		const MOAIShaderProgramGlobal& global = this->mGlobals [ i ];
		
		if ( global.mUniformID == INVALID_INDEX ) continue;
		
		MOAIShaderUniform* uniform = this->GetUniform ( global.mUniformID );
		if ( !uniform ) continue;
		
		MOAIShaderUniformInstance& instance = this->mUniformInstances [global.mUniformID ];
		
		if ( global.mGlobalID < MOAIGfxGlobalsCache::TOTAL_MATRICES ) {
		
			uniform->SetValue ( instance, global.mIndex, gfxMgr.mGfxState.GetMtx ( global.mGlobalID ));
		}
		else {
		
			switch ( global.mGlobalID ) {
				
				case MOAIGfxGlobalsCache::PEN_COLOR:
				
					uniform->SetValue ( instance, global.mIndex, gfxMgr.mGfxState.GetFinalColor ());
					break;
				
				case MOAIGfxGlobalsCache::VIEW_HALF_HEIGHT:
				
					uniform->SetValue ( instance, global.mIndex, viewRect.Height () * 0.5f );
					break;
					
				case MOAIGfxGlobalsCache::VIEW_HALF_WIDTH: {
				
					uniform->SetValue ( instance, global.mIndex, viewRect.Width () * 0.5f );
					break;
				}
				case MOAIGfxGlobalsCache::VIEW_HEIGHT:
				
					uniform->SetValue ( instance, global.mIndex, viewRect.Height ());
					break;
					
				case MOAIGfxGlobalsCache::VIEW_WIDTH:
				
					uniform->SetValue ( instance, global.mIndex, viewRect.Width ());
					break;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIShader::BindUniforms () {
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	size_t nUniforms = this->mUniformInstances.Size ();
	
	bool flushed = false;
	
	for ( u32 i = 0; i < nUniforms; ++i ) {
	
		MOAIShaderUniform* uniform = this->GetUniform ( i );
		
		if ( uniform && uniform->IsValid ()) {
		
			MOAIShaderUniformInstance& instance = this->mUniformInstances [ i ];
		
			for ( u32 j = 0; j < instance.mCount; ++j ) {
				
				if ( !flushed ) {
					gfxMgr.mGfxState.GfxStateWillChange ();
					flushed = true;
				}
				uniform->Bind ( instance, j );
			}
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIShader::GetAttributeID ( u32 uniformID, u32 index ) {

	return ( uniformID * this->mMaxCount ) + index;
}

//----------------------------------------------------------------//
MOAIShaderUniform* MOAIShader::GetUniform ( u32 uniformID ) {

	return this->mProgram ? this->mProgram->GetUniform ( uniformID ) : 0;
}

//----------------------------------------------------------------//
MOAIShaderUniform* MOAIShader::GetUniformForAttributeID ( u32 attrID, u32& index ) {

	u32 uniformID = attrID / this->mMaxCount;
	MOAIShaderUniform* uniform = this->GetUniform ( uniformID );
	index = attrID - ( uniformID * this->mMaxCount );
	return uniform;
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
		{ "getAttributeID",				_getAttributeID },
		{ "reserveGlobals",				_reserveGlobals },
		{ "setGlobal",					_setGlobal },
		{ "setProgram",					_setProgram },
		{ "setUniform",					_setUniform },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShader::SetProgram ( MOAIShaderProgram* program ) {

	this->mUniformInstances.Clear ();
	this->mUniformBuffer.Clear ();
	
	this->mMaxCount = 0;
	
	this->mProgram.Set ( *this, program );
	
	if ( program ) {
		
		size_t nUniforms = program->mUniforms.Size ();
		this->mUniformInstances.Init ( nUniforms );
		
		size_t size = 0;
		
		for ( size_t i = 0; i < nUniforms; ++i ) {
			
			const MOAIShaderUniform& uniform = program->mUniforms [ i ];
			MOAIShaderUniformInstance& instance = this->mUniformInstances [ i ];
			
			instance.mBuffer = ( void* )size;
			instance.mCount = uniform.mCount;
			
			size += uniform.mWidth * instance.mCount * MOAIShaderUniform::ELEMENT_SIZE;
			
			this->mMaxCount = this->mMaxCount < instance.mCount ? instance.mCount : this->mMaxCount;
		}
		
		this->mUniformBuffer.Init ( size );
		
		for ( size_t i = 0; i < nUniforms; ++i ) {
		
			const MOAIShaderUniform& uniform = program->mUniforms [ i ];
			MOAIShaderUniformInstance& instance = this->mUniformInstances [ i ];
			
			instance.mBuffer = ( void* )(( size_t )instance.mBuffer + ( size_t )this->mUniformBuffer.Data ());
			uniform.Default ( instance );
		}
		
		this->CopyGlobals ( *program );
	}
}

//----------------------------------------------------------------//
void MOAIShader::UpdateAndBindUniforms () {

	if ( this->mProgram ) {
	
		this->ApplyGlobals ();
		this->BindUniforms ();
	}
}
