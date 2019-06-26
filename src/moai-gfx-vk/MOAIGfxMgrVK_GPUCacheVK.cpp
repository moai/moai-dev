// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_PipelineClerkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_VertexCacheVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>
#include <moai-gfx-vk/MOAIVertexArrayVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//#define MOAIGFXSTATECACHE_DEBUG_LOG

#ifdef MOAIGFXSTATECACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// MOAIGfxStateGPUCacheFrameVK
//================================================================//

////----------------------------------------------------------------//
//void MOAIGfxStateGPUCacheFrameVK::Clear () {
//
//	this->mShader			= NULL;
//	this->mFrameBuffer		= NULL;
//	this->mIdxBuffer		= NULL;
//	this->mVtxArray			= NULL;
//	this->mVtxBuffer		= NULL;
//	this->mVtxFormat		= NULL;
//	
//	this->mTextureUnits.Clear ();
//}

////----------------------------------------------------------------//
//MOAIGfxStateGPUCacheFrameVK::MOAIGfxStateGPUCacheFrameVK () :
//	mCullFunc ( 0 ),
//	mDepthFunc ( 0 ),
//	mDepthMask ( true ),
//	mBlendEnabled ( 0 ),
//	mPenWidth ( 1.0f ),
//	mScissorEnabled ( false ),
//	mScissorRect ( ZLRect::EMPTY ),
//	mViewRect ( ZLRect::EMPTY ) {
//}
//
////----------------------------------------------------------------//
//MOAIGfxStateGPUCacheFrameVK::~MOAIGfxStateGPUCacheFrameVK () {
//}

//================================================================//
// MOAIGfxMgrVK_GPUCacheVK
//================================================================//

