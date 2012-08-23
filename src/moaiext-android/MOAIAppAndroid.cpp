// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIAppAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	openURL
	@text	Open the given URL in the device browser.
	
	@in		string	url				The URL to open.
	@out 	nil
*/
int MOAIAppAndroid::_openURL ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* url = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( url, jurl );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		USLog::Print ( "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID openURL = env->GetStaticMethodID ( moai, "openURL", "(Ljava/lang/String;)V" );
    	if ( openURL == NULL ) {

			USLog::Print ( "MOAIAppAndroid: Unable to find static java method %s", "openURL" );
    	} else {

			env->CallStaticVoidMethod ( moai, openURL, jurl );	
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIAppAndroid::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAIAppAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	share
	@text	Open a generic Android dialog to allow the user to share
			via email, SMS, Facebook, Twitter, etc.
	
	@in		string	prompt			The prompt to show the user.
	@in		string	subject			The subject of the message to share.
	@in		string	text			The text of the message to share.
	@out 	nil
*/
int MOAIAppAndroid::_share ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* prompt = lua_tostring ( state, 1 );
	cc8* subject = lua_tostring ( state, 2 );
	cc8* text = lua_tostring ( state, 3 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( prompt, jprompt );
	JNI_GET_JSTRING ( subject, jsubject );
	JNI_GET_JSTRING ( text, jtext );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		USLog::Print ( "MOAIAppAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID share = env->GetStaticMethodID ( moai, "share", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( share == NULL ) {

			USLog::Print ( "MOAIAppAndroid: Unable to find static java method %s", "share" );
    	} else {

			env->CallStaticVoidMethod ( moai, share, jprompt, jsubject, jtext );	
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getExternalStorageState
	@text	Gets the current state of the primary Android "external" storage device.
	
	@out 	string
*/
int MOAIAppAndroid::_getExternalStorageState ( lua_State* L )
{
	JNI_GET_ENV ( jvm, env );
	jclass    cls = env->FindClass ( "android/os/Environment" );
	jmethodID mid = env->GetStaticMethodID ( cls, "getExternalStorageState", "()Ljava/lang/String;" );
	if ( mid == 0 )
	{
		return 0;
	}
	jstring jstr     = ( jstring ) env->CallStaticObjectMethod ( cls, mid );
	const char * str = env->GetStringUTFChars ( jstr, 0 );
	if ( str )
	{
		lua_pushstring ( L, str );
		env->ReleaseStringUTFChars ( jstr, str );
		return 1;
	}
	// else
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getExternalStorageDirectory
	@text	Gets the Android external storage directory.
	
	@out 	string
*/
int MOAIAppAndroid::_getExternalStorageDirectory ( lua_State* L ) {
	JNI_GET_ENV ( jvm, env );
	jclass cls = env->FindClass ( "android/os/Environment" );
	jmethodID mid = env->GetStaticMethodID ( cls, "getExternalStorageDirectory", "()Ljava/io/File;" );
	if ( mid == 0 )
	{
		return 0;
	}
	jobject obj = env->CallStaticObjectMethod ( cls, mid );
	cls = env->GetObjectClass ( obj );
	mid = env->GetMethodID ( cls, "getAbsolutePath", "()Ljava/lang/String;" );
	if ( mid == 0 )
	{
		return 0;
	}
	jstring jstr     = ( jstring ) env->CallObjectMethod ( obj, mid );
	const char * str = env->GetStringUTFChars ( jstr, 0 );
	if ( str )
	{
		lua_pushstring ( L, str );
		env->ReleaseStringUTFChars ( jstr, str );
		return 1;
	}
	// else
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getExternalStoragePublicDirectory
	@text	Get an Android top-level public external storage
			directory for placing files of a particular type.
	
	@in		string	type			The type of storage directory to return.
			Should be one of the following MOAIAppAndroid constants:
			DIRECTORY_MUSIC, DIRECTORY_PODCASTS, DIRECTORY_RINGTONES, DIRECTORY_ALARMS,
			DIRECTORY_NOTIFICATIONS, DIRECTORY_PICTURES, DIRECTORY_MOVIES, DIRECTORY_DOWNLOADS,
			or DIRECTORY_DCIM. May not be nil.
	@out 	string
*/
int MOAIAppAndroid::_getExternalStoragePublicDirectory ( lua_State* L ) {
	int n = lua_gettop(L);    /* number of arguments */
	if ( n < 1 )
	{
		return 0;
	}
	if ( ! lua_isstring ( L, 1 ) )
	{
		return 0;
	}
	const char *cDirType = lua_tostring ( L, 1 );
	JNI_GET_ENV ( jvm, env );
	jstring     jDirType = env->NewStringUTF ( cDirType );
	jclass cls = env->FindClass ( "android/os/Environment" );
	jmethodID mid = env->GetStaticMethodID ( cls, "getExternalStoragePublicDirectory", "(Ljava/lang/String;)Ljava/io/File;" );
	if ( mid == 0 )
	{
		env->DeleteLocalRef ( jDirType );
		return 0;
	}
	jobject obj = env->CallStaticObjectMethod ( cls, mid, jDirType );
	env->DeleteLocalRef ( jDirType );
	cls = env->GetObjectClass ( obj );
	mid = env->GetMethodID ( cls, "getAbsolutePath", "()Ljava/lang/String;" );
	if ( mid == 0 )
	{
		return 0;
	}
	jstring jstr     = ( jstring ) env->CallObjectMethod ( obj, mid );
	const char * str = env->GetStringUTFChars ( jstr, 0 );
	if ( str )
	{
		lua_pushstring ( L, str );
		env->ReleaseStringUTFChars ( jstr, str );
		return 1;
	}
	// else
	return 0;
}

//================================================================//
// MOAIAppAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIAppAndroid::MOAIAppAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIAppAndroid::~MOAIAppAndroid () {

}

//----------------------------------------------------------------//
void MOAIAppAndroid::RegisterJavaField ( MOAILuaState& state, JNIEnv *env, jclass cls, const char *name )
{
	jfieldID     fid;
	jstring      jstr;
	const char * str;

	fid = env->GetStaticFieldID ( cls, name, "Ljava/lang/String;" );
	if ( fid == 0 )
	{
		return;
	}
	jstr = ( jstring ) env->GetStaticObjectField ( cls, fid );
	str  = env->GetStringUTFChars ( jstr, 0 );
	if ( str )
	{
		state.SetField ( -1, name, str );
		env->ReleaseStringUTFChars ( jstr, str );
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::RegisterEnvironmentFields ( MOAILuaState& state )
{
	JNI_GET_ENV ( jvm, env );
	jclass cls = env->FindClass ( "android/os/Environment" );

	RegisterJavaField ( state, env, cls, "MEDIA_BAD_REMOVAL" );
	RegisterJavaField ( state, env, cls, "MEDIA_CHECKING" );
	RegisterJavaField ( state, env, cls, "MEDIA_MOUNTED" );
	RegisterJavaField ( state, env, cls, "MEDIA_MOUNTED_READ_ONLY" );
	RegisterJavaField ( state, env, cls, "MEDIA_NOFS" );
	RegisterJavaField ( state, env, cls, "MEDIA_REMOVED" );
	RegisterJavaField ( state, env, cls, "MEDIA_SHARED" );
	RegisterJavaField ( state, env, cls, "MEDIA_UNMOUNTABLE" );
	RegisterJavaField ( state, env, cls, "MEDIA_UNMOUNTED" );

	RegisterJavaField ( state, env, cls, "DIRECTORY_ALARMS" );
	RegisterJavaField ( state, env, cls, "DIRECTORY_DCIM" );
	RegisterJavaField ( state, env, cls, "DIRECTORY_DOWNLOADS" );
	RegisterJavaField ( state, env, cls, "DIRECTORY_MOVIES" );
	RegisterJavaField ( state, env, cls, "DIRECTORY_MUSIC" );
	RegisterJavaField ( state, env, cls, "DIRECTORY_NOTIFICATIONS" );
	RegisterJavaField ( state, env, cls, "DIRECTORY_PICTURES" );
	RegisterJavaField ( state, env, cls, "DIRECTORY_PODCASTS" );
	RegisterJavaField ( state, env, cls, "DIRECTORY_RINGTONES" );
}

//----------------------------------------------------------------//
void MOAIAppAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "SESSION_START",		    ( u32 )SESSION_START );
	state.SetField ( -1, "SESSION_END",			    ( u32 )SESSION_END );
	state.SetField ( -1, "BACK_BUTTON_PRESSED",		( u32 )BACK_BUTTON_PRESSED );

	MOAIAppAndroid::RegisterEnvironmentFields ( state );

	luaL_Reg regTable [] = {
		{ "openURL",		_openURL },
		{ "setListener",	_setListener },
		{ "share",			_share },
		{ "getExternalStorageState",	_getExternalStorageState },
		{ "getExternalStorageDirectory",	_getExternalStorageDirectory },
		{ "getExternalStoragePublicDirectory",	_getExternalStoragePublicDirectory },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAppAndroid::NotifyBackButtonPressed () {
	
	MOAILuaRef& callback = this->mListeners [ BACK_BUTTON_PRESSED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();

		state.DebugCall ( 0, 1 );

		return lua_toboolean ( state, -1 );
	} else {
		
		return false;
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::NotifyDidStartSession ( bool resumed ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_START ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();

		lua_pushboolean ( state, resumed );
			
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppAndroid::NotifyWillEndSession () {
	
	MOAILuaRef& callback = this->mListeners [ SESSION_END ];
	
	if ( callback ) {

		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// Miscellaneous JNI Functions
//================================================================//

//----------------------------------------------------------------//
extern "C" bool Java_com_ziplinegames_moai_Moai_AKUAppBackButtonPressed ( JNIEnv* env, jclass obj ) {

	return MOAIAppAndroid::Get ().NotifyBackButtonPressed ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_Moai_AKUAppDidStartSession ( JNIEnv* env, jclass obj, jboolean resumed ) {

	MOAIAppAndroid::Get ().NotifyDidStartSession ( resumed );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_Moai_AKUAppWillEndSession ( JNIEnv* env, jclass obj ) {

	MOAIAppAndroid::Get ().NotifyWillEndSession ();
}
