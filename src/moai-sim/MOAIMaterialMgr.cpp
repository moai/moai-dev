// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAILight.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIShaderMgr.h>

//================================================================//
// MOAIMaterialStackScope
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialStackScope::MOAIMaterialStackScope () :
	mMaterialStack ( MOAIMaterialMgr::Get ()) {
	
	this->mRestoreTop = this->mMaterialStack.mStack.GetTop ();
}

//----------------------------------------------------------------//
MOAIMaterialStackScope::~MOAIMaterialStackScope () {

	u32 top = this->mMaterialStack.mStack.GetTop ();
	for ( ; top > this->mRestoreTop; --top ) {
		this->mMaterialStack.Pop ();
	}
}

//================================================================//
// MOAIMaterialMgr
//================================================================//

//----------------------------------------------------------------//
//void MOAIMaterialMgr::Clear () {
//
//	this->mMaterialFlagsStack.Reset ();
//	this->MOAIMaterial::Clear ();
//}

//----------------------------------------------------------------//
void MOAIMaterialMgr::Compose ( const MOAIMaterial& material ) {

	u32 available = ~this->mFlags & material.mFlags;

	// only apply if there are some flags open
	if ( available ) {
	
		if ( available & DRAW_FLAGS ) {
	
			if ( available & BLEND_MODE_FLAG ) {
				this->mBlendMode = material.mBlendMode;
			}
			
			if ( available & CULL_MODE_FLAG ) {
				this->mCullMode = material.mCullMode;
			}
			
			if ( available & DEPTH_MASK_FLAG ) {
				this->mCullMode = material.mDepthMask;
			}
			
			if ( available & DEPTH_TEST_FLAG ) {
				this->mCullMode = material.mDepthTest;
			}
		}
		
		if ( available & ( SHADER_FLAG | TEXTURE_FLAG )) {
		
			if ( available & SHADER_FLAG ) {
				this->mShader = material.mShader;
			}
			
			if ( available & TEXTURE_FLAG ) {
				this->mTexture = material.mTexture;
			}
		}
		
		this->mFlags |= available;
	}
	
	size_t nLights = material.mEntries.Size ();
	for ( size_t i = 0; i < nLights; ++i ) {
	
		MOAIMaterialLight& materialLight = material.mEntries [ i ];
		this->SetLight ( materialLight.mGlobalID, materialLight.mLight );
	}
}

//----------------------------------------------------------------//
const MOAILight* MOAIMaterialMgr::GetLight ( u32 lightID ) {

	assert ( lightID < MAX_GLOBAL_LIGHTS );
	return this->mGlobalLights [ lightID ].mLight;
}


//----------------------------------------------------------------//
void MOAIMaterialMgr::LoadGfxState () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mGfxState.SetBlendMode ( this->mBlendMode );
	gfxMgr.mGfxState.SetCullFunc ( this->mCullMode );
	gfxMgr.mGfxState.SetDepthMask ( this->mDepthMask );
	gfxMgr.mGfxState.SetDepthFunc ( this->mDepthTest );
	gfxMgr.mGfxState.SetShader ( this->mShader );
	gfxMgr.mGfxState.SetTexture ( this->mTexture );
}

//----------------------------------------------------------------//
MOAIMaterialMgr::MOAIMaterialMgr () {

	this->mGlobalLights.Init ( MAX_GLOBAL_LIGHTS );
	for ( u32 i = 0; i < MAX_GLOBAL_LIGHTS; ++i ) {
		MOAIGlobalLight& state = this->mGlobalLights [ i ];
		state.mLight = 0;
		state.mStackDepth = 0;
	}
}

//----------------------------------------------------------------//
MOAIMaterialMgr::~MOAIMaterialMgr () {
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::Pop () {

	size_t stackTop = this->mStack.GetTop ();

	if ( stackTop ) {
	
		MOAIMaterialStackFrame frame = this->mStack.Pop ();
		
		if ( stackTop > 1 ) {
			this->MOAIMaterial::Clear ( ~frame.mFlags );
		}
		else {
			this->mStack.Reset ();
			this->MOAIMaterial::Clear ();
		}
		
		MOAIMaterialStackClearCmd* cursor = frame.mClearList;
		
		while ( cursor ) {
		
			MOAIMaterialStackClearCmd* clearCmd = cursor;
			
			if ( clearCmd->mType == MOAIMaterialStackClearCmd::CLEAR_LIGHT_GLOBAL ) {
			
				this->mGlobalLights [ clearCmd->mGlobalID ].mLight = 0;
				this->mGlobalLights [ clearCmd->mGlobalID ].mStackDepth = 0;
			}
			else {
			
				this->mGlobalTextures [ clearCmd->mGlobalID ].mTexture = 0;
				this->mGlobalTextures [ clearCmd->mGlobalID ].mStackDepth = 0;
			}
			
			cursor = cursor->mNext;
			this->mRestoreCmdPool.Free ( clearCmd );
		}
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::Push ( const MOAIMaterial* material ) {
	
	MOAIMaterialStackFrame& frame = this->mStack.Push ();
	
	frame.mFlags = this->mFlags;
	frame.mClearList = 0;
	
	if ( material ) {
		this->Compose ( *material );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	if ( !( this->mFlags & BLEND_MODE_FLAG )) {
		this->mBlendMode = blendMode;
		this->mFlags |= BLEND_MODE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetCullMode ( int cullMode ) {

	if ( !( this->mFlags & CULL_MODE_FLAG )) {
		this->mCullMode = cullMode;
		this->mFlags |= CULL_MODE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetDepthMask ( bool depthMask ) {

	if ( !( this->mFlags & DEPTH_MASK_FLAG )) {
		this->mDepthMask = depthMask;
		this->mFlags |= DEPTH_MASK_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetDepthTest ( int depthTest ) {

	if ( !( this->mFlags & DEPTH_TEST_FLAG )) {
		this->mDepthTest = depthTest;
		this->mFlags |= DEPTH_TEST_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetLight ( u32 lightID, MOAILight* light ) {

	assert ( lightID < MAX_GLOBAL_LIGHTS );

	u32 stackDepth = ( u32 )this->mStack.GetTop ();
	assert ( stackDepth );

	MOAIGlobalLight& state = this->mGlobalLights [ lightID ];
	if ( state.mStackDepth < stackDepth ) {
	
		if ( state.mLight ) return; // don't overwrite lights
		
		MOAIMaterialStackClearCmd* clearCmd = this->mRestoreCmdPool.Alloc ();
		assert ( clearCmd );
		
		clearCmd->mGlobalID = lightID;
		clearCmd->mType = MOAIMaterialStackClearCmd::CLEAR_LIGHT_GLOBAL;
		
		MOAIMaterialStackFrame& frame = this->mStack.Top ();
		clearCmd->mNext = frame.mClearList;
		frame.mClearList = clearCmd->mNext;
	}
	
	state.mLight = light;
	state.mStackDepth = stackDepth;
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetShader ( u32 shaderID ) {

	this->SetShader ( MOAIShaderMgr::Get ().GetShader ( shaderID ));
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetShader ( MOAIShader* shader ) {

	if ( !( this->mFlags & SHADER_FLAG )) {
		this->mShader = shader;
		this->mFlags |= SHADER_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetTexture ( MOAITextureBase* texture ) {

	if ( !( this->mFlags & TEXTURE_FLAG )) {
		this->mTexture = texture;
		this->mFlags |= TEXTURE_FLAG;
	}
}
