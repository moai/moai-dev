// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAINinePatch.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransformBase2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>() reserveColumns ( self, nCols )</tt>\n
\n
	Reserve the number of columns in the 9-patch. Should be 1 or more.
	@param self (in)
	@param nCols (in)
*/
int MOAINinePatch::_reserveColumns ( lua_State* L ) {
	LUA_SETUP ( MOAINinePatch, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mCols.Init ( total );

	self->mNeedsUpdate = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() reserveColumns ( self, nCols )</tt>\n
\n
	Reserve the number of rows in the 9-patch. Should be 1 or more.
	@param self (in)
	@param nCols (in)
*/
int MOAINinePatch::_reserveRows ( lua_State* L ) {
	LUA_SETUP ( MOAINinePatch, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mRows.Init ( total );

	self->mNeedsUpdate = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() reserveUVRects ( self, nRects )</tt>\n
\n
	Reserve number of source rects in the nine patch (for flipbook animation).
	@param self (in)
	@param nRects (in)
*/
int MOAINinePatch::_reserveUVRects ( lua_State* L ) {
	LUA_SETUP ( MOAINinePatch, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mUVRects.Init ( total );

	for ( u32 i = 0; i < total; ++i ) {
		self->mUVRects [ i ].Init ( 0.0f, 1.0f, 1.0f, 0.0f );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setColumn ( self, idx, weight, canStretch )</tt>\n
\n
	Set properties of a column.
	@param self (in)
	@param idx (in)
	@param weight (in)
	@param canStretch (in)
*/
int MOAINinePatch::_setColumn ( lua_State* L ) {
	LUA_SETUP ( MOAINinePatch, "UNNB" )

	u32 idx				= state.GetValue < u32 >( 2, 0 );
	float percent		= state.GetValue < float >( 3, 0.0f );
	bool canStretch		= state.GetValue < bool >( 4, false );

	if ( idx < self->mCols.Size ()) {
		self->mCols [ idx ].mPercent = percent;
		self->mCols [ idx ].mCanStretch = canStretch;
	}

	self->mNeedsUpdate = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setRect ( self, left, top, right, bottom )</tt>\n
\n
	Set the default geometry for the nine patch.
	@param self (in)
	@param left (in) Upper-left X coordinate.
	@param top (in) Upper-left Y coordinate.
	@param right (in) Lower-right X coordinate.
	@param bottom (in) Lower-right Y coordinate.
*/
int MOAINinePatch::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAINinePatch, "UNNNN" )
	
	self->mRect = state.GetRect < float >( 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setRow ( self, idx, weight, canStretch )</tt>\n
\n
	Set properties of a row.
	@param self (in)
	@param idx (in)
	@param weight (in)
	@param canStretch (in)
*/
int MOAINinePatch::_setRow ( lua_State* L ) {
	LUA_SETUP ( MOAINinePatch, "UNNB" )

	u32 idx				= state.GetValue < u32 >( 2, 0 );
	float percent		= state.GetValue < float >( 3, 0.0f );
	bool canStretch		= state.GetValue < bool >( 4, false );

	if ( idx < self->mRows.Size ()) {
		self->mRows [ idx ].mPercent = percent;
		self->mRows [ idx ].mCanStretch = canStretch;
	}

	self->mNeedsUpdate = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setTexture ( self, texture )</tt>\n
\n
	Set the source texture for the nine patch.
	@param self (in)
	@param texture (in) Can be a filename or a MOAITexture.
*/
int MOAINinePatch::_setTexture ( lua_State* L ) {
	LUA_SETUP ( MOAINinePatch, "U" )

	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setUVRect ( self, idx, left, top, right, bottom )</tt>\n
\n
	Set the source uv rect for the nine patch.
	@param self (in)
	@param idx (in)
	@param left (in) Upper-left X coordinate.
	@param top (in) Upper-left Y coordinate.
	@param right (in) Lower-right X coordinate.
	@param bottom (in) Lower-right Y coordinate.
*/
int MOAINinePatch::_setUVRect ( lua_State* L ) {
	LUA_SETUP ( MOAINinePatch, "UNNNNN" )
	
	u32 idx = state.GetValue < u32 >( 2, 0 );
	
	if ( idx < self->mUVRects.Size ()) {
		self->mUVRects [ idx ] = state.GetRect < float >( 3 );
	}
	return 0;
}

//================================================================//
// MOAINinePatch
//================================================================//

//----------------------------------------------------------------//
bool MOAINinePatch::Bind () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	if ( !drawBuffer.BindTexture ( this->mTexture )) return false;
	USGLQuad::BindVertexFormat ( drawBuffer );

	return true;
}

//----------------------------------------------------------------//
void MOAINinePatch::Draw ( MOAIDrawingMtx2D& transform, u32 idx ) {
	
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	drawBuffer.SetVtxTransform ( transform.GetLocalToWorldMtxNoStretch ());
	
	USVec2D stretch = transform.GetStretch ();
	
	this->UpdateParams ();
	this->Draw ( idx, stretch.mX, stretch.mY );
}

//----------------------------------------------------------------//
void MOAINinePatch::Draw ( u32 idx, float xStretch, float yStretch ) {

	USRect uvRect;

	u32 totalUVRects = this->mUVRects.Size ();
	if ( totalUVRects == 0 ) {
		uvRect.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
	}
	else {
		uvRect = this->mUVRects [ idx % totalUVRects ];
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
		
		MOAINinePatchSpan& row = this->mRows [ i ];
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
			
			MOAINinePatchSpan& col = this->mCols [ j ];
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
void MOAINinePatch::Draw ( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( c0 );
	UNUSED ( c1 );

	// TODO: don't think anyone will ever use this. BUT... implement later just the same
}

//----------------------------------------------------------------//
USRect MOAINinePatch::GetBounds ( u32 idx ) {
	UNUSED ( idx );
	
	return this->mRect;
}

//----------------------------------------------------------------//
MOAINinePatch::MOAINinePatch () :
	mNeedsUpdate ( true ) {

	RTTI_SINGLE ( MOAIContentLibrary2D )
	this->SetContentMask ( CAN_DRAW );

	this->mRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAINinePatch::~MOAINinePatch () {
}

//----------------------------------------------------------------//
void MOAINinePatch::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );

//	this->MOAIContentLibrary2D::RegisterLuaClass ( state );
//	
//	state.SetField ( -1, "FILTER_POINT", ( u32 )GL_NEAREST );
//	state.SetField ( -1, "FILTER_BILERP", ( u32 )GL_LINEAR );
}

//----------------------------------------------------------------//
void MOAINinePatch::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIContentLibrary2D::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
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
void MOAINinePatch::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
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
void MOAINinePatch::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );

//	STLString path = USFileSys::GetRelativePath ( this->mTexturePath );
//	state.SetField ( -1, "mPath", path.str ());
}

//----------------------------------------------------------------//
STLString MOAINinePatch::ToString () {

	STLString repr;

//	PrettyPrint ( repr, "mWidth", GetWidth ());
//	PrettyPrint ( repr, "mHeight", GetHeight ());
//	PrettyPrint ( repr, "U", GetU ());
//	PrettyPrint ( repr, "V", GetV ());

	return repr;
}

//----------------------------------------------------------------//
void MOAINinePatch::UpdateParams () {
	
	if ( !this->mNeedsUpdate ) return;
	
	this->mYFix = 0.0f;
	this->mYFlex = 0.0f;
	
	u32 totalRows = this->mRows.Size ();
	for ( u32 i = 0; i < totalRows; ++i ) {
		MOAINinePatchSpan& span = this->mRows [ i ];
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
		MOAINinePatchSpan& span = this->mCols [ i ];
		if ( span.mCanStretch ) {
			this->mXFlex += span.mPercent;
		}
		else {
			this->mXFix += span.mPercent;
		}
	}
	
	this->mNeedsUpdate = false;
}
