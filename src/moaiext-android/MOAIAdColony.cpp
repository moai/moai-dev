// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_ADCOLONY

#include "pch.h"

#include <jni.h>

#include <android/log.h>

#include <moaiext-android/MOAIAdColony.h>

extern JavaVM* jvm;
extern jobject mMoaiActivity;

jmethodID mInitAdColonyFunc;
jmethodID mPlayAdColonyVideoFunc;
jmethodID mAdColonyVideoReadyFunc;

//================================================================//
// Utility macros
//================================================================//

	#define GET_ENV() 	\
		JNIEnv* env; 	\
		jvm->GetEnv (( void** )&env, JNI_VERSION_1_4 );
		
	#define GET_CSTRING(jstr, cstr) \
		const char* cstr = ( jstr != NULL ) ? env->GetStringUTFChars ( jstr, NULL ) : NULL;

	#define RELEASE_CSTRING(jstr, cstr) \
		if ( cstr != NULL ) env->ReleaseStringUTFChars ( jstr, cstr );
		
	#define GET_JSTRING(cstr, jstr) \
		jstring jstr = ( cstr != NULL ) ? env->NewStringUTF (( const char* )cstr ) : NULL;
		
	#define PRINT(str) \
		__android_log_write ( ANDROID_LOG_INFO, "MoaiLog", str );

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* _luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}

//----------------------------------------------------------------//
int MOAIAdColony::_initAdColony ( lua_State* L ) {
	MOAILuaState state ( L );
     
	cc8* identifier = lua_tostring ( state, 1 );
	
	GET_ENV ();
	GET_JSTRING ( identifier, jidentifier );

	jobjectArray jzones;

	if ( state.IsType ( 2, LUA_TTABLE )) {

		int numEntries = 0;
		for ( int key = 1; ; ++key ) {

			state.GetField ( 2, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );

			if ( !value ) {
				numEntries = key - 1;
				break;
			}
		}

		jzones = env->NewObjectArray ( numEntries, env->FindClass( "java/lang/String" ), 0 );
		for ( int key = 1; ; ++key ) {

			state.GetField ( 2, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );

			if ( value ) {
				GET_JSTRING ( value, jvalue );
				env->SetObjectArrayElement ( jzones, key - 1, jvalue );
			}
			else {
				break;
			}	
		}
	}

	if (mInitAdColonyFunc == NULL) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mInitAdColonyFunc = env->GetMethodID ( moaiActivityClass, "initAdColony", "(Ljava/lang/String;[Ljava/lang/String;)V" );
	}

	env->CallVoidMethod ( mMoaiActivity , mInitAdColonyFunc, jidentifier, jzones );
	    
	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColony::_playVideo ( lua_State* L ) {
	// MOAILuaState state ( L );
	// 
	// int zone = state.GetValue < int >( 1, 1 );
	// bool prePopup = state.GetValue < bool >( 2, true );
	// bool postPopup = state.GetValue < bool >( 3, true );
	// 
	// [ AdColony playVideoAdForSlot:zone 
	// 				 withDelegate:nil /* add takeover delegate */
	// 			 withV4VCPrePopup:prePopup 
	//   				 andV4VCPostPopup:postPopup ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColony::_videoReadyForZone ( lua_State *L ) {
	// MOAILuaState state ( L );
	// 
	// int zone = state.GetValue < int >( 1, 0 );
	// 
	// int result = [ AdColony zoneStatusForSlot:zone ];
	// if ( result == ADCOLONY_ZONE_STATUS_ACTIVE ) {
	// 	
	// 	lua_pushboolean( L, true );
	// }
	// else {
	// 	
	// 	lua_pushboolean( L, false );
	// }
	
	return 0;
}
	
//================================================================//
// MOAIAdColony
//================================================================//

//----------------------------------------------------------------//
MOAIAdColony::MOAIAdColony () {
    
	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIAdColony::~MOAIAdColony () {
    
}

//----------------------------------------------------------------//
void MOAIAdColony::RegisterLuaClass ( MOAILuaState& state ) {
    
	luaL_Reg regTable [] = {
		{ "initAdColony",		_initAdColony },
		{ "playVideo",			_playVideo },
		{ "videoReadyForZone",	_videoReadyForZone },
		{ NULL, NULL }	
	};
    
	luaL_register ( state, 0, regTable );
}

#endif