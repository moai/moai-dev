// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clearTileFlags
	@text	Clears bits specified in mask.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number mask
	@out	nil
*/
int MOAIGrid::_clearTileFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	u32 mask	= state.GetValue < u32 >( 4, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	
	tile = tile & ~mask;
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getTile
	@text	Returns the value of a given tile.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@out	number tile
*/
int MOAIGrid::_getTile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	u32 tile = self->GetTile ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTileFlags
	@text	Returns the masked value of a given tile.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number mask
	@out	number tile
*/
int MOAIGrid::_getTileFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	u32 mask	= state.GetValue < u32 >( 4, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	
	tile = tile & mask;
	
	lua_pushnumber ( state, tile );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTileLoc
	@text	Returns the grid space coordinate of the tile. The optional 'position'
			flag determines the location of the coordinate within the tile.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@opt	number position		See MOAIGrid for list of positions. Default it MOAIGrid.TILE_CENTER.
	@out	number x
	@out	number y
*/
int MOAIGrid::_getTileLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )
	
	USCellCoord coord;
	
	coord.mX		= state.GetValue < int >( 2, 1 ) - 1;
	coord.mY		= state.GetValue < int >( 3, 1 ) - 1;
	u32 position	= state.GetValue < u32 >( 4, USGridSpace::TILE_CENTER );
	
	USVec2D loc = self->GetTilePoint ( coord, position );
	state.Push ( loc.mX );
	state.Push ( loc.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	locToCoord
	@text	Transforms a coordinate in grid space into a tile index.

	@in		MOAIGrid self
	@in		number x
	@in		number y
	@out	number xTile
	@out	number yTile
*/
int MOAIGrid::_locToCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0 );
	loc.mY = state.GetValue < float >( 3, 0 );
	
	USCellCoord coord;
	coord = self->GetCellCoord ( loc );

	state.Push ( coord.mX + 1 );
	state.Push ( coord.mY + 1);
	return 2;
}

