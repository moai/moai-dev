// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIMaterialBase.h>
#include <moai-sim/MOAIPartitionHull.h>

//================================================================//
// MOAIMaterialBase
//================================================================//

const MOAIMaterialBase MOAIMaterialBase::DEFAULT_MATERIAL;

//----------------------------------------------------------------//
void MOAIMaterialBase::Clear () {

	this->mShader = 0;
	this->mTexture = 0;

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
void MOAIMaterialBase::SetBlendMode () {

	this->mFlags &= ~BLEND_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetBlendMode ( const ZLBlendMode& blendMode ) {

	this->mBlendMode = blendMode;
	this->mFlags |= BLEND_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetCullMode () {

	this->mFlags &= ~CULL_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetCullMode ( int cullMode ) {

	this->mCullMode = cullMode;
	this->mFlags |= CULL_MODE_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetDepthMask () {

	this->mFlags &= ~DEPTH_MASK_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetDepthMask ( bool depthMask ) {

	this->mDepthMask = depthMask;
	this->mFlags |= DEPTH_MASK_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetDepthTest () {

	this->mFlags &= ~DEPTH_TEST_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetDepthTest ( int depthTest ) {

	this->mDepthTest = depthTest;
	this->mFlags |= DEPTH_TEST_FLAG;
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetShader () {

	this->SetShader ( 0 );
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetShader ( MOAIShader* shader ) {

	this->mShader = shader;
	
	if ( shader ) {
		this->mFlags |= SHADER_FLAG;
	}
	else {
		this->mFlags &= ~SHADER_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetTexture () {

	this->SetTexture ( 0 );
}

//----------------------------------------------------------------//
void MOAIMaterialBase::SetTexture ( MOAITextureBase* texture ) {

	this->mTexture = texture;

	if ( texture ) {
		this->mFlags |= TEXTURE_FLAG;
	}
	else {
		this->mFlags &= ~TEXTURE_FLAG;
	}
}
