// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMaterialBatch.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	clearTileFlags
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
/**	@lua	fill
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
/**	@lua	getTile
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
/**	@lua	getTileFlags
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
/**	@lua	setRow
	@text	Initializes a grid row given a variable argument list of values.

	@in		MOAIGrid self
	@in		number row
	@in		... values
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
/**	@lua	setTile
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
/**	@lua	setTileFlags
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
/**	@lua	streamTilesIn
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
		state.Push (( u32 )self->StreamTilesIn ( stream )); // TODO: overflow?
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	streamTilesOut
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
		state.Push (( u32 )self->StreamTilesOut ( stream )); // TODO: overflow?
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	toggleTileFlags
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
		{ "getTileFlags",		_getTileFlags },
		{ "setRow",				_setRow },
		{ "setTile",			_setTile },
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
		
		// TODO: all this is unsafe; don't assume sizes will be reasonable of that deflated data is guaranteed to be smaller; rewrite this with checking and recover in place
		
		void* tiles = this->mTiles;
		size_t tilesSize = this->mTiles.Size () * sizeof ( u32 );
		
		STLString base64 = lua_tostring ( state, -1 ); 
		base64.base_64_decode ( tiles, tilesSize );
		
		ZLLeanArray < u8 > unzip;
		ZLZip::Inflate ( this->mTiles, this->mTiles.Size () * sizeof ( u32 ), unzip );
		
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

	ZLLeanArray < u8 > zip;
	ZLZip::Deflate ( this->mTiles, this->mTiles.Size () * sizeof ( u32 ), zip );

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
size_t MOAIGrid::StreamTilesIn ( ZLStream* stream ) {

	if ( !stream ) return 0;
	
	size_t size = this->mTiles.Size () * sizeof ( u32 );
	return stream->ReadBytes ( this->mTiles, size );
}

//----------------------------------------------------------------//
size_t MOAIGrid::StreamTilesOut ( ZLStream* stream ) {

	if ( !stream ) return 0;

	size_t size = this->mTiles.Size () * sizeof ( u32 );
	return stream->WriteBytes ( this->mTiles, size );
}

//----------------------------------------------------------------//
void MOAIGrid::Draw ( MOAIDeck* deck, MOAIDeckRemapper* remapper, MOAIMaterialBatch& materials, const MOAICellCoord &c0, const MOAICellCoord &c1 ) {

	ZLVec3D offset	= ZLVec3D::ORIGIN;
	ZLVec3D scale	= ZLVec3D::AXIS;
	
	float tileWidth = this->GetTileWidth ();
	float tileHeight = this->GetTileHeight ();
	
	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = this->WrapCellCoord ( x, y );
			u32 idx = this->GetTile ( wrap.mX, wrap.mY );
			
			MOAICellCoord coord ( x, y );
			ZLVec2D loc = this->GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );

			offset.mX	= loc.mX;
			offset.mY	= loc.mY;
			scale.mX	= tileWidth;
			scale.mY	= tileHeight;

			deck->Draw ( MOAIDeckRemapper::Remap ( remapper, idx ), materials, offset, scale );
		}
	}
}
