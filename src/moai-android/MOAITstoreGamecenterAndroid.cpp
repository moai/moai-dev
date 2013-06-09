// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_TSTOREGAMECENTER

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAITstoreGamecenterAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	authTstore	
	@text	Authorizes the app throught Tstore
	
	@in		bool wantsLogin			
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_authTstore ( lua_State* L ) {
	
	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID authTstore = env->GetStaticMethodID ( tstore, "authTstore", "(Z)V" );
    	if ( authTstore == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "authTstore" );
    	} else {

			env->CallStaticVoidMethod ( tstore, authTstore, wantsLogin );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	checkTstoreInstalled	
	@text	Checks if the Tstore app is installed
	
	@out	bool installed
*/
int MOAITstoreGamecenterAndroid::_checkTstoreInstalled ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID checkTstoreInstalled = env->GetStaticMethodID ( tstore, "checkTstoreInstalled", "()Z" );
    	if ( checkTstoreInstalled == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "checkTstoreInstalled" );
    	} else {

			bool installed = env->CallStaticBooleanMethod ( tstore, checkTstoreInstalled );			
			lua_pushboolean ( state, installed );
			return 1;
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	disableGamecenter	
	@text	Disables Tstore Gamecenter
	
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_disableGamecenter ( lua_State* L ) {
	
	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID disableGamecenter = env->GetStaticMethodID ( tstore, "disableGamecenter", "()V" );
    	if ( disableGamecenter == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "disableGamecenter" );
    	} else {

			env->CallStaticVoidMethod ( tstore, disableGamecenter );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	enableGamecenter	
	@text	Enables Tstore Gamecenter
	
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_enableGamecenter ( lua_State* L ) {
	
	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID enableGamecenter = env->GetStaticMethodID ( tstore, "enableGamecenter", "()V" );
    	if ( enableGamecenter == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "enableGamecenter" );
    	} else {

			env->CallStaticVoidMethod ( tstore, enableGamecenter );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getRankList	
	@text	Gets the leaderboard list
	
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_getRankList ( lua_State* L ) {
	
	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID getRankList = env->GetStaticMethodID ( tstore, "getRankList", "()V" );
    	if ( getRankList == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "getRankList" );
    	} else {

			env->CallStaticVoidMethod ( tstore, getRankList );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	installGamecenter	
	@text	Installs the Tstore Gamecenter app
	
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_installGamecenter ( lua_State* L ) {
	
	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID installGamecenter = env->GetStaticMethodID ( tstore, "installGamecenter", "()V" );
    	if ( installGamecenter == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "installGamecenter" );
    	} else {

			env->CallStaticVoidMethod ( tstore, installGamecenter );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	installTstore	
	@text	Installs the Tstore app
	
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_installTstore ( lua_State* L ) {
	
	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID installTstore = env->GetStaticMethodID ( tstore, "installTstore", "()V" );
    	if ( installTstore == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "installTstore" );
    	} else {

			env->CallStaticVoidMethod ( tstore, installTstore );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
int MOAITstoreGamecenterAndroid::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAITstoreGamecenterAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPoint	
	@text	Records a point to the leaderboard
	
	@in 	string score  ( convert number to string in Lua )
	@in 	string pointName  ( score + "points" or score + "rubies" ) 
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_setPoint ( lua_State* L ) {
	
	MOAILuaState state ( L );
	cc8* score = lua_tostring ( state, 1 );
	cc8* name = lua_tostring ( state, 2 );

	JNI_GET_ENV ( jvm, env );
	JNI_GET_JSTRING ( name, jname )
	JNI_GET_JSTRING ( score, jscore )

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID setPoint = env->GetStaticMethodID ( tstore, "setPoint", "(Ljava/lang/String;Ljava/lang/String)V" );
    	if ( setPoint == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "setPoint" );
    	} else {

			env->CallStaticVoidMethod ( tstore, setPoint, jscore, jname );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	startGamecenter	
	@text	Starts the gamecenter app
	
	@out	int status - can be GAMECENTER_INSTALLED, GAMECENTER_UPGRADING, GAMECENTER_NOT_INSTALLED
*/
int MOAITstoreGamecenterAndroid::_startGamecenter ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
	
		USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID startGamecenter = env->GetStaticMethodID ( tstore, "startGamecenter", "()I" );
    	if ( startGamecenter == NULL ) {

			USLog::Print ( "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "startGamecenter" );
    	} else {

			int status = env->CallStaticBooleanMethod ( tstore, startGamecenter );			
			lua_pushnumber ( state, status );
			return 1;
		}
	}
		
	return 0;
}

//================================================================//
// MOAITstoreGamecenterAndroid
//================================================================//

//----------------------------------------------------------------//
MOAITstoreGamecenterAndroid::MOAITstoreGamecenterAndroid () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAITstoreGamecenterAndroid::~MOAITstoreGamecenterAndroid () {

}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::AKUNotifyAuthExitResponse () {
	
	MOAILuaRef& callback = this->mListeners [ AUTH_RESPONSE_EXITED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::AKUNotifyAuthSuccessResponse () {
	
	MOAILuaRef& callback = this->mListeners [ AUTH_RESPONSE_SUCCESS ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::AKUNotifyDisableSuccessResponse () {
	
	MOAILuaRef& callback = this->mListeners [ DISABLE_GAMECENTER_RESPONSE ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::AKUNotifyScoreListResponse ( cc8* jsonData ) {
	
	MOAILuaRef& callback = this->mListeners [ GET_RANKLIST_RESPONSE ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushstring ( state, jsonData );
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "AUTH_RESPONSE_SUCCESS",		( u32 )AUTH_RESPONSE_SUCCESS );
	state.SetField ( -1, "AUTH_RESPONSE_EXITED",		( u32 )AUTH_RESPONSE_EXITED );
	state.SetField ( -1, "DISABLE_GAMECENTER_RESPONSE",	( u32 )DISABLE_GAMECENTER_RESPONSE );
	state.SetField ( -1, "GET_RANKLIST_RESPONSE",		( u32 )GET_RANKLIST_RESPONSE );
	state.SetField ( -1, "SET_POINT_RESPONSE",			( u32 )SET_POINT_RESPONSE );
		
	state.SetField ( -1, "GAMECENTER_INSTALLED",		( u32 )GAMECENTER_INSTALLED );
	state.SetField ( -1, "GAMECENTER_UPGRADING",		( u32 )GAMECENTER_UPGRADING );	
	state.SetField ( -1, "GAMECENTER_NOT_INSTALLED",	( u32 )GAMECENTER_NOT_INSTALLED );

	luaL_Reg regTable [] = {
		{ "authTstore",					_authTstore	},
		{ "checkTstoreInstalled",		_checkTstoreInstalled },
		{ "disableGamecenter",  	    _disableGamecenter },	
		{ "enableGamecenter",      		_enableGamecenter },	
		{ "getRankList",     			_getRankList },		
		{ "installGamecenter",      	_installGamecenter },	
		{ "installTstore",      		_installTstore },	
		{ "setListener",      			_setListener },		
		{ "setPoint",      				_setPoint },  			
		{ "startGamecenter",      		_startGamecenter },	
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// Tstore JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyAuthExitResponse ( JNIEnv* env, jclass obj ) {

	MOAITstoreGamecenterAndroid::Get ().AKUNotifyAuthExitResponse ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyAuthSuccessResponse ( JNIEnv* env, jclass obj ) {

	MOAITstoreGamecenterAndroid::Get ().AKUNotifyAuthSuccessResponse ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyDisableSuccessResponse ( JNIEnv* env, jclass obj ) {

	MOAITstoreGamecenterAndroid::Get ().AKUNotifyDisableSuccessResponse ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyScoreListResponse ( JNIEnv* env, jclass obj, jstring jjsonData ) {

	JNI_GET_CSTRING ( jjsonData, jsonData );
	
	MOAITstoreGamecenterAndroid::Get ().AKUNotifyScoreListResponse ( jsonData );
	
	JNI_RELEASE_CSTRING ( jjsonData, jsonData );
}

#endif