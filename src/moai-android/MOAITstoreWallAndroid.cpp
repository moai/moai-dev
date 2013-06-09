// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_TSTOREWALL

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAITstoreWallAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//
//----------------------------------------------------------------//
int MOAITstoreWallAndroid::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAITstoreWallAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showOfferWall
	@text	Displays the Tstore marketplace.
				
	@out	nil
*/
int MOAITstoreWallAndroid::_showOfferWall ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreWall" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreWallAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreWall" );
    } else {

    	jmethodID showOfferWall = env->GetStaticMethodID ( tstore, "showOfferWall", "()V" );
    	if ( showOfferWall == NULL ) {

			USLog::Print ( "MOAITstoreWallAndroid: Unable to find static java method %s", "showOfferWall" );
    	} else {

			env->CallStaticVoidMethod ( tstore, showOfferWall );				
		}
	}
		
	return 0;
}

//================================================================//
// MOAITstoreWallAndroid
//================================================================//

//----------------------------------------------------------------//
MOAITstoreWallAndroid::MOAITstoreWallAndroid () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAITstoreWallAndroid::~MOAITstoreWallAndroid () {

}

//----------------------------------------------------------------//
void MOAITstoreWallAndroid::NotifyCurrencyAwarded ( int amount ) {
	
	MOAILuaRef& callback = this->mListeners [ TSTORE_CURRENCY_AWARDED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushinteger ( state, amount );
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreWallAndroid::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "TSTORE_CURRENCY_AWARDED",	( u32 )TSTORE_CURRENCY_AWARDED );

	luaL_Reg regTable [] = {
		{ "setListener",		_setListener },
		{ "showOfferWall",		_showOfferWall },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// Tstore JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTstoreWall_AKUNotifyCurrencyAwarded ( JNIEnv* env, jclass obj, jint amount) {

	MOAITstoreWallAndroid::Get ().NotifyCurrencyAwarded ( amount );
}

#endif