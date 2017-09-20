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
// MOAIGfxStateCache
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	frameBuffer = frameBuffer ? frameBuffer : this->mDefaultFrameBuffer;

	if ( this->mCurrentFrameBuffer != frameBuffer ) {
		
		this->GfxStateWillChange ();
		
		MOAIGfxMgr::GetDrawingAPI ().BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBufferID );
		this->mCurrentFrameBuffer = frameBuffer;
	}
	
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindIndexBuffer ( MOAIIndexBuffer* buffer ) {

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
bool MOAIGfxStateCache::BindShader ( MOAIShader* shader ) {

	// we don't cache the shader itself because it doesn't cause a bind;
	// we only care about binding its program (right now), which is cached.
	// later on we will re-bind the current shader's uniforms, the caching
	// of which is controlled by the shader program.
	
	bool result = true;
	
	if ( shader ) {
	
		result = this->BindShaderProgram ( shader->GetProgram ());
		this->mShader = result ? shader : 0;
		return result;
	}
	
	this->mShader = 0;
	return this->BindShaderProgram (( MOAIShaderProgram* )0 );
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindShader ( u32 preset ) {

	return this->BindShader ( MOAIShaderMgr::Get ().GetShader ( preset ));
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindShaderProgram ( MOAIShaderProgram* program ) {

	if ( this->mShaderProgram != program ) {

		 this->GfxStateWillChange ();
		
		if ( this->mShaderProgram ) {
			this->mShaderProgram->Unbind ();
		}
		
		this->mShaderProgram = program;
		
		if ( program ) {
			program->Bind ();
		}
	}
	
	return this->mShaderProgram ? this->mShaderProgram->IsReady () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCache::BindTexture ( MOAITextureBase* textureSet ) {

	bool result = true;

	if ( this->mCurrentTexture != textureSet ) {

		this->GfxStateWillChange ();

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
bool MOAIGfxStateCache::BindTexture ( u32 textureUnit, MOAISingleTexture* texture ) {
	
	MOAISingleTexture* currentTexture = this->mTextureUnits [ textureUnit ];
	MOAISingleTexture* bindTexture = texture;
	
	if ( texture && ( !texture->IsReady ())) {
		bindTexture = this->mDefaultTexture;
	}
	
	if ( currentTexture != bindTexture ) {
		
		this->GfxStateWillChange ();
		
		MOAIGfxMgr::GetDrawingAPI ().ActiveTexture ( textureUnit );
		
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
bool MOAIGfxStateCache::BindVertexArray ( MOAIVertexArray* vtxArray ) {

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
bool MOAIGfxStateCache::BindVertexBuffer ( MOAIVertexBuffer* buffer ) {

	if ( this->mCurrentVtxBuffer != buffer ) {
	
		this->GfxStateWillChange ();
		
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
bool MOAIGfxStateCache::BindVertexFormat ( MOAIVertexFormat* format, bool copyBuffer ) {

	if ( this->mCurrentVtxFormat != format ) {
	
		this->GfxStateWillChange ();
	
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
size_t MOAIGfxStateCache::CountTextureUnits () {

	return this->mTextureUnits.Size ();
}

//----------------------------------------------------------------//
u32 MOAIGfxStateCache::GetBufferHeight () const {

	assert ( this->mCurrentFrameBuffer );
	return this->mCurrentFrameBuffer->mBufferHeight;
}

//----------------------------------------------------------------//
u32 MOAIGfxStateCache::GetBufferWidth () const {

	assert ( this->mCurrentFrameBuffer );
	return this->mCurrentFrameBuffer->GetBufferWidth ();
}

//----------------------------------------------------------------//
//float MOAIGfxStateCache::GetDeviceScale () {
//
//	assert ( this->mCurrentFrameBuffer );
//	return this->mCurrentFrameBuffer->mBufferScale;
//}

//----------------------------------------------------------------//
u32 MOAIGfxStateCache::GetShaderGlobalsMask () {

	return this->mShaderProgram ? this->mShaderProgram->GetGlobalsMask () : 0;
}

//----------------------------------------------------------------//
float MOAIGfxStateCache::GetViewHeight () const {

	return this->mViewRect.Height ();
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

	return this->mViewRect.Width ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::InitTextureUnits ( size_t nTextureUnits ) {

	this->mTextureUnits.Init ( nTextureUnits );
	this->mTextureUnits.Fill ( 0 );
}

//----------------------------------------------------------------//
MOAIGfxStateCache::MOAIGfxStateCache () :
	mCullFunc ( 0 ),
	mDepthFunc ( 0 ),
	mDepthMask ( true ),
	mBlendEnabled ( 0 ),
	mPenWidth ( 1.0f ),
	mScissorEnabled ( false ),
	mShader ( 0 ),
	mShaderProgram ( 0 ),
	mActiveTextures ( 0 ),
	mCurrentFrameBuffer ( 0 ),
	mCurrentIdxBuffer ( 0 ),
	mCurrentTexture ( 0 ),
	mCurrentVtxArray ( 0 ),
	mCurrentVtxBuffer ( 0 ),
	mCurrentVtxFormat ( 0 ),
	mClient ( 0 ) {
	
	this->mScissorRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mViewRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGfxStateCache::~MOAIGfxStateCache () {
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::ResetState () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	ZGL_COMMENT ( gfx, "GFX RESET STATE" );

	// clear the CPU matrix pipeline
//	for ( u32 i = 0; i < TOTAL_VTX_TRANSFORMS; ++i ) {
//		this->mVertexTransforms [ i ].Ident ();
//	}
//	this->mUVTransform.Ident ();
//	this->mCpuVertexTransformMtx.Ident ();
	
	// reset the active texture
	gfx.ActiveTexture ( 0 );
	gfx.BindTexture ( 0 );

	// reset the shader
	gfx.UseProgram ( 0 );
	
	// turn off blending
	gfx.Disable ( ZGL_PIPELINE_BLEND );
	this->mBlendEnabled = false;
	
	// disable backface culling
	gfx.Disable ( ZGL_PIPELINE_CULL );
	this->mCullFunc = 0;
	
	// disable depth test
	gfx.Disable ( ZGL_PIPELINE_DEPTH );
	this->mDepthFunc = 0;
	
	// disable depth write
	gfx.DepthMask ( false );
	this->mDepthMask = false;
	
	// reset the pen width
	this->mPenWidth = 1.0f;
	gfx.LineWidth ( this->mPenWidth );
	
	// reset the scissor rect
	this->mScissorEnabled = false;
	gfx.Disable ( ZGL_PIPELINE_SCISSOR );
	
	for ( size_t i = 0; i < this->mTextureUnits.Size (); ++i ){
		gfx.ActiveTexture (( u32 )i );
		gfx.BindTexture ( 0 );
		this->mTextureUnits [ i ] = 0;
	}
	this->mActiveTextures = 0;

	this->mShaderProgram		= 0;
	this->mCurrentIdxBuffer		= 0;
	this->mCurrentTexture		= 0;
	this->mCurrentVtxArray		= 0;
	this->mCurrentVtxBuffer		= 0;
	this->mCurrentVtxFormat		= 0;
	
	this->mCurrentFrameBuffer = this->mDefaultFrameBuffer;
	gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, this->mCurrentFrameBuffer->mGLFrameBufferID );
	
	ZGL_COMMENT ( gfx, "" );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetBlendMode () {

	if ( this->mBlendEnabled ) {
	
		this->GfxStateWillChange ();
		
		MOAIGfxMgr::GetDrawingAPI ().Disable ( ZGL_PIPELINE_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	if ( !this->mBlendEnabled ) {
	
		this->GfxStateWillChange ();
		
		gfx.Enable ( ZGL_PIPELINE_BLEND );
		
		this->mBlendMode = blendMode;
		gfx.BlendMode ( this->mBlendMode.mEquation );
		gfx.BlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
		this->mBlendEnabled = true;
	}
	else if ( !this->mBlendMode.IsSame ( blendMode )) {
	
		this->GfxStateWillChange ();
		
		this->mBlendMode = blendMode;
		gfx.BlendMode ( this->mBlendMode.mEquation );
		gfx.BlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
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

	if ( this->mCullFunc != cullFunc ) {
	
		this->GfxStateWillChange ();
		
		this->mCullFunc = cullFunc;
	
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
		if ( cullFunc ) {
			gfx.Enable ( ZGL_PIPELINE_CULL );
			gfx.CullFace ( this->mCullFunc );
		}
		else {
			gfx.Disable ( ZGL_PIPELINE_CULL );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetDefaultFrameBuffer ( MOAILuaObject& owner, MOAIFrameBuffer* frameBuffer ) {

	this->mDefaultFrameBuffer.Set ( owner, frameBuffer );
	if ( !this->mCurrentFrameBuffer ) {
		this->mCurrentFrameBuffer = this->mDefaultFrameBuffer;
	}
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

	if ( this->mDepthFunc != depthFunc ) {
	
		this->GfxStateWillChange ();
		
		this->mDepthFunc = depthFunc;
	
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
		if ( depthFunc ) {
			gfx.Enable ( ZGL_PIPELINE_DEPTH );
			gfx.DepthFunc ( this->mDepthFunc );
		}
		else {
			gfx.Disable ( ZGL_PIPELINE_DEPTH );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetDepthMask ( bool depthMask ) {

	if ( this->mDepthMask != depthMask ) {
		this->GfxStateWillChange ();
		this->mDepthMask = depthMask;
		MOAIGfxMgr::GetDrawingAPI ().DepthMask ( this->mDepthMask );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetPenWidth ( float penWidth ) {

	if ( this->mPenWidth != penWidth ) {
		this->GfxStateWillChange ();
		this->mPenWidth = penWidth;
		MOAIGfxMgr::GetDrawingAPI ().LineWidth ( penWidth );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetScissorRect () {

	if ( this->mScissorEnabled ) {
		MOAIGfxMgr::GetDrawingAPI ().Disable ( ZGL_PIPELINE_SCISSOR );
		this->mScissorEnabled = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetScissorRect ( ZLRect rect ) {
	
	rect.Bless ();
	this->mViewRect.Clip ( rect );
	
	if ( !( this->mScissorEnabled && this->mScissorRect.IsEqual ( rect ))) {
		
		this->GfxStateWillChange ();

		ZLRect deviceRect = this->mCurrentFrameBuffer->WndRectToDevice ( rect );

		s32 x = ( s32 )deviceRect.mXMin;
		s32 y = ( s32 )deviceRect.mYMin;
		
		u32 w = ( u32 )( deviceRect.Width () + 0.5f );
		u32 h = ( u32 )( deviceRect.Height () + 0.5f );

		w = h == 0 ? 0 : w;
		h = w == 0 ? 0 : h;
		
		MOAIGfxMgr::GetDrawingAPI ().Scissor ( x, y, w, h );
		this->mScissorRect = rect;
	}
	
	if ( !this->mScissorEnabled ) {
		MOAIGfxMgr::GetDrawingAPI ().Enable ( ZGL_PIPELINE_SCISSOR );
		this->mScissorEnabled = true;
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetViewRect () {

	float width = ( float )this->mCurrentFrameBuffer->mBufferWidth;
	float height = ( float )this->mCurrentFrameBuffer->mBufferHeight;

	MOAIViewport rect;
	rect.Init ( 0.0f, 0.0f, width, height );
	
	this->SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::SetViewRect ( ZLRect rect ) {

	ZLRect deviceRect;
	
	deviceRect = this->mCurrentFrameBuffer->WndRectToDevice ( rect );
	
	s32 x = ( s32 )deviceRect.mXMin;
	s32 y = ( s32 )deviceRect.mYMin;
	
	u32 w = ( u32 )( deviceRect.Width () + 0.5f );
	u32 h = ( u32 )( deviceRect.Height () + 0.5f );
	
	MOAIGfxMgr::Get ().GetDrawingAPI ().Viewport ( x, y, w, h );
	
	this->mViewRect = rect;
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
	this->BindVertexFormat ();
	
	ZGL_COMMENT ( gfx, "" );
}

//----------------------------------------------------------------//
void MOAIGfxStateCache::UpdateAndBindUniforms () {

	if ( this->mShader ) {
		this->mShader->UpdateAndBindUniforms ();
	}
}
