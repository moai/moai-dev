// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getFrameBuffer
	@text	Returns the frame buffer associated with the device.

	@out	MOAIFrameBuffer frameBuffer
*/
int MOAIGfxDevice::_getFrameBuffer ( lua_State* L ) {

	MOAILuaState state ( L );
	state.Push ( MOAIGfxDevice::Get ().GetDefaultBuffer ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureUnits
	@text	Returns the total number of texture units available on the device.

	@out	number maxTextureUnits
*/
int MOAIGfxDevice::_getMaxTextureUnits ( lua_State* L ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	lua_pushnumber ( L, gfxDevice.mTextureUnits.Size ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getViewSize
	@text	Returns the width and height of the view
	
	@out	number width
	@out	number height
*/
int MOAIGfxDevice::_getViewSize ( lua_State* L  ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	lua_pushnumber ( L, gfxDevice.GetWidth ());
	lua_pushnumber ( L, gfxDevice.GetHeight ());
	
	return 2;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxDevice::_setDefaultTexture ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAIGfxDevice& device = MOAIGfxDevice::Get ();

	MOAITexture* texture = state.GetLuaObject < MOAITexture >( 1, false );
	
	if ( !texture ) {
		texture = new MOAITexture ();
		if ( !texture->Init ( state, 1 )) {
			// TODO: report error
			delete texture;
			texture = 0;
		}
	}

	device.mDefaultTexture.Set ( device, texture );

	if ( texture ) {
		texture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenColor

	@in		number r
	@in		number g
	@in		number b
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIGfxDevice::_setPenColor ( lua_State* L ) {

	MOAILuaState state ( L );

	float r = state.GetValue < float >( 1, 1.0f );
	float g = state.GetValue < float >( 2, 1.0f );
	float b = state.GetValue < float >( 3, 1.0f );
	float a = state.GetValue < float >( 4, 1.0f );

	MOAIGfxDevice::Get ().SetPenColor ( r, g, b, a );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth

	@in		number width
	@out	nil
*/
int MOAIGfxDevice::_setPenWidth ( lua_State* L ) {

	MOAILuaState state ( L );

	float width = state.GetValue < float >( 1, 1.0f );
	MOAIGfxDevice::Get ().SetPenWidth ( width );
	return 0;
}

//================================================================//
// MOAIGfxDevice
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDevice::Clear () {

	this->mDefaultTexture.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ClearErrors () {
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			while ( zglGetError () != ZGL_ERROR_NONE );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ClearSurface ( u32 clearFlags ) {

	if ( clearFlags ) {
		if (( clearFlags & ZGL_CLEAR_DEPTH_BUFFER_BIT ) && !this->mDepthMask ) {
			zglDepthMask ( true );
			zglClear ( clearFlags );
			zglDepthMask ( false );
		}
		else {
			zglClear ( clearFlags );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DetectContext () {

	this->mHasContext = true;
	
	zglBegin ();
	
	zglInitialize ();
	
	u32 maxTextureUnits = zglGetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
	this->mTextureUnits.Init ( maxTextureUnits );
	this->mTextureUnits.Fill ( 0 );
	
	this->mMaxTextureSize = zglGetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );

	MOAIGfxResourceMgr::Get ().RenewResources ();
	
	this->mDefaultBuffer->DetectGLFrameBufferID ();
	
	zglEnd ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DetectFramebuffer () {
	
	zglBegin ();
	
	this->mDefaultBuffer->DetectGLFrameBufferID ();
	
	zglEnd ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DisableTextureUnits ( u32 activeTextures ) {

	if ( activeTextures < this->mActiveTextures ) {
		
		this->Flush ();
	
		for ( u32 i = activeTextures; i < this->mActiveTextures; ++i ) {
			this->mTextureUnits [ i ] = 0;
		}
	}
	
	this->mActiveTextures = activeTextures;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Flush () {

	this->DrawPrims ();
}

//----------------------------------------------------------------//
float MOAIGfxDevice::GetDeviceScale () {

	return this->mFrameBuffer->mBufferScale;
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetHeight () const {

	return this->mFrameBuffer->mBufferHeight;
}

//----------------------------------------------------------------//
//ZLQuad MOAIGfxDevice::GetViewQuad () const {
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
//ZLRect MOAIGfxDevice::GetViewRect () const {
//
//	return this->mViewRect;
//}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetWidth () const {

	return this->mFrameBuffer->mBufferWidth;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::IsOpaque () const {
	
	assert ( this->mDefaultBuffer );
	return this->mDefaultBuffer->IsOpaque ();
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::LogErrors () {

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( u32 error = zglGetError (); error != ZGL_ERROR_NONE; error = zglGetError (), ++count ) {
				MOAILog ( 0, MOAILogMessages::MOAIGfxDevice_OpenGLError_S, zglGetErrorString ( error ));
			}
		}
	#endif
	return count;
}

//----------------------------------------------------------------//
MOAIGfxDevice::MOAIGfxDevice () :
	mCullFunc ( 0 ),
	mDepthFunc ( 0 ),
	mDepthMask ( true ),
	mBlendEnabled ( 0 ),
	mHasContext ( false ),
	mIsFramebufferSupported ( 0 ),
	#if defined ( MOAI_OS_NACL ) || defined ( MOAI_OS_IPHONE ) || defined ( MOAI_OS_ANDROID ) || defined ( EMSCRIPTEN )
		mIsOpenGLES ( true ),
	#else
		mIsOpenGLES ( false ),
	#endif
	mMajorVersion ( 0 ),
	mMinorVersion ( 0 ),
	mPenWidth ( 1.0f ),
	mPointSize ( 1.0f ),
	mShaderProgram ( 0 ),
	mActiveTextures ( 0 ),
	mTextureMemoryUsage ( 0 ),
	mMaxTextureSize ( 0 ),
	mVertexFormat ( 0 ),
	mVertexFormatBuffer ( 0 ) {
	
	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	this->mScissorRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mDefaultBuffer.Set ( *this, new MOAIFrameBuffer ());
	this->mFrameBuffer = this->mDefaultBuffer;
}

//----------------------------------------------------------------//
MOAIGfxDevice::~MOAIGfxDevice () {

	this->mDefaultBuffer.Set ( *this, 0 );
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void MOAIGfxDevice::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "LOADING_POLICY_CPU_GPU_ASAP",			( u32 )MOAIGfxResource::LOADING_POLICY_CPU_GPU_ASAP );
	state.SetField ( -1, "LOADING_POLICY_CPU_ASAP_GPU_NEXT",	( u32 )MOAIGfxResource::LOADING_POLICY_CPU_ASAP_GPU_NEXT );
	state.SetField ( -1, "LOADING_POLICY_CPU_ASAP_GPU_BIND",	( u32 )MOAIGfxResource::LOADING_POLICY_CPU_ASAP_GPU_BIND );
	state.SetField ( -1, "LOADING_POLICY_CPU_GPU_BIND",			( u32 )MOAIGfxResource::LOADING_POLICY_CPU_GPU_BIND );

	state.SetField ( -1, "EVENT_RESIZE", ( u32 )EVENT_RESIZE );

	luaL_Reg regTable [] = {
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIGfxDevice > },
		{ "getMaxTextureUnits",			_getMaxTextureUnits },
		{ "getViewSize",				_getViewSize },
		{ "setDefaultTexture",			_setDefaultTexture },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxDevice > },
		{ "setPenColor",				_setPenColor },
		{ "setPenWidth",				_setPenWidth },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ReportTextureAlloc ( cc8* name, size_t size ) {

	this->mTextureMemoryUsage += size;
	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
	MOAILog ( 0, MOAILogMessages::MOAITexture_MemoryUse_SDFS, "+", size, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ReportTextureFree ( cc8* name, size_t size ) {

	this->mTextureMemoryUsage -= size;
	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
	MOAILog ( 0, MOAILogMessages::MOAITexture_MemoryUse_SDFS, "-", size, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ResetDrawCount () {
	this->mDrawCount = 0;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ResetState () {

	for ( u32 i = 0; i < TOTAL_VTX_TRANSFORMS; ++i ) {
		this->mVertexTransforms [ i ].Ident ();
	}
	this->mUVTransform.Ident ();
	this->mCpuVertexTransformMtx.Ident ();
	
	this->mVertexMtxInput = VTX_STAGE_MODEL;
	this->mVertexMtxOutput = VTX_STAGE_MODEL;

	this->mTop = 0;
	this->mPrimCount = 0;

	// turn off texture
	this->mTextureUnits [ 0 ] = 0;
	
	// turn off blending
	zglDisable ( ZGL_PIPELINE_BLEND );
	this->mBlendEnabled = false;
	
	// disable backface culling
	zglDisable ( ZGL_PIPELINE_CULL );
	this->mCullFunc = 0;
	
	// disable depth test
	zglDisable ( ZGL_PIPELINE_DEPTH );
	this->mDepthFunc = 0;
	
	// disable depth write
	zglDepthMask ( false );
	this->mDepthMask = false;
	
	// clear the vertex format
	this->SetVertexFormat ();

	// clear the shader
	this->mShaderProgram = 0;
	
	// reset the pen width
	this->mPenWidth = 1.0f;
	zglLineWidth ( this->mPenWidth );
	
	// reset the point size
	this->mPointSize = 1.0f;
	
	// reset the scissor rect
	ZLRect scissorRect = this->mFrameBuffer->GetBufferRect ();
	zglScissor (( s32 )scissorRect.mXMin, ( s32 )scissorRect.mYMin, ( u32 )scissorRect.Width (), ( u32 )scissorRect.Height ());
	
	this->mScissorRect = scissorRect;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode () {

	if ( this->mBlendEnabled ) {
		this->Flush ();
		zglDisable ( ZGL_PIPELINE_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	if ( !this->mBlendEnabled ) {
		this->Flush ();
		zglEnable ( ZGL_PIPELINE_BLEND );
		this->mBlendMode = blendMode;
		zglBlendMode(this->mBlendMode.mEquation);
		zglBlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
		this->mBlendEnabled = true;
	}
	else if ( !this->mBlendMode.IsSame ( blendMode )) {
		this->Flush ();
		this->mBlendMode = blendMode;
		zglBlendMode(this->mBlendMode.mEquation);
		zglBlendFunc ( this->mBlendMode.mSourceFactor, this->mBlendMode.mDestFactor );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode ( int srcFactor, int dstFactor, int equation ) {

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( srcFactor, dstFactor );
	blendMode.SetBlendEquation( equation );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBufferScale ( float scale ) {

	this->mDefaultBuffer->SetBufferScale ( scale );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBufferSize ( u32 width, u32 height ) {

	this->mDefaultBuffer->SetBufferSize ( width, height );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetCullFunc () {

	this->SetCullFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetCullFunc ( int cullFunc ) {

	if ( this->mCullFunc != cullFunc ) {
	
		this->Flush ();
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
void MOAIGfxDevice::SetDepthFunc () {

	this->SetDepthFunc ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetDepthFunc ( int depthFunc ) {

	if ( this->mDepthFunc != depthFunc ) {
	
		this->Flush ();
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
void MOAIGfxDevice::SetDepthMask ( bool depthMask ) {

	if ( this->mDepthMask != depthMask ) {
		this->Flush ();
		this->mDepthMask = depthMask;
		zglDepthMask ( this->mDepthMask );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	this->Flush ();

	if ( frameBuffer ) {
		zglBindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, frameBuffer->mGLFrameBufferID );
		this->mFrameBuffer = frameBuffer;
	}
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetGfxState ( MOAIGfxState* gfxState ) {

	if ( gfxState ) {
		return gfxState->LoadGfxState ();
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenWidth ( float penWidth ) {

	if ( this->mPenWidth != penWidth ) {
		this->Flush ();
		this->mPenWidth = penWidth;
		zglLineWidth ( penWidth );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScissorRect () {

	this->SetScissorRect ( this->mFrameBuffer->GetBufferRect ());
	zglDisable ( ZGL_PIPELINE_SCISSOR );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScissorRect ( ZLRect rect ) {
	
	rect.Bless ();
	this->mViewRect.Clip ( rect );

	ZLRect& current = this->mScissorRect;
	
	if (	( current.mXMin != rect.mXMin ) ||
			( current.mYMin != rect.mYMin ) ||
			( current.mXMax != rect.mXMax ) ||
			( current.mYMax != rect.mYMax )) {
		
		this->Flush ();

		ZLRect deviceRect = this->mFrameBuffer->WndRectToDevice ( rect );

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
void MOAIGfxDevice::SetScreenSpace ( MOAIViewport& viewport ) {
	UNUSED ( viewport );

	// TODO

	//glMatrixMode ( GL_MODELVIEW );
	//glLoadIdentity ();
	//
	//ZLMatrix4x4 wndToNorm;
	//viewport.GetWndToNormMtx ( wndToNorm );
	//
	//glMatrixMode ( GL_PROJECTION );
	//MOAIGfxDevice::LoadMatrix ( wndToNorm );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetShader ( MOAIShader* shader ) {

	if ( shader ) {
		this->SetShaderProgram ( shader->GetProgram ());
		shader->BindUniforms ();
	}
	else {
		this->SetShaderProgram ( 0 );
	}	
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetShaderPreset ( u32 preset ) {

	MOAIShaderMgr::Get ().BindShader ( preset );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetShaderProgram ( MOAIShaderProgram* program ) {

	if ( this->mShaderProgram != program ) {
	
		this->Flush ();
		this->mShaderProgram = program;
		
		if ( program ) {
			program->Bind ();
		}
	}
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture () {
	
	this->DisableTextureUnits ( 0 );
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture ( MOAITextureBase* texture ) {
	
	if ( !texture ) {
		return this->SetTexture ();
	}
	
	this->DisableTextureUnits ( 1 );
	return this->SetTexture ( 0, texture );
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture ( MOAIMultiTexture* multi ) {

	if ( !multi ) {
		return this->SetTexture ();
	}
	
	u32 total = 0;
	u32 multiSize = multi->mTextures.Size ();
	for ( ; total < multiSize; ++total ) {
		if ( !multi->mTextures [ total ]) break;
	}
	
	if ( total > this->mTextureUnits.Size ()) {
		total = this->mTextureUnits.Size ();
	}
	
	// disable any unused textures
	this->DisableTextureUnits ( total );
	
	for ( u32 i = 0; i < total; ++i ) {
		this->SetTexture ( i, multi->mTextures [ i ]);
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture ( u32 textureUnit, MOAITextureBase* texture ) {
	
	if ( !texture ) {
		// TODO: is this right?
		this->mTextureUnits [ textureUnit ] = 0;
		return false;
	}
	
	if ( this->mTextureUnits [ textureUnit ] == texture ) return true;
	
	this->Flush ();
	
	zglActiveTexture ( textureUnit );
	
	if ( !texture->Bind ()) {
		return this->SetTexture ( textureUnit, this->mDefaultTexture );
	}
	
	this->mTextureUnits [ textureUnit ] = texture;
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat () {

	this->Flush ();
	
	if ( this->mVertexFormat ) {
		this->mVertexFormat->Unbind ();
	}
	this->mVertexFormat = 0;
	this->mVertexFormatBuffer = 0;
	this->mVertexSize = 0;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat ( const MOAIVertexFormat& format ) {

	this->SetVertexFormat ( format, this->mBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat ( const MOAIVertexFormat& format, void* buffer ) {

	if (( this->mVertexFormat != &format ) || ( this->mVertexFormatBuffer != buffer )) {

		this->SetVertexFormat ();
		this->mVertexFormat = &format;
		this->mVertexFormat->Bind ( buffer );
		this->mVertexFormatBuffer = buffer;
		this->mVertexSize = format.GetVertexSize ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexPreset ( u32 preset ) {

	MOAIVertexFormat* format = MOAIVertexFormatMgr::Get ().GetFormat ( preset );
	if ( format ) {
		this->SetVertexFormat ( *format );
	}
	else {
		this->SetVertexFormat ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetViewRect () {

	float width = ( float )this->mFrameBuffer->mBufferWidth;
	float height = ( float )this->mFrameBuffer->mBufferHeight;

	MOAIViewport rect;
	rect.Init ( 0.0f, 0.0f, width, height );
	
	this->SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetViewRect ( ZLRect rect ) {

	ZLRect deviceRect;
	
	deviceRect = this->mFrameBuffer->WndRectToDevice ( rect );
	
	s32 x = ( s32 )deviceRect.mXMin;
	s32 y = ( s32 )deviceRect.mYMin;
	
	u32 w = ( u32 )( deviceRect.Width () + 0.5f );
	u32 h = ( u32 )( deviceRect.Height () + 0.5f );
	
	zglViewport ( x, y, w, h );
	
	this->mViewRect = rect;
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::UpdateShaderGlobals () {

	if ( this->mShaderProgram && this->mShaderDirty ) {
		this->mShaderProgram->UpdateGlobals ();
	}
	this->mShaderDirty = false;
}