////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::ApplyStateChange ( u32 stateID ) {
//
//	switch ( stateID ) {
//
//		case BLEND_MODE:
//
//			this->FlushBlendMode ( this->mPendingState.mBlendEnabled, this->mPendingState.mBlendMode );
//			break;
//
//		case CULL_FUNC:
//
//			this->FlushCullFunc ( this->mPendingState.mCullFunc );
//			break;
//
//		case DEPTH_MODE:
//
//			this->FlushDepthFunc ( this->mPendingState.mDepthFunc );
//			this->FlushDepthMask ( this->mPendingState.mDepthMask );
//			break;
//
//		case FRAME_BUFFER:
//
//			this->FlushFrameBuffer ( this->mPendingState.mFrameBuffer );
//			break;
//
//		case INDEX_BUFFER:
//
//			this->FlushIndexBuffer ( this->mPendingState.mIdxBuffer );
//			break;
//
//		case PEN_WIDTH:
//
//			this->FlushPenWidth ( this->mPendingState.mPenWidth );
//			break;
//
//		case SCISSOR_RECT:
//
//			this->FlushScissorRect ( this->mPendingState.mScissorEnabled, this->mPendingState.mScissorRect );
//			break;
//
//		case SHADER:
//
//			this->FlushShader ( this->mPendingState.mShader );
//			break;
//
//		case VERTEX_ARRAY:
//
//			this->FlushVertexArray ( this->mPendingState.mVtxArray );
//			break;
//
//		case VERTEX_BUFFER:
//
//			this->mActiveState.mVtxFormat = this->mPendingState.mVtxFormat;
//			this->FlushVertexBuffer ( this->mPendingState.mVtxBuffer );
//			break;
//
//		case VERTEX_FORMAT:
//
//			this->FlushVertexFormat ( this->mPendingState.mVtxFormat );
//			break;
//
//		case VIEW_RECT:
//
//			this->FlushViewRect ( this->mPendingState.mViewRect );
//			break;
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::ApplyStateChanges () {
//
//	if ( !this->mApplyingStateChanges ) {
//
//		if ( this->mPendingState.mShader ) {
//			this->mPendingState.mShader->ScheduleTextures ();
//		}
//
//		if ( this->mDirtyFlags || this->mTextureDirtyFlags ) {
//
//			this->SuspendChanges ();
//			this->mCurrentState = &this->mActiveState;
//
//			u32 dirtyFlags = this->mDirtyFlags;
//			this->mDirtyFlags = 0;
//
//			DEBUG_LOG ( "APPLY STATE CHANGES\n" );
//
//			for ( u32 i = 0; dirtyFlags; ++i ) {
//				u32 mask = 1 << i;
//				if ( dirtyFlags & mask ) {
//					this->ApplyStateChange ( mask );
//					dirtyFlags &= ~mask;
//				}
//			}
//
//			u32 textureDirtyFlags = this->mTextureDirtyFlags;
//			this->mTextureDirtyFlags = 0;
//
//			for ( ZLIndex i = ZLIndexOp::ZERO; textureDirtyFlags; ++i ) {
//				u32 mask = 1 << i;
//				if ( textureDirtyFlags & mask ) {
//					this->FlushTexture ( i, this->mPendingState.mTextureUnits [ i ]);
//					textureDirtyFlags &= ~mask;
//				}
//			}
//
//			this->mCurrentState = &this->mPendingState;
//			this->ResumeChanges ();
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::Clear () {
//
//	this->mCurrentState = NULL;
//	this->mActiveState.Clear ();
//	this->mPendingState.Clear ();
//
//	this->mDefaultFrameBuffer = NULL;
//	this->mDefaultTexture = NULL;
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushBlendMode ( bool blendEnabled, const MOAIBlendMode& blendMode ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( blendEnabled ) {
//
//		bool updateBlend = false;
//
//		if ( !active.mBlendEnabled ) {
//
//			this->GfxStateWillChange ();
//
//			DEBUG_LOG ( "  enabling blend\n" );
//			gfx.Enable ( ZGL_PIPELINE_BLEND );
//			active.mBlendEnabled = true;
//			updateBlend = true;
//		}
//
//		if ( updateBlend || !active.mBlendMode.IsSame ( blendMode )) {
//
//			this->GfxStateWillChange ();
//
//			DEBUG_LOG ( "  updating blend mode\n" );
//			active.mBlendMode = blendMode;
//			gfx.BlendMode ( active.mBlendMode.mEquation );
//			gfx.BlendFunc ( active.mBlendMode.mSourceFactor, active.mBlendMode.mDestFactor );
//		}
//	}
//	else {
//
//		if ( active.mBlendEnabled ) {
//
//			this->GfxStateWillChange ();
//
//			DEBUG_LOG ( "  disabling blend\n" );
//			gfx.Disable ( ZGL_PIPELINE_BLEND );
//			active.mBlendEnabled = false;
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushCullFunc ( int cullFunc ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( active.mCullFunc != cullFunc ) {
//
//		this->GfxStateWillChange ();
//
//		active.mCullFunc = cullFunc;
//
//		if ( active.mCullFunc ) {
//			DEBUG_LOG ( "  enabling/setting cull func\n" );
//			gfx.Enable ( ZGL_PIPELINE_CULL );
//			gfx.CullFace ( active.mCullFunc );
//		}
//		else {
//			DEBUG_LOG ( "  disabling cull func\n" );
//			gfx.Disable ( ZGL_PIPELINE_CULL );
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushDepthFunc ( int depthFunc ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( active.mDepthFunc != depthFunc ) {
//
//		this->GfxStateWillChange ();
//
//		DEBUG_LOG ( "  setting depth func: %d\n", pending.mDepthFunc );
//
//		active.mDepthFunc = depthFunc;
//
//		if ( active.mDepthFunc ) {
//			gfx.Enable ( ZGL_PIPELINE_DEPTH );
//			gfx.DepthFunc ( active.mDepthFunc );
//		}
//		else {
//			gfx.Disable ( ZGL_PIPELINE_DEPTH );
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushDepthMask ( bool depthMask ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( active.mDepthMask != depthMask ) {
//
//		this->GfxStateWillChange ();
//
//		DEBUG_LOG ( "  setting depth mask: %s\n", depthMask ? "true" : "false" );
//		active.mDepthMask = depthMask;
//		gfx.DepthMask ( active.mDepthMask );
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushFrameBuffer ( MOAIFrameBufferVK* frameBuffer ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( active.mFrameBuffer != frameBuffer ) {
//
//		DEBUG_LOG ( "  binding frame buffer: %p\n", frameBuffer );
//
//		this->GfxStateWillChange ();
//
//		if ( frameBuffer ) {
//			frameBuffer->AffirmBuffers ();
//		}
//
//		gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBuffer );
//		active.mFrameBuffer = frameBuffer;
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushIndexBuffer ( MOAIIndexBufferVK* buffer ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	ZLSharedConstBuffer* bufferForBind = buffer ? buffer->GetBufferForBind ( gfx ) : 0;
//
//	if (( active.mIdxBuffer != buffer ) || ( this->mBoundIdxBuffer != bufferForBind )) {
//
//		this->GfxStateWillChange ();
//
//		DEBUG_LOG ( "  binding index buffer: %p\n", buffer );
//
//		if ( active.mIdxBuffer ) {
//			active.mIdxBuffer->Unbind ();
//		}
//
//		active.mIdxBuffer = buffer;
//		this->mBoundIdxBuffer = 0;
//
//		if ( buffer ) {
//			buffer->Bind ();
//			this->mBoundIdxBuffer = bufferForBind;
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushPenWidth ( float penWidth ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( active.mPenWidth != penWidth ) {
//
//		DEBUG_LOG ( "  setting pen width: %f\n", penWidth );
//
//		this->GfxStateWillChange ();
//
//		active.mPenWidth = penWidth;
//		gfx.LineWidth ( active.mPenWidth );
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushScissorRect ( bool scissorEnabled, ZLRect rect ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( scissorEnabled ) {
//
//		bool updateRect = false;
//
//		if ( !active.mScissorEnabled ) {
//
//			this->GfxStateWillChange ();
//
//			DEBUG_LOG ( "  enabling scissor rect\n" );
//			gfx.Enable ( ZGL_PIPELINE_SCISSOR );
//			active.mScissorEnabled = true;
//			updateRect = true;
//		}
//
//		if ( updateRect || !active.mScissorRect.IsEqual ( rect )) {
//
//			this->GfxStateWillChange ();
//
//			active.mScissorRect = rect;
//
//			ZLRect rect = active.mFrameBuffer->WndRectToDevice ( rect );
//
//			s32 x = ( s32 )active.mScissorRect.mXMin;
//			s32 y = ( s32 )active.mScissorRect.mYMin;
//
//			u32 w = ( u32 )( active.mScissorRect.Width () + 0.5f );
//			u32 h = ( u32 )( active.mScissorRect.Height () + 0.5f );
//
//			w = h == 0 ? 0 : w;
//			h = w == 0 ? 0 : h;
//
//			DEBUG_LOG ( "  update scissor rect: %d %d %d %d\n", x, y, w, h );
//
//			gfx.Scissor ( x, y, w, h );
//		}
//	}
//	else {
//
//		if ( active.mScissorEnabled == true ) {
//
//			this->GfxStateWillChange ();
//
//			DEBUG_LOG ( "  disabling scissor rect\n" );
//			gfx.Disable ( ZGL_PIPELINE_SCISSOR );
//			active.mScissorEnabled = false;
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushShader ( MOAIShaderVK* shader ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	MOAIShaderProgramVK* program = shader ? shader->GetProgram () : 0;
//	shader = program ? shader : 0;
//
//	if ( shader ) {
//		shader->UpdateUniforms ();
//	}
//
//	bool applyUniforms	= ( shader && shader->HasDirtyUniforms ());
//	bool changeShader	= ( shader != this->mActiveState.mShader );
//
//	applyUniforms = true;
//	changeShader = true;
//
//	if ( applyUniforms || changeShader ) {
//
//		this->GfxStateWillChange ();
//
//		DEBUG_LOG ( "  binding shader program: %p\n", program );
//
//		if ( changeShader ) {
//
//			if ( active.mShader ) {
//				active.mShader->GetProgram ()->Unbind ();
//			}
//			if ( shader ) {
//				program->Bind ();
//			}
//			active.mShader = shader;
//		}
//
//		if ( applyUniforms ) {
//			shader->ApplyUniforms ();
//			shader->BindUniforms ();
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushTexture ( ZLIndex textureUnit, MOAITextureVK* texture ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	texture = texture && texture->Affirm () ? texture : ( MOAITextureVK* )this->mDefaultTexture;
//	MOAITextureVK* prevTexture = active.mTextureUnits [ textureUnit ];
//
//	if ( prevTexture != texture ) {
//
//		this->GfxStateWillChange ();
//
//		DEBUG_LOG ( "  binding texture set: %p\n", textureSet );
//
//		gfx.ActiveTexture ( textureUnit );
//
//		if ( prevTexture ) {
//			prevTexture->Unbind ();
//		}
//
//		active.mTextureUnits [ textureUnit ] = texture;
//
//		if ( texture ) {
//
//			DEBUG_LOG ( "    binding texture: %d %p\n", i, bindTexture );
//			texture->Bind ();
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushVertexArray ( MOAIVertexArrayVK* vtxArray ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( active.mVtxArray != vtxArray ) {
//
//		this->GfxStateWillChange ();
//
//		DEBUG_LOG ( "  binding vertex array: %p\n", vtxArray );
//
//		if ( active.mVtxArray ) {
//			active.mVtxArray->Unbind ();
//		}
//
//		active.mVtxArray = vtxArray;
//
//		if ( vtxArray ) {
//
//			this->FlushVertexBuffer ( 0 ); // force the unbind in case it hasn't happened yet
//			vtxArray->Bind ();
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushVertexBuffer ( MOAIVertexBufferVK* buffer ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	ZLSharedConstBuffer* bufferForBind = buffer ? buffer->GetBufferForBind ( gfx ) : 0;
//
//	MOAIVertexFormatVK* format = active.mVtxFormat;
//
//	if (( active.mVtxBuffer != buffer ) || ( this->mBoundVtxBuffer != bufferForBind )) {
//
//		this->GfxStateWillChange ();
//
//		DEBUG_LOG ( "  binding vertex buffer: (%p)\n", buffer );
//
//		if ( active.mVtxBuffer ) {
//			assert ( active.mVtxFormat );
//			active.mVtxFormat->Unbind ( gfx );
//		}
//
//		active.mVtxBuffer = 0;
//		this->mBoundVtxBuffer = 0;
//
//		if ( format && buffer ) {
//
//			this->FlushVertexArray ( 0 ); // force the unbind in case it hasn't happened yet
//
//			buffer->Bind ();
//			format->Bind ( gfx, bufferForBind );
//			buffer->Unbind ();
//
//			active.mVtxBuffer = buffer;
//			this->mBoundVtxBuffer = bufferForBind;
//		}
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushVertexFormat ( MOAIVertexFormatVK* vtxFormat ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( active.mVtxFormat != vtxFormat ) {
//
//		DEBUG_LOG ( "  binding vertex format: (%p)\n", vtxFormat );
//
//		this->GfxStateWillChange ();
//
//		if ( active.mVtxFormat && active.mVtxBuffer ) {
//			active.mVtxFormat->Unbind ( gfx );
//		}
//
//		active.mVtxFormat = vtxFormat;
//		active.mVtxBuffer = 0; // must be set in a later step
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::FlushViewRect ( ZLRect rect ) {
//
//	assert ( this->mApplyingStateChanges );
//
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	if ( !active.mViewRect.IsEqual ( rect )) {
//
//		this->GfxStateWillChange ();
//
//		active.mViewRect = rect;
//
//		ZLRect rect = this->mActiveState.mFrameBuffer->WndRectToDevice ( rect );
//
//		s32 x = ( s32 )active.mViewRect.mXMin;
//		s32 y = ( s32 )active.mViewRect.mYMin;
//
//		u32 w = ( u32 )( active.mViewRect.Width () + 0.5f );
//		u32 h = ( u32 )( active.mViewRect.Height () + 0.5f );
//
//		DEBUG_LOG ( "  update view rect: %d %d %d %d\n", x, y, w, h );
//
//		gfx.Viewport ( x, y, w, h );
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::GfxStateWillChange () {
//
//	this->GetVertexCache ().FlushToGPU ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::InitTextureUnits ( size_t nTextureUnits ) {
//
//	if ( MAX_TEXTURE_UNITS < nTextureUnits ) {
//		ZLLog_Warning ( "Hardware textures units (%d) exceed Moai maximum supported texture units (%d)\n", nTextureUnits, MAX_TEXTURE_UNITS );
//		nTextureUnits = MAX_TEXTURE_UNITS;
//	}
//
//	this->mPendingState.mTextureUnits.Grow ( nTextureUnits, 0 );
//	this->mActiveState.mTextureUnits.Grow ( nTextureUnits, 0 );
//
//	this->mMaxTextureUnits = nTextureUnits;
//}
//
////----------------------------------------------------------------//
//MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgrVK_GPUCacheVK () :
//	mCurrentState ( 0 ),
//	mDirtyFlags ( 0 ),
//	mTextureDirtyFlags ( 0 ),
//	mMaxTextureUnits ( 0 ),
//	mApplyingStateChanges ( 0 ),
//	mBoundIdxBuffer ( 0 ),
//	mBoundVtxBuffer ( 0 ) {
//
//	this->mCurrentState = &this->mPendingState;
//}

//----------------------------------------------------------------//
MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgrVK_GPUCacheVK () {

	this->mDefaultFrameBuffer = new MOAIFrameBufferVK ();
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_GPUCacheVK::~MOAIGfxMgrVK_GPUCacheVK () {

	delete ( this->mDefaultFrameBuffer );
}

////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::RecalculateDirtyFlags () {
//
//	if ( this->mPendingState.mBlendEnabled ) {
//		this->SetBlendMode ( this->mPendingState.mBlendMode );
//	}
//	else {
//		this->SetBlendMode ();
//	}
//
//	this->SetCullFunc ( this->mPendingState.mCullFunc );
//	this->SetDepthFunc ( this->mPendingState.mDepthFunc );
//	this->SetDepthMask ( this->mPendingState.mDepthMask );
//	this->SetFrameBuffer ( this->mPendingState.mFrameBuffer );
//	this->SetIndexBuffer ( this->mPendingState.mIdxBuffer );
//	this->SetPenWidth ( this->mPendingState.mPenWidth );
//
//	if ( this->mPendingState.mScissorEnabled ) {
//		this->SetScissorRect ( this->mPendingState.mScissorRect );
//	}
//	else {
//		this->SetScissorRect ();
//	}
//
//	this->SetShader ( this->mPendingState.mShader );
//	this->SetVertexArray ( this->mPendingState.mVtxArray );
//	this->SetViewRect ( this->mPendingState.mViewRect );
//	this->SetVertexBuffer ( this->mPendingState.mVtxBuffer );
//	this->SetVertexFormat ( this->mPendingState.mVtxFormat );
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < MAX_TEXTURE_UNITS; ++i ) {
//		this->SetTexture ( this->mPendingState.mTextureUnits [ i ], i );
//	}
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::RestoreGPUState ( const MOAIGfxStateGPUCacheFrameVK& frame ) {
//
//	this->mPendingState = frame;
//	this->RecalculateDirtyFlags ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::ResumeChanges () {
//
//	this->mApplyingStateChanges--;
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::StoreGPUState ( MOAIGfxStateGPUCacheFrameVK& frame ) const {
//
//	if ( frame.mTextureUnits.Size () < this->mMaxTextureUnits ) {
//		frame.mTextureUnits.Grow ( this->mMaxTextureUnits, 0 );
//	}
//	frame = this->mPendingState;
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK_GPUCacheVK::SuspendChanges () {
//
//	this->mApplyingStateChanges++;
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_ClearSurface () {
	
//	MOAIGfxMgr_CPUCache& cpuCache = this->GetCPUCache ();
//
//	u32 clearFlags = cpuCache.GetClearFlags ();
//
//	if ( clearFlags ) {
//
//		ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//		// discard this if all buffers are to be cleared?
//		// (may still need to draw if depth or color only)
//		this->GetVertexCache ().FlushToGPU ();
//
//		this->ApplyStateChanges ();
//
//		if ( clearFlags & ZGL_CLEAR_COLOR_BUFFER_BIT ) {
//
//			const ZLColorVec& clearColor = cpuCache.GetClearColor ();
//
//			gfx.ClearColor (
//				clearColor.mR,
//				clearColor.mG,
//				clearColor.mB,
//				clearColor.mA
//			);
//		}
//
//		if (( clearFlags & ZGL_CLEAR_DEPTH_BUFFER_BIT ) && !this->GetDepthMask ()) {
//			gfx.DepthMask ( true );
//			gfx.Clear ( clearFlags );
//			gfx.DepthMask ( false );
//		}
//		else {
//			gfx.Clear ( clearFlags );
//		}
//	}
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_CountTextureUnits () {

//	return this->mActiveState.mTextureUnits.Size ();
	return 0;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_DrawPrims ( u32 primType, u32 base, u32 count ) {

//	DEBUG_LOG ( "DRAW PRIMS: %d %d %d\n", primType, base, count );
//
//	this->ApplyStateChanges ();
//
//	MOAIShaderVK* shader = this->mActiveState.mShader;
//
//	if ( shader && ( this->mActiveState.mVtxBuffer || this->mActiveState.mVtxArray )) {
//
//		ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//		MOAIIndexBufferVK* idxBuffer = this->mActiveState.mIdxBuffer;
//
//		if ( idxBuffer ) {
//
//			DEBUG_LOG ( "drawing prims with index and vertex buffer\n" );
//
//			size_t indexSize = idxBuffer->GetIndexSize ();
//			u32 indexType = indexSize == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT;
//			gfx.DrawElements ( primType, count, indexType, this->mBoundIdxBuffer, base * indexSize );
//		}
//		else {
//			DEBUG_LOG ( "drawing prims with vertex buffer\n" );
//			gfx.DrawArrays ( primType, base, count );
//		}
//	}
}

//----------------------------------------------------------------//
MOAIBlendMode MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetBlendMode () const {
//	this->mCurrentState->mBlendMode;
}

//----------------------------------------------------------------//
u32 MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetBufferHeight () const {

//	assert ( this->mCurrentState );
//	const MOAIFrameBufferVK* frameBuffer = this->mCurrentState->mFrameBuffer;
//	return frameBuffer ? frameBuffer->mBufferHeight : 0;
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetBufferWidth () const {

//	assert ( this->mCurrentState );
//	const MOAIFrameBufferVK* frameBuffer = this->mCurrentState->mFrameBuffer;
//	return frameBuffer ? frameBuffer->mBufferWidth : 0;
	return 0;
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetCurrentFrameBuffer () {
//	return this->mCurrentState->mFrameBuffer;
	return 0;
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetCurrentShader () {
//	return this->mCurrentState->mShader;
	return 0;
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetCurrentVtxFormat () {
//	return this->mCurrentState->mVtxFormat;
	return 0;
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetDefaultFrameBuffer () {
//	return this->mDefaultFrameBuffer;
	return this->mDefaultFrameBuffer;;
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetDefaultTexture () {
//	return this->mDefaultTexture;
	return 0;
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetDepthMask () const {
//	return this->mCurrentState->mDepthMask;
	return false;
}

//----------------------------------------------------------------//
float MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetViewHeight () const {

//	assert ( this->mCurrentState );
//	return this->mCurrentState->mViewRect.Height ();
	return 0;
}

//----------------------------------------------------------------//
ZLRect MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetViewRect () const {
//	return this->mCurrentState->mViewRect;
	return ZLRect::EMPTY;
}

//----------------------------------------------------------------//
float MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_GetViewWidth () const {

//	assert ( this->mCurrentState );
//	return this->mCurrentState->mViewRect.Width ();
	return 0;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_ResetGPUState () {

//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//	MOAIGfxStateGPUCacheFrameVK& pending = this->mPendingState;
//	MOAIGfxStateGPUCacheFrameVK& active = this->mActiveState;
//
//	ZGL_COMMENT ( gfx, "GFX RESET STATE" );
//
//	// reset the shader
//	gfx.UseProgram ( ZLGfxResource::UNBIND );
//
//	// turn off blending
//	gfx.Disable ( ZGL_PIPELINE_BLEND );
//	pending.mBlendEnabled = false;
//	active.mBlendEnabled = false;
//
//	// disable backface culling
//	gfx.Disable ( ZGL_PIPELINE_CULL );
//	pending.mCullFunc = 0;
//	active.mCullFunc = 0;
//
//	// disable depth test
//	gfx.Disable ( ZGL_PIPELINE_DEPTH );
//	pending.mDepthFunc = 0;
//	active.mDepthFunc = 0;
//
//	// disable depth write
//	gfx.DepthMask ( false );
//	pending.mDepthMask = false;
//	active.mDepthMask = 0;
//
//	// reset the pen width
//	pending.mPenWidth = 1.0f;
//	active.mPenWidth = 1.0f;
//	gfx.LineWidth ( 1.0f );
//
//	// reset the scissor rect
//	pending.mScissorEnabled = false;
//	active.mScissorEnabled = false;
//	gfx.Disable ( ZGL_PIPELINE_SCISSOR );
//
//	// TODO: seems like overkill
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mMaxTextureUnits; ++i ){
//		gfx.ActiveTexture (( u32 )i );
//		gfx.BindTexture ( ZLGfxResource::UNBIND );
//		pending.mTextureUnits [ i ] = 0;
//		active.mTextureUnits [ i ] = 0;
//	}
//	gfx.ActiveTexture ( 0 );
//
//	if ( active.mVtxArray ) {
//		active.mVtxArray->Unbind ();
//	}
//
//	pending.mShader			= 0;
//	pending.mIdxBuffer		= 0;
//	pending.mVtxArray		= 0;
//	pending.mVtxBuffer		= 0;
//	pending.mVtxFormat		= 0;
//
//	active.mShader			= 0;
//	active.mIdxBuffer		= 0;
//	active.mVtxArray		= 0;
//	active.mVtxBuffer		= 0;
//	active.mVtxFormat		= 0;
//
//	pending.mFrameBuffer = this->mDefaultFrameBuffer;
//	active.mFrameBuffer = this->mDefaultFrameBuffer;
//	gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, pending.mFrameBuffer->mGLFrameBuffer );
//
//	this->mDirtyFlags = 0;
//	this->mTextureDirtyFlags = 0;
//
//	ZGL_COMMENT ( gfx, "" );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetBlendMode () {

//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mBlendEnabled = false;
//	this->mDirtyFlags = ( !this->mActiveState.mBlendEnabled ) ? ( this->mDirtyFlags & ~BLEND_MODE ) : ( this->mDirtyFlags | BLEND_MODE );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetBlendMode ( const MOAIBlendMode& blendMode ) {

//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mBlendEnabled = true;
//	this->mPendingState.mBlendMode = blendMode;
//	this->mDirtyFlags = ( this->mActiveState.mBlendEnabled && ( this->mActiveState.mBlendMode.IsSame ( blendMode ))) ? ( this->mDirtyFlags & ~BLEND_MODE ) : ( this->mDirtyFlags | BLEND_MODE );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetBlendMode ( int srcFactor, int dstFactor, int equation ) {

//	MOAIBlendMode blendMode;
//	blendMode.SetBlend ( equation, srcFactor, dstFactor );
//
//	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetCullFunc () {

//	this->SetCullFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetCullFunc ( int cullFunc ) {

//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mCullFunc = cullFunc;
//	this->mDirtyFlags = ( this->mActiveState.mCullFunc == cullFunc ) ? ( this->mDirtyFlags & ~CULL_FUNC ) : ( this->mDirtyFlags | CULL_FUNC );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetDefaultFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

//	MOAIFrameBufferVK* frameBufferVK = MOAICast < MOAIFrameBufferVK >( frameBuffer );
//	assert ( frameBufferVK || ( frameBufferVK == NULL ));
//
//	this->mDefaultFrameBuffer = frameBufferVK;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetDefaultTexture ( MOAITexture* texture ) {

//	MOAITextureVK* textureVK = MOAICast < MOAITextureVK >( texture );
//	assert ( textureVK || ( texture == NULL ));
//
//	this->mDefaultTexture = textureVK;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetDepthFunc () {

//	this->SetDepthFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetDepthFunc ( int depthFunc ) {

//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mDepthFunc = depthFunc;
//	this->mDirtyFlags = (( this->mActiveState.mDepthFunc == depthFunc ) && ( this->mActiveState.mDepthMask == this->mPendingState.mDepthMask )) ? ( this->mDirtyFlags & ~DEPTH_MODE ) : ( this->mDirtyFlags | DEPTH_MODE );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetDepthMask ( bool depthMask ) {

//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mDepthMask = depthMask;
//	this->mDirtyFlags = (( this->mActiveState.mDepthMask == depthMask ) && ( this->mActiveState.mDepthFunc == this->mPendingState.mDepthFunc )) ? ( this->mDirtyFlags & ~DEPTH_MODE ) : ( this->mDirtyFlags | DEPTH_MODE );
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

//	MOAIFrameBufferVK* frameBufferVK = MOAICast < MOAIFrameBufferVK >( frameBuffer );
//	assert ( frameBufferVK || ( frameBufferVK == NULL ));
//
//	assert ( !this->mApplyingStateChanges );
//
//	frameBufferVK = frameBufferVK ? frameBufferVK : ( MOAIFrameBufferVK* )this->mDefaultFrameBuffer;
//	this->mPendingState.mFrameBuffer = frameBufferVK;
//	this->mDirtyFlags = ( this->mActiveState.mFrameBuffer == frameBufferVK ) ? ( this->mDirtyFlags & ~FRAME_BUFFER ) : ( this->mDirtyFlags | FRAME_BUFFER );
//
//	return true;
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetIndexBuffer ( MOAIIndexBuffer* buffer ) {
	
//	MOAIIndexBufferVK* indexBufferVK = MOAICast < MOAIIndexBufferVK >( buffer );
//	assert ( indexBufferVK || ( indexBufferVK == NULL ));
//
//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mIdxBuffer = indexBufferVK;
//	this->mDirtyFlags = ( this->mActiveState.mIdxBuffer == indexBufferVK ) ? ( this->mDirtyFlags & ~INDEX_BUFFER ) : ( this->mDirtyFlags | INDEX_BUFFER );
//
//	return indexBufferVK ? indexBufferVK->IsReady () : true;
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetPenWidth ( float penWidth ) {

//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mPenWidth = penWidth;
//	this->mDirtyFlags = ( this->mActiveState.mPenWidth == penWidth ) ? ( this->mDirtyFlags & ~PEN_WIDTH ) : ( this->mDirtyFlags | PEN_WIDTH );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetScissorRect () {

//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mScissorEnabled = false;
//	this->mDirtyFlags = ( !this->mActiveState.mScissorEnabled ) ? ( this->mDirtyFlags & ~SCISSOR_RECT ) : ( this->mDirtyFlags | SCISSOR_RECT );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetScissorRect ( ZLRect rect ) {
	
//	rect.Bless ();
//
//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mScissorEnabled = true;
//	this->mPendingState.mScissorRect = rect;
//	this->mDirtyFlags = ( this->mActiveState.mScissorEnabled && ( this->mActiveState.mScissorRect.IsEqual ( rect ))) ? ( this->mDirtyFlags & ~SCISSOR_RECT ) : ( this->mDirtyFlags | SCISSOR_RECT );
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetShader ( MOAIShader* shader ) {

//	assert ( !this->mApplyingStateChanges );
//
//	MOAIShaderVK* shaderVK = MOAICastAssert < MOAIShaderVK >( shader );
//
//	MOAIShaderProgramVK* program = shaderVK ? shaderVK->GetProgram () : 0;
//	shaderVK = program ? shaderVK : 0;
//
//	this->mPendingState.mShader = shaderVK;
//
//	// shader dirty flag only ceared if both are null
//	this->mDirtyFlags = ( !( shaderVK || this->mActiveState.mShader )) ? ( this->mDirtyFlags & ~SHADER ) : ( this->mDirtyFlags | SHADER );
//
//	return program ? program->IsReady () : true;
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetTexture ( MOAITexture* texture, ZLIndex textureUnit ) {

//	assert ( !this->mApplyingStateChanges );
//
//	MOAITextureVK* textureVK = MOAICastAssert < MOAITextureVK >( texture );
//
//	u32 mask = 1 << textureUnit;
//	this->mPendingState.mTextureUnits [ textureUnit ] = textureVK;
//	if ( this->mActiveState.mTextureUnits [ textureUnit ] == textureVK ) {
//		this->mTextureDirtyFlags = this->mTextureDirtyFlags & ~mask;
//	}
//	else {
//		this->mTextureDirtyFlags = this->mTextureDirtyFlags | mask;
//	}
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetVertexArray ( MOAIVertexArray* vtxArray ) {

//	assert ( !this->mApplyingStateChanges );
//
//	MOAIVertexArrayVK* vertexArrayVK = MOAICastAssert < MOAIVertexArrayVK >( vtxArray );
//
//	if ( vtxArray ) {
//		this->mPendingState.mVtxBuffer = 0;
//		this->mPendingState.mVtxFormat = 0;
//		this->mDirtyFlags &= ~VERTEX_BUFFER;
//	}
//
//	this->mPendingState.mVtxArray = vertexArrayVK;
//	this->mDirtyFlags = ( this->mActiveState.mVtxArray == vertexArrayVK ) ? ( this->mDirtyFlags & ~VERTEX_ARRAY ) : ( this->mDirtyFlags | VERTEX_ARRAY );
//
//	return vertexArrayVK ? vertexArrayVK->IsReady () : true;

	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetVertexBuffer ( MOAIVertexBuffer* buffer ) {

//	MOAIVertexBufferVK* vertexBufferVK = MOAICast < MOAIVertexBufferVK >( buffer );
//	assert ( vertexBufferVK || ( vertexBufferVK == NULL ));
//
//	assert ( !this->mApplyingStateChanges );
//
//	if ( buffer ) {
//		this->mPendingState.mVtxArray = 0;
//		this->mDirtyFlags &= ~VERTEX_ARRAY;
//	}
//
//	this->mPendingState.mVtxBuffer = vertexBufferVK;
//	this->mDirtyFlags = ( this->mActiveState.mVtxBuffer == vertexBufferVK ) ? ( this->mDirtyFlags & ~VERTEX_BUFFER ) : ( this->mDirtyFlags | VERTEX_BUFFER );
//
//	return vertexBufferVK ? vertexBufferVK->IsReady () : true;
	
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetVertexFormat ( MOAIVertexFormat* format ) {

//	MOAIVertexFormatVK* vertexFormatVK = MOAICast < MOAIVertexFormatVK >( format );
//	assert ( vertexFormatVK || ( vertexFormatVK == NULL ));
//
//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mVtxFormat = vertexFormatVK;
//	this->mDirtyFlags = ( this->mActiveState.mVtxFormat == vertexFormatVK ) ? ( this->mDirtyFlags & ~VERTEX_FORMAT ) : ( this->mDirtyFlags | VERTEX_FORMAT );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetViewRect () {

//	float width = ( float )this->GetBufferWidth ();
//	float height = ( float )this->GetBufferHeight ();
//
//	ZLRect rect;
//	rect.Init ( 0.0f, 0.0f, width, height );
//
//	this->SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_SetViewRect ( ZLRect rect ) {
	
//	rect.Bless ();
//
//	assert ( !this->mApplyingStateChanges );
//
//	this->mPendingState.mViewRect = rect;
//	this->mDirtyFlags = ( this->mActiveState.mViewRect.IsEqual ( rect )) ? ( this->mDirtyFlags & ~VIEW_RECT ) : ( this->mDirtyFlags | VIEW_RECT );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_UnbindAll () {

//	ZLGfx& gfx = this->GetPipelineClerkVK ().GetDrawingAPI ();
//
//	ZGL_COMMENT ( gfx, "GFX UNBIND ALL" );
//
//	this->SetFrameBuffer ();
//	this->SetIndexBuffer ();
//	this->SetShader ();
//	this->SetTexture ();
//	this->SetVertexArray ();
//	this->SetVertexBuffer ();
//	this->SetVertexFormat ();
//	
//	ZGL_COMMENT ( gfx, "" );
}

