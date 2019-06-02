// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIImageTextureGL.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIShaderMgrGL.h>
#include <moai-gfx-gl/MOAITexture2DGL.h>
#include <moai-gfx-gl/MOAIVertexArrayGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexFormatMgrGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxMgrGL::_enablePipelineLogging ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	MOAIGfxMgrGL::Get ().EnablePipelineLogging ( state.GetValue < bool >( 1, false ));

	ZLFileSys::DeleteDirectory ( GFX_PIPELINE_LOGGING_FOLDER, true, true );
	ZLFileSys::AffirmPath ( GFX_PIPELINE_LOGGING_FOLDER );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getFrameBuffer
	@text	Returns the frame buffer associated with the device.

	@out	MOAIFrameBufferGL frameBuffer
*/
int MOAIGfxMgrGL::_getFrameBuffer ( lua_State* L ) {

	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgrGL::Get ().GetDefaultFrameBuffer ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureSize
	@text	Returns the maximum texture size supported by device
 
	@out	number maxTextureSize
*/
int MOAIGfxMgrGL::_getMaxTextureSize ( lua_State* L ) {
	
	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgrGL::Get ().mMaxTextureSize );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureUnits
	@text	Returns the total number of texture units available on the device.

	@out	number maxTextureUnits
*/
int MOAIGfxMgrGL::_getMaxTextureUnits ( lua_State* L ) {

	lua_pushnumber ( L, ( double )MOAIGfxMgrGL::Get ().CountTextureUnits ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getViewSize
	@text	Returns the width and height of the view
 
	@out	number width
	@out	number height
*/
int MOAIGfxMgrGL::_getViewSize ( lua_State* L  ) {

	MOAIFrameBufferGL* frameBuffer = MOAICast < MOAIFrameBufferGL >( MOAIGfxMgrGL::Get ().GetCurrentFrameBuffer ());
	
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
int MOAIGfxMgrGL::_purgeResources ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 age = state.GetValue < u32 >( 1, 0 );

	MOAIGfxMgrGL::Get ().PurgeResources ( age );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	renewResources
	@text	Renews all resources.
 
	@out	nil
*/
int MOAIGfxMgrGL::_renewResources ( lua_State* L ) {
	MOAILuaState state ( L );

	MOAIGfxMgrGL::Get ().RenewResources ();
	
	return 0;
}

//================================================================//
// MOAIGfxMgrGL
//================================================================//

////----------------------------------------------------------------//
//void MOAIGfxMgrGL::Clear () {
//
//	this->MOAIGfxMgrGL_GPUCacheGL::Clear ();
//	this->MOAIGfxMgr_VertexCache::Clear ();
//}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ClearErrors () {

	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			while ( ZLGfxDevice::GetError () != ZGL_ERROR_NONE );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::DetectContext () {

	this->mHasContext = true;
	
	ZLGfxDevice::Initialize ();
	
	u32 maxTextureUnits = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
	this->InitTextureUnits ( maxTextureUnits );
	
	this->mMaxTextureSize = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );

	// renew resources in immediate mode
	this->SelectDrawingAPI ();
	
	MOAICast < MOAIFrameBufferGL >( this->GetDefaultFrameBuffer ())->DetectGLFrameBufferID ( *this );
	
	// TODO: ZLGfx - does this need to be called after shader mgr setup>
	this->RenewResources ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::DetectFramebuffer () {
	
	MOAICast < MOAIFrameBufferGL >( this->GetDefaultFrameBuffer ())->DetectGLFrameBufferID ( *this );
	this->SetFrameBuffer ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::FinishFrame () {

	this->FlushToGPU (); // TODO: need to do this here?
	this->UnbindAll ();
	this->Reset ();
}

//----------------------------------------------------------------//
u32 MOAIGfxMgrGL::LogErrors () {

	// TODO: ZLGfx

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( u32 error = ZLGfxDevice::GetError (); error != ZGL_ERROR_NONE; error = ZLGfxDevice::GetError (), ++count ) {
//				MOAILogF ( 0, ZLLog::LOG_ERROR, MOAISTRING_MOAIGfxDevice_OpenGLError_S, ZLGfxDevice::GetErrorString ( error ));
			}
		}
	#endif
	return count;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL::MOAIGfxMgrGL () :
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
	mMaxTextureSize ( 0 ),
	mRenderCounter ( 0 ),
	mStateStackTop ( ZLIndexOp::ZERO ) {
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGfxMgr )
	RTTI_END
	
	this->SetDefaultFrameBuffer ( new MOAIFrameBufferGL ());
}

//----------------------------------------------------------------//
MOAIGfxMgrGL::~MOAIGfxMgrGL () {

	this->Clear ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mStateStack.Size (); ++i ) {
		delete this->mStateStack [ i ];
	}
	
	this->SetDefaultFrameBuffer ( 0 );
	this->SetDefaultTexture ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::OnGlobalsInitialize () {

	this->AffirmBuffers ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	state.SetField ( -1, "EVENT_RESIZE",	( u32 )EVENT_RESIZE );
	
	state.SetField ( -1, "DRAWING_PIPELINE",	( u32 )MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE );
	state.SetField ( -1, "LOADING_PIPELINE",	( u32 )MOAIGfxMgrGL_PipelineClerkGL::LOADING_PIPELINE );

	luaL_Reg regTable [] = {
		{ "enablePipelineLogging",		_enablePipelineLogging },
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIGfxMgrGL > },
		{ "getMaxTextureSize",			_getMaxTextureSize },
		{ "getMaxTextureUnits",			_getMaxTextureUnits },
		{ "getViewSize",				_getViewSize },
		{ "purgeResources",				_purgeResources },
		{ "renewResources",				_renewResources },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxMgrGL > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ReportTextureAlloc ( cc8* name, size_t size ) {
	UNUSED ( name );
	UNUSED ( size );

	// TODO: ZLGfx

//	this->mTextureMemoryUsage += size;
//	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
//	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "+", size / 1024, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ReportTextureFree ( cc8* name, size_t size ) {
	UNUSED ( name );
	UNUSED ( size );

	// TODO: ZLGfx

//	this->mTextureMemoryUsage -= size;
//	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
//	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "-", size / 1024, mb, name );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ResetDrawCount () {
	//this->mDrawCount = 0;
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgrGL::MOAIGfxMgr_AffirmShader ( MOAILuaState& state, int idx ) const {

	MOAIShaderGL* shader = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = MOAIShaderMgrGL::Get ().GetShader (( MOAIShaderPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShaderGL >( idx, true );
	}
	return shader;
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgrGL::MOAIGfxMgr_AffirmTexture ( MOAILuaState& state, int idx ) const {

	MOAITextureGL* textureBase = 0;
	
	textureBase = state.GetLuaObject < MOAITextureGL >( idx, false );
	if ( textureBase ) return textureBase;
	
	MOAITexture2DGL* texture = new MOAITexture2DGL ();
	if ( !texture->Init ( state, idx )) {
		// TODO: report error
		delete texture;
		texture = 0;
	}
	return texture;
}

//----------------------------------------------------------------//
MOAIImageTexture* MOAIGfxMgrGL::MOAIGfxMgr_CreateImageTexture () const {

	return new MOAIImageTextureGL ();
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIGfxMgrGL::MOAIGfxMgr_CreateIndexBuffer () const {

	return new MOAIIndexBufferGL ();
}

//----------------------------------------------------------------//
MOAITexture2D* MOAIGfxMgrGL::MOAIGfxMgr_CreateTexture2D () const {

	return new MOAITexture2DGL ();
}

//----------------------------------------------------------------//
MOAIVertexArray* MOAIGfxMgrGL::MOAIGfxMgr_CreateVertexArray () const {

	return new MOAIVertexArrayGL ();
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIGfxMgrGL::MOAIGfxMgr_CreateVertexBuffer () const {

	return new MOAIVertexBufferGL ();
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgrGL::MOAIGfxMgr_GetShaderPreset ( MOAIShaderPresetEnum preset ) const {

	return MOAIShaderMgrGL::Get ().GetShader ( preset );
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrGL::MOAIGfxMgr_GetTextureMemoryUsage () const {

	return this->mTextureMemoryUsage;
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgrGL::MOAIGfxMgr_GetVertexFormatPreset ( MOAIVertexFormatPresetEnum preset ) const {

	return MOAIVertexFormatMgrGL::Get ().GetFormat ( preset );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::MOAIGfxMgr_PopState () {

	assert ( this->mStateStackTop > 0 );
	
	this->FlushToGPU ();
	
	MOAIGfxStateFrameGL* frame = this->mStateStack [ --this->mStateStackTop ];
	
	this->RestoreCPUState ( *frame );
	this->RestoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::MOAIGfxMgr_PushState () {

	this->FlushToGPU ();

	this->mStateStack.Grow (( ZLSize )this->mStateStackTop + 1, 0 );
	if ( !this->mStateStack [ this->mStateStackTop ]) {
		this->mStateStack [ this->mStateStackTop ] = new MOAIGfxStateFrameGL ();
	}
	
	MOAIGfxStateFrameGL* frame = this->mStateStack [ this->mStateStackTop++ ];
	
	this->StoreCPUState ( *frame );
	this->StoreGPUState ( *frame );
}

//----------------------------------------------------------------//
MOAIGfxMgrGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetGfxMgrGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_GPUCacheGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetGPUCacheGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_PipelineClerkGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetPipelineClerkGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_RenderTreeGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetRenderTreeGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_ResourceClerkGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetResourceClerkGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_VertexCacheGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetVertexCacheGL () {
	return *this;
}
