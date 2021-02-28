// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMetaTileDeck2D.h>

//================================================================//
// MOAIMetaTile
//================================================================//
	
//----------------------------------------------------------------//
MOAIMetaTile::MOAIMetaTile () :
	mMin ( 0, 0 ),
	mMax ( 0, 0 ),
	mOffset ( 0.0f, 0.0f ) {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserveMetaTiles
	@text	Set capacity of grid deck.
	
	@in		MOAIMetaTileDeck2D self
	@in		number nBrushes
	@out	nil
*/
int MOAIMetaTileDeck2D::_reserveMetaTiles ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMetaTileDeck2D, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mBrushes.Init ( total );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDeck
	@text	Sets or clears the deck to be indexed by the grid.
	
	@in		MOAIMetaTileDeck2D self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIMetaTileDeck2D::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMetaTileDeck2D, "U" )
	
	MOAIDeck* deck = state.GetLuaObject < MOAIDeck >( 2, true );
	self->mDeck.Set ( *self, deck );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGrid
	@text	Sets or clears the grid to be sampled by the brushes.
	
	@in		MOAIMetaTileDeck2D self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIMetaTileDeck2D::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMetaTileDeck2D, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2, true );
	self->mGrid.Set ( *self, grid );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMetaTile
	@text	Initializes a metaTile.
	
	@in		MOAIMetaTileDeck2D self
	@in		number idx		Index of the brush.
	@in		number xTile
	@in		number yTile
	@in		number width
	@in		number height
	@opt	number xOff		Default value is 0.
	@opt	number yOff		Default value is 0.
	@out	nil
*/
int MOAIMetaTileDeck2D::_setMetaTile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMetaTileDeck2D, "UNNNNN" )
	
	ZLIndex idx = state.GetValue < MOAILuaIndex >( 2, 0 );
	if ( idx < self->mBrushes.Size ()) {
	
		MOAIMetaTile& brush = self->mBrushes [ idx ];
		
		brush.mMin.mX		= state.GetValue < int >( 3, 1 ) - 1;
		brush.mMin.mY		= state.GetValue < int >( 4, 1 ) - 1;
		brush.mMax.mX		= state.GetValue < u32 >( 5, 0 ) + brush.mMin.mX - 1;
		brush.mMax.mY		= state.GetValue < u32 >( 6, 0 ) + brush.mMin.mY - 1;
		brush.mOffset.mX	= state.GetValue < float >( 7, 0.0f );
		brush.mOffset.mY	= state.GetValue < float >( 8, 0.0f );
	}
	return 0;
}

//================================================================//
// MOAIMetaTileDeck2D
//================================================================//

//----------------------------------------------------------------//
MOAIMetaTileDeck2D::MOAIMetaTileDeck2D ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIDeck ( context ) {
	
	RTTI_BEGIN ( MOAIMetaTileDeck2D )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIMetaTileDeck2D >)
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMetaTileDeck2D::~MOAIMetaTileDeck2D () {

	this->mGrid.Set ( *this, 0 );
	this->mDeck.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMetaTileDeck2D::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIMetaTileDeck2D::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "reserveMetaTiles",	_reserveMetaTiles },
		{ "setDeck",			_setDeck },
		{ "setGrid",			_setGrid },
		{ "setMetaTile",		_setMetaTile },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLBounds MOAIMetaTileDeck2D::MOAIDeck_GetBounds () {

	ZLSize size = this->mBrushes.Size ();
	if ( size == 0 ) {
		return ZLBounds::EMPTY;
	}

	ZLBox aabb;
	for ( ZLIndex i = 0; i < this->mBrushes.Size (); ++i ) {
		aabb.Grow ( this->GetBounds ( i ).mAABB, i > 0 );
	}
	return ZLBounds ( aabb );
}

//----------------------------------------------------------------//
ZLBounds MOAIMetaTileDeck2D::MOAIDeck_GetBounds ( ZLIndex idx ) {
	
	ZLSize size = this->mBrushes.Size ();
	if ( this->mGrid && size ) {
		
		// TODO: handle oversized decks (don't assume unit sized deck items)
		idx = ZLIndexOp::Wrap ( idx, size );
		
		MOAIMetaTile& brush = this->mBrushes [ idx ];
		ZLRect frame = this->mGrid->GetFrame ( brush.mMin, brush.mMax );
		frame.Offset ( brush.mOffset.mX - frame.mXMin, brush.mOffset.mY - frame.mYMin );
		return ZLBounds ( frame );
	}
	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
void MOAIMetaTileDeck2D::MOAIDeck_Render ( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase ) {
	UNUSED ( idx );

	u32 size = ( u32 )this->mBrushes.Size ();
	if ( !size ) return;
	
	if ( !this->mGrid ) return;
	if ( !this->mDeck ) return;
	
	idx =  ZLIndexOp::SubtractAndWrap ( idx, 1, size );
	MOAIMetaTile& brush = this->mBrushes [ idx ];
	
	ZLGridCoord c0 = brush.mMin;
	ZLGridCoord c1 = brush.mMax;
	
	MOAIGrid& grid = *this->mGrid;
	
	float tileWidth = grid.GetTileWidth ();
	float tileHeight = grid.GetTileHeight ();
	
	//scale.Scale ( tileWidth, tileHeight, 1.0f );
	
	//offset.mX = offset.mX - ( c0.mX * tileWidth ) + brush.mOffset.mX;
	//offset.mY = offset.mY - ( c0.mY * tileHeight ) + brush.mOffset.mY;
	
	float xOff = brush.mOffset.mX - ( c0.mX * tileWidth );
	float yOff = brush.mOffset.mY - ( c0.mY * tileHeight );
	
	MOAIGfxMgr& gfxMgr = this->Get < MOAIGfxMgr >();
	const ZLAffine3D& modelToWorldMtx = gfxMgr.GetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX );
	
	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			ZLGridCoord wrap = grid.WrapCellCoord ( x, y );
			idx = grid.GetTile ( wrap.mX, wrap.mY );
			
			ZLGridCoord coord ( x, y );
			ZLVec3D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER, 0.0f );
			
			ZLAffine3D mtx = modelToWorldMtx;
			mtx.PrependSclTr2D ( tileWidth, tileHeight, xOff + loc.mX, yOff + loc.mY );
			gfxMgr.SetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX, mtx );

			this->mDeck->Render ( idx, renderPhase );
		}
	}
}
