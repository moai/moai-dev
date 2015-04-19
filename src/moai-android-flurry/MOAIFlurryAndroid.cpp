// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com


#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android-flurry/MOAIFlurryAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_enableLogging ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )
		
	bool enable = state.GetValue < bool >( 1, false );
	jmethodID enableLogging = self->GetStaticMethod ( "enableLogging", "(Z)V" );
	self->CallStaticVoidMethod ( enableLogging, enable );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_endEvent ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jstring eventId = self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jmethodID logEvent = self->GetStaticMethod ( "endEvent", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( logEvent, eventId );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_init ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )
		
	jstring apiKey = self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jmethodID init = self->GetStaticMethod ( "init", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( init, apiKey );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_logEvent ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	jstring eventId = self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jobject hashMap = 0;

	if ( state.IsType ( 2, LUA_TTABLE )) {
		hashMap = self->HashMapFromLua ( L, 2 );
	}
	
	bool timed = state.GetValue < bool >( 3, false );

	jmethodID logEvent = self->GetStaticMethod ( "logEvent", "(Ljava/lang/String;Ljava/util/Map;Z)V" );
	
	self->CallStaticVoidMethod ( logEvent, eventId, hashMap, timed );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFlurryAndroid::_setUserID ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFlurryAndroid, "" )

	return 0;
}

//================================================================//
// MOAIFlurryAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIFlurryAndroid::MOAIFlurryAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->SetClass ( "com/ziplinegames/moai/MoaiFlurry" );
}

//----------------------------------------------------------------//
MOAIFlurryAndroid::~MOAIFlurryAndroid () {
}

//----------------------------------------------------------------//
void MOAIFlurryAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "enableLogging",		_enableLogging },
		{ "endEvent",			_endEvent },
		{ "init",				_init },
		{ "logEvent",			_logEvent },
		{ "setUserID",			_setUserID },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

