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
// MOAIMaterialGlobal
//================================================================//
	
//----------------------------------------------------------------//
MOAIMaterialGlobal::MOAIMaterialGlobal () :
	mPtr ( 0 ),
	mStackDepth ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMaterialGlobal::~MOAIMaterialGlobal () {
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
				this->mDepthMask = material.mDepthMask;
			}
			
			if ( available & DEPTH_TEST_FLAG ) {
				this->mDepthTest = material.mDepthTest;
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
	
	MOAIMaterialNamedGlobal < MOAILight >* namedLightIt = material.mLights;
	for ( ; namedLightIt; namedLightIt = namedLightIt->mNext ) {
		this->SetLight ( namedLightIt->mName, namedLightIt->mValue );
	}
	
	MOAIMaterialNamedGlobal < MOAITextureBase >* namedTexturelIt = material.mTextures;
	for ( ; namedTexturelIt; namedTexturelIt = namedTexturelIt->mNext ) {
		this->SetTexture ( namedTexturelIt->mName, namedTexturelIt->mValue );
	}
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterialMgr::GetLight ( u32 lightID ) {

	assert ( lightID < MAX_GLOBAL_LIGHTS );
	return this->mNamedLights [ lightID ].mLight;
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIMaterialMgr::GetTexture ( u32 textureID ) {

	assert ( textureID < MAX_GLOBAL_TEXTURES );
	MOAITextureBase* texture = this->mNamedTextures [ textureID ].mTexture;
	return texture ? texture : ( MOAITextureBase* )this->mTexture;
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::LoadGfxState () {

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	gfxState.SetBlendMode ( this->mBlendMode );
	gfxState.SetCullFunc ( this->mCullMode );
	gfxState.SetDepthMask ( this->mDepthMask );
	gfxState.SetDepthFunc ( this->mDepthTest );
	gfxState.SetTexture ( this->mTexture );
	
	// load shader last!
	gfxState.SetShader ( this->mShader );
}

//----------------------------------------------------------------//
MOAIMaterialMgr::MOAIMaterialMgr () {

	this->mNamedLights.Init ( MAX_GLOBAL_LIGHTS );
	this->mNamedTextures.Init ( MAX_GLOBAL_TEXTURES );
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
			this->MOAIMaterialBase::Clear ( ~frame.mFlags );
		}
		else {
			this->mStack.Reset ();
			this->MOAIMaterialBase::Clear ();
		}
		
		MOAIMaterialStackClearCmd* cursor = frame.mClearList;
		
		while ( cursor ) {
		
			MOAIMaterialStackClearCmd* clearCmd = cursor;
			cursor = cursor->mNext;
			
			MOAIMaterialGlobal* global = clearCmd->mGlobal;
			
			global->mPtr = 0;
			global->mStackDepth = 0;
			
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
void MOAIMaterialMgr::SetGlobal ( MOAIMaterialGlobal& global, void* ptr ) {

	u32 stackDepth = ( u32 )this->mStack.GetTop ();
	assert ( stackDepth );

	if ( global.mStackDepth < stackDepth ) {
	
		if ( global.mPtr ) return; // don't overwrite
		
		MOAIMaterialStackClearCmd* clearCmd = this->mRestoreCmdPool.Alloc ();
		assert ( clearCmd );
		
		clearCmd->mGlobal = &global;
		
		MOAIMaterialStackFrame& frame = this->mStack.Top ();
		clearCmd->mNext = frame.mClearList;
		frame.mClearList = clearCmd;
	}
	
	global.mPtr = ptr;
	global.mStackDepth = stackDepth;
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetLight ( u32 lightID, MOAILight* light ) {

	assert ( lightID < MAX_GLOBAL_LIGHTS );
	this->SetGlobal ( this->mNamedLights [ lightID ], light );
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

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetTexture ( u32 textureID, MOAITextureBase* texture ) {
	
	assert ( textureID < MAX_GLOBAL_TEXTURES );
	this->SetGlobal ( this->mNamedTextures [ textureID ], texture );
}
