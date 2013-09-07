// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIDeck.h>

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
/**	@name	addColor
	@text	Adds color flagging to the grid, specifying a defualt color. 0-4 numbers are accepted; a single value is greyscale, three are RGB, two is greyscale + alpha, four are RGBA, and the default is opaque white.

	@in		MOAIGrid self
	@opt		number grey_or_red
	@opt		number alpha_or_green
	@opt		number blue
	@opt		number alpha
	@out	nil
*/
int MOAIGrid::_addColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "U" )

	ZLColorVec color;
	int remaining = lua_gettop ( state ) - 1;

	switch (remaining) {
	case 0:
	default:
		color.mR = color.mG = color.mB = color.mA = 1.0f;
		break;
	case 2:
		color.mA = state.GetValue < float >( 3, 1.0 );
		/* fallthrough */
	case 1:
		color.mR = color.mG = color.mB = state.GetValue < float >( 2, 1.0 );
		break;
	case 4:
		color.mA = state.GetValue < float >( 5, 1.0 );
		/* fallthrough */
	case 3:
		color.mR = state.GetValue < float >( 2, 1.0 );
		color.mG = state.GetValue < float >( 3, 1.0 );
		color.mB = state.GetValue < float >( 4, 1.0 );
		break;
	}

	self->AddColor ( color );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addScale
	@text	Adds tile scaling to the grid.

	@in		MOAIGrid self
	@out	nil
*/
int MOAIGrid::_addScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "U" )

	self->AddScale ();

	return 0;
}


//----------------------------------------------------------------//
/**	@name	removeColor
	@text	Rmoves tile color from the grid.

	@in		MOAIGrid self
	@out	nil
*/
int MOAIGrid::_removeColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "U" )

	self->RemoveColor ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	removeScale
	@text	Removes tile scaling from the grid.

	@in		MOAIGrid self
	@out	nil
*/
int MOAIGrid::_removeScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "U" )

	self->RemoveScale ();

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
/**	@name	getTileColor
	@text	Returns the color value of a given tile.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@out	number tileR
	@out	number tileG
	@out	number tileB
	@out	number tileA
*/
int MOAIGrid::_getTileColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	ZLColorVec tile = self->GetTileColor ( xTile, yTile );
	state.Push ( tile.mR );
	state.Push ( tile.mG );
	state.Push ( tile.mB );
	state.Push ( tile.mA );
	return 4;
}

//----------------------------------------------------------------//
/**	@name	getTileScale
	@text	Returns the scale factor of a given tile.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@out	number scale
*/
int MOAIGrid::_getTileScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	float scale = self->GetTileScale ( xTile, yTile );
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
/**	@name	setTileScale
	@text	Sets the scale factor of a given tile

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number value
	@out	nil
*/
int MOAIGrid::_setTileScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	float scale	= state.GetValue < float >( 4, 0 );
	
	self->SetTileScale ( xTile, yTile, scale );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTileColor
	@text	Sets the color of a given tile. 0-4 numbers are accepted; a single value is greyscale, three are RGB, two is greyscale + alpha, four are RGBA, and the default is opaque white.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@opt		number grey_or_red
	@opt		number alpha_or_green
	@opt		number blue
	@opt		number alpha
	@out	nil
*/
int MOAIGrid::_setTileColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	ZLColorVec color;
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

	self->SetTileColor ( xTile, yTile, color );
	
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
		state.Push ( self->StreamTilesIn ( stream->GetZLStream ()));
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
		state.Push ( self->StreamTilesOut ( stream->GetZLStream ()));
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
void MOAIGrid::FillColor ( const ZLColorVec& value ) {

	if (this->mHasColors) {
		this->mColors.Fill ( value );
	}
	return;
}

