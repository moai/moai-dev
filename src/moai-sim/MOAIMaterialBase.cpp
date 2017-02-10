// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMaterialBase.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIShaderMgr.h>

//================================================================//
// MOAIMaterialBase
//================================================================//

const MOAIMaterialBase MOAIMaterialBase::DEFAULT_MATERIAL;

//----------------------------------------------------------------//
void MOAIMaterialBase::Clear () {

	*this = DEFAULT_MATERIAL;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::Clear ( u32 flags ) {
	
	if ( this->mFlags & flags ) {
		
		if (( flags & ALL_FLAGS ) == ALL_FLAGS ) {
		
			this->Clear ();
		}
		else if ( this->mFlags & flags ) {
		
			if ( flags & BLEND_MODE_FLAG ) {
				this->mBlendMode = DEFAULT_MATERIAL.mBlendMode;
			}

			if ( flags & CULL_MODE_FLAG ) {
				this->mCullMode = DEFAULT_MATERIAL.mCullMode;
			}
			
			if ( flags & DEPTH_MASK_FLAG ) {
				this->mDepthMask = DEFAULT_MATERIAL.mDepthMask;
			}
			
			if ( flags & DEPTH_TEST_FLAG ) {
				this->mDepthTest = DEFAULT_MATERIAL.mDepthTest;
			}

			if ( flags & SHADER_FLAG ) {
				this->mShader = 0;
			}

			if ( flags & TEXTURE_FLAG ) {
				this->mTexture = 0;
			}

			this->mFlags &= ~flags;
		}
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBase::ClearBlendMode () {

	this->mFlags &= ~BLEND_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::ClearCullMode () {

	this->mFlags &= ~CULL_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::ClearDepthMask () {

	this->mFlags &= ~DEPTH_MASK_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::ClearDepthTest () {

	this->mFlags &= ~DEPTH_TEST_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::ClearShader () {

	this->mFlags &= ~SHADER_FLAG;
	this->mShader = 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::ClearTexture () {

	this->mFlags &= ~TEXTURE_FLAG;
	this->mTexture = 0;
}

//----------------------------------------------------------------//
MOAIMaterialBase::MOAIMaterialBase () :
	mShader ( 0 ),
	mTexture ( 0 ),
	mCullMode ( 0 ),
	mDepthTest ( 0 ),
	mDepthMask ( true ),
	mFlags ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMaterialBase::~MOAIMaterialBase () {

	ZLRelease ( this->mShader );
	ZLRelease ( this->mTexture );
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	this->mBlendMode = blendMode;
	this->mFlags |= BLEND_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetCullMode ( int cullMode ) {

	this->mCullMode = cullMode;
	this->mFlags |= CULL_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetDepthMask ( bool depthMask ) {

	this->mDepthMask = depthMask;
	this->mFlags |= DEPTH_MASK_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetDepthTest ( int depthTest ) {

	this->mDepthTest = depthTest;
	this->mFlags |= DEPTH_TEST_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetShader ( u32 shaderID ) {

	this->SetShader ( MOAIShaderMgr::Get ().GetShader ( shaderID ));
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetShader ( MOAIShader* shader ) {

	ZLRetain ( shader );
	ZLRelease ( this->mShader );

	this->mShader = shader;
	this->mFlags |= SHADER_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetTexture ( MOAITextureBase* texture ) {

	ZLRetain ( texture );
	ZLRelease ( this->mTexture );

	this->mTexture = texture;
	this->mFlags |= TEXTURE_FLAG;
}
