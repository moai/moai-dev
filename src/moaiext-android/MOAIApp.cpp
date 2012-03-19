// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIApp.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
const cc8* _getGuid () {

	JNI_GET_ENV ( jvm, env );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		USLog::Print ( "MOAIApp: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID getGUID = env->GetStaticMethodID ( moai, "getGUID", "()Ljava/lang/String;" );
    	if ( getGUID == NULL ) {

			USLog::Print ( "MOAIApp: Unable to find static java method %s", "getGUID" );
    	} else {

			jstring jguid = ( jstring )env->CallStaticObjectMethod ( moai, getGUID );	
			
			JNI_GET_CSTRING ( jguid, guid );

			// NOTE: This is a hack for two reasons: (1) we're assuming that a guid is less
			// than 512 characters (pretty safe) and (2) we know that upon return, this string
			// will be immediately copied, such that when this function returns, the system 
			// can safely reclaim the stack space we're utilizing.
			char hack [ 512 ];
			strcpy ( hack, guid );

			JNI_RELEASE_CSTRING ( jguid, guid );
			
			return hack;
		}
	}

	return NULL;
}

//----------------------------------------------------------------//
int MOAIApp::_openURL ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* url = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( url, jurl );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		USLog::Print ( "MOAIApp: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID openURL = env->GetStaticMethodID ( moai, "openURL", "(Ljava/lang/String;)V" );
    	if ( openURL == NULL ) {

			USLog::Print ( "MOAIApp: Unable to find static java method %s", "openURL" );
    	} else {

			env->CallStaticVoidMethod ( moai, openURL, jurl );	
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIApp::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAIApp::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIApp::_share ( lua_State* L ) {
	
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

		USLog::Print ( "MOAIApp: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID share = env->GetStaticMethodID ( moai, "share", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( share == NULL ) {

			USLog::Print ( "MOAIApp: Unable to find static java method %s", "share" );
    	} else {

			env->CallStaticVoidMethod ( moai, share, jprompt, jsubject, jtext );	
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIApp::_showDialog ( lua_State* L ) {
	
	MOAILuaState state ( L );	
	
	cc8* title = lua_tostring ( state, 1 );
	cc8* message = lua_tostring ( state, 2 );
	cc8* positive = lua_tostring ( state, 3 );
	cc8* neutral = lua_tostring ( state, 4 );
	cc8* negative = lua_tostring ( state, 5 );
	bool cancelable = lua_toboolean ( state, 6 );
	
	if ( state.IsType ( 7, LUA_TFUNCTION )) {
		
		// NOTE: This is fragile. We're storing the callback function in a global variable,
		// effectively. Invoking the showDialog method multiple times in succession can
		// therefore lead to unpredictable results. In fact, it's unknown how Android itself
		// handles multiple invocations - are they queued? On iOS, UIAlertView is LIFO and
		// new invocations supersede previous ones, but once dismissed, the system continues
		// down the alert stack.
		MOAIApp::Get ().mDialogCallback.SetStrongRef ( state, 7 );
	}	
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( title, jtitle );
	JNI_GET_JSTRING ( message, jmessage );
	JNI_GET_JSTRING ( positive, jpositive );
	JNI_GET_JSTRING ( neutral, jneutral );
	JNI_GET_JSTRING ( negative, jnegative );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		USLog::Print ( "MOAIApp: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID showDialog = env->GetStaticMethodID ( moai, "showDialog", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V" );
    	if ( showDialog == NULL ) {

			USLog::Print ( "MOAIApp: Unable to find static java method %s", "showDialog" );
    	} else {

			env->CallStaticVoidMethod ( moai, showDialog, jtitle, jmessage, jpositive, jneutral, jnegative, cancelable );	
		}
	}
	
	return 0;
}

//================================================================//
// MOAIApp
//================================================================//

//----------------------------------------------------------------//
MOAIApp::MOAIApp () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIApp::~MOAIApp () {

}

//----------------------------------------------------------------//
void MOAIApp::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "SESSION_START",		    ( u32 )SESSION_START );
	state.SetField ( -1, "SESSION_END",			    ( u32 )SESSION_END );
	state.SetField ( -1, "BACK_BUTTON_PRESSED",		( u32 )BACK_BUTTON_PRESSED );

	state.SetField ( -1, "DIALOG_RESULT_POSITIVE",	( u32 )DIALOG_RESULT_POSITIVE );
	state.SetField ( -1, "DIALOG_RESULT_NEUTRAL",	( u32 )DIALOG_RESULT_NEUTRAL );
	state.SetField ( -1, "DIALOG_RESULT_NEGATIVE",	( u32 )DIALOG_RESULT_NEGATIVE );
	state.SetField ( -1, "DIALOG_RESULT_CANCEL",	( u32 )DIALOG_RESULT_CANCEL );

	luaL_Reg regTable [] = {
		{ "openURL",		_openURL },
		{ "setListener",	_setListener },
		{ "showDialog",		_showDialog },
		{ "share",			_share },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
	
	MOAIEnvironment::Get ().SetGUIDFunc ( &_getGuid );
}

//----------------------------------------------------------------//
bool MOAIApp::NotifyBackButtonPressed () {
	
	MOAILuaRef& callback = this->mListeners [ BACK_BUTTON_PRESSED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();

		state.DebugCall ( 0, 1 );

		return lua_toboolean ( state, -1 );
	} else {
		
		return false;
	}
}

//----------------------------------------------------------------//
void MOAIApp::NotifyDialogDismissed ( int dialogResult ) {

	if ( !mDialogCallback.IsNil ()) {
		
		MOAILuaStateHandle state = mDialogCallback.GetSelf ();

		lua_pushinteger ( state, dialogResult );
		
		state.DebugCall ( 1, 0 );
		
		mDialogCallback.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIApp::NotifyDidStartSession ( bool resumed ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_START ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();

		lua_pushboolean ( state, resumed );
			
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::NotifyWillEndSession () {
	
	MOAILuaRef& callback = this->mListeners [ SESSION_END ];
	
	if ( callback ) {

		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// Miscellaneous JNI Functions
//================================================================//

//----------------------------------------------------------------//
extern "C" bool Java_com_ziplinegames_moai_Moai_AKUAppBackButtonPressed ( JNIEnv* env, jclass obj ) {

	return MOAIApp::Get ().NotifyBackButtonPressed ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_Moai_AKUAppDialogDismissed ( JNIEnv* env, jclass obj, jint code ) {

	MOAIApp::Get ().NotifyDialogDismissed ( code );
}	

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_Moai_AKUAppDidStartSession ( JNIEnv* env, jclass obj, jboolean resumed ) {

	MOAIApp::Get ().NotifyDidStartSession ( resumed );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_Moai_AKUAppWillEndSession ( JNIEnv* env, jclass obj ) {

	MOAIApp::Get ().NotifyWillEndSession ();
}