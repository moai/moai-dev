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
int MOAIShader::_resizeUniformArray ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "UN" )
	
	u32 uniformID		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 count			= state.GetValue < u32 >( 3, 0 );
	
	self->ResizeUniformArray ( uniformID, count );
	
	return 0;
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

	self->Bless ();
	self->SetUniform ( L, 3, uniformID, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIShader::_setUniformArrayItem ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )

	u32 uniformID	= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 index		= state.GetValue < u32 >( 3, 1 ) - 1;

	self->Bless ();
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

	if ( !this->mProgram ) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	ZLRect viewRect = gfxMgr.mGfxState.GetViewRect ();

	// NOTE: matrices are submitted *column major*; it is up to the shader to transform vertices correctly
	// vert * matrix implicitely transposes the matrix; martix * vert uses the matrix as submitted

	u32 nGlobals = this->mProgram->mGlobals.Size ();

	for ( u32 i = 0; i < nGlobals; ++i ) {
	
		const MOAIShaderProgramGlobal& global = this->mProgram->mGlobals [ i ];
		
		if ( global.mUniformID == INVALID_INDEX ) continue;
		
		void* element;
		MOAIShaderUniformFormatter* uniform = this->GetUniform ( global.mUniformID, global.mIndex, element );
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
void MOAIShader::BindTextures () {

	if ( this->mProgram ) {
		this->mProgram->BindTextures ();
	}
}

//----------------------------------------------------------------//
void MOAIShader::BindUniforms () {
	
	MOAIShaderProgram* program = this->mProgram;
	if ( !program ) return;
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	size_t nUniforms = this->mUniformInstances.Size ();
	
	bool flushed = false;
	
	for ( u32 i = 0; i < nUniforms; ++i ) {
	
		MOAIShaderUniform* uniform = program->GetUniform ( i );
		
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
			
			size += uniform.GetSize ( instance.mCount );
			
			this->mMaxCount = this->mMaxCount < instance.mCount ? instance.mCount : this->mMaxCount;
		}
		
		this->mUniformBuffer.Init ( size );
		
		for ( size_t i = 0; i < nUniforms; ++i ) {
		
			const MOAIShaderUniform& uniform = this->mProgram->mUniforms [ i ];
			MOAIShaderUniformInstance& instance = this->mUniformInstances [ i ];
			
			instance.mBuffer = ( void* )(( size_t )instance.mBuffer + ( size_t )this->mUniformBuffer.Data ());
			uniform.Default ( instance.mBuffer, instance.mCount );
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
		{ "getAttributeID",				_getAttributeID },
		{ "resizeUniformArray",			_resizeUniformArray },
		{ "setProgram",					_setProgram },
		{ "setUniform",					_setUniform },
		{ "setUniformArrayItem",		_setUniformArrayItem },
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
	}
}

//----------------------------------------------------------------//
void MOAIShader::UpdateUniforms () {

	if ( this->mProgram ) {
		this->Bless ();
		this->ApplyGlobals ();
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIShader::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	if ( this->mProgram ) {
	
		this->Bless ();
		return this->MOAIShaderUniformBuffer::ApplyAttrOp ( attrID, attr, op );
	}
	return false;
}

//----------------------------------------------------------------//
MOAIShaderUniformFormatter* MOAIShader::MOAIShaderUniformBuffer_GetUniform ( u32 uniformID, void*& buffer ) {

	MOAIShaderUniform* uniform = 0;

	if ( this->mProgram ) {
		uniform = this->mProgram->GetUniform ( uniformID );
		buffer = this->mUniformInstances [ uniformID ].mBuffer;
	}
	return uniform;
}
