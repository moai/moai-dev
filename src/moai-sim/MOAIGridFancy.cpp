// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGridFancy.h>
#include <moai-sim/MOAIGfxDevice.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	fillAlpha
	@text	Set all tiles to a single alpha

	@in		MOAIGridFancy self
	@in		number value
	@out	nil
*/
int MOAIGridFancy::_fillAlpha ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UN" )

	float value	= state.GetValue < float >( 2, 1 );
	
	self->FillAlpha ( value );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	fillScale
	@text	Set all tiles to a single scale

	@in		MOAIGridFancy self
	@in		number value
	@out	nil
*/
int MOAIGridFancy::_fillScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UN" )

	float value	= state.GetValue < float >( 2, 1 );
	
	self->FillScale ( value );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	fillColor
	@text	Set all tiles to a single color

	@in		MOAIGridFancy self
	@in		number value
	@out	nil
*/
int MOAIGridFancy::_fillColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UN" )

	u32 value	= state.GetValue < u32 >( 2, 1 );
	
	self->FillColor ( value );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAlpha
	@text	Returns the alpha channel of a given tile.

	@in		MOAIGridFancy self
	@in		number xTile
	@in		number yTile
	@out	number alpha
*/
int MOAIGridFancy::_getAlpha ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	float tile = self->GetAlpha ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getColor
	@text	Returns the color index of a given tile.

	@in		MOAIGridFancy self
	@in		number xTile
	@in		number yTile
	@out	number color (index into palette)
*/
int MOAIGridFancy::_getColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	u32 tile = self->GetColor ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getScale
	@text	Returns the scale multiplier of a given tile.

	@in		MOAIGridFancy self
	@in		number xTile
	@in		number yTile
	@out	number scale
*/
int MOAIGridFancy::_getScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	float tile = self->GetScale ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPalette
	@text	Returns the color components of a given palette index

	@in		MOAIGridFancy self
	@in		number index
	@out	number r
	@out	number g
	@out	number b
	@out	number a
*/
int MOAIGridFancy::_getPalette ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UN" )

	int idx	= state.GetValue < int >( 2, 1 );
	
	ZLColorVec color = self->GetPalette ( idx );
	state.Push ( color.mR );
	state.Push ( color.mG );
	state.Push ( color.mB );
	state.Push ( color.mA );
	return 4;
}

