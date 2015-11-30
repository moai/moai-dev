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
	state.Push ( MOAIGfxDevice::Get ().GetDefaultFrameBuffer ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureSize
	@text	Returns the maximum texture size supported by device
 
	@out	number maxTextureSize
*/
int MOAIGfxDevice::_getMaxTextureSize ( lua_State* L ) {
	
	MOAILuaState state ( L );
	state.Push ( MOAIGfxDevice::Get ().mMaxTextureSize );
	
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
/**	@lua	setDefaultTexture
	@text	Specify a fallback texture to use when textures are
			unavailable (pending load, missing or in error state).
	
	@in		MOAITexture texture
	@out	MOAITexture texture		Texture that was passed in or created.
*/
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
	
	this->mDefaultFrameBuffer->DetectGLFrameBufferID ();
	
	zglEnd ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DetectFramebuffer () {
	
	zglBegin ();
	
	this->mDefaultFrameBuffer->DetectGLFrameBufferID ();
	
	zglEnd ();
}

//----------------------------------------------------------------//
float MOAIGfxDevice::GetDeviceScale () {

	return this->mCurrentFrameBuffer->mBufferScale;
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetHeight () const {

	return this->mCurrentFrameBuffer->mBufferHeight;
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

	return this->mCurrentFrameBuffer->mBufferWidth;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::IsOpaque () const {
	
	assert ( this->mDefaultFrameBuffer );
	return this->mDefaultFrameBuffer->IsOpaque ();
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::LogErrors () {

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( u32 error = zglGetError (); error != ZGL_ERROR_NONE; error = zglGetError (), ++count ) {
				MOAILogF ( 0, ZLLog::LOG_ERROR, MOAILogMessages::MOAIGfxDevice_OpenGLError_S, zglGetErrorString ( error ));
			}
		}
	#endif
	return count;
}

//----------------------------------------------------------------//
MOAIGfxDevice::MOAIGfxDevice () :
	mHasContext ( false ),
	mIsFramebufferSupported ( 0 ),
	#if defined ( MOAI_OS_NACL ) || defined ( MOAI_OS_IPHONE ) || defined ( MOAI_OS_ANDROID ) || defined ( EMSCRIPTEN )
		mIsOpenGLES ( true ),
	#else
		mIsOpenGLES ( false ),
	#endif
	mMajorVersion ( 0 ),
	mMinorVersion ( 0 ),
	mTextureMemoryUsage ( 0 ),
	mMaxTextureSize ( 0 ) {
	
	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	this->mScissorRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mDefaultFrameBuffer.Set ( *this, new MOAIFrameBuffer ());
	this->mCurrentFrameBuffer = this->mDefaultFrameBuffer;
}

//----------------------------------------------------------------//
MOAIGfxDevice::~MOAIGfxDevice () {

	this->mDefaultFrameBuffer.Set ( *this, 0 );
	this->mDefaultTexture.Set ( *this, 0 );
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
		{ "getMaxTextureSize",			_getMaxTextureSize },
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
	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAILogMessages::MOAITexture_MemoryUse_SDFS, "+", size, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ReportTextureFree ( cc8* name, size_t size ) {

	this->mTextureMemoryUsage -= size;
	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAILogMessages::MOAITexture_MemoryUse_SDFS, "-", size, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ResetDrawCount () {
	this->mDrawCount = 0;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ResetState () {

	this->OnGfxStateWillChange ();

	for ( u32 i = 0; i < TOTAL_VTX_TRANSFORMS; ++i ) {
		this->mVertexTransforms [ i ].Ident ();
	}
	this->mUVTransform.Ident ();
	this->mCpuVertexTransformMtx.Ident ();
	
	this->mVertexMtxInput = VTX_STAGE_MODEL;
	this->mVertexMtxOutput = VTX_STAGE_MODEL;

	//this->mTop = 0;
	//this->mPrimCount = 0;

	// turn off texture
	this->mTextureUnits [ 0 ] = 0;
	this->mCurrentTexture = 0;
	
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
	this->UnbindBufferedDrawing ();

	// clear the shader
	this->mShaderProgram = 0;
	
	// reset the pen width
	this->mPenWidth = 1.0f;
	zglLineWidth ( this->mPenWidth );
	
	// reset the scissor rect
	ZLRect scissorRect = this->mCurrentFrameBuffer->GetBufferRect ();
	zglScissor (( s32 )scissorRect.mXMin, ( s32 )scissorRect.mYMin, ( u32 )scissorRect.Width (), ( u32 )scissorRect.Height ());
	
	this->mScissorRect = scissorRect;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBufferScale ( float scale ) {

	this->mDefaultFrameBuffer->SetBufferScale ( scale );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBufferSize ( u32 width, u32 height ) {

	this->mDefaultFrameBuffer->SetBufferSize ( width, height );
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
void MOAIGfxDevice::SetViewRect () {

	float width = ( float )this->mCurrentFrameBuffer->mBufferWidth;
	float height = ( float )this->mCurrentFrameBuffer->mBufferHeight;

	MOAIViewport rect;
	rect.Init ( 0.0f, 0.0f, width, height );
	
	this->SetViewRect ( rect );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetViewRect ( ZLRect rect ) {

	ZLRect deviceRect;
	
	deviceRect = this->mCurrentFrameBuffer->WndRectToDevice ( rect );
	
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
