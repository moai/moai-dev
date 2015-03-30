// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <moai-android/JniUtils.h>
#include <moai-android-chartboost/MOAIChartBoostAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_cacheInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIChartBoostAndroid, "" )
	
	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));
	
	jmethodID cacheInterstitial = self->GetStaticMethod ( "cacheInterstitial", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( cacheInterstitial, ( jstring )jlocation );
	return 0;
}

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_hasCachedInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIChartBoostAndroid, "" )

	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID cacheInterstitial = self->GetStaticMethod ( "hasCachedInterstitial", "(Ljava/lang/String;)Z" );
	state.Push ( self->CallStaticBooleanMethod ( cacheInterstitial, ( jstring )jlocation ));
	return 1;
}

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIChartBoostAndroid, "" )
	
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartBoostAndroid::_init\n" );

	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartBoostAndroid: Getting jappID...\n" );
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartBoostAndroid: state.GetValue < cc8* >( 1, 0 ): %s\n",state.GetValue < cc8* >( 1, 0 ) );
	MOAIJString jappID			= self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartBoostAndroid: Getting jappSignature...\n" );

	MOAIJString jappSignature	= self->GetJString ( state.GetValue < cc8* >( 2, 0 ));
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartBoostAndroid: Got jappSignature\n" );
	jmethodID init = self->GetStaticMethod ( "init", "(Ljava/lang/String;Ljava/lang/String;)V" );

	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartBoostAndroid: Calling init\n" );
	self->CallStaticVoidMethod ( init, ( jstring )jappID, ( jstring )jappSignature );
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIChartBoostAndroid: Done calling init\n" );
	return 0;
}

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_showInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIChartBoostAndroid, "" )

	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID showInterstitial = self->GetStaticMethod ( "showInterstitial", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( showInterstitial, ( jstring )jlocation );
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
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiChartBoost_AKUInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MoaiChartBoost_AKUInvokeListener\n" );
	MOAIChartBoostAndroid::Get ().InvokeListener (( u32 )eventID );
}
