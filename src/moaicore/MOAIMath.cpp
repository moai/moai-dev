// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIMath.h>

extern "C" {
	#include <sfmt.h>
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMath::_randSFMT ( lua_State* L ) {
	MOAILuaState state ( L );
	
	double lower = 0.0;
	double upper = 1.0;
	
	if ( state.IsType ( 1, LUA_TNUMBER )) {
	
		upper = state.GetValue < double >( 1, 0.0 );
	
		if ( state.IsType ( 2, LUA_TNUMBER )) {
			lower = upper;
			upper = state.GetValue < double >( 2, 0.0 );
		}
	}
	
	double r = sfmt_genrand_real1 ( MOAIMath::Get ().mSFMT ); // [0, 1]
	state.Push ( lower + ( r * ( upper - lower )));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMath::_seedSFMT ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 seed = state.GetValue < u32 >( 1, 0 );
	sfmt_init_gen_rand ( MOAIMath::Get ().mSFMT, seed );
	
	return 0;
}

//================================================================//
// MOAIMath
//================================================================//

//----------------------------------------------------------------//
MOAIMath::MOAIMath () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
	
	this->mSFMT = ( SFMT_T* )calloc ( 1, sizeof ( SFMT_T ));
}

//----------------------------------------------------------------//
MOAIMath::~MOAIMath () {

	free ( this->mSFMT );
}

//----------------------------------------------------------------//
void MOAIMath::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "randSFMT",			_randSFMT },
		{ "seedSFMT",			_seedSFMT },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
