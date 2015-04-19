// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICRITTERCISMANDROID_H
#define	MOAICRITTERCISMANDROID_H



#include <moai-core/headers.h>

//================================================================//
// MOAICrittercismAndroid
//================================================================//
/**	@lua	MOAICrittercismAndroid
	@text	Wrapper for Crittercism integration on Android devices.
			Crittercism provides real-time, actionable crash reports 
			for mobile apps. Exposed to Lua via MOAICrittercism on
			all mobile platforms.
*/
class MOAICrittercismAndroid :
	public MOAIGlobalClass < MOAICrittercismAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init 				( lua_State* L );
	static int	_leaveBreadcrumb 	( lua_State* L );
	static int	_setUser		 	( lua_State* L );
	static int  _forceException	    ( lua_State* L );
public:

	DECL_LUA_SINGLETON ( MOAICrittercismAndroid );
	
			MOAICrittercismAndroid		();
			~MOAICrittercismAndroid		();
	void	RegisterLuaClass			( MOAILuaState& state );
};



#endif  //MOAICRITTERCISM_H
