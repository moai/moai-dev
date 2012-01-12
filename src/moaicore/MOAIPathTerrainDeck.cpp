// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPathTerrainDeck.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getMask
	@text	Returns mask for cell.

	@in		MOAIPathTerrainDeck self
	@in		number idx
	@out	number mask
*/
int MOAIPathTerrainDeck::_getMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UN" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	if ( idx < self->mMasks.Size ()) {
		lua_pushnumber ( state, self->mMasks [ idx ]);
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getTerrainVec
	@text	Returns terrain vector for cell.

	@in		MOAIPathTerrainDeck self
	@in		number idx
	@out	...
*/
int MOAIPathTerrainDeck::_getTerrainVec ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UN" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	float* vector = self->GetVector ( idx );
	
	for ( u32 i = 0; i < self->mVectorSize; ++i ) {
		lua_pushnumber ( state, vector [ i ]);
	}
	return self->mVectorSize;
}

//----------------------------------------------------------------//
/**	@name	setMask
	@text	Returns mask for cell.

	@in		MOAIPathTerrainDeck self
	@in		number idx
	@in		number mask
	@out	nil
*/
int MOAIPathTerrainDeck::_setMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UNN" )
	
	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 mask	= state.GetValue < int >( 3, 0 );
	
	if ( idx < self->mMasks.Size ()) {
		self->mMasks [ idx ] = mask;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTerrainVec
	@text	Sets terrain vector for cell.

	@in		MOAIPathTerrainDeck self
	@in		number idx
	@in		...
	@out	nil
*/
int MOAIPathTerrainDeck::_setTerrainVec ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UN" )
	
	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total	= lua_gettop ( state ) - 2;
	
	if ( total > self->mVectorSize ) {
		total = self->mVectorSize;
	}
	
	float* vector = self->GetVector ( idx );
	
	for ( u32 i = 0; i < total; ++i ) {
		vector [ i ] = state.GetValue < float >( 3 + i, 0.0f );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserve
	@text	Allocates terrain vectors.

	Pin		MOAIPathTerrainDeck self
	@in		number deckSize
	@in		number terrainVecSize
	@out	nil
*/
int MOAIPathTerrainDeck::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathTerrainDeck, "UN" )
	
	self->mDeckSize		= state.GetValue < u32 >( 2, 0 );
	self->mVectorSize	= state.GetValue < u32 >( 3, 0 );
	
	self->mMasks.Init ( self->mDeckSize );
	self->mMasks.Fill ( 0xffffffff );
	
	self->mVectors.Init ( self->mDeckSize * self->mVectorSize );
	self->mVectors.Fill ( 0.0f );

	return 0;
}

//================================================================//
// MOAIPathTerrainDeck
//================================================================//

//----------------------------------------------------------------//
u32 MOAIPathTerrainDeck::GetMask ( u32 idx ) {

	return this->mMasks [ idx - 1 ];
}

//----------------------------------------------------------------//
float* MOAIPathTerrainDeck::GetVector ( u32 idx ) {

	return &this->mVectors [( idx - 1 ) * this->mVectorSize ];
}

//----------------------------------------------------------------//
MOAIPathTerrainDeck::MOAIPathTerrainDeck () :
	mDeckSize ( 0 ),
	mVectorSize ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIPathTerrainDeck::~MOAIPathTerrainDeck () {
}

//----------------------------------------------------------------//
void MOAIPathTerrainDeck::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPathTerrainDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getMask",				_getMask },
		{ "getTerrainVec",			_getTerrainVec },
		{ "setMask",				_setMask },
		{ "setTerrainVec",			_setTerrainVec },
		{ "reserve",				_reserve },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
