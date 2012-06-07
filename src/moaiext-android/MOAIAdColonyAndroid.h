// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIADCOLONYANDROID_H
#define MOAIADCOLONYANDROID_H

#ifndef DISABLE_ADCOLONY

#include <moaicore/moaicore.h>

//================================================================//
// MOAIAdColonyAndroid
//================================================================//
class MOAIAdColonyAndroid :
	public MOAIGlobalClass < MOAIAdColonyAndroid, MOAILuaObject > {
private:
    
	//----------------------------------------------------------------//
	static int	_getDeviceID		( lua_State* L );
	static int	_init				( lua_State* L );
	static cc8*	_luaParseTable 		( lua_State* L, int idx );
	static int	_playVideo			( lua_State* L );
	static int	_setListener		( lua_State* L );
	static int	_videoReadyForZone	( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIAdColonyAndroid );

	enum {
		VIDEO_BEGAN_IN_ZONE,
		VIDEO_ENDED_IN_ZONE,
		VIDEO_FAILED_IN_ZONE,
		VIDEO_PAUSED_IN_ZONE,
		VIDEO_RESUMED_IN_ZONE,
		TOTAL
	};
	
	MOAILuaRef		mListeners [ TOTAL ];
	
			MOAIAdColonyAndroid		();
			~MOAIAdColonyAndroid	();
	void 	NotifyVideoComplete		();
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //DISABLE_ADCOLONY

#endif  //MOAIADCOLONY_H
