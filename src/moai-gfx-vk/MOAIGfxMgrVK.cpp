// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIImageTextureVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderMgrVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAIVertexArrayVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatMgrVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIGfxMgrVK::_enablePipelineLogging ( lua_State* L ) {
//	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrVK, "" )
//
//	MOAIGfxMgrVK::Get ().EnablePipelineLogging ( state.GetValue < bool >( 1, false ));
//
//	ZLFileSys::DeleteDirectory ( GFX_PIPELINE_LOGGING_FOLDER, true, true );
//	ZLFileSys::AffirmPath ( GFX_PIPELINE_LOGGING_FOLDER );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	getFrameBuffer
//	@text	Returns the frame buffer associated with the device.
//
//	@out	MOAIFrameBufferVK frameBuffer
//*/
//int MOAIGfxMgrVK::_getFrameBuffer ( lua_State* L ) {
//
//	MOAILuaState state ( L );
//	state.Push ( MOAIGfxMgrVK::Get ().GetDefaultFrameBuffer ());
//
//	return 1;
//}
//
////----------------------------------------------------------------//
///**	@lua	getMaxTextureSize
//	@text	Returns the maximum texture size supported by device
//
//	@out	number maxTextureSize
//*/
//int MOAIGfxMgrVK::_getMaxTextureSize ( lua_State* L ) {
//
//	MOAILuaState state ( L );
//	state.Push ( MOAIGfxMgrVK::Get ().mMaxTextureSize );
//
//	return 1;
//}
//
////----------------------------------------------------------------//
///**	@lua	getMaxTextureUnits
//	@text	Returns the total number of texture units available on the device.
//
//	@out	number maxTextureUnits
//*/
//int MOAIGfxMgrVK::_getMaxTextureUnits ( lua_State* L ) {
//
//	lua_pushnumber ( L, ( double )MOAIGfxMgrVK::Get ().CountTextureUnits ());
//
//	return 1;
//}
//
////----------------------------------------------------------------//
///**	@lua	getViewSize
//	@text	Returns the width and height of the view
//
//	@out	number width
//	@out	number height
//*/
//int MOAIGfxMgrVK::_getViewSize ( lua_State* L  ) {
//
//	MOAIFrameBufferVK* frameBuffer = MOAICast < MOAIFrameBufferVK >( MOAIGfxMgrVK::Get ().GetCurrentFrameBuffer ());
//
//	lua_pushnumber ( L, frameBuffer->GetBufferWidth ());
//	lua_pushnumber ( L, frameBuffer->GetBufferHeight ());
//
//	return 2;
//}
//
////----------------------------------------------------------------//
///**	@lua	purgeResources
//	@text	Purges all resources older that a given age (in render cycles).
//			If age is 0 then all resources are purged.
//
//	@opt	number age		Default value is 0.
//	@out	nil
//*/
//int MOAIGfxMgrVK::_purgeResources ( lua_State* L ) {
//	MOAILuaState state ( L );
//
//	u32 age = state.GetValue < u32 >( 1, 0 );
//
//	MOAIGfxMgrVK::Get ().PurgeResources ( age );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	renewResources
//	@text	Renews all resources.
//
//	@out	nil
//*/
//int MOAIGfxMgrVK::_renewResources ( lua_State* L ) {
//	MOAILuaState state ( L );
//
//	MOAIGfxMgrVK::Get ().RenewResources ();
//
//	return 0;
//}

//================================================================//
// MOAIGfxMgrVK
//================================================================//

