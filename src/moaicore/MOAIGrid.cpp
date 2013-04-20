// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIStream.h>

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
/**	@name	fill
	@text	Set all tiles to a single value

	@in		MOAIGrid self
	@in		number value
	@out	nil
*/
int MOAIGrid::_fill ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UN" )

	u32 value	= state.GetValue < u32 >( 2, 1 );
	
	self->Fill ( value );
	
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
/**	@name	getColor
	@text	Returns the color of a given tile.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@out	number r
	@out	number g
	@out	number b
	@out	number a
*/
int MOAIGrid::_getColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	USColorVec color = self->GetColor ( xTile, yTile );
	state.Push ( color.mR );
	state.Push ( color.mG );
	state.Push ( color.mB );
	state.Push ( color.mA );
	return 4;
}

//----------------------------------------------------------------//
/**	@name	getScale
	@text	Returns the scale of a given tile.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@out	number scale
*/
int MOAIGrid::_getScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	float scale = self->GetScale ( xTile, yTile );
	state.Push ( scale );
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
/**	@name	setColor
	@text	Sets the color of a given tile. Four arguments = RGBA, three = RGB, 2 = greyscale + alpha, 1 = greyscale. Alpha is 1.0 if not specified.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number r
	@in		number g
	@in		number b
	@in		number a
	@out	nil
*/
int MOAIGrid::_setColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	USColorVec color;
	color.mA = 1.0f;

	int remaining = lua_gettop ( state ) - 3;
	switch (remaining) {
	case 0:
	default:
		color.mR = color.mG = color.mB = color.mA = 1.0f;
		break;
	case 2:
		color.mA = state.GetValue < float >( 5, 1.0 );
		/* fallthrough */
	case 1:
		color.mR = color.mG = color.mB = state.GetValue < float >( 4, 1.0 );
		break;
	case 4:
		color.mA = state.GetValue < float >( 7, 1.0 );
		/* fallthrough */
	case 3:
		color.mR = state.GetValue < float >( 4, 1.0 );
		color.mG = state.GetValue < float >( 5, 1.0 );
		color.mB = state.GetValue < float >( 6, 1.0 );
		break;
	}

	self->SetColor ( xTile, yTile, color );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setScale
	@text	Sets the relative scale of a given tile

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number scale
	@out	nil
*/
int MOAIGrid::_setScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	float scale	= state.GetValue < float >( 4, 0 );
	
	self->SetScale ( xTile, yTile, scale );
	
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
/**	@name	streamTilesIn
	@text	Reads tiles directly from a stream. Call this only after
			initializing the grid. Only the content of the tiles
			buffer is read.

	@in		MOAIGrid self
	@in		MOAIStream stream
	@out	number bytesRead
*/
int MOAIGrid::_streamTilesIn ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UU" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
		state.Push ( self->StreamTilesIn ( stream->GetUSStream ()));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	streamTilesOut
	@text	Writes tiles directly to a stream. Only the content of
			the tiles buffer is written.

	@in		MOAIGrid self
	@in		MOAIStream stream
	@out	number bytesWritten
*/
int MOAIGrid::_streamTilesOut ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UU" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
		state.Push ( self->StreamTilesOut ( stream->GetUSStream ()));
		return 1;
	}
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

//================================================================//
// MOAIGrid
//================================================================//

//----------------------------------------------------------------//
void MOAIGrid::Fill ( u32 value ) {

	this->mTiles.Fill ( value );
	return;
}

