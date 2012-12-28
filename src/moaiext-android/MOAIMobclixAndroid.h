// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOBCLIXIOS_H
#define	MOAIMOBCLIXIOS_H

#ifndef DISABLE_MOBCLIX


#import <moaicore/moaicore.h>

//================================================================//
// MOAIMobclixAndroid
//================================================================//
/**	@name	MOAIMobclixAndroid
	@text	Wrapper for Crittercism integration on iOS devices.
			Crittercism provides real-time, actionable crash reports 
			for mobile apps. Exposed to lua via MOAICrittercism on
			all mobile platforms.
*/

class MOAIMobclixAndroid :
	public MOAIGlobalClass < MOAIMobclixAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init 				( lua_State* L );
	static int	_showAds            ( lua_State* L );
	static int	_hideAds            ( lua_State* L );
	static int	_dealloc			( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIMobclixAndroid );
	
			MOAIMobclixAndroid		();
			~MOAIMobclixAndroid	();
	void	RegisterLuaClass		( MOAILuaState& state );
};



#endif  //DISABLE_MOBCLIX

#endif  //MOAIMOBCLIXIOS_H
