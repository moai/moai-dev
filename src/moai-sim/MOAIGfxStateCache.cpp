// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
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

//#define MOAIGFXSTATECACHE_DEBUG_LOG

#ifdef MOAIGFXSTATECACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

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

	MOAIGfxState& active = this->mActiveState;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
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
		
			this->BindFrameBuffer ( this->mPendingState.mFrameBuffer );
			break;
		
		case INDEX_BUFFER:
		
			this->BindIndexBuffer ( this->mPendingState.mIdxBuffer );
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
		
			this->BindShader ( this->mPendingState.mShader );
			break;
		
		case TEXTURE:
		
			this->BindTexture ( this->mPendingState.mTextureSet );
			break;
		
		case VERTEX_ARRAY:
		
			this->BindVertexArray ( this->mPendingState.mVtxArray );
			break;
		
		case VERTEX_BUFFER:
			
			this->BindVertexBuffer ( this->mPendingState.mVtxBuffer );
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

	if ( this->mDirtyFlags && ( !this->mApplyingStateChanges )) {
		
		this->SuspendChanges ();
		this->mCurrentState = &this->mActiveState;
		
		u32 dirtyFlags = this->mDirtyFlags;
		this->mDirtyFlags = 0;
		
		DEBUG_LOG ( "APPLY STATE CHANGES\n" );

		for ( u32 i = 1; i < END_STATE_FLAGS; i <<= 1 ) {
			if ( dirtyFlags & i ) {
				this->ApplyStateChange ( i );
			}
		}
		
		this->mCurrentState = &this->mPendingState;
		this->ResumeChanges ();
	}
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;

		if ( active.mFrameBuffer != frameBuffer ) {
			
			DEBUG_LOG ( "  binding frame buffer: %p\n", frameBuffer );
			
			this->GfxStateWillChange ();
			
			MOAIGfxMgr::GetDrawingAPI ().BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBufferID );
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
bool MOAIGfxStateCache::BindIndexBuffer ( MOAIIndexBuffer* buffer ) {
	
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
bool MOAIGfxStateCache::BindShader ( MOAIShaderMgr::Preset preset ) {

	return this->BindShader ( MOAIShaderMgr::Get ().GetShader ( preset ));
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindShader ( MOAIShader* shader ) {

	MOAIShaderProgram* program = shader ? shader->GetProgram () : 0;

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
	
		if ( active.mShaderProgram != program ) {

			DEBUG_LOG ( "  binding shader program: %p\n", program );

			this->GfxStateWillChange ();
			
			if ( active.mShaderProgram ) {
				active.mShaderProgram->Unbind ();
			}
			
			active.mShader = shader;
			active.mShaderProgram = program;
			
			if ( program ) {
				program->Bind ();
			}
		}
	}
	else {
	
		this->mPendingState.mShader = shader;
		this->mPendingState.mShaderProgram = program;
		this->mDirtyFlags = ( this->mActiveState.mShaderProgram == program ) ? ( this->mDirtyFlags & ~SHADER ) : ( this->mDirtyFlags | SHADER );
	}
	
	return program ? program->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindTexture ( MOAITextureBase* textureSet ) {

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
		if ( active.mTextureSet != textureSet ) {

			this->GfxStateWillChange ();

			DEBUG_LOG ( "  binding texture set: %p\n", textureSet );

			active.mTextureSet = textureSet;

			u32 unitsEnabled = 0;

			if ( textureSet ) {

				unitsEnabled = textureSet->CountActiveUnits ();
				
				// bind or unbind textues depending on state of texture set
				for ( u32 i = 0; i < unitsEnabled; ++i ) {
					
					MOAISingleTexture* currentTexture = this->mTextureUnits [ i ];
					MOAISingleTexture* bindTexture = textureSet->GetTextureForUnit ( i );
				
					if ( bindTexture && ( !bindTexture->IsReady ())) {
						bindTexture = this->mDefaultTexture;
					}
					
					if ( currentTexture != bindTexture ) {
						
						gfx.ActiveTexture ( i );
						
						if ( currentTexture ) {
							currentTexture->Unbind ();
						}
						
						this->mTextureUnits [ i ] = bindTexture;
						
						if ( bindTexture ) {
							DEBUG_LOG ( "    binding texture: %d %p\n", i, bindTexture );
							bindTexture->Bind ();
						}
					}
				}
			}
			
			// unbind/disable any excess textures
			for ( u32 i = unitsEnabled; i < this->mActiveTextures; ++i ) {
			
				MOAISingleTexture* currentTexture = this->mTextureUnits [ i ];
			
				if ( currentTexture ) {
					gfx.ActiveTexture ( i );
					currentTexture->Unbind ();
					this->mTextureUnits [ i ] = 0;
				}
			}

			this->mActiveTextures = unitsEnabled;
			gfx.ActiveTexture ( 0 );
		}
	
	}
	else {

		this->mPendingState.mTextureSet = textureSet;
		this->mDirtyFlags = ( this->mActiveState.mTextureSet == textureSet ) ? ( this->mDirtyFlags & ~TEXTURE ) : ( this->mDirtyFlags | TEXTURE );
	}
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindVertexArray ( MOAIVertexArray* vtxArray ) {

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
				vtxArray->Bind ();
			}
		}
	}
	else {

		this->SetVertexFormat ();
		this->mPendingState.mVtxArray = vtxArray;
		this->mDirtyFlags = ( this->mActiveState.mVtxArray == vtxArray ) ? ( this->mDirtyFlags & ~VERTEX_ARRAY ) : ( this->mDirtyFlags | VERTEX_ARRAY );
	}
	return vtxArray ? vtxArray->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindVertexBuffer ( MOAIVertexBuffer* buffer ) {

	if ( this->mApplyingStateChanges ) {
	
		MOAIGfxState& active = this->mActiveState;
		MOAIVertexFormat* format = active.mVtxFormat;
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		ZLSharedConstBuffer* bufferForBind = buffer ? buffer->GetBufferForBind ( gfx ) : 0;
		
		if (( active.mVtxBuffer != buffer ) || ( this->mBoundVtxBuffer != bufferForBind )) {

			this->GfxStateWillChange ();

			DEBUG_LOG ( "  binding vertex buffer: (%p)\n", buffer );

			if ( active.mVtxBuffer ) {
				active.mVtxBuffer->Unbind ();
			}
			
			active.mVtxBuffer = 0;
			this->mBoundVtxBuffer = 0;
		
			if ( format && buffer ) {
				
				buffer->Bind ();
				active.mVtxBuffer = buffer;
				this->mBoundVtxBuffer = bufferForBind;
				format->Bind ( bufferForBind );
			}
		}
	}
	else {
	
		this->mPendingState.mVtxBuffer = buffer;
		this->mDirtyFlags = ( this->mActiveState.mVtxBuffer == buffer ) ? ( this->mDirtyFlags & ~VERTEX_BUFFER ) : ( this->mDirtyFlags | VERTEX_BUFFER );
	}
	
	return buffer ? buffer->IsReady () : true;
}

