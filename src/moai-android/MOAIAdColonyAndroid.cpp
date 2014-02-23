// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_ADCOLONY

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIAdColonyAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getDeviceID
	@text	Request a unique ID for the device.

	@out 	string	id			The device ID.
*/
int MOAIAdColonyAndroid::_getDeviceID ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAdColonyAndroid, "" )

	jstring jidentifier = ( jstring )self->CallStaticObjectMethod ( self->mJava_GetDeviceID );

	cc8* identifier = self->GetCString ( jidentifier );
	lua_pushstring ( state, identifier );
	self->ReleaseCString ( jidentifier, identifier );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize AdColony.

	@in		string	appId			Available in AdColony dashboard settings.
	@in 	table	zones			A list of zones to configure. Available in AdColony dashboard settings.
	@out 	nil
*/
int MOAIAdColonyAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAdColonyAndroid, "" )

	jstring jidentifier = self->GetJString ( lua_tostring ( state, 1 ));

	jobjectArray jzones = NULL;

	if ( state.IsType ( 2, LUA_TTABLE )) {

		int numEntries = 0;
		for ( int key = 1; ; ++key ) {

			state.GetField ( 2, key );
			cc8* value = state.GetValue < cc8* >( -1, "" );
			lua_pop ( state, 1 );

			if ( !value ) {

				numEntries = key - 1;
				break;
			}
		}

		jzones = self->mEnv->NewObjectArray ( numEntries, self->mEnv->FindClass( "java/lang/String" ), 0 );
		for ( int key = 1; ; ++key ) {

			state.GetField ( 2, key );
			cc8* value = state.GetValue < cc8* >( -1, "" );
			lua_pop ( state, 1 );

			if ( value ) {
				jstring jvalue = self->GetJString ( value );
				self->mEnv->SetObjectArrayElement ( jzones, key - 1, jvalue );
			}
			else {
				break;
			}
		}
	}

	if ( jzones == NULL ) {
		jzones = self->mEnv->NewObjectArray ( 0, self->mEnv->FindClass( "java/lang/String" ), 0 );
	}

	self->CallStaticVoidMethod ( self->mJava_Init, jidentifier, jzones );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	playVideo
	@text	Play an AdColony video ad.

	@in 	string	zone			The zone from which to play a video ad.
	@opt	bool	prompt			Determines whether the user is asked whether they want to play a video ad or not. Default is true.
	@opt	bool	confirm			Determines whether the user is presented with a confirmation dialog after video ad playback completes. Default is true.
	@out 	nil
*/
int MOAIAdColonyAndroid::_playVideo ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAdColonyAndroid, "" )

	jstring jzone = self->GetJString ( lua_tostring ( state, 1 ));

	bool prompt = state.GetValue < bool >( 2, true );
	bool confirmation = state.GetValue < bool >( 3, true );

	self->CallStaticVoidMethod ( self->mJava_PlayVideo, jzone, prompt, confirmation );

	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColonyAndroid::_setListener ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAdColonyAndroid, "" )

	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		self->mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	videoReadyForZone
	@text	Check the readiness of a video ad for a given zone.

	@in 	string	zone			The zone from which to check for a video ad.
	@out 	bool					True, if a video ad is ready to play.
*/
int MOAIAdColonyAndroid::_videoReadyForZone ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAdColonyAndroid, "" )
	
	jstring jzone = self->GetJString ( lua_tostring ( state, 1 ));
	lua_pushboolean ( state, self->CallStaticBooleanMethod ( self->mJava_IsVideoReady, jzone ));
	return 1;
}

//================================================================//
// MOAIAdColonyAndroid
//================================================================//
//----------------------------------------------------------------//
void MOAIAdColonyAndroid::NotifyVideoComplete ( int success ) {

	MOAILuaRef& callback = ( success == 1 ) ? this->mListeners [ VIDEO_ENDED_IN_ZONE ] : this->mListeners [ VIDEO_FAILED_IN_ZONE ];

	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
MOAIAdColonyAndroid::MOAIAdColonyAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->SetClass ( "com/ziplinegames/moai/MoaiAdColony" );
	
	this->mJava_GetDeviceID		= this->GetStaticMethod ( "getDeviceID", "()Ljava/lang/String;" );
	this->mJava_Init			= this->GetStaticMethod ( "init", "(Ljava/lang/String;[Ljava/lang/String;)V" );
	this->mJava_IsVideoReady	= this->GetStaticMethod ( "isVideoReady", "(Ljava/lang/String;)Z" );
	this->mJava_PlayVideo		= this->GetStaticMethod ( "playVideo", "(Ljava/lang/String;ZZ)V" );
}

//----------------------------------------------------------------//
MOAIAdColonyAndroid::~MOAIAdColonyAndroid () {
}

//----------------------------------------------------------------//
void MOAIAdColonyAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "VIDEO_BEGAN_IN_ZONE", 	( u32 )VIDEO_BEGAN_IN_ZONE );
	state.SetField ( -1, "VIDEO_ENDED_IN_ZONE", 	( u32 )VIDEO_ENDED_IN_ZONE );
	state.SetField ( -1, "VIDEO_FAILED_IN_ZONE", 	( u32 )VIDEO_FAILED_IN_ZONE );
	state.SetField ( -1, "VIDEO_PAUSED_IN_ZONE", 	( u32 )VIDEO_PAUSED_IN_ZONE );
	state.SetField ( -1, "VIDEO_RESUMED_IN_ZONE",	( u32 )VIDEO_RESUMED_IN_ZONE );

	luaL_Reg regTable [] = {
		{ "getDeviceID",		_getDeviceID },
		{ "init",				_init },
		{ "playVideo",			_playVideo },
		{ "setListener",		_setListener },
		{ "videoReadyForZone",	_videoReadyForZone },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// AdColony JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAdColony_AKUNotifyAdColonyVideoComplete ( JNIEnv* env, jclass obj, jint success ) {

	MOAIAdColonyAndroid::Get ().NotifyVideoComplete ( success );
}

#endif