// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUASTATEHANDLE_H
#define LUASTATEHANDLE_H

#include <uslscore/USLuaState.h>

//================================================================//
// USLuaStateHandle
//================================================================//
class USLuaStateHandle :
	public USLuaState {
private:

	int mRestoreTop;

public:

	//----------------------------------------------------------------//
	void			PinTop					();
	void			PinTop					( int top );
	void			Take					( const USLuaStateHandle& assign );
					USLuaStateHandle		();
					USLuaStateHandle		( lua_State* L );
					USLuaStateHandle		( USLuaState& state );
					USLuaStateHandle		( const USLuaStateHandle& assign );
	virtual			~USLuaStateHandle		();

	//----------------------------------------------------------------//
	inline void operator = ( const USLuaStateHandle& assign ) {
		this->Take ( assign );
	}
};

#endif