////----------------------------------------------------------------//
//void MOAIGfxMgrVK::Clear () {
//
//	this->MOAIGfxMgrVK_GPUCacheVK::Clear ();
//	this->MOAIGfxMgr_VertexCache::Clear ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::ClearErrors () {
//
//	#ifndef MOAI_OS_NACL
//		if ( this->mHasContext ) {
//			while ( ZLGfxDevice::GetError () != ZGL_ERROR_NONE );
//		}
//	#endif
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::DetectContext () {
//
//	this->mHasContext = true;
//
//	ZLGfxDevice::Initialize ();
//
//	u32 maxTextureUnits = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
//	this->InitTextureUnits ( maxTextureUnits );
//
//	this->mMaxTextureSize = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );
//
//	// renew resources in immediate mode
//	this->SelectDrawingAPI ();
//
//	MOAICast < MOAIFrameBufferVK >( this->GetDefaultFrameBuffer ())->DetectGLFrameBufferID ( *this );
//
//	// TODO: ZLGfx - does this need to be called after shader mgr setup>
//	this->RenewResources ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::DetectFramebuffer () {
//
//	MOAICast < MOAIFrameBufferVK >( this->GetDefaultFrameBuffer ())->DetectGLFrameBufferID ( *this );
//	this->SetFrameBuffer ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::FinishFrame () {
//
//	this->FlushToGPU (); // TODO: need to do this here?
//	this->UnbindAll ();
//	this->Reset ();
//}
//
////----------------------------------------------------------------//
//u32 MOAIGfxMgrVK::LogErrors () {
//
//	// TODO: ZLGfx
//
//	u32 count = 0;
//	#ifndef MOAI_OS_NACL
//		if ( this->mHasContext ) {
//			for ( u32 error = ZLGfxDevice::GetError (); error != ZGL_ERROR_NONE; error = ZLGfxDevice::GetError (), ++count ) {
////				MOAILogF ( 0, ZLLog::LOG_ERROR, MOAISTRING_MOAIGfxDevice_OpenGLError_S, ZLGfxDevice::GetErrorString ( error ));
//			}
//		}
//	#endif
//	return count;
//}
//
////----------------------------------------------------------------//
//MOAIGfxMgrVK::MOAIGfxMgrVK () :
//	mHasContext ( false ),
//	mIsFramebufferSupported ( 0 ),
//	#if defined ( MOAI_OS_NACL ) || defined ( MOAI_OS_IPHONE ) || defined ( MOAI_OS_ANDROID ) || defined ( EMSCRIPTEN )
//		mIsOpenGLES ( true ),
//	#else
//		mIsOpenGLES ( false ),
//	#endif
//	mMajorVersion ( 0 ),
//	mMinorVersion ( 0 ),
//	mTextureMemoryUsage ( 0 ),
//	mMaxTextureSize ( 0 ),
//	mRenderCounter ( 0 ),
//	mStateStackTop ( ZLIndexOp::ZERO ) {
//
//	RTTI_BEGIN
//		RTTI_SINGLE ( MOAIGfxMgr )
//	RTTI_END
//
//	this->SetDefaultFrameBuffer ( new MOAIFrameBufferVK ());
//}

//----------------------------------------------------------------//
MOAIGfxMgrVK::MOAIGfxMgrVK () {
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGfxMgr )
	RTTI_END
	
//	this->SetDefaultFrameBuffer ( new MOAIFrameBufferVK ());
}

//----------------------------------------------------------------//
MOAIGfxMgrVK::~MOAIGfxMgrVK () {

//	this->Clear ();
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mStateStack.Size (); ++i ) {
//		delete this->mStateStack [ i ];
//	}
//
//	this->SetDefaultFrameBuffer ( 0 );
//	this->SetDefaultTexture ( 0 );
}

