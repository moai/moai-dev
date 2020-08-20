// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractDrawable.h>
#include <moai-gfx/MOAIGfxMgr.h>

//================================================================//
// MOAIAbstractDrawable
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDrawable::MOAIAbstractDrawable () {
}

//----------------------------------------------------------------//
MOAIAbstractDrawable::~MOAIAbstractDrawable () {
}

//----------------------------------------------------------------//
void MOAIAbstractDrawable::Draw ( MOAILuaMemberRef& ref, bool debug ) {

	if ( ref ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( ref );
		MOAIAbstractDrawable::Draw ( state, -1, debug );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAIAbstractDrawable::Draw ( MOAILuaState& state, int idx, bool debug ) {

	idx = state.AbsIndex ( idx );
	int valType = lua_type ( state, idx );
	
	switch ( valType ) {
	
		case LUA_TUSERDATA: {
		
			MOAIAbstractDrawable* drawable = state.GetLuaObject < MOAIAbstractDrawable >( idx, false );
			
			if ( drawable ) {
				if ( debug ) {
					drawable->DrawDebug ();
				}
				else {
					drawable->Draw ();
				}
			}
			break;
		}
		
		case LUA_TTABLE: {
		
			size_t tableSize = state.GetTableSize ( idx );
			for ( size_t i = 0; i < tableSize; ++i ) {
				lua_rawgeti ( state, idx, i + 1 );
				MOAIAbstractDrawable::Draw ( state, -1, debug );
				lua_pop ( state, 1 );
			}
			break;
		}
		
		case LUA_TFUNCTION: {
				
//			state.CopyToTop ( idx ); // copy the function to the top
//			state.Push ( MOAIGfxMgr::Get ().GetDrawingObject ());
//			state.DebugCall ( 1, 0 );
			break;
		}
	}
}
