// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
	MOAI_LUA_SETUP ( MOAIShader, "U" )

	return self->ReserveGlobals ( L, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_resizeUniformArray ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "UN" )
	
	u32 uniformID		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 count			= state.GetValue < u32 >( 3, 0 );
	
	self->ResizeUniformArray ( uniformID, count );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_setGlobal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "UNNN" )
	
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

	self->SetUniform ( L, 3, uniformID, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIShader::_setUniformComponent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )

	u32 uniformID	= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 index		= state.GetValue < u32 >( 3, 1 ) - 1;

	self->SetUniform ( L, 4, uniformID, index );
	
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
void MOAIShader::ApplyGlobals () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	ZLRect viewRect = gfxMgr.mGfxState.GetViewRect ();

	// NOTE: matrices are submitted *column major*; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	for ( u32 i = 0; i < this->mGlobals.Size (); ++i ) {
	
		const MOAIShaderProgramGlobal& global = this->mGlobals [ i ];
		
		if ( global.mUniformID == INVALID_INDEX ) continue;
		
		void* element;
		MOAIShaderUniform* uniform = this->GetUniform ( global.mUniformID, global.mIndex, element );
		if ( !uniform ) continue;
		
		if ( global.mGlobalID < MOAIGfxGlobalsCache::TOTAL_MATRICES ) {
		
			uniform->SetValue ( element, gfxMgr.mGfxState.GetMtx ( global.mGlobalID ));
		}
		else {
		
			switch ( global.mGlobalID ) {
				
				case MOAIGfxGlobalsCache::PEN_COLOR:
				
					uniform->SetValue ( element, gfxMgr.mGfxState.GetFinalColor ());
					break;
				
				case MOAIGfxGlobalsCache::VIEW_HALF_HEIGHT:
				
					uniform->SetValue ( element, viewRect.Height () * 0.5f );
					break;
					
				case MOAIGfxGlobalsCache::VIEW_HALF_WIDTH: {
				
					uniform->SetValue ( element, viewRect.Width () * 0.5f );
					break;
				}
				case MOAIGfxGlobalsCache::VIEW_HEIGHT:
				
					uniform->SetValue ( element, viewRect.Height ());
					break;
					
				case MOAIGfxGlobalsCache::VIEW_WIDTH:
				
					uniform->SetValue ( element, viewRect.Width ());
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
		
			if ( !flushed ) {
				gfxMgr.mGfxState.GfxStateWillChange ();
				flushed = true;
			}
			uniform->Bind ( instance );
		}
	}
}

