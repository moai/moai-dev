// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIBrushLibrary.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILog.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransformBase2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>reserve ( self, size )</tt>\n
	\n
	Set total capacity of brush library.
	@param self (in)
	@param size (in)
*/
int MOAIBrushLibrary::_reserveBrushes ( lua_State* L ) {
	LUA_SETUP ( MOAIBrushLibrary, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mQuads.Init ( total );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() scaleCoords ( self, xScale, yScale )</tt>\n
\n
	Scales the screen coords in the library.
	@param self (in)
	@param xScale (in)
	@param yScale (in)
*/
int	MOAIBrushLibrary::_scaleCoords ( lua_State* L ) {
	LUA_SETUP ( MOAIBrushLibrary, "UNN" )

	float xScale = state.GetValue < float >( 2, 1.0f );
	float yScale = state.GetValue < float >( 3, 1.0f );

	self->ScaleScreenCoords ( xScale, yScale );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() scaleUVCoords ( self, xScale, yScale )</tt>\n
\n
	Scales the uv coords in the library.
	@param self (in)
	@param xScale (in)
	@param yScale (in)
*/
int	MOAIBrushLibrary::_scaleUVCoords ( lua_State* L ) {
	LUA_SETUP ( MOAIBrushLibrary, "UNN" )

	float xScale = state.GetValue < float >( 2, 1.0f );
	float yScale = state.GetValue < float >( 3, 1.0f );

	self->ScaleUVCoords ( xScale, yScale );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setRect ( self, idx, left, top, right, bottom )</tt>\n
	\n
	Sets the rectangular area to display the brush relative to the origin.
	@param self (in)
	@param idx Index of brush.
	@param left Upper-left X coordinate.
	@param top Upper-left Y coordinate.
	@param right Lower-right X coordinate.
	@param bottom Lower-right Y coordinate.
*/
int MOAIBrushLibrary::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAIBrushLibrary, "UNNNNN" )

	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAI_LUA_VALID_INDEX ( idx, self->mQuads.Size ())
	
	float x0	= state.GetValue < float >( 3, 0.0f );
	float y0	= state.GetValue < float >( 4, 0.0f );
	float x1	= state.GetValue < float >( 5, 0.0f );
	float y1	= state.GetValue < float >( 6, 0.0f );
	
	USGLQuad* quad = self->GetGLQuad ( idx );
	if ( quad ) {
		quad->SetVerts ( x0, y0, x1, y1 );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>reserve ( self, size )</tt>\n
	\n
	Set total capacity of brush library.
	@param self (in)
	@param size (in)
*/
int MOAIBrushLibrary::_setTexture ( lua_State* L ) {
	LUA_SETUP ( MOAIBrushLibrary, "U" )

	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setUVs ( self, idx, u0, v0, u1, v1 )</tt>\n
	\n
	Set the UV coordinates in the brush's source image.
	@param self (in)
	@param idx Index of brush.
	@param u0 Top-left U coordinate.
	@param v0 Top-left V coordinate.
	@param u1 Bottom-right U coordinate.
	@param v1 Bottom-right V coordinate.
*/
int MOAIBrushLibrary::_setUVRect ( lua_State* L ) {
	LUA_SETUP ( MOAIBrushLibrary, "UNNNNN" )

	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAI_LUA_VALID_INDEX ( idx, self->mQuads.Size ())
	
	float u0	= state.GetValue < float >( 3, 0.0f );
	float v0	= state.GetValue < float >( 4, 0.0f );
	float u1	= state.GetValue < float >( 5, 0.0f );
	float v1	= state.GetValue < float >( 6, 0.0f );

	USGLQuad* quad = self->GetGLQuad ( idx );
	if ( quad ) {
		quad->SetUVs ( u0, v0, u1, v1 );
	}

	return 0;
}

//================================================================//
// MOAIBrushLibrary
//================================================================//

//----------------------------------------------------------------//
bool MOAIBrushLibrary::Bind () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	if ( !drawBuffer.BindTexture ( this->mTexture )) return false;
	USGLQuad::BindVertexFormat ( drawBuffer );

	return true;
}

//----------------------------------------------------------------//
void MOAIBrushLibrary::Draw ( MOAIDrawingMtx2D& transform, u32 idx ) {

	USGLQuad* quad = this->GetGLQuad ( idx );
	if ( quad ) {
		
		USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
		drawBuffer.SetVtxTransform ( transform.GetLocalToWorldMtx ());
			
		quad->Draw ();
	}
}

//----------------------------------------------------------------//
void MOAIBrushLibrary::Draw ( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {
	
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	drawBuffer.SetVtxTransform ( transform.GetLocalToWorldMtx ());
	
	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			u32 tile = grid.GetTile ( x, y );
			if ( tile & USTile::HIDDEN ) continue;
			
			USVec2D loc = grid.GetTilePoint ( x, y, USGridSpace::TILE_CENTER );
			
			float xScale = ( tile & USTile::XFLIP ) ? -1.0f : 1.0f;
			float yScale = ( tile & USTile::YFLIP ) ? -1.0f : 1.0f;
			
			USGLQuad* quad = this->GetGLQuad ( tile & USTile::CODE_MASK );
			if ( quad ) {
				quad->Draw ( loc.mX, loc.mY, xScale, yScale );
			}
		}
	}
}

//----------------------------------------------------------------//
USRect MOAIBrushLibrary::GetBounds ( u32 idx ) {
	
	USGLQuad* quad = this->GetGLQuad ( idx );
	if ( quad ) {
		return quad->GetVtxBounds ();
	}
	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	return rect;
}

//----------------------------------------------------------------//
USGLQuad* MOAIBrushLibrary::GetGLQuad ( u32 idx ) {

	idx = this->GetContentAddr ( idx, this->mQuads.Size ());
	if ( idx != NO_CONTENT ) {
		return &this->mQuads [ idx ];
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIBrushLibrary::LoadTexture ( cc8* filename ) {

	if ( !USFileSys::CheckFileExists ( filename )) return;
	
	this->mTexture = new MOAITexture ();
	this->mTexture->Init ( filename, USImageTransform::TRUECOLOR | USImageTransform::QUANTIZE );
	this->mTexture->SetFilter ( GL_LINEAR, GL_LINEAR );
	this->mTexture->SetWrap ( GL_REPEAT );
}

//----------------------------------------------------------------//
MOAIBrushLibrary::MOAIBrushLibrary () {

	RTTI_SINGLE ( MOAIContentLibrary2D )
	this->SetContentMask ( CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIBrushLibrary::~MOAIBrushLibrary () {
}

//----------------------------------------------------------------//
void MOAIBrushLibrary::RegisterLuaClass ( USLuaState& state ) {

	MOAIContentLibrary2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBrushLibrary::RegisterLuaFuncs ( USLuaState& state ) {

	MOAIContentLibrary2D::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "reserveBrushes",		_reserveBrushes },
		{ "scaleCoords",		_scaleCoords },
		{ "scaleUVCoords",		_scaleUVCoords },
		{ "setRect",			_setRect },
		{ "setTexture",			_setTexture },
		{ "setUVRect",			_setUVRect },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBrushLibrary::ScaleScreenCoords ( float xScale, float yScale ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].ScaleVerts ( xScale, yScale );
	}
}

//----------------------------------------------------------------//
void MOAIBrushLibrary::ScaleUVCoords ( float xScale, float yScale ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].ScaleUVs ( xScale, yScale );
	}
}

//----------------------------------------------------------------//
STLString MOAIBrushLibrary::ToString () {

	STLString repr ( MOAIContentLibrary2D::ToString () );
	PRETTY_PRINT ( repr, mTexture )
	return repr;
}
