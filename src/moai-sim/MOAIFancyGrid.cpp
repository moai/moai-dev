// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIFancyGrid.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIMaterialBatch.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	fillColor
	@text	Set all tiles to a single color

	@in		MOAIFancyGrid self
	@in		number value
	@out	nil
*/
int MOAIFancyGrid::_fillColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFancyGrid, "U" )

	self->FillColor ( state.GetTop () >= 4 ? state.GetColor32 ( 2, 0.0f, 0.0f, 0.0f, 0.0f ) : state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getColor
	@text	Returns the color index of a given tile.

	@in		MOAIFancyGrid self
	@in		number xTile
	@in		number yTile
	@out	number color	index into palette
*/
int MOAIFancyGrid::_getColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFancyGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	u32 tile = self->GetColor ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFancyGrid::_getPaletteColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFancyGrid, "UN" )

	u32 idx	= state.GetValue < u32 >( 2, 1 );
	
	state.Push ( self->GetPaletteColor ( ZLIndex ( idx, ZLIndex::ZERO ))); // TODO: index from 1?
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFancyGrid::_getTileColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFancyGrid, "UN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;

	ZLColorVec color = self->GetTileColor ( xTile, yTile );
	return state.Push ( color );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFancyGrid::_reservePalette ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFancyGrid, "UN" )

	u32 size = state.GetValue < u32 >( 2, 0 );
	self->ReservePalette ( size );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setColor
	@text	Sets the color index of a given tile

	@in		MOAIFancyGrid self
	@in		number xTile
	@in		number yTile
	@in		number value
	@out	nil
*/
int MOAIFancyGrid::_setColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFancyGrid, "U" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	u32 value	= state.GetTop () >= 6 ? state.GetColor32 ( 4, 0.0f, 0.0f, 0.0f, 0.0f ) : state.GetValue < u32 >( 4, 0 );
	
	self->SetColor ( xTile, yTile, value );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFancyGrid::_setColorRow ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFancyGrid, "UN" )

	u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total = lua_gettop ( state ) - 2;
	
	for ( u32 i = 0; i < total; ++i ) {
	
		u32 color = state.GetValue < u32 >( 3 + i, 0 );
		self->SetColor ( i, row, color );
	}
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFancyGrid::_setPaletteColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFancyGrid, "U" )

	u32 idx				= state.GetValue < u32 >( 2, 1 );
	MOAIColor* color	= state.GetLuaObject < MOAIColor >( 3, true );
	
	self->SetPaletteColor ( ZLIndex ( idx, ZLIndex::LIMIT ), color ); // TODO: index from one?
	return 0;
}


//================================================================//
// MOAIFancyGrid
//================================================================//

//----------------------------------------------------------------//
void MOAIFancyGrid::AffirmColorSet () {

	size_t totalCells = this->GetTotalCells ();
	if ( totalCells ) {
		this->mColorSet = malloc ( totalCells * this->mColorSize );
	}
}

//----------------------------------------------------------------//
void MOAIFancyGrid::DiscardColorSet () {

	if ( this->mColorSet ) {
		free ( this->mColorSet );
		this->mColorSet = 0;
	}
}

//----------------------------------------------------------------//
void MOAIFancyGrid::DiscardPalette () {

	for ( ZLIndex i = ZLIndex::ZERO; i < this->mPalette.Size (); ++i ) {
		this->LuaRelease ( this->mPalette [ i ]);
	}
	this->mPalette.Clear ();
}

