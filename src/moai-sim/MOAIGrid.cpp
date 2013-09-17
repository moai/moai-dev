// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>

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
void MOAIGrid::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITypedGrid<u32>::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGrid::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITypedGrid<u32>::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "clearTileFlags",		_clearTileFlags },
		{ "getTileFlags",		_getTileFlags },
		{ "setTileFlags",		_setTileFlags },
		{ "toggleTileFlags",	_toggleTileFlags },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
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

template <> int MOAITypedGrid<u32>::_fill ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UN" )

	u32 value	= state.GetValue < u32 >( 2, 1 );
	
	self->Fill ( value );
	
	return 0;
}

template <> int MOAITypedGrid<u32>::_getTile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	u32 tile = self->GetTile ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

template <> int MOAITypedGrid<u32>::_setRow ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UN" )

	u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total = lua_gettop ( state ) - 2;
	
	for ( u32 i = 0; i < total; ++i ) {
	
		u32 tile = state.GetValue < u32 >( 3 + i, 0 );
		self->SetTile ( i, row, tile );
	}

	return 0;
}

template <> int MOAITypedGrid<u32>::_setTile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	u32 tile	= state.GetValue < u32 >( 4, 0 );
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

MOAIGrid::MOAIGrid () {
	
	RTTI_SINGLE ( MOAITypedGrid < u32 > )
	default_value = 0;
}

MOAIGrid::~MOAIGrid () {
}
