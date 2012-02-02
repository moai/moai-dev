// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICRITTERCISM_H
#define	MOAICRITTERCISM_H

#ifndef DISABLE_CRITTERCISM

#include <moaicore/moaicore.h>

//================================================================//
// MOAICrittercism
//================================================================//
class MOAICrittercism :
	public MOAIGlobalClass < MOAICrittercism, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int		_init 	( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAICrittercism );
	
					MOAICrittercism					();
					~MOAICrittercism				();
	void			RegisterLuaClass				( MOAILuaState& state );
};

#endif  //DISABLE_CRITTERCISM

#endif  //MOAICRITTERCISM_H
