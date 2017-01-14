// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDrawable.h>

//================================================================//
// MOAIDrawable
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawable::Draw ( MOAILuaMemberRef& ref ) {

	if ( ref ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( ref );
		MOAIDrawable::Draw ( state, -1 );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAIDrawable::Draw ( MOAILuaState& state, int idx ) {

	idx = state.AbsIndex ( idx );
	int valType = lua_type ( state, idx );
	
	switch ( valType ) {
	
		case LUA_TUSERDATA: {
		
			MOAIDrawable* drawable = state.GetLuaObject < MOAIDrawable >( idx, false );
			if ( drawable ) {
				drawable->Draw ();
			}
			break;
		}
		
		case LUA_TTABLE: {
		
			size_t tableSize = state.GetTableSize ( idx );
			for ( size_t i = 0; i < tableSize; ++i ) {
				lua_rawgeti ( state, idx, i + 1 );
				MOAIDrawable::Draw ( state, -1 );
				lua_pop ( state, 1 );
			}
			break;
		}
		
		case LUA_TFUNCTION:
		
			state.CopyToTop ( idx );
			state.DebugCall ( 0, 0 );
			break;
	}
}