//----------------------------------------------------------------//
size_t MOAIGfxStateCache::CountTextureUnits () {

	return this->mTextureUnits.Size ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::DrawPrims ( u32 primType, u32 base, u32 count ) {

	DEBUG_LOG ( "DRAW PRIMS: %d %d %d\n", primType, base, count );

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	this->ApplyStateChanges ();

	MOAIShader* shader = this->mActiveState.mShader;

	if ( shader && ( this->mActiveState.mVtxBuffer || this->mActiveState.mVtxArray )) {
		
		shader->UpdateAndBindUniforms ();
				
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
u32 MOAIGfxStateCache::GetShaderGlobalsMask () {

	assert ( this->mCurrentState );
	MOAIShaderProgram* program = this->mCurrentState->mShaderProgram;
	return program ? program->GetGlobalsMask () : 0;
}

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

	this->mTextureUnits.Init ( nTextureUnits );
	this->mTextureUnits.Fill ( 0 );
}

//----------------------------------------------------------------//
MOAIGfxStateCache::MOAIGfxStateCache () :
	mCurrentState ( 0 ),
	mDirtyFlags ( 0 ),
	mApplyingStateChanges ( 0 ),
	mActiveTextures ( 0 ),
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
	gfx.UseProgram ( 0 );
	
	// turn off blending
	gfx.Disable ( ZGL_PIPELINE_BLEND );
	pending.mBlendEnabled = false;
	active.mBlendEnabled = 0;
	
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
	for ( size_t i = 0; i < this->mTextureUnits.Size (); ++i ){
		gfx.ActiveTexture (( u32 )i );
		gfx.BindTexture ( 0 );
		this->mTextureUnits [ i ] = 0;
	}
	this->mActiveTextures = 0;

	pending.mShaderProgram	= 0;
	pending.mIdxBuffer		= 0;
	pending.mTextureSet		= 0;
	pending.mVtxArray		= 0;
	pending.mVtxBuffer		= 0;
	pending.mVtxFormat		= 0;
	
	active.mShaderProgram	= 0;
	active.mIdxBuffer		= 0;
	active.mTextureSet		= 0;
	active.mVtxArray		= 0;
	active.mVtxBuffer		= 0;
	active.mVtxFormat		= 0;
	
	pending.mFrameBuffer = this->mDefaultFrameBuffer;
	active.mFrameBuffer = this->mDefaultFrameBuffer;
	gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, pending.mFrameBuffer->mGLFrameBufferID );
	
	this->mDirtyFlags = 0;
	
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
	blendMode.SetBlend ( srcFactor, dstFactor );
	blendMode.SetBlendEquation( equation );
	
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
void MOAIGfxStateCache::SetDefaultTexture ( MOAILuaObject& owner, MOAITexture* texture ) {

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
		this->mDirtyFlags = ( this->mActiveState.mDepthFunc == depthFunc ) ? ( this->mDirtyFlags & ~DEPTH_MODE ) : ( this->mDirtyFlags | DEPTH_MODE );
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
		this->mDirtyFlags = ( this->mActiveState.mDepthMask == depthMask ) ? ( this->mDirtyFlags & ~DEPTH_MODE ) : ( this->mDirtyFlags | DEPTH_MODE );
	}
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
			
			if ( active.mVtxFormat ) {
				active.mVtxFormat->Unbind ();
			}
			active.mVtxFormat = format;
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

	this->BindFrameBuffer ();
	this->BindIndexBuffer ();
	this->BindShader ();
	this->BindTexture ();
	this->BindVertexArray ();
	this->BindVertexBuffer ();
	this->SetVertexFormat ();
	
	ZGL_COMMENT ( gfx, "" );
}
