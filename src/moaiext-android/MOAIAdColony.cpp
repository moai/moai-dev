// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_ADCOLONY

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIAdColony.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* MOAIAdColony::_luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}

//----------------------------------------------------------------//
int MOAIAdColony::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
     
	cc8* identifier = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( identifier, jidentifier );
		
	jobjectArray jzones = NULL;
	
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
				
				JNI_GET_JSTRING ( value, jvalue );
				env->SetObjectArrayElement ( jzones, key - 1, jvalue );
			}
			else {
				
				break;
			}	
		}
	}
	
	if ( jzones == NULL ) {
		
		jzones = env->NewObjectArray ( 0, env->FindClass( "java/lang/String" ), 0 );
	}
	
	jclass adcolony = env->FindClass ( "com/ziplinegames/moai/MoaiAdColony" );
    if ( adcolony == NULL ) {

		USLog::Print ( "MOAIAdColony: Unable to find java class %s", "com/ziplinegames/moai/MoaiAdColony" );
    } else {

    	jmethodID init = env->GetStaticMethodID ( adcolony, "init", "(Ljava/lang/String;[Ljava/lang/String;)V" );
    	if ( init == NULL ) {

			USLog::Print ( "MOAIAdColony: Unable to find static java method %s", "init" );
    	} else {

			env->CallStaticVoidMethod ( adcolony, init, jidentifier, jzones );				
		}
	}
	    
	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColony::_playVideo ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* zone = lua_tostring ( state, 1 );
	
	bool prompt = state.GetValue < bool >( 2, true );
	bool confirmation = state.GetValue < bool >( 3, true );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( zone, jzone );

	jclass adcolony = env->FindClass ( "com/ziplinegames/moai/MoaiAdColony" );
    if ( adcolony == NULL ) {

		USLog::Print ( "MOAIAdColony: Unable to find java class %s", "com/ziplinegames/moai/MoaiAdColony" );
    } else {

    	jmethodID playVideo = env->GetStaticMethodID ( adcolony, "playVideo", "(Ljava/lang/String;ZZ)V" );
    	if ( playVideo == NULL ) {

			USLog::Print ( "MOAIAdColony: Unable to find static java method %s", "playVideo" );
    	} else {

			env->CallStaticVoidMethod ( adcolony, playVideo, jzone, prompt, confirmation );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColony::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAIAdColony::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColony::_videoReadyForZone ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	cc8* zone = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( zone, jzone );

	jclass adcolony = env->FindClass ( "com/ziplinegames/moai/MoaiAdColony" );
    if ( adcolony == NULL ) {

		USLog::Print ( "MOAIAdColony: Unable to find java class %s", "com/ziplinegames/moai/MoaiAdColony" );
    } else {

    	jmethodID isVideoReady = env->GetStaticMethodID ( adcolony, "isVideoReady", "(Ljava/lang/String;)Z" );
    	if ( isVideoReady == NULL ) {

			USLog::Print ( "MOAIAdColony: Unable to find static java method %s", "isVideoReady" );
    	} else {

			jboolean jready = ( jboolean )env->CallStaticBooleanMethod ( adcolony, isVideoReady, jzone );				

			lua_pushboolean ( state, jready );
			
			return 1;
		}
	}

	lua_pushboolean ( state, false );

	return 1;
}
	
//================================================================//
// MOAIAdColony
//================================================================//
//----------------------------------------------------------------//
void MOAIAdColony::NotifyVideoComplete () {	
	
	MOAILuaRef& callback = this->mListeners [ VIDEO_ENDED_IN_ZONE ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
MOAIAdColony::MOAIAdColony () {
    
	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIAdColony::~MOAIAdColony () {
    
}

//----------------------------------------------------------------//
void MOAIAdColony::RegisterLuaClass ( MOAILuaState& state ) {
    
	state.SetField ( -1, "VIDEO_ENDED_IN_ZONE", ( u32 )VIDEO_ENDED_IN_ZONE );
	
	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ "playVideo",			_playVideo },
		{ "setListener",		_setListener },
		{ "videoReadyForZone",	_videoReadyForZone },
		{ NULL, NULL }	
	};
    
	luaL_register ( state, 0, regTable );
}

//================================================================//
// AdColony JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAdColony_AKUNotifyAdColonyVideoComplete ( JNIEnv* env, jclass obj ) {

	MOAIAdColony::Get ().NotifyVideoComplete ();
}
#endif