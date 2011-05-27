// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIStretchPatch2D.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserveColumns
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
/**	@name	reserveRows
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
/**	@name	reserveUVRects
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
/**	@name	setColumn
	@text	Set the stretch properties of a patch column.
	
	@in		MOAIStretchPatch2D self
	@in		number idx
	@in		number weight
	@in		boolean conStretch
	@out	nil
*/
int MOAIStretchPatch2D::_setColumn ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UNNB" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	float percent		= state.GetValue < float >( 3, 0.0f );
	bool canStretch		= state.GetValue < bool >( 4, false );

	MOAI_CHECK_INDEX ( idx, self->mCols.Size ())
	self->mCols [ idx ].mPercent = percent;
	self->mCols [ idx ].mCanStretch = canStretch;

	self->mNeedsUpdate = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
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
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRow
	@text	Set the stretch properties of a patch row.
	
	@in		MOAIStretchPatch2D self
	@in		number idx
	@in		number weight
	@in		boolean conStretch
	@out	nil
*/
int MOAIStretchPatch2D::_setRow ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "UNNB" )

	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	float percent		= state.GetValue < float >( 3, 0.0f );
	bool canStretch		= state.GetValue < bool >( 4, false );

	MOAI_CHECK_INDEX ( idx, self->mRows.Size ())
	self->mRows [ idx ].mPercent = percent;
	self->mRows [ idx ].mCanStretch = canStretch;

	self->mNeedsUpdate = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Set or load a texture for this deck.
	
	@in		MOAIStretchPatch2D self
	@in		variant texture		A MOAITexture, a MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITexture.QUANTIZE, MOAITexture.TRUECOLOR, MOAITexture.PREMULTIPLY_ALPHA
	@out	MOAITexture texture
*/
int MOAIStretchPatch2D::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchPatch2D, "U" )

	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );
	if ( self->mTexture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVRect
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
	
	MOAI_CHECK_INDEX ( idx, self->mUVRects.Size ())
	self->mUVRects [ idx ] = state.GetRect < float >( 3 );
	return 0;
}

//================================================================//
// MOAIStretchPatch2D
//================================================================//

//----------------------------------------------------------------//
bool MOAIStretchPatch2D::Bind () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	if ( !drawBuffer.SetTexture ( this->mTexture )) return false;
	USGLQuad::BindVertexFormat ( drawBuffer );

	return true;
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::Draw ( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	
	USVec2D stretch = transform.GetStretch ();
	
	USAffine2D noStretch;
	noStretch.Scale ( 1.0f / stretch.mX, 1.0f / stretch.mY );
	noStretch.Append ( transform );
	
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	drawBuffer.SetVtxTransform ( noStretch );
	
	this->UpdateParams ();
	this->Draw ( idx, remapper, stretch.mX, stretch.mY );
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::Draw ( u32 idx, MOAIDeckRemapper* remapper, float xStretch, float yStretch ) {

	USRect uvRect;
	u32 totalUVRects = this->mUVRects.Size ();

	idx = remapper ? remapper->Remap ( idx ) : idx;
	idx = ( idx - 1 ) % totalUVRects;

	if ( totalUVRects == 0 ) {
		uvRect.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
	}
	else {
		uvRect = this->mUVRects [ idx ];
	}

	float nativeWidth = this->mRect.Width ();
	float nativeHeight = this->mRect.Height ();
	
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
		yPatchScale = rectHeight / nativeWidth;
		yStretchPatchScale = yPatchScale;
	}
	
	u32 totalRows = this->mRows.Size ();
	u32 totalCols = this->mCols.Size ();
	
	USGLQuad quad;
	
	float uSpan = uvRect.mXMax - uvRect.mXMin;
	float vSpan = uvRect.mYMax - uvRect.mYMin;
	
	float y = yMin;
	float v = vMin;
	
	for ( u32 i = 0; i < totalRows; ++i ) {
		
		MOAIStretchPatchSpan& row = this->mRows [ i ];
		float vStep = row.mPercent * vSpan;
		
		float h = nativeWidth * row.mPercent;
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
void MOAIStretchPatch2D::Draw ( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );

	// TODO: don't think anyone will ever use this. BUT... implement later just the same
}

//----------------------------------------------------------------//
USRect MOAIStretchPatch2D::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	
	return this->mRect;
}

//----------------------------------------------------------------//
MOAIStretchPatch2D::MOAIStretchPatch2D () :
	mNeedsUpdate ( true ) {

	RTTI_SINGLE ( MOAIDeck )
	this->SetContentMask ( MOAIProp::CAN_DRAW );

	this->mRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIStretchPatch2D::~MOAIStretchPatch2D () {
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );

	this->MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "reserveColumns",		_reserveColumns },
		{ "reserveRows",		_reserveRows },
		{ "reserveUVRects",		_reserveUVRects },
		{ "setColumn",			_setColumn },
		{ "setRect",			_setRect },
		{ "setRow",				_setRow },
		{ "setTexture",			_setTexture },
		{ "setUVRect",			_setUVRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );

//	STLString path = state.GetField ( -1, "mPath", "" );
//	
//	if ( path.size ()) {
//		USFilename filename;
//		filename.Bless ( path.str ());
//		this->Load ( filename.mBuffer );
//	}
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );

//	STLString path = USFileSys::GetRelativePath ( this->mTexturePath );
//	state.SetField ( -1, "mPath", path.str ());
}

//----------------------------------------------------------------//
STLString MOAIStretchPatch2D::ToString () {

	STLString repr;

//	PrettyPrint ( repr, "mWidth", GetWidth ());
//	PrettyPrint ( repr, "mHeight", GetHeight ());
//	PrettyPrint ( repr, "U", GetU ());
//	PrettyPrint ( repr, "V", GetV ());

	return repr;
}

//----------------------------------------------------------------//
void MOAIStretchPatch2D::UpdateParams () {
	
	if ( !this->mNeedsUpdate ) return;
	
	this->mYFix = 0.0f;
	this->mYFlex = 0.0f;
	
	u32 totalRows = this->mRows.Size ();
	for ( u32 i = 0; i < totalRows; ++i ) {
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
	
	u32 totalCols = this->mCols.Size ();
	for ( u32 i = 0; i < totalCols; ++i ) {
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
