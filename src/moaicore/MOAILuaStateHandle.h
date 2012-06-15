// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUASTATEHANDLE_H
#define MOAILUASTATEHANDLE_H

#include <moaicore/MOAILuaState.h>

//================================================================//
// MOAILuaStateHandle
//================================================================//
class MOAILuaStateHandle :
	public MOAILuaState {
private:

	int mRestoreTop;

public:

	//----------------------------------------------------------------//
	void			PinTop					();
	void			PinTop					( int top );
	void			Take					( const MOAILuaStateHandle& assign );
					MOAILuaStateHandle		();
					MOAILuaStateHandle		( lua_State* L );
					MOAILuaStateHandle		( MOAILuaState& state );
					MOAILuaStateHandle		( const MOAILuaStateHandle& assign );
	virtual			~MOAILuaStateHandle		();

	//----------------------------------------------------------------//
	inline void operator = ( const MOAILuaStateHandle& assign ) {
		this->Take ( assign );
	}
};

#endif
