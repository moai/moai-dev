// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAIDrawGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIImageTextureGL.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIMeshGL.h>
#include <moai-gfx-gl/MOAIRenderRootGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIShaderMgrGL.h>
#include <moai-gfx-gl/MOAITexture2DGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxMgrGL::_enablePipelineLogging ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	self->EnableQueueLogging ( state.GetValue < bool >( 1, false ));

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
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	state.Push ( self->GetDefaultFrameBuffer ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureSize
	@text	Returns the maximum texture size supported by device
 
	@out	number maxTextureSize
*/
int MOAIGfxMgrGL::_getMaxTextureSize ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )
	
	state.Push ( self->mMaxTextureSize );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureUnits
	@text	Returns the total number of texture units available on the device.

	@out	number maxTextureUnits
*/
int MOAIGfxMgrGL::_getMaxTextureUnits ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	lua_pushnumber ( L, ( double )self->CountTextureUnits ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getViewSize
	@text	Returns the width and height of the view
 
	@out	number width
	@out	number height
*/
int MOAIGfxMgrGL::_getViewSize ( lua_State* L  ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	MOAIFrameBufferGL* frameBuffer = MOAICast < MOAIFrameBufferGL >( self->GetFrameBuffer ());
	
	lua_pushnumber ( L, frameBuffer->GetWidth ());
	lua_pushnumber ( L, frameBuffer->GetHeight ());
	
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
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	u32 age = state.GetValue < u32 >( 1, 0 );

	self->PurgeResources ( age );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	renewResources
	@text	Renews all resources.
 
	@out	nil
*/
int MOAIGfxMgrGL::_renewResources ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	self->RenewResources ();
	
	return 0;
}

//================================================================//
// MOAIGfxMgrGL
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrGL::ClearErrors () {

	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			while ( ZLGfxDevice::GetError () != ZLGfxEnum::ERROR_NONE );
		}
	#endif
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::DetectContext () {

	this->mHasContext = true;
	
	ZLGfxDevice::Initialize ();
	
	u32 maxTextureUnits = ZLGfxDevice::GetCap ( ZLGfxEnum::CAPS_MAX_TEXTURE_UNITS );
	this->InitTextureUnits ( maxTextureUnits );
	
	this->mMaxTextureSize = ZLGfxDevice::GetCap ( ZLGfxEnum::CAPS_MAX_TEXTURE_SIZE );

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
u32 MOAIGfxMgrGL::LogErrors () {

	// TODO: ZLGfx

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( u32 error = ZLGfxDevice::GetError (); error != ZLGfxEnum::ERROR_NONE; error = ZLGfxDevice::GetError (), ++count ) {
//				MOAILogF ( 0, ZLLog::LOG_ERROR, MOAISTRING_MOAIGfxDevice_OpenGLError_S, ZLGfxDevice::GetErrorString ( error ));
			}
		}
	#endif
	return count;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL::MOAIGfxMgrGL ( ZLContext& context ) :
	ZLHasContext ( context ),
	ZLContextClass ( context ),
	MOAILuaObject ( context ),
	MOAIEventSource ( context ),
	MOAIGlobalEventSource ( context ),
	MOAIGfxMgrComponents ( context ),
	MOAIGfxMgr_CPUCache ( context ),
	MOAIGfxMgr_GPUCache ( context ),
	MOAIGfxMgr_RenderTree ( context ),
	MOAIGfxMgr_VertexCache ( context ),
	MOAIGfxMgr ( context ),
	MOAIGfxMgrGLComponents ( context ),
	MOAIGfxMgrGL_DisplayListClerkGL ( context ),
	MOAIGfxMgrGL_GPUCacheGL ( context ),
	MOAIGfxMgrGL_ResourceClerkGL ( context ),
	MOAIGfxMgrGL_VertexCacheGL ( context ),

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
	mStateStackTop ( 0 ) {
	
	RTTI_BEGIN ( MOAIGfxMgrGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxMgrGL >)
		RTTI_EXTEND ( MOAIGfxMgr )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxMgrGL::~MOAIGfxMgrGL () {

	this->Clear ();

	for ( ZLIndex i = 0; i < this->mStateStack.Size (); ++i ) {
		delete this->mStateStack [ i ];
	}
	
	this->SetDefaultFrameBuffer ( 0 );
	this->SetDefaultTexture ( 0 );
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
void MOAIGfxMgrGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "EVENT_RESIZE",	( u32 )EVENT_RESIZE );
	
	state.SetField ( -1, "DRAWING_QUEUE",	( u32 )MOAIGfxMgrGL_DisplayListClerkGL::DRAWING_QUEUE );
	state.SetField ( -1, "LOADING_QUEUE",	( u32 )MOAIGfxMgrGL_DisplayListClerkGL::LOADING_QUEUE );

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
MOAIShader* MOAIGfxMgrGL::MOAIGfxMgr_AffirmShader ( MOAILuaState& state, int idx ) const {

	MOAIShaderGL* shader = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = this->Get < MOAIShaderMgrGL >().GetShader (( MOAIShaderPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER ));
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
	
	MOAITexture2DGL* texture = new MOAITexture2DGL ( this->GetContext ());
	if ( !texture->Init ( state, idx )) {
		// TODO: report error
		delete texture;
		texture = 0;
	}
	return texture;
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::MOAIGfxMgr_BeginFrame () {

	this->ResetDrawingAPIs ();
	this->UpdateResources ();
	
	MOAIGfxMgrGL& gfxMgr = this->Get < MOAIGfxMgrGL >();
	ZLGfx* gfx = this->SelectDrawingAPI ( MOAIGfxMgrGL_DisplayListClerkGL::DRAWING_QUEUE );
	if ( gfx ) {
		ZGL_COMMENT ( *gfx, "RENDER MGR RENDER" );
	}
}

//----------------------------------------------------------------//
MOAIImageTexture* MOAIGfxMgrGL::MOAIGfxMgr_CreateImageTexture () {

	return new MOAIImageTextureGL ( this->GetContext ());
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIGfxMgrGL::MOAIGfxMgr_CreateIndexBuffer () {

	return new MOAIIndexBufferGL ( this->GetContext ());
}

//----------------------------------------------------------------//
MOAIMesh* MOAIGfxMgrGL::MOAIGfxMgr_CreateMesh () {

	return new MOAIMeshGL ( this->GetContext ());
}

//----------------------------------------------------------------//
MOAIRenderNode* MOAIGfxMgrGL::MOAIGfxMgr_CreateRenderRoot () {

	return new MOAIRenderRootGL ( this->GetContext ());
}

//----------------------------------------------------------------//
MOAITexture2D* MOAIGfxMgrGL::MOAIGfxMgr_CreateTexture2D () {

	return new MOAITexture2DGL ( this->GetContext ());
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIGfxMgrGL::MOAIGfxMgr_CreateVertexBuffer () {

	return new MOAIVertexBufferGL ( this->GetContext ());
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgrGL::MOAIGfxMgr_CreateVertexFormat () {

	return new MOAIVertexFormatGL ( this->GetContext ());
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::MOAIGfxMgr_EndFrame () {

	this->FlushToGPU (); // TODO: need to do this here?
	this->UnbindAll ();
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgrGL::MOAIGfxMgr_GetShaderPreset ( MOAIShaderPresetEnum preset ) const {

	return this->Get < MOAIShaderMgrGL >().GetShader ( preset );
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrGL::MOAIGfxMgr_GetTextureMemoryUsage () const {

	return this->mTextureMemoryUsage;
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
MOAIGfxMgrGL_DisplayListClerkGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetDisplayListClerkGL () {
	return *this;
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
MOAIGfxMgrGL_ResourceClerkGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetResourceClerkGL () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_VertexCacheGL& MOAIGfxMgrGL::MOAIGfxMgrGLComponents_GetVertexCacheGL () {
	return *this;
}
