// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>

//#define MOAIGFXSTATECACHE_DEBUG_LOG

#ifdef MOAIGFXSTATECACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// MOAIVertexBufferWithFormat
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBufferWithFormat::MOAIVertexBufferWithFormat () :
	mBoundVtxBuffer ( 0 ),
	mIsBound ( false ) {
}

//----------------------------------------------------------------//
MOAIVertexBufferWithFormat::~MOAIVertexBufferWithFormat () {
}

//----------------------------------------------------------------//
void MOAIVertexBufferWithFormat::SetBufferAndFormat ( MOAILuaObject& owner, MOAIVertexBuffer* buffer, MOAIVertexFormat* format ) {

	this->mBuffer.Set ( owner, buffer );
	this->mFormat.Set ( owner, format );
}

//================================================================//
// MOAIGfxStateCacheClient
//================================================================//

//----------------------------------------------------------------//
MOAIGfxStateCacheClient::MOAIGfxStateCacheClient () {
}

//----------------------------------------------------------------//
MOAIGfxStateCacheClient::~MOAIGfxStateCacheClient () {
}

//----------------------------------------------------------------//
void MOAIGfxStateCacheClient::OnGfxStateWillChange () {
}

//================================================================//
// MOAIGfxState
//================================================================//

//----------------------------------------------------------------//
MOAIGfxState::MOAIGfxState () :
	mCullFunc ( 0 ),
	mDepthFunc ( 0 ),
	mDepthMask ( true ),
	mBlendEnabled ( 0 ),
	mPenWidth ( 1.0f ),
	mScissorEnabled ( false ) {
		
	this->mScissorRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mViewRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGfxState::~MOAIGfxState () {
}

//================================================================//
// MOAIGfxStateCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxStateCache::ApplyStateChange ( u32 stateID ) {
	
	switch ( stateID ) {

		case BLEND_MODE:
			
			if ( this->mPendingState.mBlendEnabled ) {
				this->SetBlendMode ( this->mPendingState.mBlendMode );
			}
			else {
				this->SetBlendMode ();
			}
			break;
		
		case CULL_FUNC:
			
			this->SetCullFunc ( this->mPendingState.mCullFunc );
			break;
		
		case DEPTH_MODE:
			
			this->SetDepthFunc ( this->mPendingState.mDepthFunc );
			this->SetDepthMask ( this->mPendingState.mDepthMask );
			break;

		case FRAME_BUFFER:
			
			this->SetFrameBuffer ( this->mPendingState.mFrameBuffer );
			break;
		
		case INDEX_BUFFER:
		
			this->SetIndexBuffer ( this->mPendingState.mIdxBuffer );
			break;
		
		case PEN_WIDTH:
		
			this->SetPenWidth ( this->mPendingState.mPenWidth );
			break;
		
		case SCISSOR_RECT:
	
			if ( this->mPendingState.mScissorEnabled ) {
				this->SetScissorRect ( this->mPendingState.mScissorRect );
			}
			else {
				this->SetScissorRect ();
			}
			break;
		
		case SHADER:
		
			this->SetShader ( this->mPendingState.mShader );
			break;
		
		case VERTEX_ARRAY:
		
			this->SetVertexArray ( this->mPendingState.mVtxArray );
			break;
		
		case VERTEX_BUFFER:
			
			this->mActiveState.mVtxFormat = this->mPendingState.mVtxFormat;
			this->SetVertexBuffer ( this->mPendingState.mVtxBuffer );
			break;
		
		case VERTEX_FORMAT:
			
			this->SetVertexFormat ( this->mPendingState.mVtxFormat );
			break;

		case VIEW_RECT:
		
			this->SetViewRect ( this->mPendingState.mViewRect );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::ApplyStateChanges () {

	if (( this->mDirtyFlags || this->mTextureDirtyFlags ) && ( !this->mApplyingStateChanges )) {
		
		this->SuspendChanges ();
		this->mCurrentState = &this->mActiveState;
		
		u32 dirtyFlags = this->mDirtyFlags;
		this->mDirtyFlags = 0;
		
		u32 textureDirtyFlags = this->mTextureDirtyFlags;
		this->mTextureDirtyFlags = 0;
		
		DEBUG_LOG ( "APPLY STATE CHANGES\n" );

		// TODO: use recursion here to a depth-first pass through the
		// flags as opposed to iterating over every one individually

		if ( dirtyFlags ) {
			for ( u32 i = 1; i < END_STATE_FLAGS; i <<= 1 ) {
				if ( dirtyFlags & i ) {
					this->ApplyStateChange ( i );
				}
			}
		}
		
		if ( textureDirtyFlags ) {
			for ( u32 i = 0; i < this->mTopDirtyTexture; ++i ) {
				if ( textureDirtyFlags & ( 1 << i )) {
					this->SetTexture ( this->mPendingState.mTextureUnits [ i ], i );
				}
			}
		}
		
		this->mCurrentState = &this->mPendingState;
		this->ResumeChanges ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::BindVertexBufferWithFormat ( MOAIVertexBufferWithFormat& bufferWithFormat, bool useVAOs ) {

	UNUSED(useVAOs);

	MOAIVertexBuffer* buffer = bufferWithFormat.mBuffer;
	MOAIVertexFormat* format = bufferWithFormat.mFormat;
	
	if ( buffer && format ) {
	
		assert ( !bufferWithFormat.mIsBound );
		assert (( useVAOs && buffer->IsUsingVBOs ()) || ( !useVAOs )); // buffer objects must use VBOs to work with VAOs
		
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		ZLSharedConstBuffer* bufferForBind = buffer->GetBufferForBind ( gfx );
		
		buffer->Bind ();
		format->Bind ( bufferForBind );
		buffer->Unbind ();
		
		bufferWithFormat.mBoundVtxBuffer = bufferForBind;
		bufferWithFormat.mIsBound = true;
	}
	else {
		assert ( false );
	}
}

//----------------------------------------------------------------//
size_t MOAIGfxStateCache::CountTextureUnits () {

	return this->mActiveState.mTextureUnits.Size ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::DrawPrims ( u32 primType, u32 base, u32 count ) {

	DEBUG_LOG ( "DRAW PRIMS: %d %d %d\n", primType, base, count );
	
	this->ApplyStateChanges ();

	MOAIShader* shader = this->mActiveState.mShader;

	if ( shader && ( this->mActiveState.mVtxBuffer || this->mActiveState.mVtxArray )) {
		
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		MOAIIndexBuffer* idxBuffer = this->mActiveState.mIdxBuffer;
		
		if ( idxBuffer ) {
		
			DEBUG_LOG ( "drawing prims with index and vertex buffer\n" );
			
			size_t indexSize = idxBuffer->GetIndexSize ();
			u32 indexType = indexSize == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT;
			gfx.DrawElements ( primType, count, indexType, this->mBoundIdxBuffer, base * indexSize );
		}
		else {
			DEBUG_LOG ( "drawing prims with vertex buffer\n" );
			gfx.DrawArrays ( primType, base, count );
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIGfxStateCache::GetBufferHeight () const {

	assert ( this->mCurrentState );
	const MOAIFrameBuffer* frameBuffer = this->mCurrentState->mFrameBuffer;
	return frameBuffer ? frameBuffer->mBufferHeight : 0;
}

//----------------------------------------------------------------//
u32 MOAIGfxStateCache::GetBufferWidth () const {

	assert ( this->mCurrentState );
	const MOAIFrameBuffer* frameBuffer = this->mCurrentState->mFrameBuffer;
	return frameBuffer ? frameBuffer->mBufferWidth : 0;
}

//----------------------------------------------------------------//
//float MOAIGfxStateCache::GetDeviceScale () {
//
//	assert ( this->mFrameBuffer );
//	return this->mFrameBuffer->mBufferScale;
//}

//----------------------------------------------------------------//
float MOAIGfxStateCache::GetViewHeight () const {

	assert ( this->mCurrentState );
	return this->mCurrentState->mViewRect.Height ();
}

//----------------------------------------------------------------//
//ZLQuad MOAIGfxStateCache::GetViewQuad () const {
//
//	ZLQuad quad;
//
//	ZLMatrix4x4 invMtx;
//	invMtx.Inverse ( this->GetViewProjMtx ());
//	
//	quad.mV [ 0 ].Init ( -1.0f, 1.0f );
//	quad.mV [ 1 ].Init ( 1.0f, 1.0f );
//	quad.mV [ 2 ].Init ( 1.0f, -1.0f );
//	quad.mV [ 3 ].Init ( -1.0f, -1.0f );
//	
//	invMtx.TransformQuad ( quad.mV );
//	return quad;
//}

//----------------------------------------------------------------//
//ZLRect MOAIGfxStateCache::GetViewRect () const {
//
//	return this->mViewRect;
//}

//----------------------------------------------------------------//
float MOAIGfxStateCache::GetViewWidth () const {

	assert ( this->mCurrentState );
	return this->mCurrentState->mViewRect.Width ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::InitTextureUnits ( size_t nTextureUnits ) {

	if ( MAX_TEXTURE_UNITS < nTextureUnits ) {
		ZLLog_Warning ( "Hardware textures units (%d) exceed Moai maximum supported texture units (%d)\n", nTextureUnits, MAX_TEXTURE_UNITS );
		nTextureUnits = MAX_TEXTURE_UNITS;
	}

	this->mPendingState.mTextureUnits.Init ( nTextureUnits );
	this->mPendingState.mTextureUnits.Fill ( 0 );
	
	this->mActiveState.mTextureUnits.Init ( nTextureUnits );
	this->mActiveState.mTextureUnits.Fill ( 0 );
	
	this->mMaxTextureUnits = nTextureUnits;
}

//----------------------------------------------------------------//
MOAIGfxStateCache::MOAIGfxStateCache () :
	mCurrentState ( 0 ),
	mDirtyFlags ( 0 ),
	mTextureDirtyFlags ( 0 ),
	mTopDirtyTexture ( 0 ),
	mMaxTextureUnits ( 0 ),
	mApplyingStateChanges ( 0 ),
	mClient ( 0 ),
	mBoundIdxBuffer ( 0 ),
	mBoundVtxBuffer ( 0 ) {
	
	this->mCurrentState = &this->mPendingState;
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGfxStateCache::~MOAIGfxStateCache () {
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::ResetState () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	MOAIGfxState& pending = this->mPendingState;
	MOAIGfxState& active = this->mActiveState;

	ZGL_COMMENT ( gfx, "GFX RESET STATE" );

	// reset the shader
	gfx.UseProgram ( ZLGfxResource::UNBIND );
	
	// turn off blending
	gfx.Disable ( ZGL_PIPELINE_BLEND );
	pending.mBlendEnabled = false;
	active.mBlendEnabled = false;
	
	// disable backface culling
	gfx.Disable ( ZGL_PIPELINE_CULL );
	pending.mCullFunc = 0;
	active.mCullFunc = 0;
	
	// disable depth test
	gfx.Disable ( ZGL_PIPELINE_DEPTH );
	pending.mDepthFunc = 0;
	active.mDepthFunc = 0;
	
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
	gfx.Disable ( ZGL_PIPELINE_SCISSOR );
	
	// TODO: seems like overkill
	for ( size_t i = 0; i < this->mMaxTextureUnits; ++i ){
		gfx.ActiveTexture (( u32 )i );
		gfx.BindTexture ( ZLGfxResource::UNBIND );
		pending.mTextureUnits [ i ] = 0;
		active.mTextureUnits [ i ] = 0;
	}
	gfx.ActiveTexture ( 0 );
	
	if ( active.mVtxArray ) {
		active.mVtxArray->Unbind ();
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
	gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, pending.mFrameBuffer->mGLFrameBuffer );
	
	this->mDirtyFlags = 0;
	this->mTextureDirtyFlags = 0;
	this->mTopDirtyTexture = 0;
	
	ZGL_COMMENT ( gfx, "" );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::ResumeChanges () {

	this->mApplyingStateChanges--;
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetBlendMode () {

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
		
		if ( active.mBlendEnabled ) {
			
			this->GfxStateWillChange ();

			DEBUG_LOG ( "  disabling blend\n" );
			MOAIGfxMgr::GetDrawingAPI ().Disable ( ZGL_PIPELINE_BLEND );
			active.mBlendEnabled = false;
		}
	}
	else {

		this->mPendingState.mBlendEnabled = false;
		this->mDirtyFlags = ( !this->mActiveState.mBlendEnabled ) ? ( this->mDirtyFlags & ~BLEND_MODE ) : ( this->mDirtyFlags | BLEND_MODE );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		bool updateBlend = false;
		
		if ( !active.mBlendEnabled ) {
		
			this->GfxStateWillChange ();
		
			DEBUG_LOG ( "  enabling blend\n" );
			gfx.Enable ( ZGL_PIPELINE_BLEND );
			active.mBlendEnabled = true;
			updateBlend = true;
		}
		
		if ( updateBlend || !active.mBlendMode.IsSame ( blendMode )) {
		
			this->GfxStateWillChange ();
			
			DEBUG_LOG ( "  updating blend mode\n" );
			active.mBlendMode = blendMode;
			gfx.BlendMode ( active.mBlendMode.mEquation );
			gfx.BlendFunc ( active.mBlendMode.mSourceFactor, active.mBlendMode.mDestFactor );
		}
	}
	else {
	
		this->mPendingState.mBlendEnabled = true;
		this->mPendingState.mBlendMode = blendMode;
		this->mDirtyFlags = ( this->mActiveState.mBlendEnabled && ( this->mActiveState.mBlendMode.IsSame ( blendMode ))) ? ( this->mDirtyFlags & ~BLEND_MODE ) : ( this->mDirtyFlags | BLEND_MODE );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( equation, srcFactor, dstFactor );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetClient () {

	this->SetClient ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetClient ( MOAIGfxStateCacheClient* client ) {

	if ( this->mClient != client ) {
	
		this->GfxStateWillChange ();
		this->mClient = client;
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetCullFunc () {

	this->SetCullFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetCullFunc ( int cullFunc ) {

	if ( this->mApplyingStateChanges ) {

		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		if ( active.mCullFunc != cullFunc ) {
	
			this->GfxStateWillChange ();
			
			active.mCullFunc = cullFunc;
			
			if ( active.mCullFunc ) {
				DEBUG_LOG ( "  enabling/setting cull func\n" );
				gfx.Enable ( ZGL_PIPELINE_CULL );
				gfx.CullFace ( active.mCullFunc );
			}
			else {
				DEBUG_LOG ( "  disabling cull func\n" );
				gfx.Disable ( ZGL_PIPELINE_CULL );
			}
		}
	}
	else {
	
		this->mPendingState.mCullFunc = cullFunc;
		this->mDirtyFlags = ( this->mActiveState.mCullFunc == cullFunc ) ? ( this->mDirtyFlags & ~CULL_FUNC ) : ( this->mDirtyFlags | CULL_FUNC );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetDefaultFrameBuffer ( MOAILuaObject& owner, MOAIFrameBuffer* frameBuffer ) {

	this->mDefaultFrameBuffer.Set ( owner, frameBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetDefaultTexture ( MOAILuaObject& owner, MOAITextureBase* texture ) {

	this->mDefaultTexture.Set ( owner, texture );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetDepthFunc () {

	this->SetDepthFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetDepthFunc ( int depthFunc ) {

	if ( this->mApplyingStateChanges ) {

		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

		if ( active.mDepthFunc != depthFunc ) {
		
			this->GfxStateWillChange ();
		
			DEBUG_LOG ( "  setting depth func: %d\n", depthFunc );
			
			active.mDepthFunc = depthFunc;
			
			if ( active.mDepthFunc ) {
				gfx.Enable ( ZGL_PIPELINE_DEPTH );
				gfx.DepthFunc ( active.mDepthFunc );
			}
			else {
				gfx.Disable ( ZGL_PIPELINE_DEPTH );
			}
		}
	}
	else {
	
		this->mPendingState.mDepthFunc = depthFunc;
		this->mDirtyFlags = (( this->mActiveState.mDepthFunc == depthFunc ) && ( this->mActiveState.mDepthMask == this->mPendingState.mDepthMask )) ? ( this->mDirtyFlags & ~DEPTH_MODE ) : ( this->mDirtyFlags | DEPTH_MODE );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetDepthMask ( bool depthMask ) {

	if ( this->mApplyingStateChanges ) {

		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

		if ( active.mDepthMask != depthMask ) {
		
			this->GfxStateWillChange ();
			
			DEBUG_LOG ( "  setting depth mask: %s\n", depthMask ? "true" : "false" );
			active.mDepthMask = depthMask;
			gfx.DepthMask ( active.mDepthMask );
		}
	}
	else {

		this->mPendingState.mDepthMask = depthMask;
		this->mDirtyFlags = (( this->mActiveState.mDepthMask == depthMask ) && ( this->mActiveState.mDepthFunc == this->mPendingState.mDepthFunc )) ? ( this->mDirtyFlags & ~DEPTH_MODE ) : ( this->mDirtyFlags | DEPTH_MODE );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;

		if ( active.mFrameBuffer != frameBuffer ) {
			
			DEBUG_LOG ( "  binding frame buffer: %p\n", frameBuffer );
			
			this->GfxStateWillChange ();
			
			MOAIGfxMgr::GetDrawingAPI ().BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBuffer );
			active.mFrameBuffer = frameBuffer;
		}
	}
	else {

		frameBuffer = frameBuffer ? frameBuffer : this->mDefaultFrameBuffer;
		this->mPendingState.mFrameBuffer = frameBuffer;
		this->mDirtyFlags = ( this->mActiveState.mFrameBuffer == frameBuffer ) ? ( this->mDirtyFlags & ~FRAME_BUFFER ) : ( this->mDirtyFlags | FRAME_BUFFER );
	}
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::SetIndexBuffer ( MOAIIndexBuffer* buffer ) {
	
	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		ZLSharedConstBuffer* bufferForBind = buffer ? buffer->GetBufferForBind ( gfx ) : 0;
	
		if (( active.mIdxBuffer != buffer ) || ( this->mBoundIdxBuffer != bufferForBind )) {
		
			this->GfxStateWillChange ();
		
			DEBUG_LOG ( "  binding index buffer: %p\n", buffer );
			
			if ( active.mIdxBuffer ) {
				active.mIdxBuffer->Unbind ();
			}
			
			active.mIdxBuffer = buffer;
			this->mBoundIdxBuffer = 0;
			
			if ( buffer ) {
				buffer->Bind ();
				this->mBoundIdxBuffer = bufferForBind;
			}
		}
	}
	else {
	
		this->mPendingState.mIdxBuffer = buffer;
		this->mDirtyFlags = ( this->mActiveState.mIdxBuffer == buffer ) ? ( this->mDirtyFlags & ~INDEX_BUFFER ) : ( this->mDirtyFlags | INDEX_BUFFER );
	}

	return buffer ? buffer->IsReady () : true;
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetPenWidth ( float penWidth ) {

	if ( this->mApplyingStateChanges ) {

		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

		if ( active.mPenWidth != penWidth ) {
			
			DEBUG_LOG ( "  setting pen width: %f\n", penWidth );
		
			this->GfxStateWillChange ();
			
			active.mPenWidth = penWidth;
			gfx.LineWidth ( active.mPenWidth );
		}
	}
	else {

		this->mPendingState.mPenWidth = penWidth;
		this->mDirtyFlags = ( this->mActiveState.mPenWidth == penWidth ) ? ( this->mDirtyFlags & ~PEN_WIDTH ) : ( this->mDirtyFlags | PEN_WIDTH );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetScissorRect () {

	if ( this->mApplyingStateChanges ) {

		MOAIGfxState& active = this->mActiveState;
		
		if ( active.mScissorEnabled == true ) {

			this->GfxStateWillChange ();

			DEBUG_LOG ( "  disabling scissor rect\n" );
			MOAIGfxMgr::GetDrawingAPI ().Disable ( ZGL_PIPELINE_SCISSOR );
			active.mScissorEnabled = false;
		}
	}
	else {

		this->mPendingState.mScissorEnabled = false;
		this->mDirtyFlags = ( !this->mActiveState.mScissorEnabled ) ? ( this->mDirtyFlags & ~SCISSOR_RECT ) : ( this->mDirtyFlags | SCISSOR_RECT );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetScissorRect ( ZLRect rect ) {
	
	rect.Bless ();
	
	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		bool updateRect = false;
		
		if ( !active.mScissorEnabled ) {
		
			this->GfxStateWillChange ();
		
			DEBUG_LOG ( "  enabling scissor rect\n" );
			gfx.Enable ( ZGL_PIPELINE_SCISSOR );
			active.mScissorEnabled = true;
			updateRect = true;
		}
		
		if ( updateRect || !active.mScissorRect.IsEqual ( rect )) {
		
			this->GfxStateWillChange ();
			
			active.mScissorRect = rect;
		
			rect = active.mFrameBuffer->WndRectToDevice ( rect );
		
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
	
		this->mPendingState.mScissorEnabled = true;
		this->mPendingState.mScissorRect = rect;
		this->mDirtyFlags = ( this->mActiveState.mScissorEnabled && ( this->mActiveState.mScissorRect.IsEqual ( rect ))) ? ( this->mDirtyFlags & ~SCISSOR_RECT ) : ( this->mDirtyFlags | SCISSOR_RECT );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::SetShader ( MOAIShaderMgr::Preset preset ) {

	return this->SetShader ( MOAIShaderMgr::Get ().GetShader ( preset ));
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::SetShader ( MOAIShader* shader ) {

	MOAIShaderProgram* program = shader ? shader->GetProgram () : 0;
	shader = program ? shader : 0;

	if ( this->mApplyingStateChanges ) {
		
		MOAIGfxState& active = this->mActiveState;
		
		bool applyUniforms = ( shader && shader->IsDirty ());
		bool changeShader = ( shader != this->mActiveState.mShader );
		
		if ( applyUniforms || changeShader ) {
		
			this->GfxStateWillChange ();

			DEBUG_LOG ( "  binding shader program: %p\n", program );
			
			if ( changeShader ) {
			
				if ( active.mShader ) {
					active.mShader->GetProgram ()->Unbind ();
				}
				if ( shader ) {
					program->Bind ();
				}
				active.mShader = shader;
			}
			
			if ( applyUniforms ) {
				shader->ApplyUniforms ();
				shader->BindUniforms ();
			}
		}
	}
	else {
		
		bool isClean = ( this->mActiveState.mShader == shader );
		
		if ( shader ) {
			shader->ScheduleTextures ();
			shader->UpdateUniforms ();
			
			if ( isClean ) {
				isClean = !shader->IsDirty ();
			}
		}
		
		this->mPendingState.mShader = shader;
		this->mDirtyFlags = ( isClean ) ? ( this->mDirtyFlags & ~SHADER ) : ( this->mDirtyFlags | SHADER );
	}
	
	return program ? program->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::SetTexture ( MOAITextureBase* texture, u32 textureUnit ) {

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
	
		texture = texture && texture->IsReady () ? texture : this->mDefaultTexture;
		MOAITextureBase* prevTexture = active.mTextureUnits [ textureUnit ];
	
		if ( prevTexture != texture ) {

			ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

			this->GfxStateWillChange ();

			DEBUG_LOG ( "  binding texture set: %p\n", textureSet );

			gfx.ActiveTexture ( textureUnit );

			if ( prevTexture ) {
				prevTexture->Unbind ();
			}
			
			active.mTextureUnits [ textureUnit ] = texture;
			
			if ( texture ) {
			
				DEBUG_LOG ( "    binding texture: %d %p\n", i, bindTexture );
				texture->Bind ();
			}

			gfx.ActiveTexture ( 0 );
		}
	}
	else {

		u32 mask = 1 << textureUnit;
		this->mPendingState.mTextureUnits [ textureUnit ] = texture;
		if ( this->mActiveState.mTextureUnits [ textureUnit ] == texture ) {
		
			this->mTextureDirtyFlags = this->mTextureDirtyFlags & ~mask;
		}
		else {
			this->mTextureDirtyFlags = this->mTextureDirtyFlags | mask;
			this->mTopDirtyTexture = textureUnit + 1;
		}
	}
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::SetVertexArray ( MOAIVertexArray* vtxArray ) {

	if ( this->mApplyingStateChanges ) {
		
		MOAIGfxState& active = this->mActiveState;
		
		if ( active.mVtxArray != vtxArray ) {
	
			this->GfxStateWillChange ();
	
			DEBUG_LOG ( "  binding vertex array: %p\n", vtxArray );
	
			if ( active.mVtxArray ) {
				active.mVtxArray->Unbind ();
			}
			
			active.mVtxArray = vtxArray;
			
			if ( vtxArray ) {
								
				this->SetVertexBuffer (); // force the unbind in case it hasn't happened yet
			
				vtxArray->Bind ();
			}
		}
	}
	else {

		if ( vtxArray ) {
			this->mPendingState.mVtxBuffer = 0;
			this->mPendingState.mVtxFormat = 0;
			this->mDirtyFlags &= ~VERTEX_BUFFER;
		}

		this->mPendingState.mVtxArray = vtxArray;
		this->mDirtyFlags = ( this->mActiveState.mVtxArray == vtxArray ) ? ( this->mDirtyFlags & ~VERTEX_ARRAY ) : ( this->mDirtyFlags | VERTEX_ARRAY );
	}
	return vtxArray ? vtxArray->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::SetVertexBuffer ( MOAIVertexBuffer* buffer ) {

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
		
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		ZLSharedConstBuffer* bufferForBind = buffer ? buffer->GetBufferForBind ( gfx ) : 0;
		
		MOAIVertexFormat* format = active.mVtxFormat;
		
		if (( active.mVtxBuffer != buffer ) || ( this->mBoundVtxBuffer != bufferForBind )) {

			this->GfxStateWillChange ();

			DEBUG_LOG ( "  binding vertex buffer: (%p)\n", buffer );

			if ( active.mVtxBuffer ) {
				assert ( active.mVtxFormat );
				active.mVtxFormat->Unbind ();
			}
			
			active.mVtxBuffer = 0;
			this->mBoundVtxBuffer = 0;
			
			if ( format && buffer ) {
				
				this->SetVertexArray (); // force the unbind in case it hasn't happened yet
				
				buffer->Bind ();
				format->Bind ( bufferForBind );
				buffer->Unbind ();
				
				active.mVtxBuffer = buffer;
				this->mBoundVtxBuffer = bufferForBind;
			}
		}
	}
	else {
	
		if ( buffer ) {
			this->mPendingState.mVtxArray = 0;
			this->mDirtyFlags &= ~VERTEX_ARRAY;
		}
	
		this->mPendingState.mVtxBuffer = buffer;
		this->mDirtyFlags = ( this->mActiveState.mVtxBuffer == buffer ) ? ( this->mDirtyFlags & ~VERTEX_BUFFER ) : ( this->mDirtyFlags | VERTEX_BUFFER );
	}
	
	return buffer ? buffer->IsReady () : true;
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetVertexFormat ( MOAIVertexFormatMgr::Preset preset ) {

	this->SetVertexFormat ( MOAIVertexFormatMgr::Get ().GetFormat ( preset ));
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetVertexFormat ( MOAIVertexFormat* format ) {

	if ( this->mApplyingStateChanges ) {
		
		MOAIGfxState& active = this->mActiveState;
		
		if ( active.mVtxFormat != format ) {
			
			DEBUG_LOG ( "  binding vertex format: (%p)\n", format );
		
			this->GfxStateWillChange ();
			
			if ( active.mVtxFormat && active.mVtxBuffer ) {
				active.mVtxFormat->Unbind ();
			}
			
			active.mVtxFormat = format;
			active.mVtxBuffer = 0; // must be set in a later step
		}
	}
	else {

		this->mPendingState.mVtxFormat = format;
		this->mDirtyFlags = ( this->mActiveState.mVtxFormat == format ) ? ( this->mDirtyFlags & ~VERTEX_FORMAT ) : ( this->mDirtyFlags | VERTEX_FORMAT );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetViewRect () {

	float width = ( float )this->GetBufferWidth ();
	float height = ( float )this->GetBufferHeight ();

	MOAIViewport rect;
	rect.Init ( 0.0f, 0.0f, width, height );
	
	this->SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetViewRect ( ZLRect rect ) {
	
	rect.Bless ();
	
	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
		if ( !active.mViewRect.IsEqual ( rect )) {
			
			this->GfxStateWillChange ();
			
			active.mViewRect = rect;
	
			rect = this->mActiveState.mFrameBuffer->WndRectToDevice ( rect );
	
			s32 x = ( s32 )active.mViewRect.mXMin;
			s32 y = ( s32 )active.mViewRect.mYMin;
			
			u32 w = ( u32 )( active.mViewRect.Width () + 0.5f );
			u32 h = ( u32 )( active.mViewRect.Height () + 0.5f );
			
			DEBUG_LOG ( "  update view rect: %d %d %d %d\n", x, y, w, h );
			
			gfx.Viewport ( x, y, w, h );
		}
	}
	else {
	
		this->mPendingState.mViewRect = rect;
		this->mDirtyFlags = ( this->mActiveState.mViewRect.IsEqual ( rect )) ? ( this->mDirtyFlags & ~VIEW_RECT ) : ( this->mDirtyFlags | VIEW_RECT );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SuspendChanges () {

	this->mApplyingStateChanges++;
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::UnbindAll () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

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

//----------------------------------------------------------------//
void MOAIGfxStateCache::UnbindVertexBufferWithFormat ( MOAIVertexBufferWithFormat& bufferWithFormat ) {

	assert ( bufferWithFormat.mIsBound );
	assert ( bufferWithFormat.mBuffer && bufferWithFormat.mFormat );
		
	bufferWithFormat.mFormat->Unbind ();
	bufferWithFormat.mBoundVtxBuffer = 0;
		
	bufferWithFormat.mIsBound = false;
}
