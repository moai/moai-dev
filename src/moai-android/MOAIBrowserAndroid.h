// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBROWSERANDROID_H
#define	MOAIBROWSERANDROID_H

#include <moai-core/headers.h>

//================================================================//
// MOAIBrowserAndroid
//================================================================//
/**	@name	MOAIBrowserAndroid
	@text	Wrapper for access to the native web browser.
			Exposed to Lua via MOAIBrowser on all mobile platforms.
*/
class MOAIBrowserAndroid :
	public MOAIGlobalClass < MOAIBrowserAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_canOpenURL			( lua_State* L );
	static int	_openURL			( lua_State* L );
	static int	_openURLWithParams	( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIBrowserAndroid )

			MOAIBrowserAndroid				();
			~MOAIBrowserAndroid				();
	void	RegisterLuaClass			( MOAILuaState& state );
};

#endif
