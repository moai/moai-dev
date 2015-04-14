// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIVUNGLEANDROID_H
#define MOAIVUNGLEANDROID_H

#include <host-modules/aku_modules_android_config.h>

#if AKU_WITH_ANDROID_VUNGLE

#include <moai-core/headers.h>
#include <moai-android/JniUtils.h>

//================================================================//
// MOAIVungleAndroid
//================================================================//
class MOAIVungleAndroid :
	public MOAIGlobalClass < MOAIVungleAndroid, MOAIGlobalEventSource >,
	public JniUtils {
private:

	//----------------------------------------------------------------//
	static int	_displayAdvert		( lua_State* L );
	static int	_init				( lua_State* L );
	static int	_isVideoAvailable	( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIVungleAndroid );

	enum {
		AD_START,
		AD_END,
		AD_VIEWED,
	};

	//----------------------------------------------------------------//
			MOAIVungleAndroid		();
			~MOAIVungleAndroid		();
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //DISABLE_VUNGLE

#endif  //MOAIADCOLONY_H
