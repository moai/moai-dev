// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_CHARTBOOST

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIChartBoostAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_cacheInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIChartBoostAndroid, "" )
	
	jstring jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));
	
	jmethodID cacheInterstitial = self->GetStaticMethod ( "cacheInterstitial", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( cacheInterstitial, jlocation );
	return 0;
}

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_hasCachedInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIChartBoostAndroid, "" )

	jstring jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID cacheInterstitial = self->GetStaticMethod ( "hasCachedInterstitial", "(Ljava/lang/String;)Z" );
	state.Push ( self->CallStaticBooleanMethod ( cacheInterstitial, jlocation ));
	return 1;
}

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIChartBoostAndroid, "" )
	
	jstring jappID			= self->GetJString ( state.GetValue < cc8* >( 1, 0 ));
	jstring jappSignature	= self->GetJString ( state.GetValue < cc8* >( 2, 0 ));
	
	jmethodID init = self->GetStaticMethod ( "init", "(Ljava/lang/String;Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( init, jappID, jappSignature );
	return 0;
}

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_showInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIChartBoostAndroid, "" )

	jstring jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID showInterstitial = self->GetStaticMethod ( "showInterstitial", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( showInterstitial, jlocation );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hasCachedInterstitial
	@text	Determine whether or not a cached interstitial is available.

	@out 	boolean	hasCached
*/
int MOAIChartBoostAndroid::_hasCachedInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );
	
	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartBoost" );
	if ( chartboost == NULL ) {

		ZLLog::Print ( "MOAIChartBoostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartBoost" );
	} else {

		jmethodID hasCachedInterstitial = env->GetStaticMethodID ( chartboost, "hasCachedInterstitial", "()Z" );
		if ( hasCachedInterstitial == NULL ) {

			ZLLog::Print ( "MOAIChartBoostAndroid: Unable to find static java method %s", "hasCachedInterstitial" );
		} else {

			jboolean hasCached = ( jboolean )env->CallStaticBooleanMethod ( chartboost, hasCachedInterstitial );	

			lua_pushboolean ( state, hasCached );

			return 1;		
		}
	}

	return 0;
}

//================================================================//
// MOAIChartBoostAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIChartBoostAndroid::MOAIChartBoostAndroid () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	this->SetClass ( "com/ziplinegames/moai/MoaiChartBoost" );
}

//----------------------------------------------------------------//
MOAIChartBoostAndroid::~MOAIChartBoostAndroid () {
}

//----------------------------------------------------------------//
void MOAIChartBoostAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "INTERSTITIAL_LOAD_FAILED",	( u32 )INTERSTITIAL_LOAD_FAILED );
	state.SetField ( -1, "INTERSTITIAL_DISMISSED", 		( u32 )INTERSTITIAL_DISMISSED );

	luaL_Reg regTable [] = {
		{ "cacheInterstitial",			_cacheInterstitial },
		{ "hasCachedInterstitial",		_hasCachedInterstitial },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIChartBoostAndroid > },
		{ "init",						_init },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIChartBoostAndroid > },
		{ "showInterstitial",			_showInterstitial },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// ChartBoost JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiChartBoost_AKUInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MoaiChartBoost_AKUInvokeListener\n" );
	MOAIChartBoostAndroid::Get ().InvokeListener (( u32 )eventID );
}

#endif