//----------------------------------------------------------------//
/**	@name	setRowAlpha
	@text	Initializes a grid row's alpha values given a variable argument list of values.

	@in		MOAIGridFancy self
	@in		number row
	@in		...
	@out	nil
*/
int MOAIGridFancy::_setRowAlpha ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UN" )

	u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total = lua_gettop ( state ) - 2;
	
	for ( u32 i = 0; i < total; ++i ) {
	
		float tile = state.GetValue < float >( 3 + i, 0 );
		self->SetAlpha ( i, row, tile );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRowScale
	@text	Initializes a grid row's scale values given a variable argument list of values.

	@in		MOAIGridFancy self
	@in		number row
	@in		...
	@out	nil
*/
int MOAIGridFancy::_setRowScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UN" )

	u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total = lua_gettop ( state ) - 2;
	
	for ( u32 i = 0; i < total; ++i ) {
	
		float tile = state.GetValue < float >( 3 + i, 0 );
		self->SetScale ( i, row, tile );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRowColor
	@text	Initializes a grid row's color values given a variable argument list of values.

	@in		MOAIGridFancy self
	@in		number row
	@in		... [palette indexes]
	@out	nil
*/
int MOAIGridFancy::_setRowColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UN" )

	u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total = lua_gettop ( state ) - 2;
	
	for ( u32 i = 0; i < total; ++i ) {
	
		u32 tile = state.GetValue < u32 >( 3 + i, 0 );
		self->SetColor ( i, row, tile );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAlpha
	@text	Sets the alpha of a given tile

	@in		MOAIGridFancy self
	@in		number xTile
	@in		number yTile
	@in		number value
	@out	nil
*/
int MOAIGridFancy::_setAlpha ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	float alpha	= state.GetValue < float >( 4, 0 );
	
	self->SetAlpha ( xTile, yTile, alpha );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setColor
	@text	Sets the color index of a given tile

	@in		MOAIGridFancy self
	@in		number xTile
	@in		number yTile
	@in		number value
	@out	nil
*/
int MOAIGridFancy::_setColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	u32 tile	= state.GetValue < u32 >( 4, 0 );
	
	self->SetColor ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setScale
	@text	Sets the scale of a given tile

	@in		MOAIGridFancy self
	@in		number xTile
	@in		number yTile
	@in		number value
	@out	nil
*/
int MOAIGridFancy::_setScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	float scale	= state.GetValue < float >( 4, 0 );
	
	self->SetScale ( xTile, yTile, scale );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPalette
	@text	Sets the color components for a given palette entry

	@in		MOAIGridFancy self
	@in		number idx
	@in		number r
	@in		number g
	@in		number b
	@in		number a (optional, default is 1)
	@out	nil
*/
int MOAIGridFancy::_setPalette ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridFancy, "UNNNN" )

	int addr	= state.GetValue < int >( 2, 1 );
	float r		= state.GetValue < float >( 3, 1 );
	float g		= state.GetValue < float >( 4, 1 );
	float b		= state.GetValue < float >( 5, 1 );
	float a;
	if ( lua_gettop ( state ) >= 6 ) {
		a = state.GetValue < float >( 6, 1 );
	} else {
		a = 1.0;
	}
	
	self->SetPalette ( addr, ZLColorVec ( r, g, b, a ) );
	
	return 0;
}


//================================================================//
// MOAIGridFancy
//================================================================//

//----------------------------------------------------------------//
void MOAIGridFancy::FillAlpha ( float value ) {

	this->mAlphas.Fill ( value );
	return;
}

//----------------------------------------------------------------//
void MOAIGridFancy::FillColor ( u32 value ) {

	this->mColors.Fill ( value );
	return;
}

//----------------------------------------------------------------//
void MOAIGridFancy::FillScale ( float value ) {

	this->mScales.Fill ( value );
	return;
}

//----------------------------------------------------------------//
float MOAIGridFancy::GetAlpha ( int xTile, int yTile ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mAlphas.Size ()) {
			return this->mAlphas [ addr ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
float MOAIGridFancy::GetScale ( int xTile, int yTile ) {

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
ZLColorVec MOAIGridFancy::GetPalette ( u32 addr ) {

	if ( addr < this->mColorPalette.Size ()) {
		return this->mColorPalette [ addr ];
	}
	return ZLColorVec ( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------//
u32 MOAIGridFancy::GetColor ( int xTile, int yTile ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mColors.Size ()) {
			return this->mColors [ addr ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIGridFancy::MOAIGridFancy () {
	
	RTTI_SINGLE ( MOAIGrid )

	this->mColorPalette.Init ( 1 );
	this->mColorPalette [ 0 ] = ZLColorVec ( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------//
MOAIGridFancy::~MOAIGridFancy () {
}

//----------------------------------------------------------------//
void MOAIGridFancy::OnResize () {

	MOAIGrid::OnResize ();
	this->mColorPalette.Init ( 1 );
	this->mAlphas.Init ( this->GetTotalCells () );
	this->mColors.Init ( this->GetTotalCells () );
	this->mScales.Init ( this->GetTotalCells () );
	this->mAlphas.Fill ( 1.0 );
	this->mColors.Fill ( 0 );
	this->mScales.Fill ( 1.0 );
}

//----------------------------------------------------------------//
void MOAIGridFancy::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGrid::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGridFancy::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGrid::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "fillAlpha",			_fillAlpha },
		{ "fillColor",			_fillColor },
		{ "fillScale",			_fillScale },
		{ "getAlpha",			_getAlpha },
		{ "getColor",			_getColor },
		{ "getPalette",			_getPalette },
		{ "getScale",			_getScale },
		{ "setAlpha",			_setAlpha },
		{ "setColor",			_setColor },
		{ "setPalette",			_setPalette },
		{ "setRowAlpha",		_setRowAlpha },
		{ "setRowColor",		_setRowColor },
		{ "setRowScale",		_setRowScale },
		{ "setScale",			_setScale },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGridFancy::SetColor ( u32 addr, u32 value ) {

	u32 size = this->mColors.Size ();

	if ( size ) {
		addr = addr % size;
		this->mColors [ addr ] = value;
	}
}

//----------------------------------------------------------------//
void MOAIGridFancy::SetColor ( int xTile, int yTile, u32 value ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
	
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mColors.Size ()) {
			this->mColors [ addr ] = value;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGridFancy::SetAlpha ( u32 addr, float value ) {

	u32 size = this->mAlphas.Size ();

	if ( size ) {
		addr = addr % size;
		this->mAlphas [ addr ] = value;
	}
}

//----------------------------------------------------------------//
void MOAIGridFancy::SetScale ( u32 addr, float value ) {

	u32 size = this->mScales.Size ();

	if ( size ) {
		addr = addr % size;
		this->mScales [ addr ] = value;
	}
}

//----------------------------------------------------------------//
void MOAIGridFancy::SetPalette ( u32 addr, const ZLColorVec &color ) {

	/* attempt to make palette large enough for this */
	this->mColorPalette.Grow ( addr, 1, ZLColorVec ( 1.0, 1.0, 1.0, 1.0 ) );
	u32 size = this->mColorPalette.Size ();

	if ( size ) {
		addr = addr % size;
		this->mColorPalette [ addr ] = color;
	}
}

//----------------------------------------------------------------//
void MOAIGridFancy::SetAlpha ( int xTile, int yTile, float value ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
	
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mAlphas.Size ()) {
			this->mAlphas [ addr ] = value;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGridFancy::SetScale ( int xTile, int yTile, float value ) {

	MOAICellCoord coord ( xTile, yTile );
	if ( this->IsValidCoord ( coord )) {
	
		u32 addr = this->GetCellAddr ( coord );
		if ( addr < this->mScales.Size ()) {
			this->mScales [ addr ] = value;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGridFancy::Draw ( MOAIDeck *deck, MOAIDeckRemapper *remapper, const MOAICellCoord &c0, const MOAICellCoord &c1 ) {
	float tileWidth = this->GetTileWidth();
	float tileHeight = this->GetTileHeight();
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	ZLColorVec penColor = gfxDevice.GetPenColor();

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = this->WrapCellCoord ( x, y );
			u32 idx = this->GetTile ( wrap.mX, wrap.mY );
			u32 color = this->GetColor ( wrap.mX, wrap.mY );
			float alpha = this->GetAlpha ( wrap.mX, wrap.mY );
			float scale = this->GetScale ( wrap.mX, wrap.mY );
			
			MOAICellCoord coord ( x, y );
			USVec2D loc = this->GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );

			if (color) {
				gfxDevice.SetPenColor ( penColor * this->GetPalette ( color ).ScaleAlpha ( alpha ) );
			} else {
				gfxDevice.SetPenColor ( penColor * ZLColorVec ( 1.0, 1.0, 1.0, alpha ) );
			}
			deck->Draw ( idx, remapper, loc.mX, loc.mY, 0.0f, tileWidth * scale, tileHeight * scale, 1.0f);
		}
	}
	
	gfxDevice.SetPenColor(penColor);
}
