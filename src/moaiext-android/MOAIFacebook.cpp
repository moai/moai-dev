// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_FACEBOOK

#include "pch.h"

#include <jni.h>

#include <android/log.h>

#include <moaiext-android/MOAIFacebook.h>

extern JavaVM* jvm;
extern jobject mMoaiActivity;

jmethodID mFacebookGetTokenFunc;
jmethodID mFacebookInitFunc;
jmethodID mFacebookLoginFunc;
jmethodID mFacebookLogoutFunc;
jmethodID mFacebookPostToFeedFunc;
jmethodID mFacebookSessionValidFunc;
jmethodID mFacebookSetTokenFunc;

//================================================================//
// Utility macros
//================================================================//

	#define GET_ENV() 	\
		JNIEnv* env; 	\
		jvm->GetEnv (( void** )&env, JNI_VERSION_1_4 );

	#define GET_CSTRING(jstr, cstr) \
		const char* cstr = env->GetStringUTFChars ( jstr, NULL );

	#define RELEASE_CSTRING(jstr, cstr) \
		env->ReleaseStringUTFChars ( jstr, cstr );
		
	#define GET_JSTRING(cstr, jstr) \
		jstring jstr = env->NewStringUTF (( const char* )cstr );

	#define PRINT(str) \
		__android_log_write ( ANDROID_LOG_INFO, "MoaiLog", str );

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* MOAIFacebook::_luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}


//----------------------------------------------------------------//
int MOAIFacebook::_getToken ( lua_State* L ) {
	MOAILuaState state ( L );
		
	GET_ENV ();
	
	if ( mFacebookGetTokenFunc == NULL ) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mFacebookGetTokenFunc = env->GetMethodID ( moaiActivityClass, "facebookGetToken", "(V)Ljava/lang/String;" );
	}

	jstring jtoken = ( jstring )env->CallObjectMethod ( mMoaiActivity , mFacebookGetTokenFunc );
	GET_CSTRING ( jtoken, token );
	
	lua_pushstring ( state, token );
	RELEASE_CSTRING ( jtoken, token );
	return 1;
}

//----------------------------------------------------------------//
int MOAIFacebook::_init ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* apId = lua_tostring ( state, 1 );
	
	GET_ENV ();
	GET_JSTRING ( apId, japId );
	
	if ( mFacebookInitFunc == NULL ) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mFacebookInitFunc = env->GetMethodID ( moaiActivityClass, "facebookInit", "(Ljava/lang/String;)V" );
	}

	env->CallVoidMethod ( mMoaiActivity , mFacebookInitFunc, japId );
}
		
//----------------------------------------------------------------//
int MOAIFacebook::_login( lua_State *L ) {
	MOAILuaState state ( L );
	GET_ENV ();
	
	jobjectArray args;
	
	//read in the table of permissions
	if ( state.IsType ( 1, LUA_TTABLE )) {
		
		//find number of permissions in table
		int numberOfPerms = 0;
		for ( int key = 1; ; ++key ) {

			state.GetField ( 1, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );

			if ( !value ) {
				numberOfPerms = key - 1;
				break;
			}
		}
		
		//read them into java string
		args = env->NewObjectArray ( numberOfPerms, env->FindClass( "java/lang/String" ), 0 );
		for ( int key = 1; ; ++key ) {

			state.GetField ( 1, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );

			if ( value ) {
				jstring str = env->NewStringUTF( value );
				env->SetObjectArrayElement ( args, key - 1, str );
			}
			else {
				break;
			}	
		}
	}

	if ( mFacebookLoginFunc == NULL ) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );
		mFacebookLoginFunc = env->GetMethodID ( moaiActivityClass, "facebookLogin", "([Ljava/lang/String;)V" );
	}

	env->CallObjectMethod ( mMoaiActivity , mFacebookLoginFunc, args );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_logout( lua_State *L ) {
	MOAILuaState state ( L );
	
	GET_ENV ();
	
	if ( mFacebookLogoutFunc == NULL ) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mFacebookLogoutFunc = env->GetMethodID ( moaiActivityClass, "facebookLogout", "(V)V" );
	}

	env->CallVoidMethod ( mMoaiActivity , mFacebookLogoutFunc );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_postToFeed ( lua_State* L ) {
	MOAILuaState state ( L );
		
	GET_ENV ();
	
	if ( mFacebookPostToFeedFunc == NULL ) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mFacebookPostToFeedFunc = env->GetMethodID ( moaiActivityClass, "facebookPostToFeed", "(V)V" );
	}

	env->CallVoidMethod ( mMoaiActivity , mFacebookPostToFeedFunc );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_sessionValid ( lua_State* L ) {
	MOAILuaState state ( L );

	GET_ENV ();
	
	if ( mFacebookSessionValidFunc == NULL ) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mFacebookSessionValidFunc = env->GetMethodID ( moaiActivityClass, "facebookIsSessionValid", "(V)Z" );
	}

	jboolean isValid = env->CallBooleanMethod ( mMoaiActivity , mFacebookSessionValidFunc );
	lua_pushboolean ( state, isValid );

	return 1;
}

