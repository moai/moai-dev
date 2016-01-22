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
bool MOAIGfxDeviceStateCache::BindFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	frameBuffer = frameBuffer ? frameBuffer : this->GetDefaultFrameBuffer ();

	if ( this->mCurrentFrameBuffer != frameBuffer ) {
		
		this->OnGfxStateWillChange ();
		
		this->mDrawingAPI->BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBufferID );
		this->mCurrentFrameBuffer = frameBuffer;
	}
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindIndexBuffer ( MOAIIndexBuffer* buffer ) {

	if ( this->mCurrentIdxBuffer != buffer ) {
	
		if ( this->mCurrentIdxBuffer ) {
			this->mCurrentIdxBuffer->Unbind ();
		}
	
		this->mCurrentIdxBuffer = buffer;
		
		if ( buffer ) {
			buffer->Bind ();
		}
	}
	
	return buffer ? buffer->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindShader ( MOAIShader* shader ) {

	// TODO: why not caching shader?
	if ( shader ) {
		if ( !this->BindShader ( shader->GetProgram ())) return false;
		shader->BindUniforms ();
		return true;
	}
	return this->BindShader (( MOAIShaderProgram* )0 );
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindShader ( u32 preset ) {

	return MOAIShaderMgr::Get ().BindShader ( preset );
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindShader ( MOAIShaderProgram* program ) {

	if ( this->mShaderProgram != program ) {
	
		this->OnGfxStateWillChange ();
		
		if ( this->mShaderProgram ) {
			this->mShaderProgram->Unbind ();
		}
		
		this->mShaderProgram = program;
		
		if ( program ) {
			program->Bind ();
		}
	}
	this->mShaderDirty = true;
	
	return this->mShaderProgram ? this->mShaderProgram->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindTexture ( MOAITextureBase* textureSet ) {

	bool result = true;

	if ( this->mCurrentTexture != textureSet ) {

		this->OnGfxStateWillChange ();

		this->mCurrentTexture = textureSet;

		u32 unitsEnabled = 0;

		if ( textureSet ) {

			unitsEnabled = textureSet->CountActiveUnits ();
			
			// bind or unbind textues depending on state of texture set
			for ( u32 i = 0; i < unitsEnabled; ++i ) {
				if ( !this->BindTexture ( i, textureSet->GetTextureForUnit ( i ))) {
					result = false;
				}
			}
		}
		
		// unbind/disable any excess textures
		for ( u32 i = unitsEnabled; i < this->mActiveTextures; ++i ) {
			this->BindTexture ( i, 0 );
		}
		
		this->mActiveTextures = unitsEnabled;
	}
	
	return result;
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindTexture ( u32 textureUnit, MOAISingleTexture* texture ) {
	
	MOAISingleTexture* currentTexture = this->mTextureUnits [ textureUnit ];
	MOAISingleTexture* bindTexture = texture;
	
	if ( texture && ( !texture->IsReady ())) {
		bindTexture = this->GetDefaultTexture ();
	}
	
	if ( currentTexture != bindTexture ) {
		
		this->OnGfxStateWillChange ();
		
		this->mDrawingAPI->ActiveTexture ( textureUnit );
		
		if ( currentTexture ) {
			currentTexture->Unbind ();
		}
		
		this->mTextureUnits [ textureUnit ] = bindTexture;
		
		if ( bindTexture ) {
			bindTexture->Bind ();
		}
	}
	
	return texture ? ( bindTexture && bindTexture->IsReady ()) : true;
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindVertexArray ( MOAIVertexArray* vtxArray ) {

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
	
	return vtxArray ? vtxArray->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindVertexBuffer ( MOAIVertexBuffer* buffer ) {

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
	
	return buffer ? buffer->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceStateCache::BindVertexFormat ( MOAIVertexFormat* format, bool copyBuffer ) {

	if ( this->mCurrentVtxFormat != format ) {
	
		this->OnGfxStateWillChange ();
	
		if ( this->mCurrentVtxFormat ) {
			this->mCurrentVtxFormat->Unbind ();
		}
		
		this->mCurrentVtxFormat = format;
			
		if ( format ) {
		
			assert ( this->mCurrentVtxBuffer ); // must currently have a valid vertex buffer bound (to receive the vertex format)
		
			format->Bind ( this->mCurrentVtxBuffer->GetBuffer (), copyBuffer );
		}
	}
	
	return true;
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
		
		this->mDrawingAPI->Disable ( ZGL_PIPELINE_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	if ( !this->mBlendEnabled ) {
	
		this->OnGfxStateWillChange ();
		
		this->mDrawingAPI->Enable ( ZGL_PIPELINE_BLEND );
		
		this->mBlendMode = blendMode;
		this->mDrawingAPI->BlendMode ( this->mBlendMode.mEquation );
		this->mDrawingAPI->BlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
		this->mBlendEnabled = true;
	}
	else if ( !this->mBlendMode.IsSame ( blendMode )) {
	
		this->OnGfxStateWillChange ();
		
		this->mBlendMode = blendMode;
		this->mDrawingAPI->BlendMode ( this->mBlendMode.mEquation );
		this->mDrawingAPI->BlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
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
			this->mDrawingAPI->Enable ( ZGL_PIPELINE_CULL );
			this->mDrawingAPI->CullFace ( this->mCullFunc );
		}
		else {
			this->mDrawingAPI->Disable ( ZGL_PIPELINE_CULL );
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
			this->mDrawingAPI->Enable ( ZGL_PIPELINE_DEPTH );
			this->mDrawingAPI->DepthFunc ( this->mDepthFunc );
		}
		else {
			this->mDrawingAPI->Disable ( ZGL_PIPELINE_DEPTH );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetDepthMask ( bool depthMask ) {

	if ( this->mDepthMask != depthMask ) {
	
		this->OnGfxStateWillChange ();

		this->mDepthMask = depthMask;
		this->mDrawingAPI->DepthMask ( this->mDepthMask );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetPenWidth ( float penWidth ) {

	if ( this->mPenWidth != penWidth ) {
		this->OnGfxStateWillChange ();
		this->mPenWidth = penWidth;
		this->mDrawingAPI->LineWidth ( penWidth );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::SetScissorRect () {

	this->SetScissorRect ( this->mCurrentFrameBuffer->GetBufferRect ());
	this->mDrawingAPI->Disable ( ZGL_PIPELINE_SCISSOR );
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
		
		this->mDrawingAPI->Scissor ( x, y, w, h );
		this->mScissorRect = rect;
	
		this->mDrawingAPI->Enable ( ZGL_PIPELINE_SCISSOR );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceStateCache::UnbindAll () {

	this->mDrawingAPI->Comment ( "GFX UNBIND ALL" );

	this->BindFrameBuffer ();
	this->BindIndexBuffer ();
	this->BindShader ();
	this->BindTexture ();
	this->BindVertexArray ();
	this->BindVertexBuffer ();
	this->BindVertexFormat ();
	
	ZGL_COMMENT ( *this->mDrawingAPI, "" );
}