//----------------------------------------------------------------//
u32 MOAIGrid::GetTile ( int xTile, int yTile ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mTiles.Size ()) {
			return this->mTiles [ addr ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
float MOAIGrid::GetScale ( int xTile, int yTile ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mScales.Size ()) {
			return this->mScales [ addr ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
USColorVec MOAIGrid::GetColor ( int xTile, int yTile ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mColors.Size ()) {
			return this->mColors [ addr ];
		}
	}
	return USColorVec ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIGrid::MOAIGrid () {
	
	RTTI_SINGLE ( MOAIGridSpace )
}

//----------------------------------------------------------------//
MOAIGrid::~MOAIGrid () {
}

//----------------------------------------------------------------//
void MOAIGrid::OnResize () {

	this->mTiles.Init ( this->GetTotalCells ());
	this->mTiles.Fill ( 0 );
	this->mColors.Init ( this->GetTotalCells ());
	this->mColors.Fill ( USColorVec (1.0f, 1.0f, 1.0f, 1.0f ) );
	this->mScales.Init ( this->GetTotalCells ());
	this->mScales.Fill ( 1.0f );
}

//----------------------------------------------------------------//
void MOAIGrid::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGridSpace::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGrid::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGridSpace::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "clearTileFlags",		_clearTileFlags },
		{ "fill",				_fill },
		{ "getTile",			_getTile },
		{ "getColor",			_getColor },
		{ "getScale",			_getScale },
		{ "getTileFlags",		_getTileFlags },
		{ "setRow",				_setRow },
		{ "setTile",			_setTile },
		{ "setColor",			_setColor },
		{ "setScale",			_setScale },
		{ "setTileFlags",		_setTileFlags },
		{ "streamTilesIn",		_streamTilesIn },
		{ "streamTilesOut",		_streamTilesOut },
		{ "toggleTileFlags",	_toggleTileFlags },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGrid::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	this->MOAIGridSpace::SerializeIn ( state, serializer );
	this->mTiles.Init ( this->MOAIGridSpace::GetTotalCells ());

	state.GetField ( -1, "mData" );

	if ( state.IsType ( -1, LUA_TSTRING )) {
		
		void* tiles = this->mTiles;
		size_t tilesSize = this->mTiles.Size () * sizeof ( u32 );
		
		STLString base64 = lua_tostring ( state, -1 ); 
		base64.base_64_decode ( tiles, tilesSize );
		
		USLeanArray < u8 > unzip;
		USZip::Inflate ( this->mTiles, this->mTiles.Size () * sizeof ( u32 ), unzip );
		
		tiles = unzip.Data ();
		if ( unzip.Size () < tilesSize ) {
			tilesSize = unzip.Size ();
		}
		memcpy ( this->mTiles, tiles, tilesSize );
	}
	
	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAIGrid::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	this->MOAIGridSpace::SerializeOut ( state, serializer );

	USLeanArray < u8 > zip;
	USZip::Deflate ( this->mTiles, this->mTiles.Size () * sizeof ( u32 ), zip );

	STLString base64;
	base64.base_64_encode ( zip.Data (), zip.Size ());
	
	lua_pushstring ( state, base64.str ());
	lua_setfield ( state, -2, "mData" );
}

//----------------------------------------------------------------//
void MOAIGrid::SetTile ( u32 addr, u32 tile ) {

	u32 size = this->mTiles.Size ();

	if ( size ) {
		addr = addr % this->mTiles.Size ();
		this->mTiles [ addr ] = tile;
	}
}

//----------------------------------------------------------------//
void MOAIGrid::SetTile ( int xTile, int yTile, u32 tile ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
	
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mTiles.Size ()) {
			this->mTiles [ addr ] = tile;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGrid::SetColor ( u32 addr, const USColorVec& color ) {

	u32 size = this->mColors.Size ();

	if ( size ) {
		addr = addr % this->mColors.Size ();
		this->mColors [ addr ] = color;
	}
}

//----------------------------------------------------------------//
void MOAIGrid::SetColor ( int xTile, int yTile, const USColorVec& color ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
	
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mColors.Size ()) {
			this->mColors [ addr ] = color;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGrid::SetScale ( u32 addr, float scale ) {

	u32 size = this->mScales.Size ();

	if ( size ) {
		addr = addr % this->mScales.Size ();
		this->mScales [ addr ] = scale;
	}
}

//----------------------------------------------------------------//
void MOAIGrid::SetScale ( int xTile, int yTile, float scale ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
	
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mScales.Size ()) {
			this->mScales [ addr ] = scale;
		}
	}
}

//----------------------------------------------------------------//
size_t MOAIGrid::StreamTilesIn ( USStream* stream ) {

	if ( !stream ) return 0;
	
	size_t size = this->mTiles.Size () * sizeof ( u32 );
	return stream->ReadBytes ( this->mTiles, size );
}

//----------------------------------------------------------------//
size_t MOAIGrid::StreamTilesOut ( USStream* stream ) {

	if ( !stream ) return 0;

	size_t size = this->mTiles.Size () * sizeof ( u32 );
	return stream->WriteBytes ( this->mTiles, size );
}