////----------------------------------------------------------------//
//void MOAIGfxMgrVK::OnGlobalsFinalize () {
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::OnGlobalsInitialize () {
//
//	this->AffirmBuffers ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::ReportTextureAlloc ( cc8* name, size_t size ) {
//	UNUSED ( name );
//	UNUSED ( size );
//
//	// TODO: ZLGfx
//
////	this->mTextureMemoryUsage += size;
////	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
////	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "+", size / 1024, mb, name );
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::ReportTextureFree ( cc8* name, size_t size ) {
//	UNUSED ( name );
//	UNUSED ( size );
//
//	// TODO: ZLGfx
//
////	this->mTextureMemoryUsage -= size;
////	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
////	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "-", size / 1024, mb, name );
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::ResetDrawCount () {
//	//this->mDrawCount = 0;
//}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgrVK::MOAIGfxMgr_AffirmShader ( MOAILuaState& state, int idx ) const {

//	MOAIShaderVK* shader = 0;
//
//	if ( state.IsType ( idx, LUA_TNUMBER )) {
//		shader = MOAIShaderMgrVK::Get ().GetShader (( MOAIShaderPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER ));
//	}
//	else {
//		shader = state.GetLuaObject < MOAIShaderVK >( idx, true );
//	}
//	return shader;
	return new MOAIShaderVK ();
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgrVK::MOAIGfxMgr_AffirmTexture ( MOAILuaState& state, int idx ) const {

//	MOAITextureVK* textureBase = 0;
//
//	textureBase = state.GetLuaObject < MOAITextureVK >( idx, false );
//	if ( textureBase ) return textureBase;
//
//	MOAITexture2DVK* texture = new MOAITexture2DVK ();
//	if ( !texture->Init ( state, idx )) {
//		// TODO: report error
//		delete texture;
//		texture = 0;
//	}
//	return texture;

	return new MOAITexture2DVK ();
}

//----------------------------------------------------------------//
MOAIImageTexture* MOAIGfxMgrVK::MOAIGfxMgr_CreateImageTexture () const {

	return new MOAIImageTextureVK ();
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIGfxMgrVK::MOAIGfxMgr_CreateIndexBuffer () const {

	return new MOAIIndexBufferVK ();
}

//----------------------------------------------------------------//
MOAITexture2D* MOAIGfxMgrVK::MOAIGfxMgr_CreateTexture2D () const {

	return new MOAITexture2DVK ();
}

//----------------------------------------------------------------//
MOAIVertexArray* MOAIGfxMgrVK::MOAIGfxMgr_CreateVertexArray () const {

	return new MOAIVertexArrayVK ();
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIGfxMgrVK::MOAIGfxMgr_CreateVertexBuffer () const {

	return new MOAIVertexBufferVK ();
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgrVK::MOAIGfxMgr_GetShaderPreset ( MOAIShaderPresetEnum preset ) const {

	return MOAIShaderMgrVK::Get ().GetShader ( preset );
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrVK::MOAIGfxMgr_GetTextureMemoryUsage () const {

//	return this->mTextureMemoryUsage;
	return 0;
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgrVK::MOAIGfxMgr_GetVertexFormatPreset ( MOAIVertexFormatPresetEnum preset ) const {

	return MOAIVertexFormatMgrVK::Get ().GetFormat ( preset );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::MOAIGfxMgr_PopState () {

//	assert ( this->mStateStackTop > 0 );
//
//	this->FlushToGPU ();
//
//	MOAIGfxStateFrameVK* frame = this->mStateStack [ --this->mStateStackTop ];
//
//	this->RestoreCPUState ( *frame );
//	this->RestoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::MOAIGfxMgr_PushState () {

//	this->FlushToGPU ();
//
//	this->mStateStack.Grow (( ZLSize )this->mStateStackTop + 1, 0 );
//	if ( !this->mStateStack [ this->mStateStackTop ]) {
//		this->mStateStack [ this->mStateStackTop ] = new MOAIGfxStateFrameVK ();
//	}
//
//	MOAIGfxStateFrameVK* frame = this->mStateStack [ this->mStateStackTop++ ];
//
//	this->StoreCPUState ( *frame );
//	this->StoreGPUState ( *frame );
}

//----------------------------------------------------------------//
MOAIGfxMgrVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetGfxMgrVK () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_GPUCacheVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetGPUCacheVK () {
	return *this;
}

////----------------------------------------------------------------//
//MOAIGfxMgrVK_PipelineClerkVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetPipelineClerkVK () {
//	return *this;
//}

//----------------------------------------------------------------//
MOAIGfxMgrVK_RenderTreeVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetRenderTreeVK () {
	return *this;
}

////----------------------------------------------------------------//
//MOAIGfxMgrVK_ResourceClerkVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetResourceClerkVK () {
//	return *this;
//}

//----------------------------------------------------------------//
MOAIGfxMgrVK_VertexCacheVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetVertexCacheVK () {
	return *this;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

//	state.SetField ( -1, "EVENT_RESIZE",	( u32 )EVENT_RESIZE );
//	
//	state.SetField ( -1, "DRAWING_PIPELINE",	( u32 )MOAIGfxMgrVK_PipelineClerkVK::DRAWING_PIPELINE );
//	state.SetField ( -1, "LOADING_PIPELINE",	( u32 )MOAIGfxMgrVK_PipelineClerkVK::LOADING_PIPELINE );
//
//	luaL_Reg regTable [] = {
//		{ "enablePipelineLogging",		_enablePipelineLogging },
//		{ "getFrameBuffer",				_getFrameBuffer },
//		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIGfxMgrVK > },
//		{ "getMaxTextureSize",			_getMaxTextureSize },
//		{ "getMaxTextureUnits",			_getMaxTextureUnits },
//		{ "getViewSize",				_getViewSize },
//		{ "purgeResources",				_purgeResources },
//		{ "renewResources",				_renewResources },
//		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxMgrVK > },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
}