//----------------------------------------------------------------//
void MOAIShader::Bless () {

	if ( this->mMaxCount ) return;

	this->mUniformBuffer.Clear ();
	
	if ( this->mProgram ) {
		
		size_t nUniforms = this->mProgram->mUniforms.Size ();
		
		size_t size = 0;
		
		for ( size_t i = 0; i < nUniforms; ++i ) {
			
			const MOAIShaderUniform& uniform = this->mProgram->mUniforms [ i ];
			MOAIShaderUniformInstance& instance = this->mUniformInstances [ i ];
			
			instance.mBuffer = ( void* )size;
			
			size += uniform.mWidth * instance.mCount * MOAIShaderUniform::ELEMENT_SIZE;
			
			this->mMaxCount = this->mMaxCount < instance.mCount ? instance.mCount : this->mMaxCount;
		}
		
		this->mUniformBuffer.Init ( size );
		
		for ( size_t i = 0; i < nUniforms; ++i ) {
		
			const MOAIShaderUniform& uniform = this->mProgram->mUniforms [ i ];
			MOAIShaderUniformInstance& instance = this->mUniformInstances [ i ];
			
			instance.mBuffer = ( void* )(( size_t )instance.mBuffer + ( size_t )this->mUniformBuffer.Data ());
			uniform.Default ( instance );
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIShader::GetAttributeID ( u32 uniformID, u32 index ) {

	this->Bless ();
	return ( uniformID * this->mMaxCount ) + index;
}

//----------------------------------------------------------------//
MOAIShaderUniform* MOAIShader::GetUniform ( u32 uniformID ) {

	return this->mProgram ? this->mProgram->GetUniform ( uniformID ) : 0;
}

//----------------------------------------------------------------//
MOAIShaderUniform* MOAIShader::GetUniform ( u32 uniformID, u32 index, void*& element ) {

	MOAIShaderUniform* uniform = 0;

	uniform = this->GetUniform ( uniformID );
	element = uniform ? uniform->GetElement ( this->mUniformInstances [ uniformID ], index ) : 0;

	return uniform;
}

//----------------------------------------------------------------//
MOAIShaderUniform* MOAIShader::GetUniformForAttributeID ( u32 attrID, void*& element) {

	attrID = ( attrID & MOAIAttribute::ATTR_ID_MASK ) - 1;

	u32 uniformID = attrID / this->mMaxCount;
	u32 index = attrID - ( uniformID * this->mMaxCount );
	
	return this->GetUniform ( uniformID, index, element );
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
		{ "resizeUniformArray",			_resizeUniformArray },
		{ "setGlobal",					_setGlobal },
		{ "setProgram",					_setProgram },
		{ "setUniform",					_setUniform },
		{ "setUniformComponent",		_setUniformComponent },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShader::ResizeUniformArray ( u32 uniformID, u32 count ) {

	if ( uniformID < this->mUniformInstances.Size ()) {
		this->mUniformInstances [ uniformID ].mCount = count;
		this->mMaxCount = 0;
	}
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
		
		for ( size_t i = 0; i < nUniforms; ++i ) {
			
			const MOAIShaderUniform& uniform = program->mUniforms [ i ];
			MOAIShaderUniformInstance& instance = this->mUniformInstances [ i ];
			
			instance.mBuffer = 0;
			instance.mCount = uniform.mCount;
		}
		
		this->CopyGlobals ( *program );
	}
}

//----------------------------------------------------------------//
void MOAIShader::SetUniform ( lua_State* L, int idx, u32 uniformID, u32 index ) {

	MOAILuaState state ( L );

	this->Bless ();

	void* element;
	MOAIShaderUniform* uniform = this->GetUniform ( uniformID, index, element );
	
	if ( uniform ) {
		
		int size = state.GetStackSize ( idx );
		
		if ( size <= 4 ) {
		
			ZLVec4D vec = state.GetValue < ZLVec4D >( idx, ZLVec4D::ZERO );
			
			uniform->SetValue ( element, vec );
		}
		else {

			ZLMatrix4x4 mtx = ZLMatrix4x4::IDENT;

			switch ( size ) {
			
				case 6:
					uniform->SetValue ( element, state.GetValue < ZLAffine2D >( idx, ZLAffine2D::IDENT ));
					break;
				
				case 9:
					uniform->SetValue ( element, state.GetValue < ZLMatrix3x3 >( idx, ZLMatrix3x3::IDENT ));
					break;
				
				case 12:
					uniform->SetValue ( element, state.GetValue < ZLAffine3D >( idx, ZLAffine3D::IDENT ));
					break;
				
				case 16:
					uniform->SetValue ( element, state.GetValue < ZLMatrix4x4 >( idx, ZLMatrix4x4::IDENT ));
					break;
				
				default:
					MOAILogF ( L, ZLLog::LOG_ERROR, "Error setting matrix. Did not recognize number of paramaters (6, 9, 12 or 16)." );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIShader::UpdateAndBindUniforms () {

	if ( this->mProgram ) {
		this->Bless ();
		this->ApplyGlobals ();
		this->BindUniforms ();
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIShader::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	if ( this->mProgram ) {
		
		this->Bless ();
		
		void* element;
		MOAIShaderUniform* uniform = this->GetUniformForAttributeID ( attrID, element );
		
		if ( uniform ) {
		
			switch ( op ) {

				case MOAIAttribute::CHECK:
					attr.SetFlags ( MOAIAttribute::ATTR_WRITE );
					return true;

				case MOAIAttribute::SET:
					uniform->SetValue ( element, attr );
					return true;

				case MOAIAttribute::ADD:
					uniform->AddValue ( element, attr );
					return true;
			}
		}
	}
	return false;
}
