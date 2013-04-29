// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_FACEBOOK

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIFacebookAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* MOAIFacebookAndroid::_luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}

//----------------------------------------------------------------//
/**	@name	extendToken
	@text	Extend the token if needed
				
	@out	nil
*/
int MOAIFacebookAndroid::_extendToken ( lua_State* L ) {
	
	JNI_GET_ENV ( jvm, env );
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID extendToken = env->GetStaticMethodID ( facebook, "extendToken", "()V" );
   		if ( extendToken == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "extendToken" );
		} else {
	
			env->CallStaticVoidMethod ( facebook, extendToken );
			
			return 0;
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getExpirationDate
	@text	Retrieve the Facebook login token expiration date.
 
	@in		nil
	@out	string	token expiration date
 */
int	MOAIFacebookAndroid::_getExpirationDate	( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID getExpirationDate = env->GetStaticMethodID ( facebook, "getExpirationDate", "()J" );
   		if ( getExpirationDate == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "getExpirationDate" );
		} else {
	
			jlong jexpires = env->CallStaticLongMethod ( facebook, getExpirationDate );			
			lua_pushnumber ( state, jexpires );
						
			return 1;
		}
	}
	
	lua_pushnil ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getToken
	@text	Retrieve the Facebook login token.
				
	@out	string	token
*/
int MOAIFacebookAndroid::_getToken ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID getToken = env->GetStaticMethodID ( facebook, "getToken", "()Ljava/lang/String;" );
   		if ( getToken == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "getToken" );
		} else {
	
			jstring jtoken = ( jstring )env->CallStaticObjectMethod ( facebook, getToken );
			
			JNI_GET_CSTRING ( jtoken, token );

			lua_pushstring ( state, token );
			
			JNI_RELEASE_CSTRING ( jtoken, token );
			
			return 1;
		}
	}
	
	lua_pushnil ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	graphRequest
	@text	Performs a requset on the Facebook Graph API

	@in		string  path
	@out	string	token
*/
int MOAIFacebookAndroid::_graphRequest ( lua_State* L ) {
	
	MOAILuaState state ( L );
	cc8* path = lua_tostring ( state, 1 );
		
	JNI_GET_ENV ( jvm, env );	
	JNI_GET_JSTRING ( path, jpath )
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID graphRequest = env->GetStaticMethodID ( facebook, "graphRequest", "(Ljava/lang/String;)Ljava/lang/String;" );
   		if ( graphRequest == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "graphRequest" );
		} else {
	
			jstring jresult = ( jstring )env->CallStaticObjectMethod ( facebook, graphRequest, jpath );
			
			JNI_GET_CSTRING ( jresult, result );

			lua_pushstring ( state, result );
			
			JNI_RELEASE_CSTRING ( jresult, result );
			
			return 1;
		}
	}
	
	lua_pushnil ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Facebook.
				
	@in		string	appId			Available in Facebook developer settings.
	@out 	nil
*/
int MOAIFacebookAndroid::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* identifier = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( identifier, jidentifier );
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID init = env->GetStaticMethodID ( facebook, "init", "(Ljava/lang/String;)V" );
   		if ( init == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "init" );
		} else {
	
			env->CallStaticVoidMethod ( facebook, init, jidentifier );		
		}
	}
	
	return 0;
}
		
