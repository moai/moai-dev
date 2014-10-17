// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIBrowserAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	canOpenURL
	@text	Return true if the device has an app installed that can open the URL.

	@in		string	url				The URL to check.
	@out 	boolean
*/
int MOAIBrowserAndroid::_canOpenURL ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIBrowserAndroid, "" )

	jstring jurl = self->GetJString ( lua_tostring ( state, 1 ));
	lua_pushboolean( state, self->CallStaticBooleanMethod ( self->mJava_CanOpenURL, jurl ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	openURL
	@text	Open the given URL in the device browser.

	@in		string	url				The URL to open.
	@out 	nil
*/
int MOAIBrowserAndroid::_openURL ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIBrowserAndroid, "" )

	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIBrowserAndroid: _openURL");

	jstring jurl = self->GetJString ( lua_tostring ( state, 1 ));
	self->CallStaticVoidMethod ( self->mJava_OpenURL, jurl );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openURLWithParams
	@text	Open the native device web browser at the specified URL
			with the specified list of query string parameters.

	@in		string url
	@in		table params
	@out	nil
*/
int MOAIBrowserAndroid::_openURLWithParams ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIBrowserAndroid, "" )
	
	cc8* url = lua_tostring ( state, 1 );

    jobject params;
    if ( state.IsType ( 2, LUA_TTABLE ) ) {
        params = self->BundleFromLua( L, 2 );
    }

	if ( url == NULL || params == NULL ) return 0;

	jstring jurl = self->GetJString ( url );
	self->CallStaticVoidMethod ( self->mJava_OpenURLWithParams, jurl, params );

	return 0;
}

//================================================================//
// MOAIBrowserAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIBrowserAndroid::MOAIBrowserAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
	
	//if ( this->SetClass ( "com/ziplinegames/moai/MoaiBrowser" )) {
	//	this->mJava_CanOpenURL			= this->GetStaticMethod ( "canOpenURL", "(Ljava/lang/String;)Z" );
	//	this->mJava_OpenURL				= this->GetStaticMethod ( "openURL", "(Ljava/lang/String;)V" );
	//	this->mJava_OpenURLWithParams	= this->GetStaticMethod ( "openURLWithParams", "(Ljava/lang/String;Landroid/os/Bundle;)V" );
	//}
}

//----------------------------------------------------------------//
MOAIBrowserAndroid::~MOAIBrowserAndroid () {}

//----------------------------------------------------------------//
void MOAIBrowserAndroid::RegisterLuaClass ( MOAILuaState& state ) {
	luaL_Reg regTable [] = {
		{ "canOpenURL",				_canOpenURL },
		{ "openURL",				_openURL },
		{ "openURLWithParams",		_openURLWithParams },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
