// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxConstsGL.h>
#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_DisplayListClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_GPUCacheGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_VertexCacheGL.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIShaderProgramGL.h>
#include <moai-gfx-gl/MOAITextureGL.h>
#include <moai-gfx-gl/MOAIVertexArrayGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>

//#define MOAIGFXSTATECACHE_DEBUG_LOG

#ifdef MOAIGFXSTATECACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// MOAIGfxMgrGL_GPUCacheGL
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::ApplyStateChange ( u32 stateID ) {
	
	switch ( stateID ) {

		case BLEND_MODE:
			
			this->FlushBlendMode ();
			break;
		
		case CULL_FUNC:
			
			this->FlushCullFunc ();
			break;
		
		case DEPTH_MODE:
			
			this->FlushDepthFunc ();
			this->FlushDepthMask ();
			break;

		case FRAME_BUFFER:
			
			this->FlushFrameBuffer ();
			break;
		
		case INDEX_BUFFER:
		
			this->FlushIndexBuffer ();
			break;
		
		case PEN_WIDTH:
		
			this->FlushPenWidth ();
			break;
		
		case SCISSOR_RECT:
	
			this->FlushScissorRect ();
			break;
		
		case SHADER:
		
			this->FlushShader ();
			break;
		
		case VERTEX_ARRAY:
		
			this->FlushVertexArray ();
			break;
		
		case VERTEX_BUFFER:
			
			this->FlushVertexBuffer ();
			break;
		
		case VERTEX_FORMAT:
			
			this->FlushVertexFormat ();
			break;

		case VIEW_RECT:
		
			this->FlushViewRect ();
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::Clear () {

	this->mCurrentState = NULL;
	this->mActiveState->Clear ();
	this->mPendingState->Clear ();
	
	this->mDefaultFrameBuffer = NULL;
	this->mDefaultTexture = NULL;
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushBlendMode () {

	assert ( this->mApplyingStateChanges );

	bool blendEnabled = this->mPendingState->mBlendEnabled;
	const MOAIBlendMode& blendMode = this->mPendingState->mBlendMode;

	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	if ( blendEnabled ) {
	
		bool updateBlend = false;
	
		if ( !active.mBlendEnabled ) {
		
			this->GfxStateWillChange ();
		
			DEBUG_LOG ( "  enabling blend\n" );
			gfx.Enable ( ZLGfxEnum::PIPELINE_BLEND );
			active.mBlendEnabled = true;
			updateBlend = true;
		}
		
		if ( updateBlend || !active.mBlendMode.IsSame ( blendMode )) {
		
			this->GfxStateWillChange ();
			
			DEBUG_LOG ( "  updating blend mode\n" );
			active.mBlendMode = blendMode;
			gfx.BlendMode ( MOAIGfxConstsGL::Remap ( active.mBlendMode.mEquation ));
			gfx.BlendFunc ( MOAIGfxConstsGL::Remap ( active.mBlendMode.mSourceFactor ), MOAIGfxConstsGL::Remap ( active.mBlendMode.mDestFactor ));
		}
	}
	else {
		
		if ( active.mBlendEnabled ) {
		
			this->GfxStateWillChange ();

			DEBUG_LOG ( "  disabling blend\n" );
			gfx.Disable ( ZLGfxEnum::PIPELINE_BLEND );
			active.mBlendEnabled = false;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushCullFunc () {

	assert ( this->mApplyingStateChanges );
	
	MOAICullFuncEnum::_ cullFunc = this->mPendingState->mCullFunc;
	
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	if ( active.mCullFunc != cullFunc ) {
	
		this->GfxStateWillChange ();
	
		active.mCullFunc = cullFunc;
	
		if ( active.mCullFunc ) {
			DEBUG_LOG ( "  enabling/setting cull func\n" );
			gfx.Enable ( ZLGfxEnum::PIPELINE_CULL );
			gfx.CullFace ( MOAIGfxConstsGL::Remap ( active.mCullFunc ));
		}
		else {
			DEBUG_LOG ( "  disabling cull func\n" );
			gfx.Disable ( ZLGfxEnum::PIPELINE_CULL );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushDepthFunc () {

	assert ( this->mApplyingStateChanges );
	
	MOAIDepthFuncEnum::_ depthFunc = this->mPendingState->mDepthFunc;
	
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();

	if ( active.mDepthFunc != depthFunc ) {
	
		this->GfxStateWillChange ();
	
		DEBUG_LOG ( "  setting depth func: %d\n", pending.mDepthFunc );
		
		active.mDepthFunc = depthFunc;
		
		if ( active.mDepthFunc ) {
			gfx.Enable ( ZLGfxEnum::PIPELINE_DEPTH );
			gfx.DepthFunc ( MOAIGfxConstsGL::Remap ( active.mDepthFunc ));
		}
		else {
			gfx.Disable ( ZLGfxEnum::PIPELINE_DEPTH );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushDepthMask () {

	assert ( this->mApplyingStateChanges );
	
	bool depthMask = this->mPendingState->mDepthMask;
	
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	if ( active.mDepthMask != depthMask ) {
		
		this->GfxStateWillChange ();
	
		DEBUG_LOG ( "  setting depth mask: %s\n", depthMask ? "true" : "false" );
		active.mDepthMask = depthMask;
		gfx.DepthMask ( active.mDepthMask );
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushFrameBuffer () {

	assert ( this->mApplyingStateChanges );
	
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	MOAIFrameBufferGL* prevBuffer = MOAICast < MOAIFrameBufferGL >( this->mActiveState->mFrameBuffer );
	MOAIFrameBufferGL* nextBuffer = MOAICast < MOAIFrameBufferGL >( this->mPendingState->mFrameBuffer );
	
	if ( prevBuffer != nextBuffer ) {
		
		DEBUG_LOG ( "  binding frame buffer: %p\n", nextBuffer );
	
		this->GfxStateWillChange ();
	
		if ( nextBuffer ) {
			nextBuffer->AffirmBuffers ();
		}
	
		gfx.BindFramebuffer ( ZLGfxEnum::FRAMEBUFFER_TARGET_DRAW_READ, nextBuffer->mGLFrameBuffer );
		this->mActiveState->mFrameBuffer = nextBuffer;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushIndexBuffer () {

	assert ( this->mApplyingStateChanges );
	
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	MOAIIndexBufferGL* prevBuffer = MOAICast < MOAIIndexBufferGL >( this->mActiveState->mIdxBuffer );
	MOAIIndexBufferGL* nextBuffer = MOAICast < MOAIIndexBufferGL >( this->mPendingState->mIdxBuffer );
	
	ZLSharedConstBuffer* bufferForBind = nextBuffer ? nextBuffer->GetBufferForBind ( gfx ) : 0;
	
	if (( prevBuffer != nextBuffer ) || ( this->mBoundIdxBuffer != bufferForBind )) {
	
		this->GfxStateWillChange ();
	
		DEBUG_LOG ( "  binding index buffer: %p\n", buffer );
		
		if ( prevBuffer ) {
			prevBuffer->Unbind ();
		}
		
		active.mIdxBuffer = nextBuffer;
		this->mBoundIdxBuffer = 0;
		
		if ( nextBuffer ) {
			nextBuffer->Bind ();
			this->mBoundIdxBuffer = bufferForBind;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushPenWidth () {

	assert ( this->mApplyingStateChanges );
	
	float penWidth = this->mPendingState->mPenWidth;
	
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	if ( active.mPenWidth != penWidth ) {
		
		DEBUG_LOG ( "  setting pen width: %f\n", penWidth );
	
		this->GfxStateWillChange ();
	
		active.mPenWidth = penWidth;
		gfx.LineWidth ( active.mPenWidth );
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushScissorRect () {

	assert ( this->mApplyingStateChanges );
	
	bool scissorEnabled = this->mPendingState->mScissorEnabled;
	ZLRect rect = this->mPendingState->mScissorRect;
	
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	if ( scissorEnabled ) {
	
		bool updateRect = false;
		
		if ( !active.mScissorEnabled ) {
		
			this->GfxStateWillChange ();
		
			DEBUG_LOG ( "  enabling scissor rect\n" );
			gfx.Enable ( ZLGfxEnum::PIPELINE_SCISSOR );
			active.mScissorEnabled = true;
			updateRect = true;
		}
		
		if ( updateRect || !active.mScissorRect.IsEqual ( rect )) {
		
			this->GfxStateWillChange ();
			
			active.mScissorRect = rect;
		
			ZLRect rect = active.mFrameBuffer->WndRectToDevice ( rect );
		
			s32 x = ( s32 )active.mScissorRect.mXMin;
			s32 y = ( s32 )active.mScissorRect.mYMin;
			
			u32 w = ( u32 )( active.mScissorRect.Width () + 0.5f );
			u32 h = ( u32 )( active.mScissorRect.Height () + 0.5f );

			w = h == 0 ? 0 : w;
			h = w == 0 ? 0 : h;
			
			DEBUG_LOG ( "  update scissor rect: %d %d %d %d\n", x, y, w, h );
			
			gfx.Scissor ( x, y, w, h );
		}
	}
	else {
	
		if ( active.mScissorEnabled == true ) {

			this->GfxStateWillChange ();

			DEBUG_LOG ( "  disabling scissor rect\n" );
			gfx.Disable ( ZLGfxEnum::PIPELINE_SCISSOR );
			active.mScissorEnabled = false;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushShader () {

	assert ( this->mApplyingStateChanges );
	
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	MOAIShaderGL* prevShader = MOAICast < MOAIShaderGL >( this->mActiveState->mShader );
	MOAIShaderGL* nextShader = MOAICast < MOAIShaderGL >( this->mPendingState->mShader );
	
	MOAIShaderProgramGL* program = nextShader ? nextShader->GetProgram () : 0;
	nextShader = program ? nextShader : 0;
	
	if ( nextShader ) {
		nextShader->ComposeUniforms ();
	}
	
	bool applyUniforms	= ( nextShader && nextShader->HasDirtyUniforms ());
	bool changeShader	= ( nextShader != prevShader );

	applyUniforms = true;
	changeShader = true;

	if ( applyUniforms || changeShader ) {
	
		this->GfxStateWillChange ();

		DEBUG_LOG ( "  binding shader program: %p\n", program );
		
		if ( changeShader ) {
			
			if ( prevShader ) {
				prevShader->GetProgram ()->Unbind ();
			}
			if ( nextShader ) {
				program->Bind ();
			}
			this->mActiveState->mShader = nextShader;
		}
		
		if ( applyUniforms ) {
			nextShader->ApplyUniforms ();
			nextShader->BindUniforms ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushTexture ( ZLIndex textureUnit ) {

	assert ( this->mApplyingStateChanges );
	
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	MOAITextureGL* prevTexture = MOAICast < MOAITextureGL >( this->mActiveState->mTextureUnits [ textureUnit ]);
	MOAITextureGL* nextTexture = MOAICast < MOAITextureGL >( this->mPendingState->mTextureUnits [ textureUnit ]);
	
	nextTexture = nextTexture && nextTexture->IsReadyForUse () ? nextTexture : MOAICast < MOAITextureGL >( this->mDefaultTexture );

	if ( prevTexture != nextTexture ) {

		this->GfxStateWillChange ();

		DEBUG_LOG ( "  binding texture set: %p\n", textureSet );

		gfx.ActiveTexture ( textureUnit );

		if ( prevTexture ) {
			prevTexture->Unbind ();
		}
		
		this->mActiveState->mTextureUnits [ textureUnit ] = nextTexture;
		
		if ( nextTexture ) {
		
			DEBUG_LOG ( "    binding texture: %d %p\n", i, bindTexture );
			nextTexture->Bind ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushVertexArray () {

	assert ( this->mApplyingStateChanges );
	
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	MOAIVertexArrayGL* prevArray = MOAICast < MOAIVertexArrayGL >( this->mActiveState->mVtxArray );
	MOAIVertexArrayGL* nextArray = MOAICast < MOAIVertexArrayGL >( this->mPendingState->mVtxArray );
	
	if ( prevArray != nextArray ) {

		this->GfxStateWillChange ();

		DEBUG_LOG ( "  binding vertex array: %p\n", nextArray );

		if ( prevArray ) {
			prevArray->Unbind ();
		}
		
		this->mActiveState->mVtxArray = nextArray;
		
		if ( nextArray ) {
			this->mPendingState->mVtxBuffer = NULL;
			this->FlushVertexBuffer (); // force the unbind in case it hasn't happened yet
			nextArray->Bind ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushVertexBuffer () {

	assert ( this->mApplyingStateChanges );

	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	MOAIVertexBufferGL* prevBuffer = MOAICast < MOAIVertexBufferGL >( this->mActiveState->mVtxBuffer );
	MOAIVertexBufferGL* nextBuffer = MOAICast < MOAIVertexBufferGL >( this->mPendingState->mVtxBuffer );
	
	ZLSharedConstBuffer* bufferForBind = nextBuffer ? nextBuffer->GetBufferForBind ( gfx ) : 0;

	MOAIVertexFormatGL* prevFormat = MOAICast < MOAIVertexFormatGL >( this->mActiveState->mVtxFormat );
	MOAIVertexFormatGL* nextFormat = MOAICast < MOAIVertexFormatGL >( this->mPendingState->mVtxFormat );

	if (( prevBuffer != nextBuffer ) || ( this->mBoundVtxBuffer != bufferForBind )) {

		this->GfxStateWillChange ();

		DEBUG_LOG ( "  binding vertex buffer: (%p)\n", buffer );

		if ( prevBuffer ) {
			assert ( prevFormat );
			prevFormat->Unbind ( gfx );
		}
		
		this->mActiveState->mVtxBuffer = NULL;
		this->mBoundVtxBuffer = NULL;
		
		if ( nextFormat && nextBuffer ) {
			
			this->mPendingState->mVtxArray = NULL;
			this->FlushVertexArray (); // force the unbind in case it hasn't happened yet
			
			nextBuffer->Bind ();
			nextFormat->Bind ( gfx, bufferForBind );
			nextBuffer->Unbind ();
			
			this->mActiveState->mVtxBuffer = nextBuffer;
			this->mBoundVtxBuffer = bufferForBind;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushVertexFormat () {

	assert ( this->mApplyingStateChanges );
	
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	MOAIVertexFormatGL* prevFormat = MOAICast < MOAIVertexFormatGL >( this->mActiveState->mVtxFormat );
	MOAIVertexFormatGL* nextFormat = MOAICast < MOAIVertexFormatGL >( this->mPendingState->mVtxFormat );
	
	if ( prevFormat != nextFormat ) {
		
		DEBUG_LOG ( "  binding vertex format: (%p)\n", vtxFormat );
	
		this->GfxStateWillChange ();
		
		if ( prevFormat && this->mActiveState->mVtxBuffer ) {
			prevFormat->Unbind ( gfx );
		}
		
		this->mActiveState->mVtxFormat = nextFormat;
		this->mActiveState->mVtxBuffer = NULL; // must be set in a later step
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::FlushViewRect () {

	assert ( this->mApplyingStateChanges );
	
	ZLRect rect = this->mPendingState->mViewRect;
	
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	if ( !active.mViewRect.IsEqual ( rect )) {
		
		this->GfxStateWillChange ();
	
		active.mViewRect = rect;

		ZLRect rect = this->mActiveState->mFrameBuffer->WndRectToDevice ( rect );

		s32 x = ( s32 )active.mViewRect.mXMin;
		s32 y = ( s32 )active.mViewRect.mYMin;
	
		u32 w = ( u32 )( active.mViewRect.Width () + 0.5f );
		u32 h = ( u32 )( active.mViewRect.Height () + 0.5f );
	
		DEBUG_LOG ( "  update view rect: %d %d %d %d\n", x, y, w, h );
	
		gfx.Viewport ( x, y, w, h );
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::ForceIndexBuffer ( MOAIIndexBufferGL* buffer ) {

	assert ( this->mApplyingStateChanges );
	
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;
	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	DEBUG_LOG ( "  binding index buffer: %p\n", buffer );
	
	active.mIdxBuffer = buffer;
	this->mBoundIdxBuffer = NULL;
	
	if ( buffer ) {
		buffer->Bind ();
		this->mBoundIdxBuffer = buffer->GetBufferForBind ( gfx );
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::ForceVertexBuffer ( MOAIVertexBufferGL* buffer ) {

	assert ( this->mApplyingStateChanges );
	assert ( !this->mActiveState->mVtxArray );

	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
	MOAIVertexFormatGL* format = MOAICast < MOAIVertexFormatGL >( this->mActiveState->mVtxFormat );
	
	if ( format && buffer ) {
		
		ZLSharedConstBuffer* bufferForBind = buffer->GetBufferForBind ( gfx );
		
		buffer->Bind ();
		format->Bind ( gfx, bufferForBind );
		buffer->Unbind ();
		
		this->mActiveState->mVtxBuffer = buffer;
		this->mBoundVtxBuffer = bufferForBind;
	}
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_GPUCacheGL::MOAIGfxMgrGL_GPUCacheGL () :
	mBoundIdxBuffer ( NULL ),
	mBoundVtxBuffer ( NULL ) {
	
	this->mActiveState = new MOAIGfxStateGPUCacheFrameGL ();
	this->mPendingState = new MOAIGfxStateGPUCacheFrameGL ();
	
	this->mCurrentState = this->mPendingState;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_GPUCacheGL::~MOAIGfxMgrGL_GPUCacheGL () {
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::RecalculateDirtyFlags () {

	if ( this->mPendingState->mBlendEnabled ) {
		this->SetBlendMode ( this->mPendingState->mBlendMode );
	}
	else {
		this->SetBlendMode ();
	}
	
	this->SetCullFunc ( this->mPendingState->mCullFunc );
	this->SetDepthFunc ( this->mPendingState->mDepthFunc );
	this->SetDepthMask ( this->mPendingState->mDepthMask );
	this->SetFrameBuffer ( this->mPendingState->mFrameBuffer );
	this->SetIndexBuffer ( this->mPendingState->mIdxBuffer );
	this->SetPenWidth ( this->mPendingState->mPenWidth );
	
	if ( this->mPendingState->mScissorEnabled ) {
		this->SetScissorRect ( this->mPendingState->mScissorRect );
	}
	else {
		this->SetScissorRect ();
	}
	
	this->SetShader ( this->mPendingState->mShader );
	this->SetVertexArray ( this->mPendingState->mVtxArray );
	this->SetViewRect ( this->mPendingState->mViewRect );
	this->SetVertexBuffer ( this->mPendingState->mVtxBuffer );
	this->SetVertexFormat ( this->mPendingState->mVtxFormat );
	
	for ( ZLIndex i = 0; i < MAX_TEXTURE_UNITS; ++i ) {
		this->SetTexture ( this->mPendingState->mTextureUnits [ i ], i );
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::RestoreGPUState ( const MOAIGfxStateGPUCacheFrame& frame ) {

	*this->mPendingState = frame;
	this->RecalculateDirtyFlags ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::StoreGPUState ( MOAIGfxStateGPUCacheFrame& frame ) const {

	if ( frame.mTextureUnits.Size () < this->mMaxTextureUnits ) {
		frame.mTextureUnits.Grow ( this->mMaxTextureUnits, 0 );
	}
	frame = *this->mPendingState;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::MOAIGfxMgr_GPUCache_ApplyStateChanges () {

	if ( !this->mApplyingStateChanges ) {
	
		if ( this->mPendingState->mShader ) {
			this->mPendingState->mShader->SelectTextures ();
		}

		if ( this->mDirtyFlags || this->mTextureDirtyFlags ) {
			
			this->SuspendChanges ();
			this->mCurrentState = this->mActiveState;
			
			u32 dirtyFlags = this->mDirtyFlags;
			this->mDirtyFlags = 0;
			
			DEBUG_LOG ( "APPLY STATE CHANGES\n" );

			for ( u32 i = 0; dirtyFlags; ++i ) {
				u32 mask = 1 << i;
				if ( dirtyFlags & mask ) {
					this->ApplyStateChange ( mask );
					dirtyFlags &= ~mask;
				}
			}
			
			u32 textureDirtyFlags = this->mTextureDirtyFlags;
			this->mTextureDirtyFlags = 0;
			
			for ( ZLIndex i = 0; textureDirtyFlags; ++i ) {
				u32 mask = 1 << i;
				if ( textureDirtyFlags & mask ) {
					this->FlushTexture ( i );
					textureDirtyFlags &= ~mask;
				}
			}
			
			this->mCurrentState = this->mPendingState;
			this->ResumeChanges ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::MOAIGfxMgr_GPUCache_ClearSurface () {
	
	MOAIGfxMgr_CPUCache& cpuCache = this->GetCPUCache ();
	
	u32 clearFlags = cpuCache.GetClearFlags ();

	if ( clearFlags ) {
	
		ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	
		// discard this if all buffers are to be cleared?
		// (may still need to draw if depth or color only)
		this->GetVertexCache ().FlushToGPU ();
		
		this->ApplyStateChanges ();
	
		if ( clearFlags & ZLGfxClearFlags::COLOR_BUFFER_BIT ) {
		
			const ZLColorVec& clearColor = cpuCache.GetClearColor ();
		
			gfx.ClearColor (
				clearColor.mR,
				clearColor.mG,
				clearColor.mB,
				clearColor.mA
			);
		}
	
		if (( clearFlags & ZLGfxClearFlags::DEPTH_BUFFER_BIT ) && !this->GetDepthMask ()) {
			gfx.DepthMask ( true );
			gfx.Clear ( clearFlags );
			gfx.DepthMask ( false );
		}
		else {
			gfx.Clear ( clearFlags );
		}
	}
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrGL_GPUCacheGL::MOAIGfxMgr_GPUCache_CountTextureUnits () {

	return this->mActiveState->mTextureUnits.Size ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::MOAIGfxMgr_GPUCache_DrawPrims ( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count ) {

	DEBUG_LOG ( "DRAW PRIMS: %d %d %d\n", primType, base, count );
	
	this->ApplyStateChanges ();

	ZLGfxEnum::_ primTypeZGL = MOAIGfxConstsGL::Remap ( primType );

	MOAIShaderGL* shader = MOAICast < MOAIShaderGL >( this->mActiveState->mShader );

	if ( shader && ( this->mActiveState->mVtxBuffer || this->mActiveState->mVtxArray )) {
		
		ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
		
		MOAIIndexBufferGL* idxBuffer = MOAICast < MOAIIndexBufferGL >( this->mActiveState->mIdxBuffer );
		
		if ( idxBuffer ) {
		
			DEBUG_LOG ( "drawing prims with index and vertex buffer\n" );
			
			size_t indexSize = idxBuffer->GetIndexSize ();
			ZLGfxEnum::_ indexType = indexSize == 2 ? ZLGfxEnum::TYPE_UNSIGNED_SHORT : ZLGfxEnum::TYPE_UNSIGNED_INT;
			gfx.DrawElements ( primTypeZGL, count, indexType, this->mBoundIdxBuffer, base * indexSize );
		}
		else {
			DEBUG_LOG ( "drawing prims with vertex buffer\n" );
			gfx.DrawArrays ( primTypeZGL, base, count );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::MOAIGfxMgr_GPUCache_ResetGPUState () {

	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();
	MOAIGfxStateGPUCacheFrame& pending = *this->mPendingState;
	MOAIGfxStateGPUCacheFrame& active = *this->mActiveState;

	ZGL_COMMENT ( gfx, "GFX RESET STATE" );

	// reset the shader
	gfx.UseProgram ( ZLGfxResource::UNBIND );
	
	// turn off blending
	gfx.Disable ( ZLGfxEnum::PIPELINE_BLEND );
	pending.mBlendEnabled = false;
	active.mBlendEnabled = false;
	
	// disable backface culling
	gfx.Disable ( ZLGfxEnum::PIPELINE_CULL );
	pending.mCullFunc = MOAICullFuncEnum::NONE;
	active.mCullFunc = MOAICullFuncEnum::NONE;
	
	// disable depth test
	gfx.Disable ( ZLGfxEnum::PIPELINE_DEPTH );
	pending.mDepthFunc = MOAIDepthFuncEnum::NONE;
	active.mDepthFunc = MOAIDepthFuncEnum::NONE;
	
	// disable depth write
	gfx.DepthMask ( false );
	pending.mDepthMask = false;
	active.mDepthMask = 0;
	
	// reset the pen width
	pending.mPenWidth = 1.0f;
	active.mPenWidth = 1.0f;
	gfx.LineWidth ( 1.0f );
	
	// reset the scissor rect
	pending.mScissorEnabled = false;
	active.mScissorEnabled = false;
	gfx.Disable ( ZLGfxEnum::PIPELINE_SCISSOR );
	
	// TODO: seems like overkill
	for ( ZLIndex i = 0; i < this->mMaxTextureUnits; ++i ){
		gfx.ActiveTexture (( u32 )i );
		gfx.BindTexture ( ZLGfxResource::UNBIND );
		pending.mTextureUnits [ i ] = 0;
		active.mTextureUnits [ i ] = 0;
	}
	gfx.ActiveTexture ( 0 );
	
	MOAIVertexArrayGL* activeVtxArray = MOAICast < MOAIVertexArrayGL >( active.mVtxArray );
	if ( activeVtxArray ) {
		activeVtxArray->Unbind ();
	}
	
	pending.mShader			= 0;
	pending.mIdxBuffer		= 0;
	pending.mVtxArray		= 0;
	pending.mVtxBuffer		= 0;
	pending.mVtxFormat		= 0;
	
	active.mShader			= 0;
	active.mIdxBuffer		= 0;
	active.mVtxArray		= 0;
	active.mVtxBuffer		= 0;
	active.mVtxFormat		= 0;
	
	pending.mFrameBuffer = this->mDefaultFrameBuffer;
	active.mFrameBuffer = this->mDefaultFrameBuffer;
	
	MOAIFrameBufferGL* defaultFrameBuffer = MOAICast < MOAIFrameBufferGL >( this->mDefaultFrameBuffer );
	gfx.BindFramebuffer ( ZLGfxEnum::FRAMEBUFFER_TARGET_DRAW_READ, defaultFrameBuffer->mGLFrameBuffer );
	
	this->mDirtyFlags = 0;
	this->mTextureDirtyFlags = 0;
	
	ZGL_COMMENT ( gfx, "" );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_GPUCacheGL::MOAIGfxMgr_GPUCache_UnbindAll () {

	ZLGfx& gfx = this->GetDisplayListClerkGL ().GetDrawingAPI ();

	ZGL_COMMENT ( gfx, "GFX UNBIND ALL" );

	this->SetFrameBuffer ();
	this->SetIndexBuffer ();
	this->SetShader ();
	this->SetTexture ();
	this->SetVertexArray ();
	this->SetVertexBuffer ();
	this->SetVertexFormat ();
	
	ZGL_COMMENT ( gfx, "" );
}

