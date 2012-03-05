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
int MOAIAdColony::_init ( lua_State* L ) {
	MOAILuaState state ( L );
     
	// cc8* identifier = lua_tostring ( state, 1 );
	// 
	// GET_ENV ();
	// GET_JSTRING ( identifier, jidentifier );
	// 
	// jobjectArray jzones;
	// 
	// if ( state.IsType ( 2, LUA_TTABLE )) {
	// 
	// 	int numEntries = 0;
	// 	for ( int key = 1; ; ++key ) {
	// 
	// 		state.GetField ( 2, key );
	// 		cc8* value = _luaParseTable ( state, -1 );
	// 		lua_pop ( state, 1 );
	// 
	// 		if ( !value ) {
	// 			numEntries = key - 1;
	// 			break;
	// 		}
	// 	}
	// 
	// 	jzones = env->NewObjectArray ( numEntries, env->FindClass( "java/lang/String" ), 0 );
	// 	for ( int key = 1; ; ++key ) {
	// 
	// 		state.GetField ( 2, key );
	// 		cc8* value = _luaParseTable ( state, -1 );
	// 		lua_pop ( state, 1 );
	// 
	// 		if ( value ) {
	// 			GET_JSTRING ( value, jvalue );
	// 			env->SetObjectArrayElement ( jzones, key - 1, jvalue );
	// 		}
	// 		else {
	// 			break;
	// 		}	
	// 	}
	// }
	// 
	// if ( mInitAdColonyFunc == NULL ) {
	// 	
	// 	jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
	// 	mInitAdColonyFunc = env->GetMethodID ( moaiActivityClass, "initAdColony", "(Ljava/lang/String;[Ljava/lang/String;)V" );
	// }
	// 
	// env->CallVoidMethod ( mMoaiActivity , mInitAdColonyFunc, jidentifier, jzones );
	    
	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColony::_playVideo ( lua_State* L ) {
	MOAILuaState state ( L );
	
	// cc8* zone = lua_tostring ( state, 1 );
	// 
	// GET_ENV ();
	// GET_JSTRING ( zone, jzone );
	// 
	// bool prompt = state.GetValue < bool >( 2, true );
	// bool confirmation = state.GetValue < bool >( 3, true );
	// 
	// if ( mPlayAdColonyVideoFunc == NULL ) {
	// 	
	// 	jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
	// 	mPlayAdColonyVideoFunc = env->GetMethodID ( moaiActivityClass, "playAdColonyVideo", "(Ljava/lang/String;ZZ)V" );
	// }
	// 
	// env->CallVoidMethod ( mMoaiActivity , mPlayAdColonyVideoFunc, jzone, prompt, confirmation );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColony::_videoReadyForZone ( lua_State *L ) {
	MOAILuaState state ( L );
	
	// cc8* zone = lua_tostring ( state, 1 );
	// 
	// GET_ENV ();
	// GET_JSTRING ( zone, jzone );
	// 
	// if ( mAdColonyVideoReadyFunc == NULL ) {
	// 	
	// 	jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
	// 	mAdColonyVideoReadyFunc = env->GetMethodID ( moaiActivityClass, "isAdColonyVideoReady", "(Ljava/lang/String;)Z" );
	// }
	// 
	// bool ready = ( bool )env->CallBooleanMethod ( mMoaiActivity , mAdColonyVideoReadyFunc, jzone );
	// 
	// lua_pushboolean ( L, ready );
	return 0;
//	return 1;
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
		{ "initAdColony",		_init },
		{ "playVideo",			_playVideo },
		{ "videoReadyForZone",	_videoReadyForZone },
		{ NULL, NULL }	
	};
    
	luaL_register ( state, 0, regTable );
}

#endif