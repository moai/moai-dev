// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIFloatGrid.h>

template <> int MOAITypedGrid<float>::_fill ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFloatGrid, "UN" )

	float value	= state.GetValue < float >( 2, 1 );
	
	self->Fill ( value );
	
	return 0;
}

template <> int MOAITypedGrid<float>::_getTile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFloatGrid, "UNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	
	float tile = self->GetTile ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

template <> int MOAITypedGrid<float>::_setRow ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFloatGrid, "UN" )

	u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total = lua_gettop ( state ) - 2;
	
	for ( float i = 0; i < total; ++i ) {
	
		float tile = state.GetValue < float >( 3 + i, 0 );
		self->SetTile ( i, row, tile );
	}

	return 0;
}

template <> int MOAITypedGrid<float>::_setTile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFloatGrid, "UNNN" )

	int xTile	= state.GetValue < int >( 2, 1 ) - 1;
	int yTile	= state.GetValue < int >( 3, 1 ) - 1;
	float tile	= state.GetValue < float >( 4, 0 );
	
	self->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
void MOAIFloatGrid::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITypedGrid<float>::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFloatGrid::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITypedGrid<float>::RegisterLuaFuncs ( state );

}

MOAIFloatGrid::MOAIFloatGrid () {
	
	RTTI_SINGLE ( MOAITypedGrid < float > )
	default_value = 1.0;
}

MOAIFloatGrid::~MOAIFloatGrid () {
}
