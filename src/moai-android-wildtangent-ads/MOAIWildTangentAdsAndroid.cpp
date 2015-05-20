// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <moai-android/JniUtils.h>
#include <moai-android-wildtangent-ads/MOAIWildTangentAdsAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIWildTangentAdsAndroid::_cacheInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIWildTangentAdsAndroid, "" )
	
	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));
	
	jmethodID cacheInterstitial = self->GetStaticMethod ( "cacheInterstitial", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( cacheInterstitial, ( jstring )jlocation );
	return 0;
}

//----------------------------------------------------------------//
int MOAIWildTangentAdsAndroid::_hasCachedInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIWildTangentAdsAndroid, "" )

	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID cacheInterstitial = self->GetStaticMethod ( "hasCachedInterstitial", "(Ljava/lang/String;)Z" );
	state.Push ( self->CallStaticBooleanMethod ( cacheInterstitial, ( jstring )jlocation ));
	return 1;
}

//----------------------------------------------------------------//
int MOAIWildTangentAdsAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIWildTangentAdsAndroid, "" )
	
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIWildTangentAdsAndroid::_init\n" );
	MOAIJString jappID			= self->GetJString ( state.GetValue < cc8* >( 1, 0 ));
	MOAIJString jappSignature	= self->GetJString ( state.GetValue < cc8* >( 2, 0 ));
	jmethodID init = self->GetStaticMethod ( "init", "()V" );
	self->CallStaticVoidMethod ( init );
	return 0;
}

//----------------------------------------------------------------//
int MOAIWildTangentAdsAndroid::_showInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIWildTangentAdsAndroid, "" )

	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID showInterstitial = self->GetStaticMethod ( "showInterstitial", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( showInterstitial, ( jstring )jlocation );
	return 0;
}

//----------------------------------------------------------------//
MOAIWildTangentAdsAndroid::MOAIWildTangentAdsAndroid () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
	this->SetClass ( "com/ziplinegames/moai/MoaiWildTangentAds" );
}

//----------------------------------------------------------------//
MOAIWildTangentAdsAndroid::~MOAIWildTangentAdsAndroid () {

}

//----------------------------------------------------------------//
void MOAIWildTangentAdsAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "INTERSTITIAL_LOAD_FAILED",	( u32 )INTERSTITIAL_LOAD_FAILED );
	state.SetField ( -1, "INTERSTITIAL_DISMISSED", 		( u32 )INTERSTITIAL_DISMISSED );

	luaL_Reg regTable [] = {
		{ "cacheInterstitial",			_cacheInterstitial },
		{ "hasCachedInterstitial",		_hasCachedInterstitial },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIWildTangentAdsAndroid > },
		{ "init",						_init },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIWildTangentAdsAndroid > },
		{ "showInterstitial",			_showInterstitial },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

// AKU Callbacks

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiWildTangentAds_AKUInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MoaiWildTangentAds_AKUInvokeListener\n" );
	MOAIWildTangentAdsAndroid::Get ().InvokeListener (( u32 )eventID );
}
