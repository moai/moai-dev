// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_FACEBOOK

#include "pch.h"

#include <jni.h>

#include <android/log.h>

#include <moaiext-android/MOAIFacebook.h>

extern JavaVM* jvm;
extern jobject mMoaiActivity;

jmethodID mFacebookLoginFunc;

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
int MOAIFacebook::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL_LISTENERS );

	if ( idx < TOTAL_LISTENERS ) {
		MOAIFacebook::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
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

	state.SetField ( -1, "FACEBOOK_LOGIN_LISTENER", ( u32 ) FACEBOOK_LOGIN_LISTENER );
	
	luaL_Reg regTable[] = {
		{ "login",					_login },
		{ "setListener",			_setListener },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebook::NotifyFacebookLogin ( int code ) {
	MOAILuaRef& callback = this->mListeners [ FACEBOOK_LOGIN_LISTENER ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushinteger ( state, code );
		
		state.DebugCall ( 1, 0 );
	}
}

#endif
