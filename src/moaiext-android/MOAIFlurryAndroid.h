//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIFLURRYANDROID_H
#define MOAIFLURRYANDROID_H

#ifndef DISABLE_FLURRY

#include <moaicore/moaicore.h>

//================================================================//
// MOAIFlurryIOS
//================================================================//
/**	@name	MOAIFlurryAndroid
 @text	Wrapper for Flurry integration on Android devices.
 Flurry provides analytics and behaviour statistics.
 Exposed to lua via MOAIFlurry on
 all mobile platforms.
 
 */
class MOAIFlurryAndroid :
public MOAIGlobalClass < MOAIFlurryAndroid, MOAILuaObject > {
private:
    
	//----------------------------------------------------------------//
	static int _startSession		( lua_State* L );
	static int _logEvent			( lua_State* L );
	static int _logTimedEvent		( lua_State* L );
	static int _endTimedEvent		( lua_State* L );
    
	static cc8*	_luaParseTable 		( lua_State * L, int idx );
	static jobject _luaReadMap		( lua_State * L, int idx );
    
public:
    
	DECL_LUA_SINGLETON ( MOAIFlurryAndroid );
    
	MOAIFlurryAndroid		();
	~MOAIFlurryAndroid		();
	void	RegisterLuaClass	( MOAILuaState& state );
};

#endif  //ENABLE_FLURRY

#endif // MOAIFLURRY_H