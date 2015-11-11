// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/JniUtils.h>
#include <moai-android-facebook/MOAIFacebookAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getUserID
	@text	Retrieve the Facebook user ID

	@out	string	ID
*/
int MOAIFacebookAndroid::_getUserID ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	MOAIJString jID = ( jstring )self->CallStaticObjectMethod ( self->mJava_GetUserID );
	cc8* id = self->GetCString ( jID );
	lua_pushstring ( state, id );
	self->ReleaseCString ( jID, id );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getUserName
	@text	Retrieve the Facebook user name

	@out	string	name
*/
int MOAIFacebookAndroid::_getUserName ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	MOAIJString jName = ( jstring )self->CallStaticObjectMethod ( self->mJava_GetUserName );
	cc8* name = self->GetCString ( jName );
	lua_pushstring ( state, name );
	self->ReleaseCString ( jName, name );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getToken
	@text	Retrieve the Facebook login token.
				
	@out	string	token
*/
int MOAIFacebookAndroid::_getToken ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	MOAIJString jtoken = ( jstring )self->CallStaticObjectMethod ( self->mJava_GetToken );
	cc8* token = self->GetCString ( jtoken );
	lua_pushstring ( state, token );
	self->ReleaseCString ( jtoken, token );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getTokenExpireTime
	@text	Retrieve the Facebook login token.

	@out	string	token
*/
int MOAIFacebookAndroid::_getTokenExpireTime ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	MOAIJString jtoken = ( jstring )self->CallStaticObjectMethod ( self->mJava_GetTokenExpireTime );
	cc8* token = self->GetCString ( jtoken );
	lua_pushstring ( state, token );
	self->ReleaseCString ( jtoken, token );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getTokenRefreshTime
	@text	Retrieve the Facebook login token.

	@out	string	token
*/
int MOAIFacebookAndroid::_getTokenRefreshTime ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	MOAIJString jtoken = ( jstring )self->CallStaticObjectMethod ( self->mJava_GetTokenRefreshTime );
	cc8* token = self->GetCString ( jtoken );
	lua_pushstring ( state, token );
	self->ReleaseCString ( jtoken, token );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	graphRequest
    @text	Make a request on Facebook's Graph API

	@in		string  path
	@opt	table  parameters
    @out	nil
*/
int MOAIFacebookAndroid::_graphRequest ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	//MOAIJString jpath = self->GetJString ( lua_tostring ( state, 1 ));

    //jobject bundle;
    //if ( state.IsType ( 2, LUA_TTABLE ) ) {
    //    bundle = self->BundleFromLua( L, 2 );
    //}

	//self->CallStaticVoidMethod ( self->mJava_GraphRequest, jpath, bundle );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize Facebook.
				
	@in		string	appId			Available in Facebook developer settings.
	@out 	nil
*/
int MOAIFacebookAndroid::_init ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	MOAIJString jidentifier = self->GetJString ( lua_tostring ( state, 1 ));
	self->CallStaticVoidMethod ( self->mJava_Init, ( jstring )jidentifier );		
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	login
	@text	Prompt the user to login to Facebook.
				
	@opt	table	permissions			Optional set of required permissions. See Facebook documentation for a full list. Default is nil.
	@out 	nil
*/
int MOAIFacebookAndroid::_login ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	jobjectArray jpermissions 		= NULL;
	MOAIJString  prevToken			= NULL;
	MOAIJString  fbId				= NULL;
	MOAIJString  tokenExpireTime	= NULL;
	MOAIJString  tokenRefreshTime	= NULL;
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
        jpermissions = self->StringArrayFromLua ( L, 1 );
	}

	if ( state.IsType ( 2, LUA_TSTRING )) {
		prevToken = self->GetJString ( lua_tostring ( state, 2 ));
	}

	if ( state.IsType ( 3, LUA_TSTRING )) {
		fbId = self->GetJString ( lua_tostring ( state, 3 ));
	}

	if ( state.IsType ( 4, LUA_TSTRING )) {
		tokenExpireTime = self->GetJString ( lua_tostring ( state, 4 ));
	}

	if ( state.IsType ( 5, LUA_TSTRING )) {
		tokenRefreshTime = self->GetJString ( lua_tostring ( state, 5 ));
	}

	if ( jpermissions == NULL ) {
		jpermissions = self->Env ()->NewObjectArray ( 0, self->Env ()->FindClass( "java/lang/String" ), 0 );
	}

	self->CallStaticVoidMethod ( self->mJava_Login, jpermissions, ( jstring ) prevToken, ( jstring ) fbId, ( jstring ) tokenExpireTime, ( jstring ) tokenRefreshTime );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	logout
	@text	Log the user out of Facebook.
				
	@out 	nil
