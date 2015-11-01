// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDeviceMtxCache.h>

//================================================================//
// MOAIGfxDeviceMtxCache
//================================================================//

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetNormToWndMtx () const {

	return this->GetNormToWndMtx ( this->mViewRect );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetNormToWndMtx ( const ZLRect& wndRect ) const {

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
void MOAIGfxDeviceMtxCache::GetUVMtxMode ( u32& input, u32& output ) const {

	input = this->mUVMtxInput;
	output = this->mUVMtxOutput;
}

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxDeviceMtxCache::GetUVTransform () const {

	return this->mUVTransform;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::GetVertexMtxMode ( u32& input, u32& output ) const {

	input = this->mVertexMtxInput;
	output = this->mVertexMtxOutput;
}

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxDeviceMtxCache::GetVertexTransform ( u32 id ) const {

	return this->mVertexTransforms [ id ];
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetViewProjMtx () const {

	ZLMatrix4x4 mtx = this->mVertexTransforms [ VTX_VIEW_TRANSFORM ];
	mtx.Append ( this->mVertexTransforms [ VTX_PROJ_TRANSFORM ]);
	return mtx;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetWorldToWndMtx () const {

	return this->GetWorldToWndMtx ( this->mViewRect );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetWorldToWndMtx ( const ZLRect& wndRect ) const {

	ZLMatrix4x4 worldToWnd = this->GetViewProjMtx ();
	worldToWnd.Append ( MOAIGfxDeviceMtxCache::GetNormToWndMtx ( wndRect ));
	
	return worldToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetWndToNormMtx () const {

	return this->GetWndToNormMtx ( this->mViewRect );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetWndToNormMtx ( const ZLRect& wndRect ) const {

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
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetWndToWorldMtx () const {

	return this->GetWndToWorldMtx ( this->mViewRect );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxDeviceMtxCache::GetWndToWorldMtx ( const ZLRect& wndRect ) const {

	ZLMatrix4x4 wndToWorld = MOAIGfxDeviceMtxCache::GetWndToNormMtx ( wndRect );
	
	// inv viewproj
	ZLMatrix4x4 mtx = this->GetViewProjMtx ();
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
MOAIGfxDeviceMtxCache::MOAIGfxDeviceMtxCache () :
	mVertexMtxInput ( VTX_STAGE_MODEL ),
	mVertexMtxOutput ( VTX_STAGE_MODEL ),
	mCpuVertexTransform ( false ),
	mUVMtxInput ( UV_STAGE_MODEL ),
	mUVMtxOutput ( UV_STAGE_MODEL ),
	mCpuUVTransform ( false ) {

	for ( u32 i = 0; i < TOTAL_VTX_TRANSFORMS; ++i ) {
		this->mVertexTransforms [ i ].Ident ();
		this->mCpuVertexTransformCache [ i ] = false;
		this->mCpuVertexTransformCacheMtx [ i ].Ident ();
	}
	this->mCpuVertexTransformMtx.Ident ();
	
	this->mUVTransform.Ident ();
}

//----------------------------------------------------------------//
MOAIGfxDeviceMtxCache::~MOAIGfxDeviceMtxCache () {
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::SetUVMtxMode ( u32 input, u32 output ) {

	if (( this->mUVMtxInput != input ) || ( this->mUVMtxOutput != output )) {
		
		this->mUVMtxInput = input;
		this->mUVMtxOutput = output;
		
		this->UpdateUVMtx ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::SetUVTransform () {

	ZLMatrix4x4 mtx;
	mtx.Ident ();
	this->SetUVTransform ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::SetUVTransform ( const ZLAffine3D& transform ) {

	ZLMatrix4x4 mtx;
	mtx.Init ( transform );
	this->SetUVTransform ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::SetUVTransform ( const ZLMatrix4x4& transform ) {

	if ( !this->mUVTransform.IsSame ( transform )) {
		this->mUVTransform = transform;
		this->UpdateUVMtx ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::SetVertexMtxMode ( u32 input, u32 output ) {
	
	if (( this->mVertexMtxInput != input ) || ( this->mVertexMtxOutput != output )) {

		this->mVertexMtxInput = input;
		this->mVertexMtxOutput = output;
		
		// Invalidate the lower level matrices (i.e. modelview, etc) matrix in this case to force recalc
		for ( u32 i = input; i < output; ++i ) {
			this->mCpuVertexTransformCache [ i ] = false;
		}
		
		this->UpdateCpuVertexMtx ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::SetVertexTransform ( u32 id ) {

	ZLMatrix4x4 mtx;
	mtx.Ident ();
	this->SetVertexTransform ( id, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::SetVertexTransform ( u32 id, const ZLAffine3D& transform ) {

	ZLMatrix4x4 mtx;
	mtx.Init ( transform );
	this->SetVertexTransform ( id, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::SetVertexTransform ( u32 id, const ZLMatrix4x4& transform ) {

	assert ( id < TOTAL_VTX_TRANSFORMS );

	if ( !this->mVertexTransforms [ id ].IsSame ( transform )) {

		this->mVertexTransforms [ id ] = transform;
		
		// check to see if this is a CPU or GPU matrix and update accordingly
		if ( id < this->mVertexMtxOutput ) {
		
			// Invalidate the lower level matrices (i.e. modelview, etc) matrix in this case to force recalc
			for ( u32 i = this->mVertexMtxInput; i <= id; ++i ) {
				this->mCpuVertexTransformCache [ i ] = false;
			}
			this->UpdateCpuVertexMtx ();
		}
	}
	
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::UpdateCpuVertexMtx () {
	
	// Used signed, so we can roll "under" to -1 without an extra range check
	int start = this->mVertexMtxInput;
	int finish = this->mVertexMtxOutput;

	// The matrices are being multiplied A*B*C, but the common case is that
	// B and C are static throughout all/most of a frame. Thus, we can
	// capitalize on the associativity of matrix multiplication by caching
	// (B*C) and save a matrix mult in the common case (assuming they haven't
	// changed since the last update request).

	int i = finish - 1;
	
	if ( this->mCpuVertexTransformCache [ i ]) {
		while ( i >= start && this->mCpuVertexTransformCache [ i ]) {
			--i;
		}
		this->mCpuVertexTransformMtx = this->mCpuVertexTransformCacheMtx [ i + 1 ];
	}
	else {
		this->mCpuVertexTransformMtx.Ident();
	}
	
	for ( ; i >= start; --i ) {
		this->mCpuVertexTransformMtx.Prepend ( this->mVertexTransforms [ i ]);
		this->mCpuVertexTransformCacheMtx [ i ] = this->mCpuVertexTransformMtx;
		this->mCpuVertexTransformCache [ i ] = true;
	}

	this->mCpuVertexTransform = !this->mCpuVertexTransformMtx.IsIdent ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::UpdateUVMtx () {

	if ( this->mUVMtxOutput == UV_STAGE_TEXTURE ) {
	
		this->mCpuUVTransform = !this->mUVTransform.IsIdent ();
	}
	else {
	
		this->mCpuUVTransform = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceMtxCache::UpdateViewVolume () {

	ZLMatrix4x4 invViewProj;
	invViewProj.Inverse ( this->GetViewProjMtx ());
	this->mViewVolume.Init ( invViewProj );
}
