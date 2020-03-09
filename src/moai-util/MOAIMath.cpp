// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIMath.h>

#if MOAI_WITH_SFMT
	extern "C" {
		#include <SFMT.h>
	}
#endif

//================================================================//
// lua
//================================================================//

#if MOAI_WITH_SFMT

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

#endif

//================================================================//
// MOAIMath
//================================================================//

//----------------------------------------------------------------//
MOAIMath::MOAIMath () {

	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIMath )
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
	
	#if MOAI_WITH_SFMT
		this->mSFMT = ( SFMT_T* )calloc ( 1, sizeof ( SFMT_T ));
		sfmt_init_gen_rand ( this->mSFMT, ( u32 )time ( 0 ));
	#endif
}

//----------------------------------------------------------------//
MOAIMath::~MOAIMath () {

	#if MOAI_WITH_SFMT
		free ( this->mSFMT );
	#endif
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMath::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );

	luaL_Reg regTable [] = {
		#if MOAI_WITH_SFMT
			{ "randSFMT",			_randSFMT },
			{ "seedSFMT",			_seedSFMT },
		#endif
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
