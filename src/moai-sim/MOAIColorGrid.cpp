// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColorGrid.h>

//----------------------------------------------------------------//
void MOAIColorGrid::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITypedGrid<ZLColorVec>::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIColorGrid::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITypedGrid<ZLColorVec>::RegisterLuaFuncs ( state );

}

MOAIColorGrid::MOAIColorGrid () {
	
	RTTI_SINGLE ( MOAITypedGrid < ZLColorVec > )
	default_value = ZLColorVec ( 1, 1, 1, 1 );
}

MOAIColorGrid::~MOAIColorGrid () {
}

static ZLColorVec getColorFromStack ( MOAILuaState &state, int offset, int remaining ) {
	ZLColorVec color;

	switch (remaining) {
	case 0:
		default:
		color.mR = color.mG = color.mB = color.mA = 1.0f;
		break;
	case 2:
		color.mA = state.GetValue < float >( offset + 2, 1.0 );
	/* fallthrough */
	case 1:
		color.mR = color.mG = color.mB = state.GetValue < float >( offset + 1, 1.0 );
		break;
	case 4:
		color.mA = state.GetValue < float >( offset + 4, 1.0 );
	/* fallthrough */
		case 3:
		color.mR = state.GetValue < float >( offset + 1, 1.0 );
		color.mG = state.GetValue < float >( offset + 2, 1.0 );
		color.mB = state.GetValue < float >( offset + 3, 1.0 );
		break;
	} 
	return color;
}

static ZLColorVec getColorFromStack ( MOAILuaState &state, int offset ) {
	int remaining = lua_gettop ( state ) - offset;
	return getColorFromStack ( state, offset, remaining );
}

template <> int MOAITypedGrid<ZLColorVec>::_fill			( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColorGrid, "U" )

	ZLColorVec value = getColorFromStack ( state, 1 );
	
	self->Fill ( value );
	
	return 0;
}
template <> int MOAITypedGrid<ZLColorVec>::_getTile		( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColorGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	ZLColorVec tile = self->GetTile ( xTile, yTile );
	state.Push ( tile.mR );
	state.Push ( tile.mG );
	state.Push ( tile.mB );
	state.Push ( tile.mA );
	return 4;
}

template <> int MOAITypedGrid<ZLColorVec>::_setRow			( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColorGrid, "UN" )

	u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total = (lua_gettop ( state ) - 2) / 4;
	
	for ( u32 i = 0; i < total; ++i ) {
		ZLColorVec color = getColorFromStack ( state, 3 + (i * 4), 4 );
		self->SetTile ( i, row, color );
	}

	return 0;
}

template <> int MOAITypedGrid<ZLColorVec>::_setTile		( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColorGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	ZLColorVec value = getColorFromStack ( state, 3 );
	
	self->SetTile ( xTile, yTile, value );
	
	return 0;
}
