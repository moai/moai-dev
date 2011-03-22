// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUASTATEHANDLE_H
#define LUASTATEHANDLE_H

/** @addtogroup LuaLib */

#include <uslscore/USLuaState.h>

//================================================================//
// USLuaStateHandle
//================================================================//
/**	@brief Convenience class that extends USLuaState.  Stores Lua stack top when
	constructed then automatically restores stack depth when destructed.
	@ingroup LuaLib
*/
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
