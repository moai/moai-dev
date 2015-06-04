// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <moai-android/JniUtils.h>
#include <moai-android-vungle/MOAIVungleAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIVungleAndroid::_displayAdvert ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIVungleAndroid, "" )

	bool incentivized		= state.GetValue < bool >( 1, true );
	bool showCloseButton	= state.GetValue < bool >( 2, false );

	jmethodID displayAdvert = self->GetStaticMethod ( "displayAdvert", "(ZZ)Z" );
	bool result = self->CallStaticBooleanMethod ( displayAdvert, incentivized, showCloseButton );
	state.Push ( result );
	return 1;
}

//----------------------------------------------------------------//
int	MOAIVungleAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIVungleAndroid, "" )
	
	MOAIJString appID = self->GetJString ( state.GetValue < cc8* >( 1, "" ));

	jmethodID init = self->GetStaticMethod ( "init", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( init, ( jstring )appID );

	return 0;
}

//----------------------------------------------------------------//
int	MOAIVungleAndroid::_isVideoAvailable ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIVungleAndroid, "" )
	
	bool debug		= state.GetValue < bool >( 1, true );
	
	jmethodID isVideoAvailable = self->GetStaticMethod ( "isVideoAvailable", "(Z)Z" );
	bool result = self->CallStaticBooleanMethod ( isVideoAvailable, debug );
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIChartBoostAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIVungleAndroid::MOAIVungleAndroid () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	this->SetClass ( "com/ziplinegames/moai/MoaiVungle" );
}

//----------------------------------------------------------------//
MOAIVungleAndroid::~MOAIVungleAndroid () {
}

//----------------------------------------------------------------//
void MOAIVungleAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "AD_START",		( u32 )AD_START );
	state.SetField ( -1, "AD_END", 			( u32 )AD_END );
	state.SetField ( -1, "AD_VIEWED", 		( u32 )AD_VIEWED );

	luaL_Reg regTable [] = {
		{ "displayAdvert",				_displayAdvert },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIVungleAndroid > },
		{ "init",						_init },
		{ "isVideoAvailable",			_isVideoAvailable },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIVungleAndroid > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// ChartBoost JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiVungle_AKUInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MoaiVungle_AKUInvokeListener\n" );
	MOAIVungleAndroid::Get ().InvokeListener (( u32 )eventID );
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiVungle_AKUOnView ( JNIEnv* env, jclass obj, jdouble watched, jdouble length ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MoaiVungle_AKUOnView\n" );
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( MOAIVungleAndroid::Get ().PushListener ( MOAIVungleAndroid::AD_VIEWED, state )) {
		state.Push ( watched == length );
		state.DebugCall ( 1, 0 );
	}
}
