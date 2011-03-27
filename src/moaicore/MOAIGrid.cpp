// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>clearTileFlags ( self, xTile, yTile, mask )</tt>\n
\n
	Clears the tile code bits masked by 'mask.'
	@param self (in)
	@param xTile (in) Column containing tile.
	@param yTile (in) Row containing tile.
	@param mask (in)
*/
int MOAIGrid::_clearTileFlags ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 0 );
	int yTile	= state.GetValue < int >( 3, 0 );
	u32 mask	= state.GetValue < u32 >( 4, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	
	tile = tile & ~mask;
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( tile ) getTile ( self, xTile, yTile, loSize, hiSize, base )</tt>\n
\n
	Returns the value of a given tile.
	@param self (in)
	@param xTile (in) Column containing tile.
	@param yTile (in) Row containing tile.
	@param tile (out) Tile code.
*/
int MOAIGrid::_getTile ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 0 );
	int yTile	= state.GetValue < int >( 3, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( flags ) getTileFlags ( self, xTile, yTile, mask )</tt>\n
\n
	Returns the tile code bits masked by 'mask.'
	@param self (in)
	@param xTile (in) Column containing tile.
	@param yTile (in) Row containing tile.
	@param mask (in)
	@param flags (out)
*/
int MOAIGrid::_getTileFlags ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 0 );
	int yTile	= state.GetValue < int >( 3, 0 );
	u32 mask	= state.GetValue < u32 >( 4, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	
	tile = tile & mask;
	
	lua_pushnumber ( state, tile );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( x, y ) getTileLoc ( self, xTile, yTile, position )</tt>\n
\n
	Returns the world space coordinate of the tile with respect to the tilemap's
	geometry and world transform.
	The optional 'position' flag determines the location of the coordinate within the tile:
	
	MOAIGrid.TILE_LEFT_TOP
	MOAIGrid.TILE_RIGHT_TOP
	MOAIGrid.TILE_LEFT_BOTTOM
	MOAIGrid.TILE_RIGHT_BOTTOM
	MOAIGrid.TILE_LEFT_CENTER
	MOAIGrid.TILE_RIGHT_CENTER
	MOAIGrid.TILE_TOP_CENTER
	MOAIGrid.TILE_BOTTOM_CENTER
	MOAIGrid.TILE_CENTER
	
	@param self (in)
	@param xTile (in)
	@param yTile (in)
	@param position (in)
	@param x (out)
	@param y (out)
*/
int MOAIGrid::_getTileLoc ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNN" )
	
	int xTile		= state.GetValue < int >( 2, 0 );
	int yTile		= state.GetValue < int >( 3, 0 );
	u32 position	= state.GetValue < u32 >( 4, USGridSpace::TILE_CENTER );
	
	USVec2D loc = self->GetTilePoint ( xTile, yTile, position );
	state.Push ( loc.mX );
	state.Push ( loc.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>( xTile, yTile ) worldToCoord ( self, x, y )</tt>\n
\n
	Transforms a world coordinate to a grid index.
	@param self (in)
	@param x (in)
	@param y (in)
	@param xTile (out)
	@param yTile (out)
*/
int MOAIGrid::_locToCoord ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0 );
	loc.mY = state.GetValue < float >( 3, 0 );
	
	USTileCoord coord;
	coord = self->GetTileCoord ( loc );

	state.Push ( coord.mX );
	state.Push ( coord.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>setRow ( self, row, id )</tt>\n
\n
	Initializes dimensions of grid and reserves storage for tiles.
	@param self (in)
	@param row (in)
	@param ... (in)
*/
int MOAIGrid::_setRow ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UN" )

	u32 row = state.GetValue < u32 >( 2, 0 );
	u32 total = lua_gettop ( state ) - 2;
	
	for ( u32 i = 0; i < total; ++i ) {
	
		u32 tile = state.GetValue < u32 >( 3 + i, 0 );
		self->SetTile ( i, row, tile );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setSize ( self, width, height, tileWidth, tileHeight )</tt>\n
\n
	Initializes dimensions of grid and reserves storage for tiles.
	@param self (in)
	@param width (in) Width of grid in tiles.
	@param height (in) Height of grid in tiles.
	@param tileWidth (in) Width of cells in grid space.
	@param tileHeight (in) Height of cells in grid space.
*/
int MOAIGrid::_setSize ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNN" )

	u32 width = state.GetValue < u32 >( 2, 0 );
	u32 height = state.GetValue < u32 >( 3, 0 );
	float tileWidth = state.GetValue < float >( 4, 1.0f );
	float tileHeight = state.GetValue < float >( 5, 1.0f );

	self->Init ( width, height, tileWidth, tileHeight, USTile::HIDDEN );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setTile ( self, xTile, yTile, tile )</tt>\n
\n
	Sets the value of a given tile.
	@param self (in)
	@param xTile (in) Column containing tile.
	@param yTile (in) Row containing tile.
	@param tile (in) Value of tile.
*/
int MOAIGrid::_setTile ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 0 );
	int yTile	= state.GetValue < int >( 3, 0 );
	u32 tile	= state.GetValue < u32 >( 4, 0 );
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setTileFlags ( self, xTile, yTile, mask )</tt>\n
\n
	Sets the tile code bits masked by 'mask.'
	@param self (in)
	@param xTile (in) Column containing tile.
	@param yTile (in) Row containing tile.
	@param mask (in)
*/
int MOAIGrid::_setTileFlags ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 0 );
	int yTile	= state.GetValue < int >( 3, 0 );
	u32 mask	= state.GetValue < u32 >( 4, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	
	tile = tile | mask;
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>toggleTileFlags ( self, xTile, yTile, mask )</tt>\n
\n
	Toggles the tile code bits masked by 'mask.'
	@param self (in)
	@param xTile (in) Column containing tile.
	@param yTile (in) Row containing tile.
	@param mask (in)
*/
int MOAIGrid::_toggleTileFlags ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 0 );
	int yTile	= state.GetValue < int >( 3, 0 );
	u32 mask	= state.GetValue < u32 >( 4, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	
	tile = ( tile & ~mask ) | ( ~tile & mask );
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( xTile, yTile ) wrapCoord ( self, xTile, yTile )</tt>\n
\n
	Wraps a given tile index ot the extents of the tilemap's grid.
	@param self (in)
	@param xTile (in)
	@param yTile (in)
	@param xTile (out)
	@param yTile (out)
*/
int MOAIGrid::_wrapCoord ( lua_State* L ) {
	LUA_SETUP ( MOAIGrid, "UNN" )
	
	USTileCoord coord;
	coord.mX = state.GetValue < int >( 2, 0 );
	coord.mY = state.GetValue < int >( 3, 0 );

	self->WrapCellCoord ( coord );
	
	state.Push ( coord.mX );
	state.Push ( coord.mY );
	return 2;
}

//================================================================//
// MOAIGrid
//================================================================//

//----------------------------------------------------------------//
u32 MOAIGrid::GetTile ( int xTile, int yTile ) {

	u32 addr = this->GetTileAddr ( xTile, yTile );
	if ( addr < this->mTiles.Size ()) {
		return this->mTiles [ addr ];
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIGrid::Init ( u32 width, u32 height, float tileWidth, float tileHeight, u32 fill ) {

	this->SetWidth ( width );
	this->SetHeight ( height );
	this->SetTileWidth ( tileWidth );
	this->SetTileHeight ( tileHeight );
	
	this->mTiles.Init ( this->GetTotalTiles ());
	this->mTiles.Fill ( fill );
}

//----------------------------------------------------------------//
MOAIGrid::MOAIGrid () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaData )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGrid::~MOAIGrid () {
}

//----------------------------------------------------------------//
void MOAIGrid::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "TILE_X_FLIP", ( u32 )USTile::XFLIP );
	state.SetField ( -1, "TILE_Y_FLIP", ( u32 )USTile::YFLIP );
	state.SetField ( -1, "TILE_XY_FLIP", ( u32 )USTile::FLIP_MASK );
	state.SetField ( -1, "TILE_HIDE", ( u32 )USTile::HIDDEN );
	
	state.SetField ( -1, "TILE_LEFT_TOP", ( u32 )USGridSpace::TILE_LEFT_TOP );
	state.SetField ( -1, "TILE_RIGHT_TOP", ( u32 )USGridSpace::TILE_RIGHT_TOP );
	state.SetField ( -1, "TILE_LEFT_BOTTOM", ( u32 )USGridSpace::TILE_LEFT_BOTTOM );
	state.SetField ( -1, "TILE_RIGHT_BOTTOM", ( u32 )USGridSpace::TILE_RIGHT_BOTTOM );
	
	state.SetField ( -1, "TILE_LEFT_CENTER", ( u32 )USGridSpace::TILE_LEFT_CENTER );
	state.SetField ( -1, "TILE_RIGHT_CENTER", ( u32 )USGridSpace::TILE_RIGHT_CENTER );
	state.SetField ( -1, "TILE_TOP_CENTER", ( u32 )USGridSpace::TILE_TOP_CENTER );
	state.SetField ( -1, "TILE_BOTTOM_CENTER", ( u32 )USGridSpace::TILE_BOTTOM_CENTER );
	
	state.SetField ( -1, "TILE_CENTER", ( u32 )USGridSpace::TILE_CENTER );
}

//----------------------------------------------------------------//
void MOAIGrid::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "clearTileFlags",		_clearTileFlags },
		{ "getTile",			_getTile },
		{ "getTileFlags",		_getTileFlags },
		{ "locToCoord",			_locToCoord },
		{ "setRow",				_setRow },
		{ "setSize",			_setSize },
		{ "setTile",			_setTile },
		{ "setTileFlags",		_setTileFlags },
		{ "toggleTileFlags",	_toggleTileFlags },
		{ "wrapCoord",			_wrapCoord },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGrid::RowFromString ( u32 rowID, cc8* str ) {

	int width = this->GetWidth ();
	u32 base = rowID * width;
	u32 strStep = USTile::TILE_STR_LEN + 2;

	for ( int i = 0; i < width; ++i ) {
		this->mTiles [ base + i ] = USTile::FromString ( &str [ i * strStep ]);
	}
}

