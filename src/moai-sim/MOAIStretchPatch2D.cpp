// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIStretchPatch2D.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	ninePatch
	@text	Set up a nine patch using Android-style sections (25%, 50%, 25%).
 
	@in		MOAIStretchPatch2D self
	@out	nil
*/
int MOAIStretchPatch2D::_ninePatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "U" )

	self->mRows.Init ( 3 );
	self->SetRow ( 0, 0.25, false );
	self->SetRow ( 1, 0.50, true );
	self->SetRow ( 2, 0.25, false );

	self->mCols.Init ( 3 );
	self->SetColumn ( 0, 0.25, false );
	self->SetColumn ( 1, 0.50, true );
	self->SetColumn ( 2, 0.25, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveColumns
	@text	Reserve total columns in patch.
	
	@in		MOAIStretchPatch2D self
	@in		number nColumns
	@out	nil
*/
int MOAIStretchPatch2D::_reserveColumns ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mCols.Init ( total );

	self->mNeedsUpdate = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveRows
	@text	Reserve total rows in patch.
	
	@in		MOAIStretchPatch2D self
	@in		number nRows
	@out	nil
*/
int MOAIStretchPatch2D::_reserveRows ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mRows.Init ( total );

	self->mNeedsUpdate = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveUVRects
	@text	Reserve total UV rects in patch. When a patch is indexed
			it will change its UV rects.
	
	@in		MOAIStretchPatch2D self
	@in		number nUVRects
	@out	nil
*/
int MOAIStretchPatch2D::_reserveUVRects ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mUVRects.Init ( total );

	for ( u32 i = 0; i < total; ++i ) {
		self->mUVRects [ i ].Init ( 0.0f, 1.0f, 1.0f, 0.0f );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setColumn
	@text	Set the stretch properties of a patch column.
	
	@in		MOAIStretchPatch2D self
	@in		number idx
	@in		number percent
	@in		boolean canStretch
	@out	nil
*/
int MOAIStretchPatch2D::_setColumn ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UNNB" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	float percent		= state.GetValue < float >( 3, 0.0f );
	bool canStretch		= state.GetValue < bool >( 4, false );

	if ( MOAILogMgr::CheckIndexPlusOne ( idx, self->mCols.Size (), L )) {
		self->SetColumn ( idx, percent, canStretch );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Set the model space dimensions of the patch.
	
	@in		MOAIStretchPatch2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIStretchPatch2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UNNNN" )
	
	self->mRect = state.GetRect < float >( 2 );
	self->SetBoundsDirty ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRow
	@text	Set the stretch properties of a patch row.
	
	@in		MOAIStretchPatch2D self
	@in		number idx
	@in		number percent
	@in		boolean canStretch
	@out	nil
*/
int MOAIStretchPatch2D::_setRow ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UNNB" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	float percent		= state.GetValue < float >( 3, 0.0f );
	bool canStretch		= state.GetValue < bool >( 4, false );

	if ( MOAILogMgr::CheckIndexPlusOne ( idx, self->mRows.Size (), L )) {
		self->SetRow ( idx, percent, canStretch );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUVRect
	@text	Set the UV space dimensions of the patch.
	
	@in		MOAIStretchPatch2D self
	@in		number idx
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIStretchPatch2D::_setUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UNNNNN" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	if ( MOAILogMgr::CheckIndexPlusOne ( idx, self->mUVRects.Size (), L )) {
		self->mUVRects [ idx ] = state.GetRect < float >( 3 );
	}
	return 0;
}

//================================================================//
// MOAIStretchPatch2D
//================================================================//

//----------------------------------------------------------------//
void MOAIStretchPatch2D::DrawStretch ( ZLIndex idx, float xStretch, float yStretch ) {

	ZLRect uvRect;
	u32 totalUVRects = ( u32 )this->mUVRects.Size ();
	
	if ( totalUVRects == 0 ) {
		uvRect.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
	}
	else {
		idx.Sub ( 1, totalUVRects );
		uvRect = this->mUVRects [ idx.mKey ];
	}

	float nativeWidth = this->mRect.Width ();
	float nativeHeight = this->mRect.Height ();
	
	// TODO: make optional
	// xStretch /= nativeWidth;
	// yStretch /= nativeHeight;
	
	float rectWidth = nativeWidth * xStretch;
	float rectHeight = nativeHeight * yStretch;
	
	float xMin = this->mRect.mXMin * xStretch;
	float yMin = this->mRect.mYMin * yStretch;

	float uMin = uvRect.mXMin;
	float vMin = uvRect.mYMin;

	// scale for x patches
	float xPatchScale = 1.0f;
	float xStretchPatchScale = 1.0f;
	
	if ( rectWidth > nativeWidth ) {
		xStretchPatchScale = ( rectWidth - ( nativeWidth * this->mXFix )) / ( nativeWidth * this->mXFlex );
	}
	else {
		xPatchScale = rectWidth / nativeWidth;
		xStretchPatchScale = xPatchScale;
	}

	// scale for y patches
	float yPatchScale = 1.0f;
	float yStretchPatchScale = 1.0f;
	
	if ( rectHeight > nativeHeight ) {
		yStretchPatchScale = ( rectHeight - ( nativeHeight * this->mYFix )) / ( nativeHeight * this->mYFlex );
	}
	else {
		yPatchScale = rectHeight / nativeHeight;
		yStretchPatchScale = yPatchScale;
	}
	
	u32 totalRows = ( u32 )this->mRows.Size ();
	u32 totalCols = ( u32 )this->mCols.Size ();
	
	MOAIQuadBrush quad;
	
	float uSpan = uvRect.mXMax - uvRect.mXMin;
	float vSpan = uvRect.mYMax - uvRect.mYMin;
	
	float y = yMin;
	float v = vMin;
	
	for ( u32 i = 0; i < totalRows; ++i ) {
		
		MOAIStretchPatchSpan& row = this->mRows [ i ];
		float vStep = row.mPercent * vSpan;
		
		float h = nativeHeight * row.mPercent;
		if ( row.mCanStretch ) {
			h *= yStretchPatchScale;
		}
		else {
			h *= yPatchScale;
		}
		
		float x = xMin;
		float u = uMin;
		
		for ( u32 j = 0; j < totalCols; ++j ) {
			
			MOAIStretchPatchSpan& col = this->mCols [ j ];
			float uStep = col.mPercent * uSpan;
			
			float w = nativeWidth * col.mPercent;
			if ( col.mCanStretch ) {
				w *= xStretchPatchScale;
			}
			else {
				w *= xPatchScale;
			}
			
			quad.SetVerts ( x, y, x + w, y + h );
			quad.SetUVs ( u, v, u + uStep, v + vStep );
			quad.Draw ();
			
			x += w;
			u += uStep;
		}
		
		y += h;
		v += vStep;
	}
}

//----------------------------------------------------------------//
MOAIStretchPatch2D::MOAIStretchPatch2D () :
	mNeedsUpdate ( true ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIMaterialBatchHolder )
	RTTI_END
	
	this->mRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIStretchPatch2D::~MOAIStretchPatch2D () {
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
	MOAIMaterialBatchHolder::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	MOAIMaterialBatchHolder::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "ninePatch",			_ninePatch },
		{ "reserveColumns",		_reserveColumns },
		{ "reserveRows",		_reserveRows },
		{ "reserveUVRects",		_reserveUVRects },
		{ "setColumn",			_setColumn },
		{ "setRect",			_setRect },
		{ "setRow",				_setRow },
		{ "setUVRect",			_setUVRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::UpdateParams () {
	
	if ( !this->mNeedsUpdate ) return;
	
	this->mYFix = 0.0f;
	this->mYFlex = 0.0f;
	
	size_t totalRows = this->mRows.Size ();
	for ( size_t i = 0; i < totalRows; ++i ) {
		MOAIStretchPatchSpan& span = this->mRows [ i ];
		if ( span.mCanStretch ) {
			this->mYFlex += span.mPercent;
		}
		else {
			this->mYFix += span.mPercent;
		}
	}
	
	this->mXFix = 0.0f;
	this->mXFlex = 0.0f;
	
	size_t totalCols = this->mCols.Size ();
	for ( size_t i = 0; i < totalCols; ++i ) {
		MOAIStretchPatchSpan& span = this->mCols [ i ];
		if ( span.mCanStretch ) {
			this->mXFlex += span.mPercent;
		}
		else {
			this->mXFix += span.mPercent;
		}
	}
	
	this->mNeedsUpdate = false;
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::SetColumn ( u32 idx, float percent, bool canStretch ) {

	assert ( idx < this->mCols.Size ());

	this->mCols [ idx ].mPercent = percent;
	this->mCols [ idx ].mCanStretch = canStretch;
	this->mNeedsUpdate = true;
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::SetRow ( u32 idx, float percent, bool canStretch ) {

	assert ( idx < this->mRows.Size ());

	this->mRows [ idx ].mPercent = percent;
	this->mRows [ idx ].mCanStretch = canStretch;
	this->mNeedsUpdate = true;
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIStretchPatch2D::MOAIDeck_ComputeMaxBounds () {

	return this->GetBounds ( 0 );
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::MOAIDeck_Draw ( ZLIndex idx ) {
	
	this->UpdateParams ();
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIQuadBrush::BindVertexFormat ();
	
	ZLMatrix4x4 worldTransform = gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX );
	ZLVec3D stretch = worldTransform.GetStretch ();
	
	ZLMatrix4x4 noStretch;
	noStretch.Scale ( 1.0f / stretch.mX, 1.0f / stretch.mY, 1.0f / stretch.mZ );
	noStretch.Append ( worldTransform );
	noStretch.Append ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX ));
	
	gfxMgr.mVertexCache.SetVertexTransform ( noStretch );
	gfxMgr.mVertexCache.SetUVTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::UV_TO_MODEL_MTX ));
	
	MOAIMaterialMgr& materialStack = MOAIMaterialMgr::Get ();
	materialStack.Push ( this->GetMaterial ( idx.mKey ));
	materialStack.SetShader ( MOAIShaderMgr::DECK2D_SHADER );
	materialStack.LoadGfxState ();
	
	this->DrawStretch ( idx, stretch.mX, stretch.mY );
	
	materialStack.Pop ();
}

//----------------------------------------------------------------//
ZLBounds MOAIStretchPatch2D::MOAIDeck_GetBounds ( ZLIndex idx ) {
	UNUSED ( idx );

	return ZLBounds ( this->mRect );
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIStretchPatch2D::MOAIDeck_GetCollisionShape ( ZLIndex idx ) {
	UNUSED ( idx );

	return 0;
}

//----------------------------------------------------------------//
bool MOAIStretchPatch2D::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( granularity );
	UNUSED ( result );

	return false;
}

//----------------------------------------------------------------//
bool MOAIStretchPatch2D::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( granularity );
	UNUSED ( result );

	return false;
}
