// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com


#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <moai-android/moaiext-jni.h>
#include <moai-android-tapjoy/MOAITapjoyAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initializes Tapjoy.
				
	@in		string	appId				Available in Tapjoy dashboard settings.
	@in		string	secretKey			Available in Tapjoy dashboard settings.
	@in		number	videoCacheCount
	@out	nil
*/
int MOAITapjoyAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAITapjoyAndroid, "" )
	
	jstring appID			= self->GetJString ( state.GetValue < cc8* >( 1, "" ));
	jstring appSignature	= self->GetJString ( state.GetValue < cc8* >( 2, "" ));
	int	videoCacheCount		= state.GetValue < int >( 3, 1 );
	
	printf ( "attempting to get init method" );
	jmethodID init = self->GetStaticMethod ( "init", "(Ljava/lang/String;Ljava/lang/String;I)V" );
	self->CallStaticVoidMethod ( init, appID, appSignature, videoCacheCount );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUserId
 	@text	Sets the tapjoy user ID.
 
 	@in		string userId
 */
int MOAITapjoyAndroid::_setUserId ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAITapjoyAndroid, "" )
	
	jstring userID = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID setUserID = self->GetStaticMethod ( "setUserId", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( setUserID, userID );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	showOffers
	@text	Displays the Tapjoy marketplace.
				
	@out	nil
*/
int MOAITapjoyAndroid::_showOffers ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAITapjoyAndroid, "" )
	
	jstring userID = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID showOffers = self->GetStaticMethod ( "showOffers", "()V" );
	self->CallStaticVoidMethod ( showOffers );
	return 0;
}

//================================================================//
// MOAITapjoyAndroid
//================================================================//

//----------------------------------------------------------------//
MOAITapjoyAndroid::MOAITapjoyAndroid () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
		
	this->SetClass ( "com/ziplinegames/moai/MoaiTapjoy" );
}

//----------------------------------------------------------------//
MOAITapjoyAndroid::~MOAITapjoyAndroid () {
}

//----------------------------------------------------------------//
void MOAITapjoyAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "TAPJOY_VIDEO_AD_START", 								( u32 )TAPJOY_VIDEO_AD_START );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_COMPLETE", 							( u32 )TAPJOY_VIDEO_AD_COMPLETE );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_ERROR", 								( u32 )TAPJOY_VIDEO_AD_ERROR );
	state.SetField ( -1, "TAPJOY_OFFERS_RESPONSE", 								( u32 )TAPJOY_OFFERS_RESPONSE );
	state.SetField ( -1, "TAPJOY_OFFERS_RESPONSE_FAILED", 						( u32 )TAPJOY_OFFERS_RESPONSE_FAILED );

	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE", 		( u32 )TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE );
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS",	( u32 )TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS );
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO", 			( u32 )TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO );

	luaL_Reg regTable [] = {
		{ "getListener",	&MOAIGlobalEventSource::_getListener < MOAITapjoyAndroid > },
		{ "init",			_init },
		{ "setListener",	&MOAIGlobalEventSource::_setListener < MOAITapjoyAndroid > },
		{ "setUserId",		_setUserId },
		{ "showOffers",		_showOffers },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// Tapjoy JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTapjoy_AKUInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MoaiTapjoy_AKUInvokeListener\n" );
	MOAITapjoyAndroid::Get ().InvokeListener (( u32 )eventID );
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTapjoy_AKUInvokeListenerWithCode ( JNIEnv* env, jclass obj, jint eventID, jint code ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MoaiTapjoy_AKUInvokeListenerWithCode\n" );
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( MOAITapjoyAndroid::Get ().PushListener ( eventID, state )) {
		state.Push ( code );
		state.DebugCall ( 1, 0 );
	}
}

