// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICRITTERCISMIOS_H
#define	MOAICRITTERCISMIOS_H

#ifndef DISABLE_CRITTERCISM

#import <Foundation/Foundation.h> 
#import <moaicore/moaicore.h>

//================================================================//
// MOAICrittercismIOS
//================================================================//
/**	@name	MOAICrittercismIOS
	@text	Wrapper for Crittercism integration on iOS devices.
			Crittercism provides real-time, actionable crash reports 
			for mobile apps. Exposed to lua via MOAICrittercism on
			all mobile platforms.
*/
class MOAICrittercismIOS :
	public MOAIGlobalClass < MOAICrittercismIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init 				( lua_State* L );
	static int	_forceException		( lua_State* L );
	static int	_leaveBreadcrumb 	( lua_State* L );
	static int	_setUser			( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAICrittercismIOS );
	
			MOAICrittercismIOS		();
			~MOAICrittercismIOS		();
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //DISABLE_CRITTERCISM

#endif  //MOAICRITTERCISM_H