//----------------------------------------------------------------//
STLString MOAIGrid::RowToString ( u32 rowID ) {

	int width = this->GetWidth ();
	u32 base = rowID * width;

	STLString rowStr;

	for ( int i = 0; i < width; ++i ) {
		if ( i ) {
			rowStr.append ( ", " );
		}
		rowStr.append ( USTile::ToString ( this->mTiles [ base + i ] ));
	}
	
	return rowStr;
}

//----------------------------------------------------------------//
void MOAIGrid::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( serializer );

	this->USGridSpace::SerializeIn ( state );

	this->mTiles.Init ( this->GetTotalTiles ());

	state.GetField ( -1, "mRows" );

	for ( int i = 0; i < this->GetHeight (); ++i ) {
		
		state.GetField ( -1, i + 1 );
		STLString rowStr = state.GetValue ( -1, "" );
		this->RowFromString ( i, rowStr );
		state.Pop ( 1 );
	}
	
	state.Pop ( 1 );
}

//----------------------------------------------------------------//
void MOAIGrid::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( serializer );

	this->USGridSpace::SerializeOut ( state );

	lua_newtable ( state );

	for ( int i = 0; i < this->GetHeight (); ++i ) {
		lua_pushnumber ( state, i + 1 );
		STLString rowStr = this->RowToString ( i );
		lua_pushstring ( state, rowStr );
		lua_settable ( state, -3 );
	}
	
	lua_setfield ( state, -2, "mRows" );
}

//----------------------------------------------------------------//
void MOAIGrid::SetTile ( u32 addr, u32 tile ) {

	addr = addr % this->mTiles.Size ();
	this->mTiles [ addr ] = tile;
}

//----------------------------------------------------------------//
void MOAIGrid::SetTile ( int xTile, int yTile, u32 tile ) {

	u32 addr = this->GetTileAddr ( xTile, yTile );
	if ( addr < this->mTiles.Size ()) {
		this->mTiles [ addr ] = tile;
	}
}

//----------------------------------------------------------------//
u32 MOAIGrid::Size () {

	return this->mTiles.Size ();
}

//----------------------------------------------------------------//
STLString MOAIGrid::ToString () {

	STLString repr;

	//PrettyPrint ( repr, "bounds", this->USTilemap::GetBounds ());

	return repr;
}
