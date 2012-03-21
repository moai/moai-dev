// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICRITTERCISM_H
#define	MOAICRITTERCISM_H

#ifndef DISABLE_CRITTERCISM

#include <moaicore/moaicore.h>

//================================================================//
// MOAICrittercismiOS
//================================================================//
/**	@name	MOAICrittercismiOS
	@text	Wrapper for Crittercism integration on iOS devices.
			Crittercism provides real-time, actionable crash reports 
			for mobile apps. Exposed to lua via MOAICrittercism on
			all mobile platforms.
*/
class MOAICrittercismiOS :
	public MOAIGlobalClass < MOAICrittercismiOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init 	( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAICrittercismiOS );
	
					MOAICrittercismiOS		();
					~MOAICrittercismiOS		();
	void			RegisterLuaClass		( MOAILuaState& state );
};

#endif  //DISABLE_CRITTERCISM

#endif  //MOAICRITTERCISM_H
