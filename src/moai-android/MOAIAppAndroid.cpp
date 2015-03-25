// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/JniUtils.h>
#include <moai-android/MOAIAppAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAppAndroid::_getPictureCode( lua_State* L ) {
	MOAILuaState state( L );
	MOAIAppAndroid::Get().PushPictureCode( state );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAppAndroid::_getPicturePath( lua_State* L ) {
	MOAILuaState state( L );
	MOAIAppAndroid::Get().PushPicturePath( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getUTCTime
	@text	Gets the UTC time.

	@out 	number time		UTC Time
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
/**	@lua	getStatusBarHeight
	@text	Gets the Height of an Android 3.x status bar

	@out 	number height
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
// TODO: doxygen
int MOAIAppAndroid::_openURL ( lua_State* L ) {

	MOAILuaState state ( L );
	
	cc8* url = lua_tostring ( state, 1 );
	
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: _openURL %s", url );
	
	JNI_GET_ENV ( jvm, env );
	
	MOAIJString jurl = JNI_GET_JSTRING ( url );
	
	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
	    if ( moai == NULL ) {
	
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
	    } else {
	
	    	jmethodID openURL = env->GetStaticMethodID ( moai, "openURL", "(Ljava/lang/String;)V" );
	    	if ( openURL == NULL ) {
	
			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find static java method %s", "openURL" );
	    	} else {
	
			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: calling java openURL" );
			env->CallStaticVoidMethod ( moai, openURL, ( jstring )jurl );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	sendMail
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

	MOAIJString jrecipient = JNI_GET_JSTRING ( recipient );
	MOAIJString jsubject = JNI_GET_JSTRING ( subject );
	MOAIJString jmessage = JNI_GET_JSTRING ( message );

    jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID sendMail = env->GetStaticMethodID ( moai, "sendMail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( sendMail == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find static java method %s", "sendMail" );
    	} else {

			env->CallStaticVoidMethod ( moai, sendMail, ( jstring )jrecipient, ( jstring )jsubject, ( jstring )jmessage );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	share
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

	MOAIJString jprompt = JNI_GET_JSTRING ( prompt );
	MOAIJString jsubject = JNI_GET_JSTRING ( subject );
	MOAIJString jtext = JNI_GET_JSTRING ( text );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID share = env->GetStaticMethodID ( moai, "share", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( share == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid: Unable to find static java method %s", "share" );
    	} else {

			env->CallStaticVoidMethod ( moai, share, ( jstring )jprompt, ( jstring )jsubject, ( jstring )jtext );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAppAndroid::_takePicture( lua_State* L ) {
    MOAILuaState state( L);

    JNI_GET_ENV( jvm, env );

    jclass      t_class;
    jmethodID   t_mid;

    t_class = env->FindClass( "com/ziplinegames/moai/MoaiCamera" );
	t_mid = env->GetStaticMethodID( t_class, "takePicture", "()V" );

    if( t_class != NULL && t_mid != NULL ) {
        	env->CallStaticVoidMethod( t_class, t_mid );
    }

    return 0;
}

//================================================================//
// MOAIAppAndroid
//================================================================//

//----------------------------------------------------------------//
void MOAIAppAndroid::AppOpenedFromURL ( jstring url ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( APP_OPENED_FROM_URL, state )) {
			
		JNI_GET_ENV ( jvm, env );
		JNI_GET_CSTRING ( url, returnurl );

		lua_pushstring ( state, returnurl );
		state.DebugCall ( 1, 0 );

		JNI_RELEASE_CSTRING ( url, returnurl );
	}
}

//----------------------------------------------------------------//
MOAIAppAndroid::MOAIAppAndroid () {

	RTTI_SINGLE ( MOAILuaObject );

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
	state.SetField ( -1, "APP_OPENED_FROM_URL",     ( u32 )APP_OPENED_FROM_URL );
	state.SetField ( -1, "BACK_BUTTON_PRESSED",		( u32 )BACK_BUTTON_PRESSED );
	state.SetField ( -1, "EVENT_PICTURE_TAKEN",		( u32 )EVENT_PICTURE_TAKEN );

	luaL_Reg regTable [] = {
        { "getPictureCode",			_getPictureCode },
        { "getPicturePath",			_getPicturePath },
		{ "getListener",			&MOAIGlobalEventSource::_getListener < MOAIAppAndroid > },
		{ "getUTCTime",				_getUTCTime },
		{ "getStatusBarHeight",		_getStatusBarHeight },
		{ "sendMail",				_sendMail },		
		{ "openURL",				_openURL },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIAppAndroid > },
		{ "share",					_share },
        { "takePicture",            _takePicture },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAppAndroid::NotifyPictureTaken() {
	JNI_GET_ENV( jvm, env );

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( MOAIAppAndroid::Get ().PushListener ( EVENT_PICTURE_TAKEN, state )) {
		
		jclass t_class = env->FindClass( "com/ziplinegames/moai/MoaiCamera" );
		jmethodID t_getResultPath_mid = env->GetStaticMethodID( t_class, "getResultPath", "()Ljava/lang/String;" );
		jmethodID t_getResultCode_mid = env->GetStaticMethodID( t_class, "getResultCode", "()I" );

		if( t_class != NULL && t_getResultCode_mid != NULL && t_getResultPath_mid != NULL ) {

			int j_code = env->CallStaticIntMethod( t_class, t_getResultCode_mid );
			MOAIJString j_path = ( jstring )env->CallStaticObjectMethod( t_class, t_getResultPath_mid );

			JNI_GET_CSTRING( j_path, c_path );
			state.Push( j_code );
			state.Push( c_path );
			state.DebugCall( 2, 0 );
			JNI_RELEASE_CSTRING( j_path, c_path );

		}
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::PushPictureCode( MOAILuaState& state ) {
	JNI_GET_ENV( jvm, env );

	jclass t_class = env->FindClass( "com/ziplinegames/moai/MoaiCamera" );
	jmethodID t_getResultCode_mid = env->GetStaticMethodID( t_class, "getResultCode", "()I" );

	if( t_class != NULL && t_getResultCode_mid != NULL ) {
		int j_code = env->CallStaticIntMethod( t_class, t_getResultCode_mid );
		state.Push( j_code );
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::PushPictureData( MOAILuaState& state ) {
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAppAndroid::PushPictureData" );
	JNI_GET_ENV( jvm, env );

	jclass t_class = env->FindClass( "com/ziplinegames/moai/MoaiCamera" );
	jmethodID t_getResultCode_mid = env->GetStaticMethodID( t_class, "getResultCode", "()I" );
	jmethodID t_getResultPath_mid = env->GetStaticMethodID( t_class, "getResultPath", "()Ljava/lang/String;" );

	if( t_class != NULL && t_getResultCode_mid != NULL && t_getResultPath_mid != NULL ) {

		int j_code = env->CallStaticIntMethod( t_class, t_getResultCode_mid );
		MOAIJString j_path = ( jstring )env->CallStaticObjectMethod( t_class, t_getResultPath_mid );

		JNI_GET_CSTRING( j_path, c_path );
		state.Push( j_code );
		state.Push( c_path );
		state.DebugCall( 2, 0 );
		JNI_RELEASE_CSTRING( j_path, c_path );
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::PushPicturePath( MOAILuaState& state ) {
	JNI_GET_ENV( jvm, env );

	jclass t_class = env->FindClass( "com/ziplinegames/moai/MoaiCamera" );
	jmethodID t_getResultPath_mid = env->GetStaticMethodID( t_class, "getResultPath", "()Ljava/lang/String;" );

	if( t_class != NULL && t_getResultPath_mid != NULL ) {
		MOAIJString j_path = ( jstring )env->CallStaticObjectMethod( t_class, t_getResultPath_mid );
		JNI_GET_CSTRING( j_path, c_path );
		state.Push( c_path );
		JNI_RELEASE_CSTRING( j_path, c_path );
	}
}

//================================================================//
// MOAIAppAndroid JNI Functions
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT jboolean JNICALL Java_com_ziplinegames_moai_Moai_AKUAppInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( MOAIAppAndroid::Get ().PushListener ( eventID, state )) {
		state.DebugCall ( 0, 1 );
		return state.GetValue < bool >( -1, false );
	}
	return false;
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiCamera_AKUNotifyPictureTaken ( JNIEnv* env, jclass obj ) {
	MOAIAppAndroid::Get ().NotifyPictureTaken ();
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_Moai_AKUAppOpenedFromURL ( JNIEnv* env, jclass obj, jstring url ) {
	MOAIAppAndroid::Get ().AppOpenedFromURL ( url );
}
