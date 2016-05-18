// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
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
// TODO: doxygen
int MOAIGfxMgr::_enablePipelineLogging ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgr, "" )

	MOAIGfxMgr::Get ().mPipelineMgr.EnablePipelineLogging ( state.GetValue < bool >( 1, false ));

	ZLFileSys::DeleteDirectory ( GFX_PIPELINE_LOGGING_FOLDER, true, true );
	ZLFileSys::AffirmPath ( GFX_PIPELINE_LOGGING_FOLDER );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getFrameBuffer
	@text	Returns the frame buffer associated with the device.

	@out	MOAIFrameBuffer frameBuffer
*/
int MOAIGfxMgr::_getFrameBuffer ( lua_State* L ) {

	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgr::Get ().mGfxState.GetDefaultFrameBuffer ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureSize
	@text	Returns the maximum texture size supported by device
 
	@out	number maxTextureSize
*/
int MOAIGfxMgr::_getMaxTextureSize ( lua_State* L ) {
	
	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgr::Get ().mMaxTextureSize );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureUnits
	@text	Returns the total number of texture units available on the device.

	@out	number maxTextureUnits
*/
int MOAIGfxMgr::_getMaxTextureUnits ( lua_State* L ) {

	lua_pushnumber ( L, ( double )MOAIGfxMgr::Get ().mGfxState.CountTextureUnits ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getViewSize
	@text	Returns the width and height of the view
	
	@out	number width
	@out	number height
*/
int MOAIGfxMgr::_getViewSize ( lua_State* L  ) {

	MOAIFrameBuffer* frameBuffer = MOAIGfxMgr::Get ().mGfxState.GetCurrentFrameBuffer ();
	
	lua_pushnumber ( L, frameBuffer->GetBufferWidth ());
	lua_pushnumber ( L, frameBuffer->GetBufferHeight ());
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	purgeResources
	@text	Purges all resources older that a given age (in render cycles).
			If age is 0 then all resources are purged.
 
	@opt	number age		Default value is 0.
	@out	nil
*/
int MOAIGfxMgr::_purgeResources ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 age = state.GetValue < u32 >( 1, 0 );

	MOAIGfxMgr::Get ().mResourceMgr.PurgeResources ( age );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	renewResources
	@text	Renews all resources.
 
	@out	nil
*/
int MOAIGfxMgr::_renewResources ( lua_State* L ) {
	MOAILuaState state ( L );

	MOAIGfxMgr::Get ().mResourceMgr.RenewResources ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDefaultTexture
	@text	Specify a fallback texture to use when textures are
			unavailable (pending load, missing or in error state).
	
	@in		MOAITexture texture
	@out	MOAITexture texture		Texture that was passed in or created.
*/
int MOAIGfxMgr::_setDefaultTexture ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	MOAITexture* texture = state.GetLuaObject < MOAITexture >( 1, false );
	
	if ( !texture ) {
		texture = new MOAITexture ();
		if ( !texture->Init ( state, 1 )) {
			// TODO: report error
			delete texture;
			texture = 0;
		}
	}

	gfxMgr.mGfxState.SetDefaultTexture ( gfxMgr, texture );

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
int MOAIGfxMgr::_setPenColor ( lua_State* L ) {

	MOAILuaState state ( L );

	float r = state.GetValue < float >( 1, 1.0f );
	float g = state.GetValue < float >( 2, 1.0f );
	float b = state.GetValue < float >( 3, 1.0f );
	float a = state.GetValue < float >( 4, 1.0f );

	MOAIGfxMgr::Get ().mGfxState.SetPenColor ( r, g, b, a );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth

	@in		number width
	@out	nil
*/
int MOAIGfxMgr::_setPenWidth ( lua_State* L ) {

	MOAILuaState state ( L );

	float width = state.GetValue < float >( 1, 1.0f );
	MOAIGfxMgr::Get ().mGfxState.SetPenWidth ( width );
	return 0;
}

//================================================================//
// MOAIGfxMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgr::ClearErrors () {

	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			while ( ZLGfxDevice::GetError () != ZGL_ERROR_NONE );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxMgr::ClearSurface ( u32 clearFlags ) {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	if ( clearFlags ) {
		if (( clearFlags & ZGL_CLEAR_DEPTH_BUFFER_BIT ) && !this->mGfxState.GetDepthMask ()) {
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
void MOAIGfxMgr::DetectContext () {

	this->mHasContext = true;
	
	ZLGfxDevice::Begin ();
	
	ZLGfxDevice::Initialize ();
	
	u32 maxTextureUnits = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
	this->mGfxState.InitTextureUnits ( maxTextureUnits );
	
	this->mMaxTextureSize = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );

	// renew resources in immediate mode
	this->mPipelineMgr.SelectDrawingAPI ();
	
	this->mGfxState.GetDefaultFrameBuffer ()->DetectGLFrameBufferID ();
	
	MOAIShaderMgr::Get ().AffirmAll ();
	
	mResourceMgr.RenewResources ();
	
	ZLGfxDevice::End ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr::DetectFramebuffer () {
	
	ZLGfxDevice::Begin ();
	
	this->mGfxState.GetDefaultFrameBuffer ()->DetectGLFrameBufferID ();
	
	ZLGfxDevice::End ();
}

//----------------------------------------------------------------//
bool MOAIGfxMgr::IsOpaque () {
	
	assert ( this->mGfxState.GetDefaultFrameBuffer ());
	return this->mGfxState.GetDefaultFrameBuffer ()->IsOpaque ();
}

//----------------------------------------------------------------//
u32 MOAIGfxMgr::LogErrors () {

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( u32 error = ZLGfxDevice::GetError (); error != ZGL_ERROR_NONE; error = ZLGfxDevice::GetError (), ++count ) {
				MOAILogF ( 0, ZLLog::LOG_ERROR, MOAILogMessages::MOAIGfxDevice_OpenGLError_S, ZLGfxDevice::GetErrorString ( error ));
			}
		}
	#endif
	return count;
}

//----------------------------------------------------------------//
MOAIGfxMgr::MOAIGfxMgr () :
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
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGfxStateCache )
		RTTI_SINGLE ( MOAIGlobalEventSource )
	RTTI_END
	
	this->mGfxState.SetDefaultFrameBuffer ( *this, new MOAIFrameBuffer ());
}

//----------------------------------------------------------------//
MOAIGfxMgr::~MOAIGfxMgr () {

	this->mGfxState.SetDefaultFrameBuffer ( *this, 0 );
	this->mGfxState.SetDefaultFrameBuffer ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGfxMgr::OnGlobalsFinalize () {

	this->mGfxState.SetDefaultFrameBuffer ( *this, 0 );
	this->mGfxState.SetDefaultTexture ( *this, 0 );
	
	mResourceMgr.ProcessDeleters ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr::OnGlobalsInitialize () {

	this->mVertexCache.InitBuffers ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "EVENT_RESIZE",	( u32 )EVENT_RESIZE );
	
	state.SetField ( -1, "DRAWING_PIPELINE",	( u32 )MOAIGfxPipelineClerk::DRAWING_PIPELINE );
	state.SetField ( -1, "LOADING_PIPELINE",	( u32 )MOAIGfxPipelineClerk::LOADING_PIPELINE );

	luaL_Reg regTable [] = {
		{ "enablePipelineLogging",		_enablePipelineLogging },
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIGfxMgr > },
		{ "getMaxTextureSize",			_getMaxTextureSize },
		{ "getMaxTextureUnits",			_getMaxTextureUnits },
		{ "getViewSize",				_getViewSize },
		{ "purgeResources",				_purgeResources },
		{ "renewResources",				_renewResources },
		{ "setDefaultTexture",			_setDefaultTexture },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxMgr > },
		{ "setPenColor",				_setPenColor },
		{ "setPenWidth",				_setPenWidth },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxMgr::ReportTextureAlloc ( cc8* name, size_t size ) {

	this->mTextureMemoryUsage += size;
	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAILogMessages::MOAITexture_MemoryUse_SDFS, "+", size, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxMgr::ReportTextureFree ( cc8* name, size_t size ) {

	this->mTextureMemoryUsage -= size;
	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAILogMessages::MOAITexture_MemoryUse_SDFS, "-", size, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxMgr::ResetDrawCount () {
	//this->mDrawCount = 0;
}

//----------------------------------------------------------------//
void MOAIGfxMgr::SetBufferScale ( float scale ) {

	this->mGfxState.GetDefaultFrameBuffer ()->SetBufferScale ( scale );
}

//----------------------------------------------------------------//
void MOAIGfxMgr::SetBufferSize ( u32 width, u32 height ) {

	this->mGfxState.GetDefaultFrameBuffer ()->SetBufferSize ( width, height );
}
