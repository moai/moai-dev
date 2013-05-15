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

public:

	//----------------------------------------------------------------//
	void			PinTop					();
	void			PinTop					( int top );
	void			Take					( const MOAIScopedLuaState& assign );
					MOAIScopedLuaState		();
					MOAIScopedLuaState		( lua_State* L );
					MOAIScopedLuaState		( MOAILuaState& state );
					MOAIScopedLuaState		( const MOAIScopedLuaState& assign );
	virtual			~MOAIScopedLuaState		();

	//----------------------------------------------------------------//
	inline void operator = ( const MOAIScopedLuaState& assign ) {
		this->Take ( assign );
	}
};

#endif
