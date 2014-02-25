// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIAppAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getUTCTime
	@text	Gets the UTC time.

	@out 	num		UTC Time
*/
int MOAIAppAndroid::_getUTCTime ( lua_State* L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	long outVal = 0;
	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID getUTCTime = env->GetStaticMethodID ( moai, "getUTCTime", "()J" );
    	if ( getUTCTime == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find static java method %s", "getUTCTime" );
    	} else {

			outVal = env->CallStaticIntMethod ( moai, getUTCTime );
		}
	}

	lua_pushnumber ( L, outVal );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getStatusBarHeight
	@text	Gets the Height of an Android 3.x status bar

	@out 	num		height
*/
int MOAIAppAndroid::_getStatusBarHeight ( lua_State* L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	int outVal = 0;
	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID getStatusBarHeight = env->GetStaticMethodID ( moai, "getStatusBarHeight", "()I" );
    	if ( getStatusBarHeight == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find static java method %s", "getStatusBarHeight" );
    	} else {

			outVal = env->CallStaticIntMethod ( moai, getStatusBarHeight );
		}
	}

	lua_pushnumber ( L, outVal );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	sendMail
    @text Send a mail with the passed in default values

    @in	string recipient
    @in	string subject
    @in	string message
    @out	nil
*/
int	MOAIAppAndroid::_sendMail ( lua_State* L ) {
    MOAILuaState state ( L );

	cc8* recipient = state.GetValue < cc8* >( 1, "" );
	cc8* subject = state.GetValue < cc8* >( 2, "" );
	cc8* message = state.GetValue < cc8* >( 3, "" );

    JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( recipient, jrecipient );
	JNI_GET_JSTRING ( subject, jsubject );
	JNI_GET_JSTRING ( message, jmessage );

    jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID sendMail = env->GetStaticMethodID ( moai, "sendMail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( sendMail == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find static java method %s", "sendMail" );
    	} else {

			env->CallStaticVoidMethod ( moai, sendMail, jrecipient, jsubject, jmessage );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	share
	@text	Open a generic Android dialog to allow the user to share
			via email, SMS, Facebook, Twitter, etc.

	@in		string	prompt			The prompt to show the user.
	@in		string	subject			The subject of the message to share.
	@in		string	text			The text of the message to share.
	@out 	nil
*/
int MOAIAppAndroid::_share ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* prompt = lua_tostring ( state, 1 );
	cc8* subject = lua_tostring ( state, 2 );
	cc8* text = lua_tostring ( state, 3 );

	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( prompt, jprompt );
	JNI_GET_JSTRING ( subject, jsubject );
	JNI_GET_JSTRING ( text, jtext );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID share = env->GetStaticMethodID ( moai, "share", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( share == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find static java method %s", "share" );
    	} else {

			env->CallStaticVoidMethod ( moai, share, jprompt, jsubject, jtext );
		}
	}

	return 0;
}

//================================================================//
// MOAIAppAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIAppAndroid::MOAIAppAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIAppAndroid::~MOAIAppAndroid () {

}

//----------------------------------------------------------------//
void MOAIAppAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "ACTIVITY_ON_CREATE",		( u32 )ACTIVITY_ON_CREATE );
	state.SetField ( -1, "ACTIVITY_ON_DESTROY",		( u32 )ACTIVITY_ON_DESTROY );
	state.SetField ( -1, "ACTIVITY_ON_START",		( u32 )ACTIVITY_ON_START );
	state.SetField ( -1, "ACTIVITY_ON_STOP",		( u32 )ACTIVITY_ON_STOP );
	state.SetField ( -1, "ACTIVITY_ON_PAUSE",		( u32 )ACTIVITY_ON_PAUSE );
	state.SetField ( -1, "ACTIVITY_ON_RESUME",		( u32 )ACTIVITY_ON_RESUME );
	state.SetField ( -1, "ACTIVITY_ON_RESTART",		( u32 )ACTIVITY_ON_RESTART );
	state.SetField ( -1, "BACK_BUTTON_PRESSED",		( u32 )BACK_BUTTON_PRESSED );

	luaL_Reg regTable [] = {
		{ "getListener",			&MOAIGlobalEventSource::_getListener < MOAIAppAndroid > },
		{ "getUTCTime",				_getUTCTime },
		{ "getStatusBarHeight",		_getStatusBarHeight },
		{ "sendMail",				_sendMail },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIAppAndroid > },
		{ "share",					_share },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// MOAIAppAndroid JNI Functions
//================================================================//

//----------------------------------------------------------------//
extern "C" bool Java_com_ziplinegames_moai_Moai_AKUAppInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( MOAIAppAndroid::Get ().PushListener ( eventID, state )) {
		state.DebugCall ( 0, 1 );
		return state.GetValue < bool >( -1, false );
	}
	return false;
}
