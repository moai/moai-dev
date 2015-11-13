// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/JniUtils.h>
#include <moai-android-deltadna/MOAIDeltaDNAAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	initialize
	@text	Initialize DeltaDNA sdk

 	@in		string  clientVersion
 	@in		bool  	debugMode
	@out	void
*/
int MOAIDeltaDNAAndroid::_initialize ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIDeltaDNAAndroid, "" )

    MOAIJString clientVersion = self->GetJString ( lua_tostring ( state, 1 ));
    bool debugMode = lua_toboolean ( state, 2 );

	self->CallStaticVoidMethod ( self->mJava_Initialize, ( jstring ) clientVersion, ( jboolean ) debugMode );
    //self->CallStaticVoidMethod ( self->mJava_Initialize );
	return 1;
}

//================================================================//
// MOAIDeltaDNAAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIDeltaDNAAndroid::MOAIDeltaDNAAndroid () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
		
	this->SetClass ( "com/moaisdk/deltadna/MoaiDeltaDNA" );

	this->mJava_Initialize  = this->GetStaticMethod ( "initialize", "(Ljava/lang/String;Z)V" );
    //this->mJava_Initialize  = this->GetStaticMethod ( "initialize", "()V" );
}

//----------------------------------------------------------------//
MOAIDeltaDNAAndroid::~MOAIDeltaDNAAndroid () {

}

//----------------------------------------------------------------//
void MOAIDeltaDNAAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "initialize",			    _initialize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}