// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGridPathGraph.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getVector
	@text	Returns vector for cell.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@out	...
*/
int MOAIGridPathGraph::_getVector ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridPathGraph, "UNN" )
	
	int xTile = state.GetValue < int >( 2, 1 ) - 1;
	int yTile = state.GetValue < int >( 3, 1 ) - 1;
	
	float* vector = self->GetVector ( xTile, yTile );
	
	for ( u32 i = 0; i < self->mVectorSize; ++i ) {
		lua_pushnumber ( state, vector [ i ]);
	}
	
	return self->mVectorSize;
}

//----------------------------------------------------------------//
/**	@name	setVector
	@text	Sets vector for cell.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		...
	@out	nil
*/
int MOAIGridPathGraph::_setVector ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridPathGraph, "UNN" )
	
	int xTile = state.GetValue < int >( 2, 1 ) - 1;
	int yTile = state.GetValue < int >( 3, 1 ) - 1;
	u32 total = lua_gettop ( state ) - 3;
	
	if ( total > self->mVectorSize ) {
		total = self->mVectorSize;
	}
	
	float* vector = self->GetVector ( xTile, yTile );
	
	for ( u32 i = 0; i < total; ++i ) {
		vector [ i ] = state.GetValue < float >( 4 + i, 0.0f );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVectorSize
	@text	Sets size of per-cell vector.

	@in		MOAIGrid self
	@in		number size
	@out	nil
*/
int MOAIGridPathGraph::_setVectorSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridPathGraph, "UN" )
	
	self->mVectorSize = state.GetValue < u32 >( 2, 0 );
	self->OnResize ();
	
	return 0;
}

//================================================================//
// MOAIGridPathGraph
//================================================================//

//----------------------------------------------------------------//
float* MOAIGridPathGraph::GetVector ( int xTile, int yTile ) {

	return &this->mVectors [ this->GetCellAddr ( xTile, yTile ) * this->mVectorSize ];
}

//----------------------------------------------------------------//
MOAIGridPathGraph::MOAIGridPathGraph () {

	RTTI_SINGLE ( MOAIPathGraph )
}

//----------------------------------------------------------------//
MOAIGridPathGraph::~MOAIGridPathGraph () {
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::OnResize () {

	u32 size = this->GetTotalCells () * this->mVectorSize;
	
	if ( size != this->mVectors.Size ()) {
		this->mVectors.Resize ( size );
		this->mVectors.Fill ( 0.0f );
	}
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );

	luaL_Reg regTable [] = {
		{ "getVector",			_getVector },
		{ "setVector",			_setVector },
		{ "setVectorSize",		_setVectorSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
