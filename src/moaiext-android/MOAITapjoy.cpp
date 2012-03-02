// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_TAPJOY

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAITapjoy.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAITapjoy::_getUserId ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );

	jclass tapjoy = env->FindClass ( "com/ziplinegames/moai/MoaiTapjoy" );
    if ( tapjoy == NULL ) {
	
		USLog::Print ( "MOAITapjoy: Unable to find java class %s", "com/ziplinegames/moai/MoaiTapjoy" );
    } else {

    	jmethodID getUserId = env->GetStaticMethodID ( tapjoy, "getUserId", "()Ljava/lang/String;" );
    	if ( getUserId == NULL ) {

			USLog::Print ( "MOAITapjoy: Unable to find static java method %s", "getUserId" );
    	} else {

			jstring jidentifier = ( jstring )env->CallStaticObjectMethod ( tapjoy, getUserId );	
			
			JNI_GET_CSTRING ( jidentifier, identifier );

			lua_pushstring ( L, identifier );

			JNI_RELEASE_CSTRING ( jidentifier, identifier );
			
			return 1;
		}
	}

	lua_pushnil ( L );
	
	return 1;
}

//----------------------------------------------------------------//
int MOAITapjoy::_initVideoAds ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tapjoy = env->FindClass ( "com/ziplinegames/moai/MoaiTapjoy" );
    if ( tapjoy == NULL ) {
	
		USLog::Print ( "MOAITapjoy: Unable to find java class %s", "com/ziplinegames/moai/MoaiTapjoy" );
    } else {

    	jmethodID initVideoAds = env->GetStaticMethodID ( tapjoy, "initVideoAds", "()V" );
    	if ( initVideoAds == NULL ) {

			USLog::Print ( "MOAITapjoy: Unable to find static java method %s", "initVideoAds" );
    	} else {

			env->CallStaticVoidMethod ( tapjoy, initVideoAds );
			
			u32 count = state.GetValue < u32 >( 1, 0 );
			if ( count > 0 ) {

				jmethodID setVideoAdCacheCount = env->GetStaticMethodID ( tapjoy, "setVideoAdCacheCount", "(I)V" );
	    		if ( setVideoAdCacheCount == NULL ) {

					USLog::Print ( "MOAITapjoy: Unable to find static java method %s", "setVideoAdCacheCount" );
	    		} else {

					env->CallStaticVoidMethod ( tapjoy, setVideoAdCacheCount, count );
				}
			}
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAITapjoy::_requestTapjoyConnect ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	cc8* secret = lua_tostring ( state, 2 );

	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( identifier, jidentifier );
	JNI_GET_JSTRING ( secret, jsecret );

	jclass tapjoy = env->FindClass ( "com/ziplinegames/moai/MoaiTapjoy" );
    if ( tapjoy == NULL ) {
	
		USLog::Print ( "MOAITapjoy: Unable to find java class %s", "com/ziplinegames/moai/MoaiTapjoy" );
    } else {

    	jmethodID requestTapjoyConnect = env->GetStaticMethodID ( tapjoy, "requestTapjoyConnect", "(Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( requestTapjoyConnect == NULL ) {

			USLog::Print ( "MOAITapjoy: Unable to find static java method %s", "requestTapjoyConnect" );
    	} else {

			env->CallStaticVoidMethod ( tapjoy, requestTapjoyConnect, jidentifier, jsecret );				
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
int MOAITapjoy::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAITapjoy::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAITapjoy::_showOffers ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tapjoy = env->FindClass ( "com/ziplinegames/moai/MoaiTapjoy" );
    if ( tapjoy == NULL ) {
	
		USLog::Print ( "MOAITapjoy: Unable to find java class %s", "com/ziplinegames/moai/MoaiTapjoy" );
    } else {

    	jmethodID showOffers = env->GetStaticMethodID ( tapjoy, "showOffers", "()V" );
    	if ( showOffers == NULL ) {

			USLog::Print ( "MOAITapjoy: Unable to find static java method %s", "showOffers" );
    	} else {

			env->CallStaticVoidMethod ( tapjoy, showOffers );				
		}
	}
		
	return 0;
}

//================================================================//
// MOAITapjoy
//================================================================//

//----------------------------------------------------------------//
MOAITapjoy::MOAITapjoy () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAITapjoy::~MOAITapjoy () {

}

//----------------------------------------------------------------//
void MOAITapjoy::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "TAPJOY_VIDEO_AD_READY", 								( u32 )TAPJOY_VIDEO_AD_READY );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_ERROR", 								( u32 )TAPJOY_VIDEO_AD_ERROR );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_CLOSE", 								( u32 )TAPJOY_VIDEO_AD_CLOSE );

	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_NO_ERROR", 						( u32 )TAPJOY_VIDEO_STATUS_NO_ERROR );
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE", 		( u32 )TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE );
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS",	( u32 )TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS );
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO", 			( u32 )TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO );

	luaL_Reg regTable [] = {
		{ "getUserId",					_getUserId },
		{ "initVideoAds",				_initVideoAds },
		{ "requestTapjoyConnect",		_requestTapjoyConnect },
		{ "setListener",				_setListener },
		{ "showOffers",					_showOffers },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITapjoy::NotifyVideoAdReady () {	
	
	MOAILuaRef& callback = this->mListeners [ TAPJOY_VIDEO_AD_READY ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITapjoy::NotifyVideoAdError ( int code ) {
	
	MOAILuaRef& callback = this->mListeners [ TAPJOY_VIDEO_AD_ERROR ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushinteger ( state, code );
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITapjoy::NotifyVideoAdClose () {
	
	MOAILuaRef& callback = this->mListeners [ TAPJOY_VIDEO_AD_CLOSE ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// Tapjoy JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTapjoy_AKUNotifyTapjoyVideoAdReady ( JNIEnv* env, jclass obj ) {

	MOAITapjoy::Get ().NotifyVideoAdReady ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTapjoy_AKUNotifyTapjoyVideoAdError ( JNIEnv* env, jclass obj, jint code ) {

	MOAITapjoy::Get ().NotifyVideoAdError ( code );
}
	
//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTapjoy_AKUNotifyTapjoyVideoAdClose ( JNIEnv* env, jclass obj ) {

	MOAITapjoy::Get ().NotifyVideoAdClose ();
}

#endif