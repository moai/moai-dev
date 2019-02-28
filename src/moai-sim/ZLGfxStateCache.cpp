// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/ZLGfxStateCache.h>

//================================================================//
// ZLGfxStateCache
//================================================================//

//----------------------------------------------------------------//
void ZLGfxStateCache::FinishFrame () {

	this->FlushVertexCache (); // TODO: need to do this here?
	this->UnbindAll ();
	this->Reset ();
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxStateCache::GetNormToWndMtx () {

	return this->GetNormToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxStateCache::GetNormToWndMtx ( const ZLRect& wndRect ) {

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
ZLMatrix4x4 ZLGfxStateCache::GetWorldToWndMtx () {

	return this->GetWorldToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxStateCache::GetWorldToWndMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 worldToWnd = this->GetMtx ( WORLD_TO_CLIP_MTX );
	worldToWnd.Append ( this->GetNormToWndMtx ( wndRect ));
	
	return worldToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxStateCache::GetWndToNormMtx () {

	return this->GetWndToNormMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxStateCache::GetWndToNormMtx ( const ZLRect& wndRect ) {

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
ZLMatrix4x4 ZLGfxStateCache::GetWndToWorldMtx () {

	return this->GetWndToWorldMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLGfxStateCache::GetWndToWorldMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 wndToWorld = this->GetWndToNormMtx ( wndRect );
	
	// inv viewproj
	ZLMatrix4x4 mtx = this->GetMtx ( WORLD_TO_CLIP_MTX );
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
ZLGfxStateCache::ZLGfxStateCache () :
	mStateStackTop ( ZLIndexOp::ZERO ) {
}

//----------------------------------------------------------------//
ZLGfxStateCache::~ZLGfxStateCache () {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mStateStack.Size (); ++i ) {
		delete this->mStateStack [ i ];
	}
}

//----------------------------------------------------------------//
void ZLGfxStateCache::PopState () {

	assert ( this->mStateStackTop > 0 );
	
	this->FlushVertexCache ();
	
	ZLGfxStateFrame* frame = this->mStateStack [ --this->mStateStackTop ];
	
	this->RestoreCPUState ( *frame );
	this->RestoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void ZLGfxStateCache::PushState () {

	this->FlushVertexCache ();

	this->mStateStack.Grow (( ZLSize )this->mStateStackTop + 1, 0 );
	if ( !this->mStateStack [ this->mStateStackTop ]) {
		this->mStateStack [ this->mStateStackTop ] = new ZLGfxStateFrame ();
	}
	
	ZLGfxStateFrame* frame = this->mStateStack [ this->mStateStackTop++ ];
	
	this->StoreCPUState ( *frame );
	this->StoreGPUState ( *frame );
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
ZLGfxStateCPUCache& ZLGfxStateCache::ZLAbstractGfxStateCache_GetGfxStateCacheCPU () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxStateGPUCache& ZLGfxStateCache::ZLAbstractGfxStateCache_GetGfxStateCacheGPU () {
	return *this;
}

//----------------------------------------------------------------//
ZLGfxStateVertexCache& ZLGfxStateCache::ZLAbstractGfxStateCache_GetGfxVertexCache () {
	return *this;
}
