// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIAppAndroid.h>

extern JavaVM* jvm;

bool 			MOAIAppAndroid::_pictureTakenFlag;
pthread_mutex_t MOAIAppAndroid::_pictureTakenMutex;
int MOAIAppAndroid::_code = -666;

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

		ZLLog::Print ( "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID getUTCTime = env->GetStaticMethodID ( moai, "getUTCTime", "()J" );
    	if ( getUTCTime == NULL ) {

			ZLLog::Print ( "MOAIAppAndroid: Unable to find static java method %s", "getUTCTime" );
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

		ZLLog::Print ( "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID getStatusBarHeight = env->GetStaticMethodID ( moai, "getStatusBarHeight", "()I" );
    	if ( getStatusBarHeight == NULL ) {

			ZLLog::Print ( "MOAIAppAndroid: Unable to find static java method %s", "getStatusBarHeight" );
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

		ZLLog::Print ( "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID sendMail = env->GetStaticMethodID ( moai, "sendMail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( sendMail == NULL ) {

			ZLLog::Print ( "MOAIAppAndroid: Unable to find static java method %s", "sendMail" );
    	} else {

			env->CallStaticVoidMethod ( moai, sendMail, jrecipient, jsubject, jmessage );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
int MOAIAppAndroid::_setListener ( lua_State* L ) {

	MOAILuaState state ( L );

	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {

		MOAIAppAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
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

		ZLLog::Print ( "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID share = env->GetStaticMethodID ( moai, "share", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( share == NULL ) {

			ZLLog::Print ( "MOAIAppAndroid: Unable to find static java method %s", "share" );
    	} else {

			env->CallStaticVoidMethod ( moai, share, jprompt, jsubject, jtext );
		}
	}

	return 0;
}

int MOAIAppAndroid::_takePicture( lua_State* L ) {
	ZLLog::Print("_takePicture");
    MOAILuaState state( L);
//        MOAILuaRef& callback 		= this->mListeners[ PICTURE_TAKEN ];
//        MOAIScopedLuaState state 	= callback.GetSelf();


    JNI_GET_ENV( jvm, env );

    jclass      t_class;
    jmethodID   t_mid;
    // jstring t_string;

    if( _pictureTakenFlag != false ) {
    	ZLLog::Print( "WARNING: MOAIAppAndroid::_pictureTakenFlag is TRUE before calling into java!" );
    }

    t_class = env->FindClass( "com/ziplinegames/moai/MoaiCamera" );

    if( t_class == NULL ) {
        ZLLog::Print( "XXX unable to find java class %s", "com/ziplinegames/moai/MoaiCamera" );
    } else {
        t_mid = env->GetStaticMethodID( t_class, "takePicture", "()V" );
        if( t_mid == NULL ) {
            ZLLog::Print( "XXX unable to find static java method %s", "takePicture");
        } else {
        	bool done = false;


//        	MutexLocker lock( _pictureTakenMutex );
        	ZLLog::Print( "before java call" );
//        	AKUPause( true );
        	env->CallStaticVoidMethod( t_class, t_mid );
        	ZLLog::Print( "after java call" );

        	ZLLog::Print( "______code: %i", _code );

//        	return 0;
//
//        	int foo = 0;
//        	do {
//        		lock.lock();
//        			if( _pictureTakenFlag == true ) {
//        				done = true;
//        			}
//        		lock.unlock();
//        		ZLLog::Print( "zZzzzZZZzzzZZZzz" );
//        		sleep(1);
////        		usleep( 100 * 1000 ); // 100 ms microseconds
//        	} while( !done && foo++ < 20 );
//
//        	ZLLog::Print( "... done" );
//
//        	jmethodID t_getResultCode_mid = env->GetStaticMethodID( t_class, "getResultCode", "()I" );
//        	if( t_getResultCode_mid == NULL ) {
//        		ZLLog::Print( "unable to find static java method %s", "getResultCode" );
//        		return 0;
//        	}
//
//        	jmethodID t_getResultPath = env->GetStaticMethodID( t_class, "getResultPath", "()Ljava/lang/String" );
//        	if( t_getResultPath == NULL ) {
//        		ZLLog::Print( "unable to find static java method %s","getResultPath" );
//        		return 0;
//        	}
//
//        	int j_code = env->CallStaticIntMethod( t_class, t_getResultCode_mid );
//        	jstring j_path = (jstring)env->CallStaticObjectMethod( t_class, t_getResultPath );
//        	JNI_GET_CSTRING( j_path, c_path );
//
//        	state.Push( j_code );
//        	state.Push( c_path );
//
//        	state.DebugCall( 2, 0 );
//
//        	JNI_RELEASE_CSTRING( j_path, c_path );
//
//        	lock.lock();
//        	_pictureTakenFlag = false;

        }
    }

    return 0;
}


//================================================================//
// MOAIAppAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIAppAndroid::MOAIAppAndroid () {

	RTTI_SINGLE ( MOAILuaObject );

	pthread_mutex_init( &_pictureTakenMutex, 0 );
	_pictureTakenFlag = false;
}

//----------------------------------------------------------------//
MOAIAppAndroid::~MOAIAppAndroid () {
	pthread_mutex_destroy( &_pictureTakenMutex );
}

//----------------------------------------------------------------//
void MOAIAppAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "SESSION_START",		    ( u32 )SESSION_START );
	state.SetField ( -1, "SESSION_END",			    ( u32 )SESSION_END );
	state.SetField ( -1, "BACK_BUTTON_PRESSED",		( u32 )BACK_BUTTON_PRESSED );
	state.SetField ( -1, "PICTURE_TAKEN",			( u32 )PICTURE_TAKEN );

	luaL_Reg regTable [] = {
		{ "getUTCTime",				_getUTCTime },
		{ "getStatusBarHeight",		_getStatusBarHeight },
		{ "sendMail",				_sendMail },
		{ "setListener",			_setListener },
		{ "share",					_share },
        { "takePicture",            _takePicture },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAppAndroid::NotifyBackButtonPressed () {

	MOAILuaRef& callback = this->mListeners [ BACK_BUTTON_PRESSED ];

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 1 );

		return lua_toboolean ( state, -1 );
	} else {

		return false;
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::NotifyDidStartSession ( bool resumed ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_START ];

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();

		lua_pushboolean ( state, resumed );

		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::NotifyWillEndSession () {

	MOAILuaRef& callback = this->mListeners [ SESSION_END ];

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::NotifyPictureTaken( int code, cc8* path ) {
	ZLLog::Print( "MOAIAppAndroid::NotifyPictureTaken" );
    MOAILuaRef& callback 		= this->mListeners[ PICTURE_TAKEN ];
    MOAIScopedLuaState state 	= callback.GetSelf();
    
    _code = code;

//    if( callback ) {
//    	state.Push( code );
//    	state.Push( path );
//
//    	state.DebugCall( 2, 0 );
//    }

}

void MOAIAppAndroid::NotifyPictureTaken() {
	ZLLog::Print( "notifying ..." );

	MutexLocker lock( _pictureTakenMutex );
	if( _pictureTakenFlag == true ) {
		ZLLog::Print( "WARNING: MOAIAppAndroid::NotifyPictureTaken(): _pictureTakenFlag is already true ..." );
	}
	_pictureTakenFlag = true;
}

//================================================================//
// Miscellaneous JNI Functions
//================================================================//

//----------------------------------------------------------------//
extern "C" bool Java_com_ziplinegames_moai_Moai_AKUAppBackButtonPressed ( JNIEnv* env, jclass obj ) {

	return MOAIAppAndroid::Get ().NotifyBackButtonPressed ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_Moai_AKUAppDidStartSession ( JNIEnv* env, jclass obj, jboolean resumed ) {

	MOAIAppAndroid::Get ().NotifyDidStartSession ( resumed );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_Moai_AKUAppWillEndSession ( JNIEnv* env, jclass obj ) {

	MOAIAppAndroid::Get ().NotifyWillEndSession ();
}


extern "C" void Java_com_ziplinegames_moai_MoaiCamera_AKUNotifyPictureTaken__OLD( JNIEnv* env, jclass obj, jint code, jstring jpath ) {
	ZLLog::Print( "AKUNotifyPictureTaken called from java" );
    JNI_GET_CSTRING( jpath, path );
    
    MOAIAppAndroid::Get().NotifyPictureTaken( code, path );

    JNI_RELEASE_CSTRING( jpath, path );
}

extern "C" void Java_com_ziplinegames_moai_MoaiCamera_AKUNotifyPictureTaken( JNIEnv* env, jclass obj ) {
	ZLLog::Print( "AKUNotifyPictureTaken called from java" );

	MOAIAppAndroid::Get().NotifyPictureTaken();

//    JNI_GET_CSTRING( jpath, path );
//
//    MOAIAppAndroid::Get().NotifyPictureTaken( code, path );
//
//    JNI_RELEASE_CSTRING( jpath, path );
}

