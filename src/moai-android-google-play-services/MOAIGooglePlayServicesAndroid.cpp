// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/JniUtils.h>
#include "MOAIGooglePlayServicesAndroid.h"

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	connect
	@text	Connects to the Google Play Game Services

	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_connect ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID connect = env->GetStaticMethodID ( playserv, "connect", "()Z" );
   		if ( connect == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "connect" );
		} else {

			bool success = ( bool ) env->CallStaticBooleanMethod ( playserv, connect );

			lua_pushboolean ( state, success );
			return 1;
		}
	}

	lua_pushboolean ( state, false );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isConnected
	@text	Connects to the Google Play Game Services

	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_isConnected ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID isConnected = env->GetStaticMethodID ( playserv, "isConnected", "()Z" );
   		if ( isConnected == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "isConnected" );
		} else {

			bool success = ( bool ) env->CallStaticBooleanMethod ( playserv, isConnected );

			lua_pushboolean ( state, success );
			return 1;
		}
	}

	lua_pushboolean ( state, false );
	return 1;
}

//----------------------------------------------------------------//
int MOAIGooglePlayServicesAndroid::_setListener ( lua_State* L ) {

	MOAILuaState state ( L );

	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {

		MOAIGooglePlayServicesAndroid::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	showAchievements
	@text	Shows the achievements

	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_showAchievements ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID showAchievements = env->GetStaticMethodID ( playserv, "showAchievements", "()V" );
   		if ( showAchievements == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "showAchievements" );
		} else {

			env->CallStaticVoidMethod ( playserv, showAchievements );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	showLeaderboard
	@text	Shows the desired leaderboard

	@in		string leaderboardID
	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_showLeaderboard ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	cc8* board = lua_tostring ( state, 1 );

	MOAIJString jboard = JNI_GET_JSTRING ( board );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID showLeaderboard = env->GetStaticMethodID ( playserv, "showLeaderboard", "(Ljava/lang/String;)V" );
   		if ( showLeaderboard == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "showLeaderboard" );
		} else {

			env->CallStaticVoidMethod ( playserv, showLeaderboard, ( jstring )jboard );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	submitScore
	@text	Submits a score for the passed in leaderboard

	@in		string leaderboardID
	@in		number score
	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_submitScore ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	cc8* board = lua_tostring ( state, 1 );
	jlong score = lua_tonumber ( state, 2 );

	MOAIJString jboard = JNI_GET_JSTRING ( board );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID submitScore = env->GetStaticMethodID ( playserv, "submitScore", "(Ljava/lang/String;J)V" );
   		if ( submitScore == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "submitScore" );
		} else {

			env->CallStaticVoidMethod ( playserv, submitScore, ( jstring )jboard, score );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	unlockAchievement
	@text	Grants an achievement to the player

	@in		string achievementID
	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_unlockAchievement ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	cc8* ach = lua_tostring ( state, 1 );

	MOAIJString jach = JNI_GET_JSTRING ( ach );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID unlockAchievement = env->GetStaticMethodID ( playserv, "unlockAchievement", "(Ljava/lang/String;)V" );
   		if ( unlockAchievement == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "unlockAchievement" );
		} else {

			env->CallStaticVoidMethod ( playserv, unlockAchievement, ( jstring )jach );
		}
	}

	return 0;
}


//================================================================//
// MOAIGooglePlayServicesAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIGooglePlayServicesAndroid::MOAIGooglePlayServicesAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGooglePlayServicesAndroid::~MOAIGooglePlayServicesAndroid () {

}

//----------------------------------------------------------------//
void MOAIGooglePlayServicesAndroid::NotifyConnectionComplete () {

	MOAILuaRef& callback = this->mListeners [ CONNECTION_COMPLETE ];

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIGooglePlayServicesAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "CONNECTION_COMPLETE",		( u32 )CONNECTION_COMPLETE );

	luaL_Reg regTable [] = {
		{ "connect", 				_connect },
		{ "isConnected",			_isConnected },
		{ "setListener",			_setListener },
		{ "showAchievements",		_showAchievements },
		{ "showLeaderboard",		_showLeaderboard },
		{ "submitScore",			_submitScore },
		{ "unlockAchievement",		_unlockAchievement },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

// AKU Callbacks

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiGooglePlayServices_AKUNotifyConnectionComplete ( JNIEnv* env, jclass obj ) {

	MOAIGooglePlayServicesAndroid::Get ().NotifyConnectionComplete ();
}