//----------------------------------------------------------------//
void MOAIFancyGrid::FillColor ( u32 value ) {

	if ( this->mColorSize ) {
	
		size_t totalCells = this->GetTotalCells ();
		
		switch ( this->mColorSize ) {
			
			case COLOR_SIZE_8:
				for ( size_t i = 0; i < totalCells; ++i ) {
					(( u8* )this->mColorSet )[ i ] = value & 0x000000ff;
				}
				break;
			
			case COLOR_SIZE_16:
				for ( size_t i = 0; i < totalCells; ++i ) {
					(( u16* )this->mColorSet )[ i ] = value & 0x0000ffff;
				}
				break;
			
			case COLOR_SIZE_32:
				for ( size_t i = 0; i < totalCells; ++i ) {
					(( u32* )this->mColorSet )[ i ] = value & 0xffffffff;
				}
				break;
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIFancyGrid::GetColor ( int addr ) const {

	if ( this->mColorSet && ( addr < this->GetTotalCells ())) {

		switch ( this->mColorSize ) {
		
			case COLOR_SIZE_8:
				return (( u8* )this->mColorSet )[ addr ];
				break;
			
			case COLOR_SIZE_16:
				return (( u16* )this->mColorSet )[ addr ];
				break;
			
			case COLOR_SIZE_32:
				return (( u32* )this->mColorSet )[ addr ];
				break;
		}
	}
	return NO_COLOR;
}

//----------------------------------------------------------------//
u32 MOAIFancyGrid::GetColor ( int xTile, int yTile ) const {

	MOAICellCoord coord ( xTile, yTile );
	
	if ( this->IsValidCoord ( coord )) {
		return this->GetColor ( this->GetCellAddr ( coord ));
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIColor* MOAIFancyGrid::GetPaletteColor ( ZLIndex idx ) const {

	if ( idx < this->mPalette.Size ()) {
		return this->mPalette [ idx ];
	}
	return 0;
}

//----------------------------------------------------------------//
ZLColorVec MOAIFancyGrid::GetTileColor ( int addr ) const {

	size_t paletteSize = this->mPalette.Size ();
	ZLColorVec result = ZLColorVec::CLEAR;

	u32 color = this->GetColor ( addr );
	if ( paletteSize ) {
	
		MOAIColor* paletteColor = this->mPalette [ ZLIndex ( color, ZLIndex::LIMIT )];
		if ( paletteColor ) {
			result = *paletteColor;
		}
	}
	else {
		result.SetRGBA ( color );
	}
	return result;
}

//----------------------------------------------------------------//
ZLColorVec MOAIFancyGrid::GetTileColor ( int xTile, int yTile ) const {

	MOAICellCoord coord ( xTile, yTile );
	
	if ( this->IsValidCoord ( coord )) {
		this->GetTileColor ( this->GetCellAddr ( coord ));
	}
	return ZLColorVec::WHITE;
}

//----------------------------------------------------------------//
MOAIFancyGrid::MOAIFancyGrid () :
	mColorSize ( COLOR_SIZE_32 ),
	mColorSet ( 0 ) {
	
	RTTI_SINGLE ( MOAIGrid )
}

//----------------------------------------------------------------//
MOAIFancyGrid::~MOAIFancyGrid () {

	this->DiscardColorSet ();
	this->DiscardPalette ();
}

//----------------------------------------------------------------//
void MOAIFancyGrid::OnResize () {

	MOAIGrid::OnResize ();
	this->DiscardColorSet ();
	this->AffirmColorSet ();
}

//----------------------------------------------------------------//
void MOAIFancyGrid::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGrid::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFancyGrid::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGrid::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "fillColor",			_fillColor },
		{ "getColor",			_getColor },
		{ "getPaletteColor",	_getPaletteColor },
		{ "reservePalette",		_reservePalette },
		{ "setColor",			_setColor },
		{ "setColorRow",		_setColorRow },
		{ "setPaletteColor",	_setPaletteColor },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFancyGrid::ReservePalette ( ZLSize size ) {

	this->DiscardPalette ();

	if ( !size ) {
		this->mColorSize = COLOR_SIZE_32;
		this->DiscardColorSet ();
		return;
	}
	
	u32 colorSizeInBytes = COLOR_SIZE_8;
	
	if ( size > 0xff ) {
	
		colorSizeInBytes = COLOR_SIZE_16;
		
		if ( size > 0xffff ) {
		
			colorSizeInBytes = COLOR_SIZE_32;
		}
	}
	
	this->mPalette.Resize ( size );
	
	if ( this->mColorSize != colorSizeInBytes ) {
	
		this->DiscardColorSet ();
		this->AffirmColorSet ();
	}
}

//----------------------------------------------------------------//
void MOAIFancyGrid::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIGrid::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFancyGrid::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIGrid::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFancyGrid::SetColor ( int addr, u32 value ) {

	if ( addr < this->GetTotalCells ()) {

		switch ( this->mColorSize ) {
		
			case COLOR_SIZE_8:
				(( u8* )this->mColorSet )[ addr ] = value & 0x000000ff;
				break;
			
			case COLOR_SIZE_16:
				(( u16* )this->mColorSet )[ addr ] = value & 0x0000ffff;
				break;
			
			case COLOR_SIZE_32:
				(( u32* )this->mColorSet )[ addr ] = value & 0xffffffff;
				break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIFancyGrid::SetColor ( int xTile, int yTile, u32 value ) {

	MOAICellCoord coord ( xTile, yTile );
	
	if ( this->IsValidCoord ( coord )) {
		this->SetColor ( this->GetCellAddr ( coord ), value );
	}
}

//----------------------------------------------------------------//
void MOAIFancyGrid::SetPaletteColor ( ZLIndex idx, MOAIColor* color ) {

	if ( idx < this->mPalette.Size ()) {
	
		MOAIColor* prevColor = this->mPalette [ idx ];
	
		if ( prevColor != color ) {
			this->LuaRelease ( prevColor );
		}
		this->LuaRetain ( color );
		this->mPalette [ idx ] = color;
	}
}

