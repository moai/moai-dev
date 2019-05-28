// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/ZLFrameBufferGL.h>
#include <zl-gfx/ZLGfxDeviceGL.h>
#include <zl-gfx/ZLGfxMgrGL.h>

//================================================================//
// ZLGfxMgrGL
//================================================================//

//----------------------------------------------------------------//
void ZLGfxMgrGL::Clear () {

	this->ZLGfxStateGPUCacheGL::Clear ();
	this->ZLGfxStateVertexCache::Clear ();
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::ClearErrors () {

	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			while ( ZLGfxDeviceGL::GetError () != ZGL_ERROR_NONE );
		}
	#endif
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::DetectContext () {

	this->mHasContext = true;
	
	ZLGfxDeviceGL::Initialize ();
	
	u32 maxTextureUnits = ZLGfxDeviceGL::GetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
	this->InitTextureUnits ( maxTextureUnits );
	
	this->mMaxTextureSize = ZLGfxDeviceGL::GetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );

	// renew resources in immediate mode
	this->SelectDrawingAPI ();
	
	this->GetDefaultFrameBuffer ()->DetectGLFrameBufferID ( *this );
	
	// TODO: ZLGfx - does this need to be called after shader mgr setup>
	this->RenewResources ();
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::DetectFramebuffer () {
	
	this->GetDefaultFrameBuffer ()->DetectGLFrameBufferID ( *this );
	this->SetFrameBuffer ();
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::FinishFrame () {

	this->FlushVertexCache (); // TODO: need to do this here?
	this->UnbindAll ();
	this->Reset ();
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgrGL::GetNormToWndMtx () {

	return this->GetNormToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgrGL::GetNormToWndMtx ( const ZLRect& wndRect ) {

	float hWidth = wndRect.Width () * 0.5f;
	float hHeight = wndRect.Height () * 0.5f;

	// Wnd
	ZLMatrix4x4 normToWnd;
	normToWnd.Scale ( hWidth, -hHeight, 1.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Translate ( hWidth + wndRect.mXMin, hHeight + wndRect.mYMin, 0.0f );
	normToWnd.Append ( mtx );
	
	return normToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgrGL::GetWorldToWndMtx () {

	return this->GetWorldToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgrGL::GetWorldToWndMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 worldToWnd = this->GetMtx ( WORLD_TO_CLIP_MTX );
	worldToWnd.Append ( this->GetNormToWndMtx ( wndRect ));
	
	return worldToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgrGL::GetWndToNormMtx () {

	return this->GetWndToNormMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgrGL::GetWndToNormMtx ( const ZLRect& wndRect ) {

	float hWidth = wndRect.Width () * 0.5f;
	float hHeight = wndRect.Height () * 0.5f;

	// Inv Wnd
	ZLMatrix4x4 wndToNorm;
	wndToNorm.Translate ( -hWidth - wndRect.mXMin, -hHeight - wndRect.mYMin, 0.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
	wndToNorm.Append ( mtx );
	
	return wndToNorm;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgrGL::GetWndToWorldMtx () {

	return this->GetWndToWorldMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgrGL::GetWndToWorldMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 wndToWorld = this->GetWndToNormMtx ( wndRect );
	
	// inv viewproj
	ZLMatrix4x4 mtx = this->GetMtx ( WORLD_TO_CLIP_MTX );
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
u32 ZLGfxMgrGL::LogErrors () {

	// TODO: ZLGfx

	u32 count = 0;
	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			for ( u32 error = ZLGfxDeviceGL::GetError (); error != ZGL_ERROR_NONE; error = ZLGfxDeviceGL::GetError (), ++count ) {
//				MOAILogF ( 0, ZLLog::LOG_ERROR, MOAISTRING_MOAIGfxDevice_OpenGLError_S, ZLGfxDeviceGL::GetErrorString ( error ));
			}
		}
	#endif
	return count;
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::PopState () {

	assert ( this->mStateStackTop > 0 );
	
	this->FlushVertexCache ();
	
	ZLGfxStateFrameGL* frame = this->mStateStack [ --this->mStateStackTop ];
	
	this->RestoreCPUState ( *frame );
	this->RestoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::PushState () {

	this->FlushVertexCache ();

	this->mStateStack.Grow (( ZLSize )this->mStateStackTop + 1, 0 );
	if ( !this->mStateStack [ this->mStateStackTop ]) {
		this->mStateStack [ this->mStateStackTop ] = new ZLGfxStateFrameGL ();
	}
	
	ZLGfxStateFrameGL* frame = this->mStateStack [ this->mStateStackTop++ ];
	
	this->StoreCPUState ( *frame );
	this->StoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::ReportTextureAlloc ( cc8* name, size_t size ) {
	UNUSED ( name );
	UNUSED ( size );

	// TODO: ZLGfx

//	this->mTextureMemoryUsage += size;
//	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
//	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "+", size / 1024, mb, name );
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::ReportTextureFree ( cc8* name, size_t size ) {
	UNUSED ( name );
	UNUSED ( size );

	// TODO: ZLGfx

//	this->mTextureMemoryUsage -= size;
//	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
//	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "-", size / 1024, mb, name );
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::ResetDrawCount () {
	//this->mDrawCount = 0;
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::SetBufferScale ( float scale ) {

	this->GetDefaultFrameBuffer ()->SetBufferScale ( scale );
}

//----------------------------------------------------------------//
void ZLGfxMgrGL::SetBufferSize ( u32 width, u32 height ) {

	ZLFrameBufferGL* defaultFrameBuffer = this->GetDefaultFrameBuffer ();
	assert ( defaultFrameBuffer );
	defaultFrameBuffer->SetBufferSize ( width, height );
}

//----------------------------------------------------------------//
ZLGfxMgrGL::ZLGfxMgrGL () :
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
}

//----------------------------------------------------------------//
ZLGfxMgrGL::~ZLGfxMgrGL () {

	this->Clear ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mStateStack.Size (); ++i ) {
		delete this->mStateStack [ i ];
	}
	
	this->SetDefaultFrameBuffer ( 0 );
	this->SetDefaultTexture ( 0 );
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
ZLGfxMgrGL& ZLGfxMgrGL::ZLAbstractGfxStateCache_GetGfxMgr () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxPipelineClerkGL& ZLGfxMgrGL::ZLAbstractGfxStateCache_GetGfxPipelineClerk () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxResourceClerkGL& ZLGfxMgrGL::ZLAbstractGfxStateCache_GetGfxResourceClerk () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxStateCPUCache& ZLGfxMgrGL::ZLAbstractGfxStateCache_GetGfxStateCacheCPU () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxStateGPUCacheGL& ZLGfxMgrGL::ZLAbstractGfxStateCache_GetGfxStateCacheGPU () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxStateVertexCache& ZLGfxMgrGL::ZLAbstractGfxStateCache_GetGfxVertexCache () {
	return *this;
}