//----------------------------------------------------------------//
int MOAIFacebook::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL_LISTENERS );

	if ( idx < TOTAL_LISTENERS ) {
		MOAIFacebook::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_setToken ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* token = lua_tostring ( state, 1 );
	
	GET_ENV ();
	GET_JSTRING ( token, jtoken );
	
	if ( mFacebookSetTokenFunc == NULL ) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mFacebookSetTokenFunc = env->GetMethodID ( moaiActivityClass, "facebookSetToken", "(Ljava/lang/String;)V" );
	}

	env->CallVoidMethod ( mMoaiActivity , mFacebookSetTokenFunc, jtoken );
}

//================================================================//
// MOAIFacebook
//================================================================//

//----------------------------------------------------------------//
MOAIFacebook::MOAIFacebook () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIFacebook::~MOAIFacebook () {

}

//----------------------------------------------------------------//
void MOAIFacebook::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DIALOG_DID_CANCEL", ( u32 ) DIALOG_DID_CANCEL );
	state.SetField ( -1, "DIALOG_DID_COMPLETE", ( u32 ) DIALOG_DID_COMPLETE );
	state.SetField ( -1, "SESSION_DID_LOGIN", ( u32 ) SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN", ( u32 ) SESSION_DID_NOT_LOGIN );
	
	luaL_Reg regTable[] = {
		{ "getToken",				_getToken },
		{ "init",					_init },
		{ "login",					_login },
		{ "logout",					_logout },
		{ "postToFeed",				_postToFeed },
		{ "sessionValid",			_sessionValid },
		{ "setListener",			_setListener },
		{ "setToken",				_setToken },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebook::NotifyFacebookDialog ( int code ) {
	
	if ( code == 1 ) {
		
		MOAILuaRef& callback = this->mListeners [ DIALOG_DID_COMPLETE ];

		if ( callback ) {
			MOAILuaStateHandle state = callback.GetSelf ();
			state.DebugCall ( 0, 0 );
		}		
	} else if ( code == 0 ) {
				
		MOAILuaRef& callback = this->mListeners [ DIALOG_DID_CANCEL ];

		if ( callback ) {
			MOAILuaStateHandle state = callback.GetSelf ();
			state.DebugCall ( 0, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFacebook::NotifyFacebookLogin ( int code ) {
	
	if ( code == 1 ) {
		
		MOAILuaRef& callback = this->mListeners [ SESSION_DID_LOGIN ];

		if ( callback ) {
			MOAILuaStateHandle state = callback.GetSelf ();
			state.DebugCall ( 0, 0 );
		}		
	} else if ( code == 0 ) {
				
		MOAILuaRef& callback = this->mListeners [ SESSION_DID_NOT_LOGIN ];

		if ( callback ) {
			MOAILuaStateHandle state = callback.GetSelf ();
			state.DebugCall ( 0, 0 );
		}
	}
}

#endif
