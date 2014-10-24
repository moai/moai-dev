// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIMoviePlayerAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize the video player with the URL of a video to play.
	
	@in		string 	url				The URL of the video to play.
	@out	nil
*/
int MOAIMoviePlayerAndroid::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* url = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( url, jurl );

	jclass movie = env->FindClass ( "com/ziplinegames/moai/MoaiMoviePlayer" );
    if ( movie == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIMoviePlayerAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiMoviePlayer" );
    } else {

    	jmethodID init = env->GetStaticMethodID ( movie, "init", "(Ljava/lang/String;)V" );
    	if ( init == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIMoviePlayerAndroid: Unable to find static java method %s", "init" );
    	} else {

			env->CallStaticVoidMethod ( movie, init, jurl );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	play
	@text	Play the video as soon as playback is ready.
	
	@out	nil
*/
int MOAIMoviePlayerAndroid::_play ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass movie = env->FindClass ( "com/ziplinegames/moai/MoaiMoviePlayer" );
    if ( movie == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIMoviePlayerAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiMoviePlayer" );
    } else {

    	jmethodID play = env->GetStaticMethodID ( movie, "play", "()V" );
    	if ( play == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIMoviePlayerAndroid: Unable to find static java method %s", "play" );
    	} else {

			env->CallStaticVoidMethod ( movie, play );
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	pause
	@text	Pause video playback.
	
	@out	nil
*/
int MOAIMoviePlayerAndroid::_pause ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass movie = env->FindClass ( "com/ziplinegames/moai/MoaiMoviePlayer" );
    if ( movie == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIMoviePlayerAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiMoviePlayer" );
    } else {

    	jmethodID pause = env->GetStaticMethodID ( movie, "pause", "()V" );
    	if ( pause == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIMoviePlayerAndroid: Unable to find static java method %s", "pause" );
    	} else {

			env->CallStaticVoidMethod ( movie, pause );	
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIMoviePlayerAndroid::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIMoviePlayerAndroid::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	stop
	@text	Stop video playback and reset the video player.
	
	@out	nil
*/
int MOAIMoviePlayerAndroid::_stop ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );
	
	jclass movie = env->FindClass ( "com/ziplinegames/moai/MoaiMoviePlayer" );
    if ( movie == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIMoviePlayerAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiMoviePlayer" );
    } else {

    	jmethodID stop = env->GetStaticMethodID ( movie, "stop", "()V" );
    	if ( stop == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIMoviePlayerAndroid: Unable to find static java method %s", "stop" );
    	} else {

			env->CallStaticVoidMethod ( movie, stop );
		}
	}
	
	return 0;
}

//================================================================//
// MOAIMoviePlayerAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIMoviePlayerAndroid::MOAIMoviePlayerAndroid () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIMoviePlayerAndroid::~MOAIMoviePlayerAndroid () {

}

//----------------------------------------------------------------//
void MOAIMoviePlayerAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "MOVIE_PLAYER_READY", 		( u32 )MOVIE_PLAYER_READY );
	state.SetField ( -1, "MOVIE_PLAYER_COMPLETED",	( u32 )MOVIE_PLAYER_COMPLETED );

	luaL_Reg regTable[] = {
		{ "init",			_init },
		{ "pause",			_pause },
		{ "play",			_play },
		{ "setListener",	_setListener },
		{ "stop",			_stop },
		{ NULL, NULL }	
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMoviePlayerAndroid::NotifyMoviePlayerCompleted () {
	
	MOAILuaRef& callback = this->mListeners [ MOVIE_PLAYER_COMPLETED ];
		
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIMoviePlayerAndroid::NotifyMoviePlayerReady () {
	
	MOAILuaRef& callback = this->mListeners [ MOVIE_PLAYER_READY ];
		
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// Movie Player JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiMoviePlayer_AKUNotifyMoviePlayerCompleted ( JNIEnv* env, jclass obj ) {

	MOAIMoviePlayerAndroid::Get ().NotifyMoviePlayerCompleted ();
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiMoviePlayer_AKUNotifyMoviePlayerReady ( JNIEnv* env, jclass obj ) {

	MOAIMoviePlayerAndroid::Get ().NotifyMoviePlayerReady ();
}