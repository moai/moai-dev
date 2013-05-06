// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICRITTERCISMANDROID_H
#define	MOAICRITTERCISMANDROID_H

#ifndef DISABLE_CRITTERCISM

#include <moaicore/moaicore.h>

//================================================================//
// MOAICrittercismAndroid
//================================================================//
/**	@name	MOAICrittercismAndroid
	@text	Wrapper for Crittercism integration on Android devices.
			Crittercism provides real-time, actionable crash reports 
			for mobile apps. Exposed to lua via MOAICrittercism on
			all mobile platforms.
*/
class MOAICrittercismAndroid :
	public MOAIGlobalClass < MOAICrittercismAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init 				( lua_State* L );
	static int	_leaveBreadcrumb 	( lua_State* L );
	static int	_setUser		 	( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAICrittercismAndroid );
	
			MOAICrittercismAndroid		();
			~MOAICrittercismAndroid		();
	void	RegisterLuaClass			( MOAILuaState& state );
};

#endif  //DISABLE_CRITTERCISM

#endif  //MOAICRITTERCISM_H
