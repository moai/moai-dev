// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMATH_H
#define MOAIMATH_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIMath
//================================================================//
// TODO: doxygen
class MOAIMath :
	public MOAIGlobalClass < MOAIMath, MOAILuaObject > {
private:
	
	struct SFMT_T*	mSFMT;
	
	//----------------------------------------------------------------//
	static int		_randSFMT			( lua_State* L );
	static int		_seedSFMT			( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIMath )

	//----------------------------------------------------------------//
					MOAIMath			();
					~MOAIMath			();
	void			RegisterLuaClass	( MOAILuaState& state );
};

#endif
