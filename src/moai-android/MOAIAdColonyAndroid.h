// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIADCOLONYANDROID_H
#define MOAIADCOLONYANDROID_H

#ifndef DISABLE_ADCOLONY

#include <moai-core/headers.h>
#include <moai-android/JniUtils.h>

//================================================================//
// MOAIAdColonyAndroid
//================================================================//
class MOAIAdColonyAndroid :
	public MOAIGlobalClass < MOAIAdColonyAndroid, MOAILuaObject >,
	public JniUtils {
private:

	JNIEnv*		mEnv;
	jclass		mClass;

	jmethodID	mJava_GetDeviceID;
	jmethodID	mJava_Init;
	jmethodID	mJava_IsVideoReady;
	jmethodID	mJava_PlayVideo;

	//----------------------------------------------------------------//
	static int	_getDeviceID		( lua_State* L );
	static int	_init				( lua_State* L );
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

	MOAILuaStrongRef		mListeners [ TOTAL ];

			MOAIAdColonyAndroid		();
			~MOAIAdColonyAndroid	();
	void	NotifyVideoComplete 	( int success );
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //DISABLE_ADCOLONY

#endif  //MOAIADCOLONY_H
