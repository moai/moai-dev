// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISCOPEDLUASTATE_H
#define MOAISCOPEDLUASTATE_H

#include <moai-core/MOAILuaState.h>

//================================================================//
// MOAIScopedLuaState
//================================================================//
class MOAIScopedLuaState :
	public MOAILuaState {
private:

	int mRestoreTop;

	//----------------------------------------------------------------//
	void			Restore					();

public:

	//----------------------------------------------------------------//
	void			PinTop					();
	void			PinTop					( int top );
	void			Set						( lua_State* L );
	void			Take					( const MOAIScopedLuaState& assign );
					MOAIScopedLuaState		();
					MOAIScopedLuaState		( lua_State* L );
					MOAIScopedLuaState		( MOAILuaState& state );
					MOAIScopedLuaState		( const MOAIScopedLuaState& assign );
	virtual			~MOAIScopedLuaState		();

	//----------------------------------------------------------------//
	inline void operator = ( const lua_State* L ) {
		this->Set (( lua_State* )L );
	}

	//----------------------------------------------------------------//
	inline void operator = ( const MOAIScopedLuaState& assign ) {
		this->Take ( assign );
	}
};

#endif