//----------------------------------------------------------------//
/**	@name	setRow
	@text	Initializes a grid row given a variable argument list of values.

	@in		MOAIGrid self
	@in		number row
	@in		...
	@out	nil
*/
int MOAIGrid::_setRow ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UN" )

	u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total = lua_gettop ( state ) - 2;
	
	for ( u32 i = 0; i < total; ++i ) {
	
		u32 tile = state.GetValue < u32 >( 3 + i, 0 );
		self->SetTile ( i, row, tile );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSize
	@text	Initializes dimensions of grid and reserves storage for tiles.

	@in		MOAIGrid self
	@in		number width
	@in		number height
	@in		number cellWidth	Default value is 1.
	@in		number cellHeight	Default value is 1.
	@opt	number xOff			X offset of the tile from the cell.
	@opt	number yOff			Y offset of the tile from the cell.
	@opt	number tileWidth	Default value is cellWidth.
	@opt	number tileHeight	Default value is cellHeight.
	@out	nil
*/
int MOAIGrid::_setSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	u32 width			= state.GetValue < u32 >( 2, 0 );
	u32 height			= state.GetValue < u32 >( 3, 0 );
	
	float cellWidth		= state.GetValue < float >( 4, 1.0f );
	float cellHeight	= state.GetValue < float >( 5, 1.0f );

	float xOff			= state.GetValue < float >( 6, 0.0f );
	float yOff			= state.GetValue < float >( 7, 0.0f );
	
	float tileWidth		= state.GetValue < float >( 8, cellWidth );
	float tileHeight	= state.GetValue < float >( 9, cellHeight );
	
	self->SetWidth ( width );
	self->SetHeight ( height );
	
	self->SetCellWidth ( cellWidth );
	self->SetCellHeight ( cellHeight );
	
	self->SetXOff ( xOff );
	self->SetYOff ( yOff );
	
	self->SetTileWidth ( tileWidth );
	self->SetTileHeight ( tileHeight );
	
	self->mTiles.Init ( self->GetTotalCells ());
	self->mTiles.Fill ( 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTile
	@text	Sets the value of a given tile

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number value
	@out	nil
*/
int MOAIGrid::_setTile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	u32 tile	= state.GetValue < u32 >( 4, 0 );
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTileFlags
	@text	Sets a tile's flags given a mask.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number mask
	@out	nil
*/
int MOAIGrid::_setTileFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	u32 mask	= state.GetValue < u32 >( 4, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	
	tile = tile | mask;
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	toggleTileFlags
	@text	Toggles a tile's flags given a mask.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number mask
	@out	nil
*/
int MOAIGrid::_toggleTileFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	u32 mask	= state.GetValue < u32 >( 4, 0 );
	
	u32 tile = self->GetTile ( xTile, yTile );
	
	tile = tile ^ mask;
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	wrapCoord
	@text	Wraps a tile index to the range of the grid.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@out	number xTile
	@out	number yTile
*/
int MOAIGrid::_wrapCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )
	
	USCellCoord coord;
	coord.mX = state.GetValue < int >( 2, 1 ) - 1;
	coord.mY = state.GetValue < int >( 3, 1 ) - 1;

	self->WrapCellCoord ( coord );
	
	state.Push ( coord.mX + 1 );
	state.Push ( coord.mY + 1 );
	return 2;
}

//================================================================//
// MOAIGrid
//================================================================//

//----------------------------------------------------------------//
u32 MOAIGrid::GetTile ( int xTile, int yTile ) {

	USCellCoord coord ( xTile, yTile );
	u32 addr = this->GetCellAddr ( coord );
	if ( addr < this->mTiles.Size ()) {
		return this->mTiles [ addr ];
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIGrid::MOAIGrid () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
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

	luaL_Reg regTable [] = {
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
	this->mTiles.Init ( this->USGridSpace::GetTotalCells ());

	state.GetField ( -1, "mData" );

	if ( state.IsType ( -1, LUA_TSTRING )) {

		size_t len;
		cc8* luaString = lua_tolstring ( state, -1, &len ); 
		
		void* buffer = malloc ( len );
		memcpy ( buffer, luaString, len );

		USByteStream inStream;
		inStream.SetBuffer ( buffer, ( u32 )len );
		inStream.SetLength (( u32 )len );

		USBase64Cipher base64cipher;
		USCipherStream cipherStream;
		cipherStream.OpenCipher ( inStream, base64cipher );
		
		USByteStream outStream;
		outStream.SetBuffer ( this->mTiles, this->mTiles.Size () * sizeof ( u32 ));
		
		USZip::Inflate ( cipherStream, outStream );
		
		free ( buffer );
	}
	
	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAIGrid::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( serializer );

	this->USGridSpace::SerializeOut ( state );

	void* buffer = this->mTiles;
	u32 size = this->mTiles.Size () * sizeof ( u32 );

	USByteStream inStream;
	inStream.SetBuffer ( buffer, size );
	inStream.SetLength ( size );

	USMemStream outStream;
	outStream.Reserve ( size );

	USBase64Cipher base64cipher;
	USCipherStream cipherStream;
	cipherStream.OpenCipher ( outStream, base64cipher );
	
	USZip::Deflate ( inStream, cipherStream, USDeflater::DEFAULT_LEVEL );
	
	size = outStream.GetLength ();
	buffer = malloc ( size );
	outStream.Seek ( 0, SEEK_SET );
	outStream.ReadBytes ( buffer, size );
	
	lua_pushlstring ( state, ( cc8* )buffer, size );
	lua_setfield ( state, -2, "mData" );
	
	free ( buffer );
}

//----------------------------------------------------------------//
void MOAIGrid::SetTile ( u32 addr, u32 tile ) {

	addr = addr % this->mTiles.Size ();
	this->mTiles [ addr ] = tile;
}

//----------------------------------------------------------------//
void MOAIGrid::SetTile ( int xTile, int yTile, u32 tile ) {

	USCellCoord coord ( xTile, yTile );
	u32 addr = this->GetCellAddr ( coord );
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
