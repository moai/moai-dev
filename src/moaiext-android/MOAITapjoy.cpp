// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_TAPJOY

#include "pch.h"

#include <jni.h>

#include <android/log.h>

#include <moaiext-android/MOAITapjoy.h>

extern JavaVM* jvm;
extern jobject mMoaiActivity;

jmethodID mRequestTapjoyFunc;
jmethodID mGetUserIdFunc;
jmethodID mInitVideoAdsFunc;
jmethodID mSetVideoAdCacheCount;
jmethodID mShowOffersFunc;

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
int MOAITapjoy::_getUserId ( lua_State *L ) {
	MOAILuaState state ( L );
	
	GET_ENV ();

	if (mGetUserIdFunc == NULL) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mGetUserIdFunc = env->GetMethodID ( moaiActivityClass, "getUserId", "()Ljava/lang/String;" );
	}

	jstring jidentifier = ( jstring )env->CallObjectMethod ( mMoaiActivity , mGetUserIdFunc );
	
	GET_CSTRING ( jidentifier, identifier );

	char buf [ 512 ];
	strcpy ( buf, identifier );
	const char* retVal = buf;
		
	RELEASE_CSTRING ( jidentifier, identifier );

	lua_pushstring ( L, retVal );
	
	return 1;
}

//----------------------------------------------------------------//
int MOAITapjoy::_initVideoAds ( lua_State* L ) {
	MOAILuaState state ( L );

	GET_ENV ();

	if (mInitVideoAdsFunc == NULL) {
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mInitVideoAdsFunc = env->GetMethodID ( moaiActivityClass, "initVideoAds", "()V" );
	}

	env->CallVoidMethod ( mMoaiActivity , mInitVideoAdsFunc );

	u32 count = state.GetValue < u32 >( 1, 0 );
	if ( count > 0 ) {

		if (mSetVideoAdCacheCount == NULL) {
			
			jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
			mSetVideoAdCacheCount = env->GetMethodID ( moaiActivityClass, "setVideoAdCacheCount", "(I)V" );
		}

		env->CallVoidMethod ( mMoaiActivity , mSetVideoAdCacheCount, count );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAITapjoy::_requestTapjoyConnect ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	cc8* secret = lua_tostring ( state, 2 );
	
	GET_ENV ();
	GET_JSTRING ( identifier, jidentifier );
	GET_JSTRING ( secret, jsecret );

	if (mRequestTapjoyFunc == NULL) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mRequestTapjoyFunc = env->GetMethodID ( moaiActivityClass, "requestTapjoyConnect", "(Ljava/lang/String;Ljava/lang/String;)V" );
	}

	env->CallVoidMethod ( mMoaiActivity , mRequestTapjoyFunc, jidentifier, jsecret );
		
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

	GET_ENV ();

	if (mShowOffersFunc == NULL) {
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mShowOffersFunc = env->GetMethodID ( moaiActivityClass, "showOffers", "()V" );
	}

	env->CallVoidMethod ( mMoaiActivity , mShowOffersFunc );
		
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

	luaL_Reg regTable[] = {
		{ "getUserId",					_getUserId },
		{ "initVideoAds",				_initVideoAds },
		{ "requestTapjoyConnect",		_requestTapjoyConnect },
		{ "setListener",				_setListener },
		{ "showOffers",					_showOffers },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
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

#endif
