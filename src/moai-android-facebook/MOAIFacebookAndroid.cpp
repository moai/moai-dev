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
	
	jobjectArray jpermissions = NULL;
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
        jpermissions = self->StringArrayFromLua ( L, 1 );
	}
	
	if ( jpermissions == NULL ) {
		jpermissions = self->Env ()->NewObjectArray ( 0, self->Env ()->FindClass( "java/lang/String" ), 0 );
	}

	self->CallStaticVoidMethod ( self->mJava_Login, jpermissions );				
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

//================================================================//
// MOAIFacebookAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIFacebookAndroid::MOAIFacebookAndroid () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
		
	this->SetClass ( "com/ziplinegames/moai/MoaiFacebook" );
	
	this->mJava_GetToken			= this->GetStaticMethod ( "getToken", "()Ljava/lang/String;" );
	//this->mJava_GraphRequest		= this->GetStaticMethod ( "graphRequest", "(Ljava/lang/String;Landroid/os/Bundle;)V" );
	this->mJava_Init				= this->GetStaticMethod ( "init", "(Ljava/lang/String;)V" );
	this->mJava_IsSessionValid		= this->GetStaticMethod ( "isSessionValid", "()Z" );
	this->mJava_Login				= this->GetStaticMethod ( "login", "([Ljava/lang/String;)V" );
	this->mJava_Logout				= this->GetStaticMethod ( "logout", "()V" );
	this->mJava_PostToFeed			= this->GetStaticMethod ( "postToFeed", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
	this->mJava_RestoreSession		= this->GetStaticMethod ( "restoreSession", "()Z" );
	this->mJava_SendRequest			= this->GetStaticMethod ( "sendRequest", "(Ljava/lang/String;)V" );
}

//----------------------------------------------------------------//
MOAIFacebookAndroid::~MOAIFacebookAndroid () {

}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DIALOG_DID_COMPLETE",		( u32 ) DIALOG_DID_COMPLETE );
	state.SetField ( -1, "DIALOG_DID_NOT_COMPLETE",	( u32 ) DIALOG_DID_NOT_COMPLETE );
	state.SetField ( -1, "REQUEST_RESPONSE", 		( u32 ) REQUEST_RESPONSE );
	state.SetField ( -1, "REQUEST_RESPONSE_FAILED", ( u32 ) REQUEST_RESPONSE_FAILED );
	state.SetField ( -1, "SESSION_DID_LOGIN",		( u32 ) SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN",	( u32 ) SESSION_DID_NOT_LOGIN );

	luaL_Reg regTable [] = {
		{ "getListener",			&MOAIGlobalEventSource::_getListener < MOAIFacebookAndroid > },
		{ "getToken",				_getToken },
		{ "graphRequest",			_graphRequest },
		{ "init",					_init },
		{ "login",					_login },
		{ "logout",					_logout },
		{ "postToFeed",				_postToFeed },
		{ "restoreSession",			_restoreSession },
		{ "sendRequest",			_sendRequest },
		{ "sessionValid",			_sessionValid },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIFacebookAndroid > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyDialogComplete ( int code ) {

	this->InvokeListener ( code == DIALOG_RESULT_SUCCESS ? DIALOG_DID_COMPLETE : DIALOG_DID_NOT_COMPLETE );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyLoginComplete ( int code ) {

	this->InvokeListener ( code == DIALOG_RESULT_SUCCESS ? SESSION_DID_LOGIN : SESSION_DID_NOT_LOGIN );
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyRequestComplete ( cc8* response ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( REQUEST_RESPONSE, state )) {
		lua_pushstring ( state, response );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookAndroid::NotifyRequestFailed () {

	this->InvokeListener ( REQUEST_RESPONSE_FAILED );
}

//================================================================//
// Facebook JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFacebookLoginComplete ( JNIEnv* env, jclass obj, jint code ) {

	MOAIFacebookAndroid::Get ().NotifyLoginComplete ( code );
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFacebookDialogComplete ( JNIEnv* env, jclass obj, jint code ) {

	MOAIFacebookAndroid::Get ().NotifyDialogComplete ( code );
}

extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFacebookRequestComplete ( JNIEnv* env, jclass obj, jstring jresponse ) {

    JNI_GET_CSTRING ( jresponse, response );
	MOAIFacebookAndroid::Get ().NotifyRequestComplete ( response );
	JNI_RELEASE_CSTRING ( jresponse, response );
}

extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFacebookRequestFailed ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().NotifyRequestFailed ( );
}

