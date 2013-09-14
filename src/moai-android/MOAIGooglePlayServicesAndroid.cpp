// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_PLAYSERVICES

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIGooglePlayServicesAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	connect
	@text	Connects to the Google Play Game Services

	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_connect ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID connect = env->GetStaticMethodID ( playserv, "connect", "()Z" );
   		if ( connect == NULL ) {

			USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "connect" );
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
/**	@name	isConnected
	@text	Connects to the Google Play Game Services

	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_isConnected ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID isConnected = env->GetStaticMethodID ( playserv, "isConnected", "()Z" );
   		if ( isConnected == NULL ) {

			USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "isConnected" );
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

		MOAIGooglePlayServicesAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	showAchievements
	@text	Shows the achievements

	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_showAchievements ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID showAchievements = env->GetStaticMethodID ( playserv, "showAchievements", "()V" );
   		if ( showAchievements == NULL ) {

			USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "showAchievements" );
		} else {

			env->CallStaticVoidMethod ( playserv, showAchievements );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	showLeaderboard
	@text	Shows the desired leaderboard

	@in		string leaderboardID
	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_showLeaderboard ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	cc8* board = lua_tostring ( state, 1 );

	JNI_GET_JSTRING ( board, jboard );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID showLeaderboard = env->GetStaticMethodID ( playserv, "showLeaderboard", "(Ljava/lang/String;)V" );
   		if ( showLeaderboard == NULL ) {

			USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "showLeaderboard" );
		} else {

			env->CallStaticVoidMethod ( playserv, showLeaderboard, jboard );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	submitScore
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

	JNI_GET_JSTRING ( board, jboard );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID submitScore = env->GetStaticMethodID ( playserv, "submitScore", "(Ljava/lang/String;J)V" );
   		if ( submitScore == NULL ) {

			USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "submitScore" );
		} else {

			env->CallStaticVoidMethod ( playserv, submitScore, jboard, score );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	unlockAchievement
	@text	Grants an achievement to the player

	@in		string achievementID
	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_unlockAchievement ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	cc8* ach = lua_tostring ( state, 1 );

	JNI_GET_JSTRING ( ach, jach );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID unlockAchievement = env->GetStaticMethodID ( playserv, "unlockAchievement", "(Ljava/lang/String;)V" );
   		if ( unlockAchievement == NULL ) {

			USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "unlockAchievement" );
		} else {

			env->CallStaticVoidMethod ( playserv, unlockAchievement, jach );
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

		MOAILuaStateHandle state = callback.GetSelf ();
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
extern "C" void Java_com_ziplinegames_moai_MoaiGooglePlayServices_AKUNotifyConnectionComplete ( JNIEnv* env, jclass obj ) {

	MOAIGooglePlayServicesAndroid::Get ().NotifyConnectionComplete ();
}

#endif