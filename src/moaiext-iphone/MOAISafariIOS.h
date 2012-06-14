// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISAFARIIOS_H
#define	MOAISAFARIIOS_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>

//================================================================//
// MOAISafariIOS
//================================================================//
/**	@name	MOAISafariIOS
	@text	Wrapper for access to the native web browser.
*/
class MOAISafariIOS :
	public MOAIGlobalClass < MOAISafariIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_openURL			( lua_State* L );
	static int	_openURLWithParams	( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAISafariIOS )
	
			MOAISafariIOS		();
			~MOAISafariIOS		();
	void	RegisterLuaClass	( MOAILuaState& state );
};

#endif