*/
int MOAIFacebookAndroid::_logout ( lua_State *L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	self->CallStaticVoidMethod ( self->mJava_Logout );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	postToFeed
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
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	MOAIJString jlink			= self->GetJString ( lua_tostring ( state, 1 ));
	MOAIJString jpicture		= self->GetJString ( lua_tostring ( state, 2 ));
	MOAIJString jname			= self->GetJString ( lua_tostring ( state, 3 ));
	MOAIJString jcaption		= self->GetJString ( lua_tostring ( state, 4 ));
	MOAIJString jdescription	= self->GetJString ( lua_tostring ( state, 5 ));
	MOAIJString jmessage		= self->GetJString ( lua_tostring ( state, 6 ));
	
	self->CallStaticVoidMethod ( self->mJava_PostToFeed, ( jstring )jlink, ( jstring )jpicture, ( jstring )jname, ( jstring )jcaption, ( jstring )jdescription, ( jstring )jmessage );	
		
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFacebookAndroid::_restoreSession ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	lua_pushboolean ( state, self->CallStaticBooleanMethod ( self->mJava_RestoreSession ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	sendRequest
	@text	Send an app request to the logged in users' friends.
				
	@opt	string	message			The message for the request. See Facebook documentation. Default is nil.
	@out 	nil
*/
int MOAIFacebookAndroid::_sendRequest ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )
	
	MOAIJString jmessage = self->GetJString ( lua_tostring ( state, 1 ));
	self->CallStaticVoidMethod ( self->mJava_SendRequest, ( jstring )jmessage );		
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	sessionValid
	@text	Determine whether or not the current Facebook session is valid.
				
	@out 	boolean	valid
*/
int MOAIFacebookAndroid::_sessionValid ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	lua_pushboolean ( state, self->CallStaticBooleanMethod ( self->mJava_IsSessionValid ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	showInviteDialog
	@text	Displays Facebook invite dialog.

	@out	void
*/
int MOAIFacebookAndroid::_showInviteDialog ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIFacebookAndroid, "" )

	MOAIJString linkURL = self->GetJString ( lua_tostring ( state, 1 ));
	MOAIJString imgURL = self->GetJString ( lua_tostring ( state, 2 ));
	self->CallStaticVoidMethod ( self->mJava_ShowInviteDialog, ( jstring )linkURL, ( jstring )imgURL );
	return 0;
}

//================================================================//
// MOAIFacebookAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIFacebookAndroid::MOAIFacebookAndroid () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
		
	this->SetClass ( "com/moaisdk/facebook/MoaiFacebook" );

	this->mJava_GetToken			= this->GetStaticMethod ( "getToken", "()Ljava/lang/String;" );
	this->mJava_GetTokenExpireTime	= this->GetStaticMethod ( "getTokenExpireTime", "()Ljava/lang/String;" );
	this->mJava_GetTokenRefreshTime	= this->GetStaticMethod ( "getTokenRefreshTime", "()Ljava/lang/String;" );
	this->mJava_GetUserID			= this->GetStaticMethod ( "getUserID", "()Ljava/lang/String;" );
	this->mJava_GetUserName			= this->GetStaticMethod ( "getUserName", "()Ljava/lang/String;" );
	//this->mJava_GraphRequest		= this->GetStaticMethod ( "graphRequest", "(Ljava/lang/String;Landroid/os/Bundle;)V" );
	this->mJava_Init				= this->GetStaticMethod ( "init", "(Ljava/lang/String;)V" );
	this->mJava_IsSessionValid		= this->GetStaticMethod ( "isSessionValid", "()Z" );
	this->mJava_Login				= this->GetStaticMethod ( "login", "([Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
	this->mJava_Logout				= this->GetStaticMethod ( "logout", "()V" );
	this->mJava_PostToFeed			= this->GetStaticMethod ( "postToFeed", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
	this->mJava_RestoreSession		= this->GetStaticMethod ( "restoreSession", "()Z" );
	this->mJava_SendRequest			= this->GetStaticMethod ( "sendRequest", "(Ljava/lang/String;)V" );
	this->mJava_ShowInviteDialog	= this->GetStaticMethod ( "showInviteDialog", "(Ljava/lang/String;Ljava/lang/String;)V" );
}

//----------------------------------------------------------------//
MOAIFacebookAndroid::~MOAIFacebookAndroid () {

}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "LOGIN_DISMISSED",	( u32 ) LOGIN_DISMISSED );
	state.SetField ( -1, "LOGIN_SUCCESS",	( u32 ) LOGIN_SUCCESS );
	state.SetField ( -1, "LOGIN_ERROR", 	( u32 ) LOGIN_ERROR );

	luaL_Reg regTable [] = {
		{ "getListener",			&MOAIGlobalEventSource::_getListener < MOAIFacebookAndroid > },
		{ "getUserID",				_getUserID },
		{ "getUserName",			_getUserName },
		{ "getToken",				_getToken },
		{ "getTokenExpireTime",		_getTokenExpireTime },
		{ "getTokenRefreshTime",	_getTokenRefreshTime },
		{ "graphRequest",			_graphRequest },
		{ "init",					_init },
		{ "login",					_login },
		{ "logout",					_logout },
		{ "postToFeed",				_postToFeed },
		{ "restoreSession",			_restoreSession },
		{ "sendRequest",			_sendRequest },
		{ "sessionValid",			_sessionValid },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIFacebookAndroid > },
		{ "showInviteDialog",		_showInviteDialog },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyLoginSuccess () {

	this->InvokeListener ( LOGIN_SUCCESS );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyLoginDismissed () {

	this->InvokeListener ( LOGIN_DISMISSED );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyLoginError () {

	this->InvokeListener ( LOGIN_ERROR );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyRequestComplete ( cc8* response ) {

	//MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	//if ( this->PushListener ( REQUEST_RESPONSE, state )) {
	//	lua_pushstring ( state, response );
	//	state.DebugCall ( 1, 0 );
	//}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyRequestFailed () {

	//this->InvokeListener ( REQUEST_RESPONSE_FAILED );
}

//================================================================//
// Facebook JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_moaisdk_facebook_MoaiFacebook_AKUNotifyFacebookLoginSuccess ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().NotifyLoginSuccess ();
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_moaisdk_facebook_MoaiFacebook_AKUNotifyFacebookLoginError ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().NotifyLoginError ();
}

extern "C" JNIEXPORT void JNICALL Java_com_moaisdk_facebook_MoaiFacebook_AKUNotifyFacebookLoginDismissed ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().NotifyLoginDismissed ();
}

extern "C" JNIEXPORT void JNICALL Java_com_moaisdk_facebook_MoaiFacebook_AKUNotifyFacebookRequestFailed ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().NotifyRequestFailed ( );
}