//----------------------------------------------------------------//
void MOAIGrid::FillScale ( float value ) {

	if (this->mHasScales) {
		this->mScales.Fill ( value );
	}
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
ZLColorVec MOAIGrid::GetTileColor ( int xTile, int yTile ) {

	if (this->mHasColors) {
		MOAICellCoord coord ( xTile, yTile );
		if ( this->IsValidCoord ( coord )) {
			u32 addr = this->GetCellAddr ( coord );
			if ( addr < this->mColors.Size ()) {
				return this->mColors [ addr ];
			}
		}
	}
	return ZLColorVec ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
float MOAIGrid::GetTileScale ( int xTile, int yTile ) {

	if (this->mHasScales) {
		MOAICellCoord coord ( xTile, yTile );
		if ( this->IsValidCoord ( coord )) {
			u32 addr = this->GetCellAddr ( coord );
			if ( addr < this->mScales.Size ()) {
				return this->mScales [ addr ];
			}
		}
	}
	return 1.0;
}

//----------------------------------------------------------------//
MOAIGrid::MOAIGrid () {
	
	RTTI_SINGLE ( MOAIGridSpace )
	this->mHasColors = 0;
	this->mHasScales = 0;
	this->pickDoDrawRegion();
}

//----------------------------------------------------------------//
MOAIGrid::~MOAIGrid () {
}

//----------------------------------------------------------------//
void MOAIGrid::OnResize () {

	this->mTiles.Init ( this->GetTotalCells ());
	this->mTiles.Fill ( 0 );
	if (this->mHasColors) {
		this->mColors.Init ( this->GetTotalCells ());
		this->mColors.Fill ( this->mDefaultColor );
	}
	if (this->mHasScales) {
		this->mScales.Init ( this->GetTotalCells ());
		this->mScales.Fill ( 1.0 );
	}
}

void MOAIGrid::RemoveColor () {
	this->mDefaultColor = ZLColorVec ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mHasColors = 0;
	this->mColors.Clear();
	this->pickDoDrawRegion();
}

void MOAIGrid::AddColor ( const ZLColorVec &defaultColor ) {
	if (!this->mHasColors) {
		this->mColors.Init ( this->GetTotalCells ());
	}
	this->mDefaultColor = defaultColor;
	this->mColors.Fill ( this->mDefaultColor );
	this->mHasColors = 1;
	this->pickDoDrawRegion();
}

void MOAIGrid::RemoveScale () {
	this->mHasScales = 0;
	this->mScales.Clear();
	this->pickDoDrawRegion();
}

void MOAIGrid::AddScale () {
	if (!this->mHasScales) {
		this->mScales.Init ( this->GetTotalCells ());
	}
	this->mScales.Fill ( 1.0 );
	this->mHasScales = 1;
	this->pickDoDrawRegion();
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
		{ "fill",			_fill },
		{ "addColor",			_addColor },
		{ "addScale",			_addScale },
		{ "removeColor",		_removeColor },
		{ "removeScale",		_removeScale },
		{ "getTile",			_getTile },
		{ "getTileFlags",		_getTileFlags },
		{ "getTileColor",		_getTileColor },
		{ "getTileScale",		_getTileScale },
		{ "setRow",			_setRow },
		{ "setTile",			_setTile },
		{ "setTileColor",		_setTileColor },
		{ "setTileScale",		_setTileScale },
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
void MOAIGrid::SetTileColor ( int xTile, int yTile, const ZLColorVec &color ) {

	if (this->mHasColors) {
		MOAICellCoord coord ( xTile, yTile );
		if ( this->IsValidCoord ( coord )) {
		
			u32 addr = this->GetCellAddr ( coord );
			if ( addr < this->mColors.Size ()) {
				this->mColors [ addr ] = color;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGrid::SetTileScale ( int xTile, int yTile, float scale ) {

	if (this->mHasScales) {
		MOAICellCoord coord ( xTile, yTile );
		if ( this->IsValidCoord ( coord )) {
		
			u32 addr = this->GetCellAddr ( coord );
			if ( addr < this->mScales.Size ()) {
				this->mScales [ addr ] = scale;
			}
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

/* DoDrawRegion implementations, so the color/scale checks are skipped
 * when not needed.
 */

static void DoDrawRegion_cs( MOAIGrid &grid, const MOAICellCoord &c0, const MOAICellCoord &c1, MOAIGfxDevice &gfxDevice, MOAILuaSharedPtr<MOAIDeck> &deck, MOAIDeckRemapper *remapper, float xOff, float yOff, float zOff, float xScl, float yScl ) {
	float tileWidth = grid.GetTileWidth () * xScl; 
	float tileHeight = grid.GetTileHeight () * yScl;

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
			int idx = grid.GetTile ( wrap.mX, wrap.mY );
			
			MOAICellCoord coord ( x, y );
			USVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
			loc.Scale ( xScl, yScl );
			
			deck->Draw ( idx, remapper, loc.mX + xOff, loc.mY + yOff, zOff, tileWidth, tileHeight, 1.0f );
		}
	}
}

static void DoDrawRegion_cS( MOAIGrid &grid, const MOAICellCoord &c0, const MOAICellCoord &c1, MOAIGfxDevice &gfxDevice, MOAILuaSharedPtr<MOAIDeck> &deck, MOAIDeckRemapper *remapper, float xOff, float yOff, float zOff, float xScl, float yScl ) {
	float tileWidth = grid.GetTileWidth () * xScl; 
	float tileHeight = grid.GetTileHeight () * yScl;

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
			int idx = grid.GetTile ( wrap.mX, wrap.mY );
			float scale = grid.GetTileScale ( wrap.mX, wrap.mY );
			
			MOAICellCoord coord ( x, y );
			USVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
			loc.Scale ( xScl, yScl );
			
			deck->Draw ( idx, remapper, loc.mX + xOff, loc.mY + yOff, zOff, tileWidth * scale, tileHeight * scale, 1.0f );
		}
	}
}

static void DoDrawRegion_Cs( MOAIGrid &grid, const MOAICellCoord &c0, const MOAICellCoord &c1, MOAIGfxDevice &gfxDevice, MOAILuaSharedPtr<MOAIDeck> &deck, MOAIDeckRemapper *remapper, float xOff, float yOff, float zOff, float xScl, float yScl ) {
	float tileWidth = grid.GetTileWidth () * xScl; 
	float tileHeight = grid.GetTileHeight () * yScl;
	const ZLColorVec amb0 = gfxDevice.GetAmbientColor ();

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
			int idx = grid.GetTile ( wrap.mX, wrap.mY );
			gfxDevice.SetAmbientColor ( amb0 * grid.GetTileColor ( wrap.mX, wrap.mY ));

			MOAICellCoord coord ( x, y );
			USVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
			loc.Scale ( xScl, yScl );
			
			deck->Draw ( idx, remapper, loc.mX + xOff, loc.mY + yOff, zOff, tileWidth, tileHeight, 1.0f );
		}
	}
	gfxDevice.SetAmbientColor ( amb0 );
}

static void DoDrawRegion_CS( MOAIGrid &grid, const MOAICellCoord &c0, const MOAICellCoord &c1, MOAIGfxDevice &gfxDevice, MOAILuaSharedPtr<MOAIDeck> &deck, MOAIDeckRemapper *remapper, float xOff, float yOff, float zOff, float xScl, float yScl ) {
	float tileWidth = grid.GetTileWidth () * xScl; 
	float tileHeight = grid.GetTileHeight () * yScl;
	const ZLColorVec amb0 = gfxDevice.GetAmbientColor ();

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
			int idx = grid.GetTile ( wrap.mX, wrap.mY );
			float scale = grid.GetTileScale ( wrap.mX, wrap.mY );
			gfxDevice.SetAmbientColor ( amb0 * grid.GetTileColor ( wrap.mX, wrap.mY ));
			
			MOAICellCoord coord ( x, y );
			USVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
			loc.Scale ( xScl, yScl );
			
			deck->Draw ( idx, remapper, loc.mX + xOff, loc.mY + yOff, zOff, tileWidth * scale, tileHeight * scale, 1.0f );
		}
	}
	gfxDevice.SetAmbientColor ( amb0 );
}

void MOAIGrid::pickDoDrawRegion ( ) {
	if (this->mHasColors) {
		if (this->mHasScales) {
			this->DoDrawRegion = DoDrawRegion_CS;
		} else {
			this->DoDrawRegion = DoDrawRegion_Cs;
		}
	} else {
		if (this->mHasScales) {
			this->DoDrawRegion = DoDrawRegion_cS;
		} else {
			this->DoDrawRegion = DoDrawRegion_cs;
		}
	}
}
