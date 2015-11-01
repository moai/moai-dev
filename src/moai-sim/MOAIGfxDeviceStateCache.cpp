// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxDeviceStateCache.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// MOAIGfxDeviceStateCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::BindFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	frameBuffer = frameBuffer ? frameBuffer : this->GetDefaultFrameBuffer ();

	if ( this->mCurrentFrameBuffer != frameBuffer ) {
		
		this->OnGfxStateWillChange ();
		
		zglBindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBufferID );
		this->mCurrentFrameBuffer = frameBuffer;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::BindIndexBuffer ( MOAIIndexBuffer* buffer ) {

	if ( this->mCurrentIdxBuffer != buffer ) {
	
		if ( this->mCurrentIdxBuffer ) {
			this->mCurrentIdxBuffer->Unbind ();
		}
	
		this->mCurrentIdxBuffer = buffer;
		
		if ( buffer ) {
			buffer->Bind ();
		}
	}
	else if ( this->mCurrentIdxBuffer && this->mCurrentIdxBuffer->NeedsFlush ()) {
		
		this->mCurrentIdxBuffer->Bind ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::BindVertexArray ( MOAIVertexArray* vtxArray ) {

	if ( this->mCurrentVtxArray != vtxArray ) {
	
		if ( this->mCurrentVtxArray ) {
			this->mCurrentVtxArray->Unbind ();
		}
		
		this->BindVertexBuffer ();
		this->mCurrentVtxArray = vtxArray;
		
		if ( vtxArray ) {
			vtxArray->Bind ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::BindVertexBuffer ( MOAIVertexBuffer* buffer ) {

	if ( this->mCurrentVtxBuffer != buffer ) {
	
		this->OnGfxStateWillChange ();
		
		if ( this->mCurrentVtxBuffer ) {
			this->mCurrentVtxBuffer->Unbind ();
		}
		
		this->BindVertexFormat ();
		this->mCurrentVtxBuffer = buffer;
	
		if ( buffer ) {
			buffer->Bind ();
		}
	}
	else if ( this->mCurrentVtxBuffer && this->mCurrentVtxBuffer->NeedsFlush ()) {
		
		this->mCurrentVtxBuffer->Bind ();
		this->mCurrentVtxFormat = 0;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::BindVertexFormat ( MOAIVertexFormat* format ) {

	if ( this->mCurrentVtxFormat != format ) {
	
		this->OnGfxStateWillChange ();
	
		if ( this->mCurrentVtxFormat ) {
			this->mCurrentVtxFormat->Unbind ();
		}
		
		this->mCurrentVtxFormat = format;
			
		if ( format ) {
		
			assert ( this->mCurrentVtxBuffer ); // must currently have a valid vertex buffer bound (to receive the vertex format)
		
			format->Bind ( this->mCurrentVtxBuffer->GetAddress ());
		}
	}
}

//----------------------------------------------------------------//
MOAIGfxDeviceStateCache::MOAIGfxDeviceStateCache () :
	mCullFunc ( 0 ),
	mDepthFunc ( 0 ),
	mDepthMask ( true ),
	mBlendEnabled ( 0 ),
	mPenWidth ( 1.0f ),
	mShaderProgram ( 0 ),
	mActiveTextures ( 0 ),
	mCurrentFrameBuffer ( 0 ),
	mCurrentIdxBuffer ( 0 ),
	mCurrentTexture ( 0 ),
	mCurrentVtxArray ( 0 ),
	mCurrentVtxBuffer ( 0 ),
	mCurrentVtxFormat ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGfxDeviceStateCache::~MOAIGfxDeviceStateCache () {
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetBlendMode () {

	if ( this->mBlendEnabled ) {
	
		this->OnGfxStateWillChange ();
		
		zglDisable ( ZGL_PIPELINE_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	if ( !this->mBlendEnabled ) {
	
		this->OnGfxStateWillChange ();
		
		zglEnable ( ZGL_PIPELINE_BLEND );
		
		this->mBlendMode = blendMode;
		zglBlendMode ( this->mBlendMode.mEquation );
		zglBlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
		this->mBlendEnabled = true;
	}
	else if ( !this->mBlendMode.IsSame ( blendMode )) {
	
		this->OnGfxStateWillChange ();
		
		this->mBlendMode = blendMode;
		zglBlendMode ( this->mBlendMode.mEquation );
		zglBlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( srcFactor, dstFactor );
	blendMode.SetBlendEquation( equation );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetCullFunc () {

	this->SetCullFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetCullFunc ( int cullFunc ) {

	if ( this->mCullFunc != cullFunc ) {
	
		this->OnGfxStateWillChange ();
		
		this->mCullFunc = cullFunc;
	
		if ( cullFunc ) {
			zglEnable ( ZGL_PIPELINE_CULL );
			zglCullFace ( this->mCullFunc );
		}
		else {
			zglDisable ( ZGL_PIPELINE_CULL );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetDepthFunc () {

	this->SetDepthFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetDepthFunc ( int depthFunc ) {

	if ( this->mDepthFunc != depthFunc ) {
	
		this->OnGfxStateWillChange ();
		
		this->mDepthFunc = depthFunc;
	
		if ( depthFunc ) {
			zglEnable ( ZGL_PIPELINE_DEPTH );
			zglDepthFunc ( this->mDepthFunc );
		}
		else {
			zglDisable ( ZGL_PIPELINE_DEPTH );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetDepthMask ( bool depthMask ) {

	if ( this->mDepthMask != depthMask ) {
	
		this->OnGfxStateWillChange ();

		this->mDepthMask = depthMask;
		zglDepthMask ( this->mDepthMask );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetPenWidth ( float penWidth ) {

	if ( this->mPenWidth != penWidth ) {
		this->OnGfxStateWillChange ();
		this->mPenWidth = penWidth;
		zglLineWidth ( penWidth );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetScissorRect () {

	this->SetScissorRect ( this->mCurrentFrameBuffer->GetBufferRect ());
	zglDisable ( ZGL_PIPELINE_SCISSOR );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetScissorRect ( ZLRect rect ) {
	
	rect.Bless ();
	this->mViewRect.Clip ( rect );

	ZLRect& current = this->mScissorRect;
	
	if (	( current.mXMin != rect.mXMin ) ||
			( current.mYMin != rect.mYMin ) ||
			( current.mXMax != rect.mXMax ) ||
			( current.mYMax != rect.mYMax )) {
		
		this->OnGfxStateWillChange ();

		ZLRect deviceRect = this->mCurrentFrameBuffer->WndRectToDevice ( rect );

		s32 x = ( s32 )deviceRect.mXMin;
		s32 y = ( s32 )deviceRect.mYMin;
		
		u32 w = ( u32 )( deviceRect.Width () + 0.5f );
		u32 h = ( u32 )( deviceRect.Height () + 0.5f );

		w = h == 0 ? 0 : w;
		h = w == 0 ? 0 : h;
		
		zglScissor ( x, y, w, h );
		this->mScissorRect = rect;
	
		zglEnable ( ZGL_PIPELINE_SCISSOR );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetShader ( MOAIShader* shader ) {

	if ( shader ) {
		this->SetShader ( shader->GetProgram ());
		shader->BindUniforms ();
	}
	else {
		this->SetShader (( MOAIShaderProgram* )0 );
	}	
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetShader ( u32 preset ) {

	MOAIShaderMgr::Get ().BindShader ( preset );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetShader ( MOAIShaderProgram* program ) {

	if ( this->mShaderProgram != program ) {
	
		this->OnGfxStateWillChange ();
		this->mShaderProgram = program;
		
		if ( program ) {
			program->Bind ();
		}
		else {
			program->Unbind ();
		}
	}
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetTexture ( MOAITextureBase* textureSet ) {

	if ( this->mCurrentTexture != textureSet ) {

		this->OnGfxStateWillChange ();
		this->mCurrentTexture = textureSet;

		u32 unitsEnabled = 0;

		if ( textureSet ) {

			unitsEnabled = textureSet->CountActiveUnits ();
			
			// bind of unbind textues depending on state of texture set
			for ( u32 i = 0; i < unitsEnabled; ++i ) {
				this->SetTexture ( i, textureSet->GetTextureForUnit ( i ));
			}
		}
		
		// unbind/disable any excess textures
		for ( u32 i = unitsEnabled; i < this->mActiveTextures; ++i ) {
			this->SetTexture ( i, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetTexture ( u32 textureUnit, MOAISingleTexture* texture ) {
	
	MOAISingleTexture* currentTexture = this->mTextureUnits [ textureUnit ];
	
	if ( currentTexture != texture ) {
	
		this->OnGfxStateWillChange ();
	
		zglActiveTexture ( textureUnit );
	
		if ( currentTexture && ( !texture )) {
			currentTexture->Unbind ();
		}
		
		this->mTextureUnits [ textureUnit ] = texture;
		
		if ( texture ) {
			if ( !texture->Bind ()) {
			
				MOAITexture* defaultTexture = this->GetDefaultTexture ();
				if ( texture != defaultTexture ) {
					this->SetTexture ( textureUnit, defaultTexture );
				}
			}
		}
	}
}
