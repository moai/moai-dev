// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIADCOLONYANDROID_H
#define MOAIADCOLONYANDROID_H

#include <moai-core/headers.h>
#include <moai-android/JniUtils.h>

//================================================================//
// MOAIAdColonyAndroid
//================================================================//
class MOAIAdColonyAndroid :
	public MOAIGlobalClass < MOAIAdColonyAndroid, MOAIGlobalEventSource >,
	public JniUtils {
private:

	jmethodID	mJava_Init;
	jmethodID	mJava_IsVideoReady;
	jmethodID	mJava_PlayVideo;

	//----------------------------------------------------------------//
	static int	_init				( lua_State* L );
	static int	_playVideo			( lua_State* L );
	static int	_videoReadyForZone	( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIAdColonyAndroid );

	enum {
		VIDEO_STARTED,
		VIDEO_SHOWN,
		VIDEO_FAILED,
	};

	//----------------------------------------------------------------//
			MOAIAdColonyAndroid		();
			~MOAIAdColonyAndroid	();
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //MOAIADCOLONY_H
