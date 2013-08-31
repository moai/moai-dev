// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBROWSERIOS_H
#define	MOAIBROWSERIOS_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moai-core/headers.h>

//================================================================//
// MOAIBrowserIOS
//================================================================//
/**	@name	MOAIBrowserIOS
	@text	Wrapper for access to the native web browser.
			Exposed to Lua via MOAIBrowser on all mobile platforms.
*/
class MOAIBrowserIOS :
	public MOAIGlobalClass < MOAIBrowserIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_canOpenURL			( lua_State* L );
	static int	_openURL			( lua_State* L );
	static int	_openURLWithParams	( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIBrowserIOS )

			MOAIBrowserIOS		();
			~MOAIBrowserIOS		();
	void	RegisterLuaClass	( MOAILuaState& state );
};

#endif
