// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/JniUtils.h>
#include <moai-android-adcolony/MOAIAdColonyAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize AdColony.

	@in		string	appId			Available in AdColony dashboard settings.
	@in 	table	zones			A list of zones to configure. Available in AdColony dashboard settings.
	@out 	nil
*/
int MOAIAdColonyAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAdColonyAndroid, "" )

	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAdColonyAndroid::_init\n" );

	MOAIJString jidentifier		= self->GetJString ( lua_tostring ( state, 1 ));
	MOAIJString joptions		= self->GetJString ( lua_tostring ( state, 2 ));
	jobjectArray jzones			= self->StringArrayFromLua ( state, 3 );

	self->CallStaticVoidMethod ( self->mJava_Init, ( jstring )jidentifier, ( jstring )joptions, ( jstring )jzones );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	playVideo
	@text	Play an AdColony video ad.

	@in 	string	zone			The zone from which to play a video ad.
	@opt	bool	prompt			Determines whether the user is asked whether they want to play a video ad or not. Default is true.
	@opt	bool	confirm			Determines whether the user is presented with a confirmation dialog after video ad playback completes. Default is true.
	@out 	nil
*/
int MOAIAdColonyAndroid::_playVideo ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAdColonyAndroid, "" )

	MOAIJString jzone = self->GetJString ( lua_tostring ( state, 1 ));

	bool prompt = state.GetValue < bool >( 2, true );
	bool confirmation = state.GetValue < bool >( 3, true );

	self->CallStaticVoidMethod ( self->mJava_PlayVideo, ( jstring )jzone, prompt, confirmation );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	videoReadyForZone
	@text	Check the readiness of a video ad for a given zone.

	@in 	string	zone			The zone from which to check for a video ad.
	@out 	bool					True, if a video ad is ready to play.
*/
int MOAIAdColonyAndroid::_videoReadyForZone ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAdColonyAndroid, "" )
	
	MOAIJString jzone = self->GetJString ( lua_tostring ( state, 1 ));
	lua_pushboolean ( state, self->CallStaticBooleanMethod ( self->mJava_IsVideoReady, ( jstring )jzone ));
	return 1;
}

//================================================================//
// MOAIAdColonyAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIAdColonyAndroid::MOAIAdColonyAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->SetClass ( "com/ziplinegames/moai/MoaiAdColony" );
	
	this->mJava_Init			= this->GetStaticMethod ( "init", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V" );
	this->mJava_IsVideoReady	= this->GetStaticMethod ( "isVideoReady", "(Ljava/lang/String;)Z" );
	this->mJava_PlayVideo		= this->GetStaticMethod ( "playVideo", "(Ljava/lang/String;ZZ)V" );
}

//----------------------------------------------------------------//
MOAIAdColonyAndroid::~MOAIAdColonyAndroid () {
}

//----------------------------------------------------------------//
void MOAIAdColonyAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "VIDEO_STARTED", 		( u32 )VIDEO_STARTED );
	state.SetField ( -1, "VIDEO_SHOWN", 		( u32 )VIDEO_SHOWN );
	state.SetField ( -1, "VIDEO_FAILED", 		( u32 )VIDEO_FAILED );

	luaL_Reg regTable [] = {
		{ "getListener",		&MOAIGlobalEventSource::_getListener < MOAIAdColonyAndroid > },
		{ "init",				_init },
		{ "playVideo",			_playVideo },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIAdColonyAndroid > },
		{ "videoReadyForZone",	_videoReadyForZone },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// AdColony JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiAdColony_AKUInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MOAIAdColonyAndroid_AKUInvokeListener\n" );
	MOAIAdColonyAndroid::Get ().InvokeListener (( u32 )eventID );
}
