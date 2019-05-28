// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShaderGL.h>
#include <moai-gfx/MOAIShaderMgrGL.h>
#include <moai-gfx/MOAITextureBaseGL.h>

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
	
	MOAIMaterialNamedGlobal < ZLAbstractTexture >* namedTexturelIt = material.mTextures;
	for ( ; namedTexturelIt; namedTexturelIt = namedTexturelIt->mNext ) {
		this->SetTexture ( namedTexturelIt->mName, namedTexturelIt->mValue );
	}
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterialMgr::GetLight ( u32 lightID ) {

	assert ( lightID < MAX_GLOBAL_LIGHTS );
	return this->mNamedLights [ ZLIndexCast ( lightID )].mLight;
}

//----------------------------------------------------------------//
ZLAbstractTexture* MOAIMaterialMgr::GetTexture ( u32 textureID ) {

	assert ( textureID < MAX_GLOBAL_TEXTURES );
	ZLAbstractTexture* texture = this->mNamedTextures [ ZLIndexCast ( textureID )].mTexture;
	return texture ? texture : ( ZLAbstractTexture* )this->mTexture;
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::LoadGfxState () {

	MOAIGfxMgrGL& gfxMgr = MOAIGfxMgrGL::Get ();

	gfxMgr.SetBlendMode ( this->mBlendMode );
	gfxMgr.SetCullFunc ( this->mCullMode );
	gfxMgr.SetDepthMask ( this->mDepthMask );
	gfxMgr.SetDepthFunc ( this->mDepthTest );
	gfxMgr.SetTexture ( this->mTexture );
	
	// load shader last!
	gfxMgr.SetShader ( this->mShader );
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
			
			MOAIMaterialGlobal* global = cursor->mGlobal;
			
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
void MOAIMaterialMgr::SetBlendMode ( const ZLBlendMode& blendMode ) {

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
		frame.mClearList = clearCmd->mNext;
	}
	
	global.mPtr = ptr;
	global.mStackDepth = stackDepth;
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetLight ( u32 lightID, MOAILight* light ) {

	assert ( lightID < MAX_GLOBAL_LIGHTS );
	this->SetGlobal ( this->mNamedLights [ ZLIndexCast ( lightID )], light );
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetShader ( u32 shaderID ) {

	this->SetShader ( MOAIShaderMgrGL::Get ().GetShader ( shaderID ));
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetShader ( ZLAbstractShader* shader ) {

	if ( !( this->mFlags & SHADER_FLAG )) {
		this->mShader = shader;
		this->mFlags |= SHADER_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetTexture ( ZLAbstractTexture* texture ) {

	if ( !( this->mFlags & TEXTURE_FLAG )) {
		this->mTexture = texture;
		this->mFlags |= TEXTURE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetTexture ( u32 textureID, ZLAbstractTexture* texture ) {
	
	assert ( textureID < MAX_GLOBAL_TEXTURES );
	this->SetGlobal ( this->mNamedTextures [ ZLIndexCast ( textureID )], texture );
}
