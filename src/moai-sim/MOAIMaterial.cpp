// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMaterial.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIShaderMgr.h>

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterial::ClearAll () {

	this->mFlags = 0;
}

//----------------------------------------------------------------//
void MOAIMaterial::ClearBlendMode () {

	this->mFlags &= ~BLEND_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterial::ClearCullMode () {

	this->mFlags &= ~CULL_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterial::ClearDepthMask () {

	this->mFlags &= ~DEPTH_MASK_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterial::ClearDepthTest () {

	this->mFlags &= ~DEPTH_TEST_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterial::ClearShader () {

	this->mFlags &= ~SHADER_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterial::ClearTexture () {

	this->mFlags &= ~TEXTURE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterial::Compose ( const MOAIMaterial& material ) {

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
}

//----------------------------------------------------------------//
void MOAIMaterial::LoadGfxState ( MOAIShader* defaultShader ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	if ( this->mFlags ) {

		if ( this->mFlags & DRAW_FLAGS ) {

			if ( this->mFlags & BLEND_MODE_FLAG ) {
				gfxMgr.mGfxState.SetBlendMode ( this->mBlendMode );
			}
			
			if ( this->mFlags & CULL_MODE_FLAG ) {
				gfxMgr.mGfxState.SetCullFunc ( this->mCullMode );
			}
			
			if ( this->mFlags & DEPTH_MASK_FLAG ) {
				gfxMgr.mGfxState.SetDepthMask ( this->mDepthMask );
			}
			
			if ( this->mFlags & DEPTH_TEST_FLAG ) {
				gfxMgr.mGfxState.SetDepthFunc ( this->mDepthTest );
			}
		}
		
		if ( this->mFlags & ( SHADER_FLAG | TEXTURE_FLAG )) {
		
			if ( this->mFlags & SHADER_FLAG ) {
				gfxMgr.mGfxState.SetShader ( this->mShader );
			}
			
			if ( this->mFlags & TEXTURE_FLAG ) {
				gfxMgr.mGfxState.SetTexture ( this->mTexture );
			}
		}
	}
	
	if ( !( this->mFlags & SHADER_FLAG ) && defaultShader ) {
		gfxMgr.mGfxState.SetShader ( defaultShader );
	}
}

//----------------------------------------------------------------//
MOAIMaterial::MOAIMaterial () :
	mShader ( 0 ),
	mTexture ( 0 ),
	mHitMask ( 0 ),
	mCullMode ( 0 ),
	mDepthTest ( 0 ),
	mDepthMask ( true ),
	mFlags ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {
}

//----------------------------------------------------------------//
void MOAIMaterial::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	if ( !( this->mFlags & BLEND_MODE_FLAG )) {
		this->mBlendMode = blendMode;
		this->mFlags |= BLEND_MODE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::SetCullMode ( int cullMode ) {

	if ( !( this->mFlags & CULL_MODE_FLAG )) {
		this->mCullMode = cullMode;
		this->mFlags |= CULL_MODE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::SetDepthMask ( bool depthMask ) {

	if ( !( this->mFlags & DEPTH_MASK_FLAG )) {
		this->mDepthMask = depthMask;
		this->mFlags |= DEPTH_MASK_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::SetDepthTest ( int depthTest ) {

	if ( !( this->mFlags & DEPTH_TEST_FLAG )) {
		this->mDepthTest = depthTest;
		this->mFlags |= DEPTH_TEST_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::SetShader ( MOAIShader* shader ) {

	if ( !( this->mFlags & SHADER_FLAG )) {
		this->mShader = shader;
		this->mFlags |= SHADER_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( MOAITextureBase* texture ) {

	if ( !( this->mFlags & TEXTURE_FLAG )) {
		this->mTexture = texture;
		this->mFlags |= TEXTURE_FLAG;
	}
}

//----------------------------------------------------------------//
bool MOAIMaterial::StateWillChange ( const MOAIMaterial& material ) {

		return ( ~this->mFlags & material.mFlags );
}
