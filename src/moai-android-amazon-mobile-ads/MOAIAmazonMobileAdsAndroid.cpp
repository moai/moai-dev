// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <moai-android/JniUtils.h>
#include <moai-android-amazon-mobile-ads/MOAIAmazonMobileAdsAndroid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAmazonMobileAdsAndroid::_cacheInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAmazonMobileAdsAndroid, "" )
	
	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));
	
	jmethodID cacheInterstitial = self->GetStaticMethod ( "cacheInterstitial", "(Ljava/lang/String;)V" );
	self->CallStaticVoidMethod ( cacheInterstitial, ( jstring )jlocation );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAmazonMobileAdsAndroid::_hasCachedInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAmazonMobileAdsAndroid, "" )

	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID cacheInterstitial = self->GetStaticMethod ( "hasCachedInterstitial", "(Ljava/lang/String;)Z" );
	state.Push ( self->CallStaticBooleanMethod ( cacheInterstitial, ( jstring )jlocation ));
	return 1;
}

//----------------------------------------------------------------//
int MOAIAmazonMobileAdsAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAmazonMobileAdsAndroid, "" )
	
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAmazonMobileAdsAndroid::_init\n" );

	MOAIJString jappID			= self->GetJString ( state.GetValue < cc8* >( 1, 0 ));
	jmethodID init = self->GetStaticMethod ( "init", "(Ljava/lang/String;)V" );

	self->CallStaticVoidMethod ( init, ( jstring )jappID );
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAIAmazonMobileAdsAndroid: Done calling init\n" );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAmazonMobileAdsAndroid::_showInterstitial ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIAmazonMobileAdsAndroid, "" )

	MOAIJString jlocation = self->GetJString ( state.GetValue < cc8* >( 1, 0 ));

	jmethodID showInterstitial = self->GetStaticMethod ( "showInterstitial", "(Ljava/lang/String;)Z" );
	self->CallStaticBooleanMethod ( showInterstitial, ( jstring )jlocation );
	return 1;
}

//================================================================//
// MOAIAmazonMobileAdsAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIAmazonMobileAdsAndroid::MOAIAmazonMobileAdsAndroid () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
	 
	this->SetClass ( "com/ziplinegames/moai/MoaiAmazonMobileAds" );
}

//----------------------------------------------------------------//
MOAIAmazonMobileAdsAndroid::~MOAIAmazonMobileAdsAndroid () {
}

//----------------------------------------------------------------//
void MOAIAmazonMobileAdsAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "INTERSTITIAL_LOAD_FAILED",	( u32 )INTERSTITIAL_LOAD_FAILED );
	state.SetField ( -1, "INTERSTITIAL_DISMISSED", 		( u32 )INTERSTITIAL_DISMISSED );

	luaL_Reg regTable [] = {
		{ "cacheInterstitial",			_cacheInterstitial },
		{ "hasCachedInterstitial",		_hasCachedInterstitial },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIAmazonMobileAdsAndroid > },
		{ "init",						_init },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIAmazonMobileAdsAndroid > },
		{ "showInterstitial",			_showInterstitial },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// ChartBoost JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MOAIAmazonMobileAds_AKUInvokeListener ( JNIEnv* env, jclass obj, jint eventID ) {

	ZLLog::LogF ( ZLLog::CONSOLE, "Java_com_ziplinegames_moai_MOAIAmazonMobileAds_AKUInvokeListener\n" );
	MOAIAmazonMobileAdsAndroid::Get ().InvokeListener (( u32 )eventID );
}