//----------------------------------------------------------------//
/**	@name	login
	@text	Prompt the user to login to Facebook.
				
	@opt	table	permissions			Optional set of required permissions. See Facebook documentation for a full list. Default is nil.
	@out 	nil
*/
int MOAIFacebookAndroid::_login ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jobjectArray jpermissions = NULL;
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
	
		int numEntries = 0;
		for ( int key = 1; ; ++key ) {
	
			state.GetField ( 1, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );
	
			if ( !value ) {
				
				numEntries = key - 1;
				break;
			}
		}
	
		jpermissions = env->NewObjectArray ( numEntries, env->FindClass( "java/lang/String" ), 0 );
		for ( int key = 1; ; ++key ) {
	
			state.GetField ( 1, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );
	
			if ( value ) {
				
				JNI_GET_JSTRING ( value, jvalue );
				env->SetObjectArrayElement ( jpermissions, key - 1, jvalue );
			}
			else {
				
				break;
			}	
		}
	}
	
	if ( jpermissions == NULL ) {
		
		jpermissions = env->NewObjectArray ( 0, env->FindClass( "java/lang/String" ), 0 );
	}
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {

		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {

    	jmethodID login = env->GetStaticMethodID ( facebook, "login", "([Ljava/lang/String;)V" );
    	if ( login == NULL ) {

			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "login" );
    	} else {

			env->CallStaticVoidMethod ( facebook, login, jpermissions );				
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	logout
	@text	Log the user out of Facebook.
				
	@out 	nil
*/
int MOAIFacebookAndroid::_logout ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID logout = env->GetStaticMethodID ( facebook, "logout", "()V" );
   		if ( logout == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "logout" );
		} else {
	
			env->CallStaticVoidMethod ( facebook, logout );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	postToFeed
	@text	Post a message to the logged in users' news feed.
				
	@in		string	link			The URL that the post links to. See Facebook documentation.
	@in		string	picture			The URL of an image to include in the post. See Facebook documentation.
	@in		string	name			The name of the link. See Facebook documentation.
	@in		string	caption			The caption of the link. See Facebook documentation.
	@in		string	description		The description of the link. See Facebook documentation.
	@in		string	message			The message for the post. See Facebook documentation.
	@out 	nil
*/
int MOAIFacebookAndroid::_postToFeed ( lua_State* L ) {

	MOAILuaState state ( L );
	
	cc8* link = lua_tostring ( state, 1 );
	cc8* picture = lua_tostring ( state, 2 );
	cc8* name = lua_tostring ( state, 3 );
	cc8* caption = lua_tostring ( state, 4 );
	cc8* description = lua_tostring ( state, 5 );
	cc8* message = lua_tostring ( state, 6 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( link, jlink );
	JNI_GET_JSTRING ( picture, jpicture );
	JNI_GET_JSTRING ( name, jname );
	JNI_GET_JSTRING ( caption, jcaption );
	JNI_GET_JSTRING ( description, jdescription );
	JNI_GET_JSTRING ( message, jmessage );
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID postToFeed = env->GetStaticMethodID ( facebook, "postToFeed", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
   		if ( postToFeed == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "postToFeed" );
		} else {
	
			env->CallStaticVoidMethod ( facebook, postToFeed, jlink, jpicture, jname, jcaption, jdescription, jmessage );	
		}
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	sendRequest
	@text	Send an app request to the logged in users' friends.
				
	@opt	string	message			The message for the request. See Facebook documentation. Default is nil.
	@out 	nil
*/
int MOAIFacebookAndroid::_sendRequest ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* message = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( message, jmessage );
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID sendRequest = env->GetStaticMethodID ( facebook, "sendRequest", "(Ljava/lang/String;)V" );
   		if ( sendRequest == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "sendRequest" );
		} else {
	
			env->CallStaticVoidMethod ( facebook, sendRequest, jmessage );		
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	sessionValid
	@text	Determine whether or not the current Facebook session is valid.
				
	@out 	boolean	valid
*/
int MOAIFacebookAndroid::_sessionValid ( lua_State* L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {

		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {

    	jmethodID isSessionValid = env->GetStaticMethodID ( facebook, "isSessionValid", "()Z" );
    	if ( isSessionValid == NULL ) {

			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "isSessionValid" );
    	} else {

			jboolean jvalid = ( jboolean )env->CallStaticBooleanMethod ( facebook, isSessionValid );	

			lua_pushboolean ( state, jvalid );

			return 1;
		}
	}

	lua_pushboolean ( state, false );

	return 1;
}

//----------------------------------------------------------------//
int MOAIFacebookAndroid::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAIFacebookAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setExpirationDate
	@text	Set the Facebook login token expiration date.
 
	@in		string	expirationDate			The login token expiration date. See Facebook documentation.
	@out 	nil
 */
int MOAIFacebookAndroid::_setExpirationDate ( lua_State* L ) {

	MOAILuaState state ( L );
	
	jlong expires = lua_tonumber ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
		
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID setExpirationDate = env->GetStaticMethodID ( facebook, "setExpirationDate", "(J)V" );
   		if ( setExpirationDate == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "setExpirationDate" );
		} else {
	
			env->CallStaticVoidMethod ( facebook, setExpirationDate, expires );		
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setToken
	@text	Set the Facebook login token.
			
	@in		string	token			The login token. See Facebook documentation.
	@out 	nil
*/
int MOAIFacebookAndroid::_setToken ( lua_State* L ) {

	MOAILuaState state ( L );
	
	cc8* token = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( token, jtoken );
	
	jclass facebook = env->FindClass ( "com/ziplinegames/moai/MoaiFacebook" );
    if ( facebook == NULL ) {
	
		USLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFacebook" );
    } else {
	
    	jmethodID setToken = env->GetStaticMethodID ( facebook, "setToken", "(Ljava/lang/String;)V" );
   		if ( setToken == NULL ) {
	
			USLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "setToken" );
		} else {
	
			env->CallStaticVoidMethod ( facebook, setToken, jtoken );		
		}
	}
	
	return 0;
}

//================================================================//
// MOAIFacebookAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIFacebookAndroid::MOAIFacebookAndroid () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIFacebookAndroid::~MOAIFacebookAndroid () {

}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DIALOG_DID_COMPLETE",		( u32 ) DIALOG_DID_COMPLETE );
	state.SetField ( -1, "DIALOG_DID_NOT_COMPLETE",	( u32 ) DIALOG_DID_NOT_COMPLETE );
	state.SetField ( -1, "SESSION_DID_LOGIN",		( u32 ) SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN",	( u32 ) SESSION_DID_NOT_LOGIN );
	
	luaL_Reg regTable [] = {
		{ "getExpirationDate", _getExpirationDate },	
		{ "getToken",		_getToken },
		{ "graphRequest",	_graphRequest },
		{ "init",			_init },
		{ "login",			_login },
		{ "logout",			_logout },
		{ "postToFeed",		_postToFeed },
		{ "sendRequest",	_sendRequest },
		{ "sessionValid",	_sessionValid },
		{ "setListener",	_setListener },
		{ "setToken",		_setToken },
		{ "extendToken",	_extendToken },
		{ "setExpirationDate", _setExpirationDate },		
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyLoginComplete ( int code ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_DID_NOT_LOGIN ];
	if ( code == DIALOG_RESULT_SUCCESS ) {
		
		callback = this->mListeners [ SESSION_DID_LOGIN ];
	}

	if ( callback ) {
	
		MOAILuaStateHandle state = callback.GetSelf ();
	
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyDialogComplete ( int code ) {
	
	MOAILuaRef& callback = this->mListeners [ DIALOG_DID_NOT_COMPLETE ];
	if ( code == DIALOG_RESULT_SUCCESS ) {
		
		callback = this->mListeners [ DIALOG_DID_COMPLETE ];
	}

	if ( callback ) {
	
		MOAILuaStateHandle state = callback.GetSelf ();
	
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// Facebook JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFacebookLoginComplete ( JNIEnv* env, jclass obj, jint code ) {

	MOAIFacebookAndroid::Get ().NotifyLoginComplete ( code );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFacebookDialogComplete ( JNIEnv* env, jclass obj, jint code ) {

	MOAIFacebookAndroid::Get ().NotifyDialogComplete ( code );
}

#endif