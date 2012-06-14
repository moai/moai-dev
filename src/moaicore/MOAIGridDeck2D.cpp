// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIQuadBrush.h>
#include <moaicore/MOAIGridDeck2D.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// MOAIGridDeckBrush
//================================================================//
	
//----------------------------------------------------------------//
MOAIGridDeckBrush::MOAIGridDeckBrush () :
	mMin ( 0, 0 ),
	mMax ( 0, 0 ),
	mOffset ( 0.0f, 0.0f ) {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserve
	@text	Set capacity of grid deck.
	
	@in		MOAIGridDeck2D self
	@in		number nBrushes
	@out	nil
*/
int MOAIGridDeck2D::_reserveBrushes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridDeck2D, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mBrushes.Init ( total );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setBrush
	@text	Initializes a brush.
	
	@in		MOAIGridDeck2D self
	@in		number idx		Index of the brush.
	@in		number xTile
	@in		number yTile
	@in		number width
	@in		number height
	@opt	number xOff		Default value is 0.
	@opt	number yOff		Default value is 0.
	@out	nil
*/
int MOAIGridDeck2D::_setBrush ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridDeck2D, "UNNNNN" )
	
	u32 idx = state.GetValue < int >( 2, 1 ) - 1;
	if ( idx < self->mBrushes.Size ()) {
	
		MOAIGridDeckBrush& brush = self->mBrushes [ idx ];
		
		brush.mMin.mX		= state.GetValue < int >( 3, 1 ) - 1;
		brush.mMin.mY		= state.GetValue < int >( 4, 1 ) - 1;
		brush.mMax.mX		= state.GetValue < u32 >( 5, 0 ) + brush.mMin.mX - 1;
		brush.mMax.mY		= state.GetValue < u32 >( 6, 0 ) + brush.mMin.mY - 1;
		brush.mOffset.mX	= state.GetValue < float >( 7, 0.0f );
		brush.mOffset.mY	= state.GetValue < float >( 8, 0.0f );
		
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDeck
	@text	Sets or clears the deck to be indexed by the grid.
	
	@in		MOAIGridDeck2D self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIGridDeck2D::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridDeck2D, "U" )
	
	MOAIDeck* deck = state.GetLuaObject < MOAIDeck >( 2, true );
	self->mDeck.Set ( *self, deck );
	self->SetBoundsDirty ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGrid
	@text	Sets or clears the grid to be sampled by the brushes.
	
	@in		MOAIGridDeck2D self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIGridDeck2D::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridDeck2D, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2, true );
	self->mGrid.Set ( *self, grid );
	self->SetBoundsDirty ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRemapper
	@text	Sets or clears the remapper (for remapping index values
			held in the grid).
	
	@in		MOAIGridDeck2D self
	@opt	MOAIDeckRemapper remapper		Default value is nil.
	@out	nil
*/
int MOAIGridDeck2D::_setRemapper ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridDeck2D, "U" )
	
	MOAIDeckRemapper* remapper = state.GetLuaObject < MOAIDeckRemapper >( 2, true );
	self->mRemapper.Set ( *self, remapper );
	
	return 0;
}

//================================================================//
// MOAIGridDeck2D
//================================================================//

//----------------------------------------------------------------//
USBox MOAIGridDeck2D::ComputeMaxBounds () {

	USBox bounds;

	u32 size = this->mBrushes.Size ();
	if ( size == 0 ) {
		bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	}
	else {
		bounds = this->GetItemBounds ( 1 );
		for ( u32 i = 1; i < this->mBrushes.Size (); ++i ) {
			bounds.Grow ( this->GetItemBounds ( i ));
		}
	}
	return bounds;
}

//----------------------------------------------------------------//
void MOAIGridDeck2D::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( zScl );
	
	u32 size = this->mBrushes.Size ();
	if ( !size ) return;
	
	if ( !this->mGrid ) return;
	if ( !this->mDeck ) return;
	
	idx = ( idx - 1 ) % size;
	MOAIGridDeckBrush& brush = this->mBrushes [ idx ];
	
	MOAICellCoord c0 = brush.mMin;
	MOAICellCoord c1 = brush.mMax;
	
	MOAIGrid& grid = *this->mGrid;
	
	float tileWidth = grid.GetTileWidth () * xScl; 
	float tileHeight = grid.GetTileHeight () * yScl;
	
	xOff = xOff - ( c0.mX * tileWidth ) + brush.mOffset.mX;
	yOff = yOff - ( c0.mY * tileHeight ) + brush.mOffset.mY;
	
	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
			idx = grid.GetTile ( wrap.mX, wrap.mY );
			
			MOAICellCoord coord ( x, y );
			USVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
			loc.Scale ( xScl, yScl );
			
			this->mDeck->Draw ( idx, this->mRemapper, loc.mX + xOff, loc.mY + yOff, zOff, tileWidth, tileHeight, 1.0f );
		}
	}
}

//----------------------------------------------------------------//
USBox MOAIGridDeck2D::GetItemBounds ( u32 idx ) {
	
	USBox bounds;
	
	u32 size = this->mBrushes.Size ();
	if ( this->mGrid && size ) {
		
		// TODO: handle oversized decks (don't assume unit sized deck items)
		
		idx = ( idx - 1 ) % size;
		MOAIGridDeckBrush& brush = this->mBrushes [ idx ];
		
		USRect rect = this->mGrid->GetBounds ( brush.mMin, brush.mMax );
		rect.Offset ( brush.mOffset.mX - rect.mXMin, brush.mOffset.mY - rect.mYMin );
		bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
		return bounds;
	}
	
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	return bounds;
}

//----------------------------------------------------------------//
void MOAIGridDeck2D::GetGfxState ( MOAIDeckGfxState& gfxState ) {

	if ( this->mDeck ) {
		this->mDeck->GetGfxState ( gfxState );
	}
	gfxState.SetShader ( this->mShader );
	gfxState.SetTexture ( this->mTexture );
}

//----------------------------------------------------------------//
MOAIGridDeck2D::MOAIGridDeck2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
	this->mMaxBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGridDeck2D::~MOAIGridDeck2D () {

	this->mGrid.Set ( *this, 0 );
	this->mDeck.Set ( *this, 0 );
	this->mRemapper.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGridDeck2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGridDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "reserveBrushes",		_reserveBrushes },
		{ "setBrush",			_setBrush },
		{ "setDeck",			_setDeck },
		{ "setGrid",			_setGrid },
		{ "setRemapper",		_setRemapper },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGridDeck2D::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIDeck::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGridDeck2D::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIDeck::SerializeOut ( state, serializer );
}
