// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_FLURRY

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIFlurryAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_beginEvent ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_endEvent ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_endSession ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jmethodID startSession = self->GetStaticMethod ( "onEndSession", "(Landroid/content/Context;)V" );
	self->CallStaticVoidMethod ( startSession, self->mActivity );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_logEvent ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_setUserID ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_startSession ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jstring japiKey = self->GetJString ( lua_tostring ( state, 1 ));
	jmethodID startSession = self->GetStaticMethod ( "onStartSession", "(Landroid/content/Context;Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( startSession, self->mActivity, japiKey );
	return 0;
}

//================================================================//
// MOAIFlurryAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIFlurryAndroid::MOAIFlurryAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->SetClassViaLoader ( "com.flurry.android.FlurryAgent" );
}

//----------------------------------------------------------------//
MOAIFlurryAndroid::~MOAIFlurryAndroid () {
}

//----------------------------------------------------------------//
void MOAIFlurryAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "beginEvent",			_beginEvent },
		{ "endEvent",			_endEvent },
		{ "endSession",			_endSession },
		{ "logEvent",			_logEvent },
		{ "setUserID",			_setUserID },
		{ "startSession",		_startSession },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

#endif