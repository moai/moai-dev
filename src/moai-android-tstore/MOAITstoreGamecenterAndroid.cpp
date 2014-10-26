// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android-tstore/MOAITstoreGamecenterAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	authTstore	
	@text	Authorizes the app through Tstore
	
	@in		boolean wantsLogin			
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_authTstore ( lua_State* L ) {

	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID authTstore = env->GetStaticMethodID ( tstore, "authTstore", "(Z)V" );
    	if ( authTstore == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "authTstore" );
    	} else {

			env->CallStaticVoidMethod ( tstore, authTstore, wantsLogin );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	checkTstoreInstalled
	@text	Checks if the Tstore app is installed

	@out	boolean installed
*/
int MOAITstoreGamecenterAndroid::_checkTstoreInstalled ( lua_State* L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID checkTstoreInstalled = env->GetStaticMethodID ( tstore, "checkTstoreInstalled", "()Z" );
    	if ( checkTstoreInstalled == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "checkTstoreInstalled" );
    	} else {

			bool installed = env->CallStaticBooleanMethod ( tstore, checkTstoreInstalled );
			lua_pushboolean ( state, installed );
			return 1;
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	disableGamecenter
	@text	Disables Tstore Gamecenter

	@out	nil
*/
int MOAITstoreGamecenterAndroid::_disableGamecenter ( lua_State* L ) {

	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID disableGamecenter = env->GetStaticMethodID ( tstore, "disableGamecenter", "()V" );
    	if ( disableGamecenter == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "disableGamecenter" );
    	} else {

			env->CallStaticVoidMethod ( tstore, disableGamecenter );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	enableGamecenter
	@text	Enables Tstore Gamecenter

	@out	nil
*/
int MOAITstoreGamecenterAndroid::_enableGamecenter ( lua_State* L ) {

	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID enableGamecenter = env->GetStaticMethodID ( tstore, "enableGamecenter", "()V" );
    	if ( enableGamecenter == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "enableGamecenter" );
    	} else {

			env->CallStaticVoidMethod ( tstore, enableGamecenter );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getRankList
	@text	Gets the leaderboard list

	@out	nil
*/
int MOAITstoreGamecenterAndroid::_getRankList ( lua_State* L ) {

	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID getRankList = env->GetStaticMethodID ( tstore, "getRankList", "()V" );
    	if ( getRankList == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "getRankList" );
    	} else {

			env->CallStaticVoidMethod ( tstore, getRankList );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getUserInfo
	@text	Gets the userinfo

	@out	nil
*/
int MOAITstoreGamecenterAndroid::_getUserInfo ( lua_State* L ) {

	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
		
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID getUserInfo = env->GetStaticMethodID ( tstore, "getUserInfo", "()V" );
    	if ( getUserInfo == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "getUserInfo" );
    	} else {

			env->CallStaticVoidMethod ( tstore, getUserInfo );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	installGamecenter
	@text	Installs the Tstore Gamecenter app

	@out	nil
*/
int MOAITstoreGamecenterAndroid::_installGamecenter ( lua_State* L ) {

	MOAILuaState state ( L );
	bool wantsLogin = lua_toboolean ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID installGamecenter = env->GetStaticMethodID ( tstore, "installGamecenter", "()V" );
    	if ( installGamecenter == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "installGamecenter" );
    	} else {

			env->CallStaticVoidMethod ( tstore, installGamecenter );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	installTstore
	@text	Installs the Tstore app

	@out	nil
*/
int MOAITstoreGamecenterAndroid::_installTstore ( lua_State* L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID installTstore = env->GetStaticMethodID ( tstore, "installTstore", "()V" );
    	if ( installTstore == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "installTstore" );
    	} else {

			env->CallStaticVoidMethod ( tstore, installTstore );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	invokeTstoreJoinPage
	@text	Invokes the Tstore join page

	@out	nil
*/
int MOAITstoreGamecenterAndroid::_invokeTstoreJoinPage ( lua_State* L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
		
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID invokeTstoreJoinPage = env->GetStaticMethodID ( tstore, "invokeTstoreJoinPage", "()V" );
    	if ( invokeTstoreJoinPage == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "invokeTstoreJoinPage" );
    	} else {

			env->CallStaticVoidMethod ( tstore, invokeTstoreJoinPage );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openGallery
	@text	opens gallery for profile pic selection

	@out	nil
*/
int MOAITstoreGamecenterAndroid::_openGallery ( lua_State* L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
		
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID invokeGallery = env->GetStaticMethodID ( tstore, "invokeGallery", "()V" );
    	if ( invokeGallery == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "invokeGallery" );
    	} else {

			env->CallStaticVoidMethod ( tstore, invokeGallery );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
int MOAITstoreGamecenterAndroid::_setListener ( lua_State* L ) {

	MOAILuaState state ( L );

	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		MOAITstoreGamecenterAndroid::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPoint
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

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID setPoint = env->GetStaticMethodID ( tstore, "setPoint", "(Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( setPoint == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "setPoint" );
    	} else {

			env->CallStaticVoidMethod ( tstore, setPoint, jscore, jname );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUserInfo
	@text	Records a new user nickname

	@in 	string nickname
	@out	nil
*/
int MOAITstoreGamecenterAndroid::_setUserInfo ( lua_State* L ) {

	MOAILuaState state ( L );
	cc8* name = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	JNI_GET_JSTRING ( name, jname )

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {
		
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID setUserInfo = env->GetStaticMethodID ( tstore, "setUserInfo", "(Ljava/lang/String;)V" );
    	if ( setUserInfo == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "setUserInfo" );
    	} else {

			env->CallStaticVoidMethod ( tstore, setUserInfo, jname );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	startGamecenter
	@text	Starts the gamecenter app
	
	@out	number status		Can be GAMECENTER_INSTALLED, GAMECENTER_UPGRADING, GAMECENTER_NOT_INSTALLED
*/
int MOAITstoreGamecenterAndroid::_startGamecenter ( lua_State* L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass tstore = env->FindClass ( "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    if ( tstore == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTstoreGamecenter" );
    } else {

    	jmethodID startGamecenter = env->GetStaticMethodID ( tstore, "startGamecenter", "()I" );
    	if ( startGamecenter == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITstoreGamecenterAndroid: Unable to find static java method %s", "startGamecenter" );
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

		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::AKUNotifyAuthSuccessResponse () {

	MOAILuaRef& callback = this->mListeners [ AUTH_RESPONSE_SUCCESS ];

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::AKUNotifyDisableSuccessResponse () {

	MOAILuaRef& callback = this->mListeners [ DISABLE_GAMECENTER_RESPONSE ];

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::AKUNotifyScoreListResponse ( cc8* jsonData ) {

	MOAILuaRef& callback = this->mListeners [ GET_RANKLIST_RESPONSE ];

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();

		lua_pushstring ( state, jsonData );

		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITstoreGamecenterAndroid::AKUNotifyUserInfoResponse ( cc8* jsonData ) {

	MOAILuaRef& callback = this->mListeners [ GET_USERINFO_RESPONSE ];

	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
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
	state.SetField ( -1, "GET_USERINFO_RESPONSE",		( u32 )GET_USERINFO_RESPONSE );

	state.SetField ( -1, "GAMECENTER_INSTALLED",		( u32 )GAMECENTER_INSTALLED );
	state.SetField ( -1, "GAMECENTER_UPGRADING",		( u32 )GAMECENTER_UPGRADING );
	state.SetField ( -1, "GAMECENTER_NOT_INSTALLED",	( u32 )GAMECENTER_NOT_INSTALLED );

	luaL_Reg regTable [] = {
		{ "openGallery",				_openGallery },
		{ "getUserInfo",				_getUserInfo },
		{ "setUserInfo",				_setUserInfo },
		{ "authTstore",					_authTstore	},
		{ "checkTstoreInstalled",		_checkTstoreInstalled },
		{ "disableGamecenter",  	    _disableGamecenter },
		{ "enableGamecenter",      		_enableGamecenter },
		{ "getRankList",     			_getRankList },
		{ "installGamecenter",      	_installGamecenter },
		{ "installTstore",      		_installTstore },
		{ "invokeTstoreJoinPage",  		_invokeTstoreJoinPage },
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
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyAuthExitResponse ( JNIEnv* env, jclass obj ) {

	MOAITstoreGamecenterAndroid::Get ().AKUNotifyAuthExitResponse ();
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyAuthSuccessResponse ( JNIEnv* env, jclass obj ) {

	MOAITstoreGamecenterAndroid::Get ().AKUNotifyAuthSuccessResponse ();
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyDisableSuccessResponse ( JNIEnv* env, jclass obj ) {

	MOAITstoreGamecenterAndroid::Get ().AKUNotifyDisableSuccessResponse ();
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyScoreListResponse ( JNIEnv* env, jclass obj, jstring jjsonData ) {

	JNI_GET_CSTRING ( jjsonData, jsonData );

	MOAITstoreGamecenterAndroid::Get ().AKUNotifyScoreListResponse ( jsonData );

	JNI_RELEASE_CSTRING ( jjsonData, jsonData );
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTstoreGamecenter_AKUNotifyUserInfoResponse ( JNIEnv* env, jclass obj, jstring jjsonData ) {

	JNI_GET_CSTRING ( jjsonData, jsonData );

	MOAITstoreGamecenterAndroid::Get ().AKUNotifyUserInfoResponse ( jsonData );

	JNI_RELEASE_CSTRING ( jjsonData, jsonData );
}
