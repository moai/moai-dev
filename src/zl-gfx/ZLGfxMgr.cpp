// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/ZLFrameBuffer.h>
#include <zl-gfx/ZLGfxDevice.h>
#include <zl-gfx/ZLGfxMgr.h>

//================================================================//
// ZLGfxMgr
//================================================================//

//----------------------------------------------------------------//
void ZLGfxMgr::ClearErrors () {

	#ifndef MOAI_OS_NACL
		if ( this->mHasContext ) {
			while ( ZLGfxDevice::GetError () != ZGL_ERROR_NONE );
		}
	#endif
}

//----------------------------------------------------------------//
void ZLGfxMgr::DetectContext () {

	this->mHasContext = true;
	
	ZLGfxDevice::Initialize ();
	
	u32 maxTextureUnits = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
	this->InitTextureUnits ( maxTextureUnits );
	
	this->mMaxTextureSize = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );

	// renew resources in immediate mode
	this->SelectDrawingAPI ();
	
	this->GetDefaultFrameBuffer ()->DetectGLFrameBufferID ( *this );
	
	// TODO: ZLGfx - does this need to be called after shader mgr setup>
	this->RenewResources ();
}

//----------------------------------------------------------------//
void ZLGfxMgr::DetectFramebuffer () {
	
	this->GetDefaultFrameBuffer ()->DetectGLFrameBufferID ( *this );
	this->SetFrameBuffer ();
}

//----------------------------------------------------------------//
void ZLGfxMgr::FinishFrame () {

	this->FlushVertexCache (); // TODO: need to do this here?
	this->UnbindAll ();
	this->Reset ();
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgr::GetNormToWndMtx () {

	return this->GetNormToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgr::GetNormToWndMtx ( const ZLRect& wndRect ) {

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
ZLMatrix4x4 ZLGfxMgr::GetWorldToWndMtx () {

	return this->GetWorldToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgr::GetWorldToWndMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 worldToWnd = this->GetMtx ( WORLD_TO_CLIP_MTX );
	worldToWnd.Append ( this->GetNormToWndMtx ( wndRect ));
	
	return worldToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgr::GetWndToNormMtx () {

	return this->GetWndToNormMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgr::GetWndToNormMtx ( const ZLRect& wndRect ) {

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
ZLMatrix4x4 ZLGfxMgr::GetWndToWorldMtx () {

	return this->GetWndToWorldMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxMgr::GetWndToWorldMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 wndToWorld = this->GetWndToNormMtx ( wndRect );
	
	// inv viewproj
	ZLMatrix4x4 mtx = this->GetMtx ( WORLD_TO_CLIP_MTX );
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
u32 ZLGfxMgr::LogErrors () {

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
void ZLGfxMgr::PopState () {

	assert ( this->mStateStackTop > 0 );
	
	this->FlushVertexCache ();
	
	ZLGfxStateFrame* frame = this->mStateStack [ --this->mStateStackTop ];
	
	this->RestoreCPUState ( *frame );
	this->RestoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void ZLGfxMgr::PushState () {

	this->FlushVertexCache ();

	this->mStateStack.Grow (( ZLSize )this->mStateStackTop + 1, 0 );
	if ( !this->mStateStack [ this->mStateStackTop ]) {
		this->mStateStack [ this->mStateStackTop ] = new ZLGfxStateFrame ();
	}
	
	ZLGfxStateFrame* frame = this->mStateStack [ this->mStateStackTop++ ];
	
	this->StoreCPUState ( *frame );
	this->StoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void ZLGfxMgr::ReportTextureAlloc ( cc8* name, size_t size ) {
	UNUSED ( name );
	UNUSED ( size );

	// TODO: ZLGfx

//	this->mTextureMemoryUsage += size;
//	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
//	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "+", size / 1024, mb, name );
}

//----------------------------------------------------------------//
void ZLGfxMgr::ReportTextureFree ( cc8* name, size_t size ) {
	UNUSED ( name );
	UNUSED ( size );

	// TODO: ZLGfx

//	this->mTextureMemoryUsage -= size;
//	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
//	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "-", size / 1024, mb, name );
}

//----------------------------------------------------------------//
void ZLGfxMgr::ResetDrawCount () {
	//this->mDrawCount = 0;
}

//----------------------------------------------------------------//
void ZLGfxMgr::SetBufferScale ( float scale ) {

	this->GetDefaultFrameBuffer ()->SetBufferScale ( scale );
}

//----------------------------------------------------------------//
void ZLGfxMgr::SetBufferSize ( u32 width, u32 height ) {

	this->GetDefaultFrameBuffer ()->SetBufferSize ( width, height );
}

//----------------------------------------------------------------//
ZLGfxMgr::ZLGfxMgr () :
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
ZLGfxMgr::~ZLGfxMgr () {

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
ZLGfxMgr& ZLGfxMgr::ZLAbstractGfxStateCache_GetGfxMgr () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxPipelineClerk& ZLGfxMgr::ZLAbstractGfxStateCache_GetGfxPipelineClerk () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxResourceClerk& ZLGfxMgr::ZLAbstractGfxStateCache_GetGfxResourceClerk () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxStateCPUCache& ZLGfxMgr::ZLAbstractGfxStateCache_GetGfxStateCacheCPU () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxStateGPUCache& ZLGfxMgr::ZLAbstractGfxStateCache_GetGfxStateCacheGPU () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxStateVertexCache& ZLGfxMgr::ZLAbstractGfxStateCache_GetGfxVertexCache () {
	return *this;
}
