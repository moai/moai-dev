//----------------------------------------------------------------//
// Using Facebook Android SDK 3.6 (January 2014)
// Copyright (c) 2013-present:
//		Author(s):
//				* An Nguyen (MeYuMe http://www.meyume.com)
//				* Ricard Sole (gh: rcsole, Herding Cats)
//----------------------------------------------------------------//

#ifndef DISABLE_FACEBOOK

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/JniUtils.h>
#include <moai-android/MOAIFacebookAndroid.h>

#define MOAI_FACEBOOK_CLASS "com/ziplinegames/moai/MoaiFacebook"

extern JavaVM* jvm;


//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Facebook.

	@in		string	appId			Available in Facebook developer settings.
	@out 	nil
*/
int MOAIFacebookAndroid::_init ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	cc8* appid = lua_tostring ( state, 1 );

	JNI_GET_JSTRING ( appid, jappid );

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID init = env->GetStaticMethodID ( facebook, "init", "(Ljava/lang/String;)V" );
		if ( init == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "init" );
		} else {

			env->CallStaticVoidMethod ( facebook, init, jappid );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	login
	@text	Prompt the user to login to Facebook with basic info permission only.

	@out 	nil
*/
int MOAIFacebookAndroid::_login ( lua_State *L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	cc8* appid = lua_tostring ( state, 1 );

	JNI_GET_JSTRING ( appid, jappid );

	jclass facebook = env->FindClass( MOAI_FACEBOOK_CLASS );

	if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID login = env->GetStaticMethodID ( facebook, "login", "(Ljava/lang/String;)V" );

		if ( login == NULL) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "login" );
		} else {

			env->CallStaticVoidMethod( facebook, login, jappid );
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

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
    if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
    } else {

    	jmethodID logout = env->GetStaticMethodID ( facebook, "logout", "()V" );
   		if ( logout == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "logout" );
		} else {

			env->CallStaticVoidMethod ( facebook, logout );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getPermissions
	@text	get current permissions array of the current session

	@out	string	json object with the permissions
*/
int MOAIFacebookAndroid::_getPermissions ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	jstring ret;

	jclass facebook = env->FindClass( MOAI_FACEBOOK_CLASS);
	if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID getPermissions = env->GetStaticMethodID ( facebook, "getPermissions", "()Ljava/lang/String;" );
		if ( getPermissions == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "getPermissions" );
		} else {

			ret = (jstring) env->CallStaticObjectMethod ( facebook, getPermissions );
		}
	}

	if ( ret == NULL ) {

		lua_pushstring ( state, "" );
	} else {

		cc8* retString = ( cc8* ) env->GetStringUTFChars ( ret, 0 );
		lua_pushstring ( state, retString );
	}

	return 1;
}

//----------------------------------------------------------------//
/**	@name	requestPermissions
	@text	Makes a request to the user for additional permissions

	@out	nil
*/
int MOAIFacebookAndroid::_requestPermissions ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	int length = lua_tointeger ( state, 1 );
	jobjectArray array = ( jobjectArray ) env->NewObjectArray ( length, env->FindClass ( "java/lang/String" ), env->NewStringUTF ( "" ) );

	for ( int i = 0; i < length; i++ ) {

		cc8* p = lua_tostring ( state, i + 2 );
		env->SetObjectArrayElement ( array, i, env->NewStringUTF ( p ) );
	}

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if (facebook == NULL) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID requestPermissions = env->GetStaticMethodID ( facebook, "requestPermissions", "([Ljava/lang/String;)V" );
		if (requestPermissions == NULL) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "requestPermissions" );
		} else {

			env->CallStaticVoidMethod(facebook, requestPermissions, array);
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	publish (replaces postToFeed)
	@text	Post a message to the logged in users' news feed.

	@docs	https://developers.facebook.com/docs/reference/dialogs/feed/ (SDK 3.6)

	@in		string	link			The link attached to this post.
	@in		string	picture			The URL of a picture attached to this post.
										The picture must be at least 200px by 200px.
	@in		string	name			The name of the link attachment.
	@in		string	caption			The caption of the link (appears beneath the link name).
										If not specified, this field is automatically
										populated with the URL of the link.
	@in		string	description		The description of the link (appears beneath the link caption).
										If not specified, this field is automatically populated
										by information scraped from the link, typically the
										title of the page.
	@out 	nil
*/
int MOAIFacebookAndroid::_publish ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	cc8* link = lua_tostring ( state, 1 );
	cc8* picture = lua_tostring ( state, 2 );
	cc8* name = lua_tostring ( state, 3 );
	cc8* caption = lua_tostring ( state, 4 );
	cc8* description = lua_tostring ( state, 5 );

	JNI_GET_JSTRING ( link, jlink );
	JNI_GET_JSTRING ( picture, jpicture );
	JNI_GET_JSTRING ( name, jname );
	JNI_GET_JSTRING ( caption, jcaption );
	JNI_GET_JSTRING ( description, jdescription );

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
    if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
    } else {

    	jmethodID publish = env->GetStaticMethodID ( facebook, "publish", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
   		if ( publish == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "publish" );
		} else {

			env->CallStaticVoidMethod ( facebook, publish, jlink, jpicture, jname, jcaption, jdescription );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	openRequestDialog
	@text	Provides a builder that allows construction of the parameters for showing the Requests Dialog.

	@in		String		title
	@in		String		message

	@docs	https://developers.facebook.com/docs/reference/android/current/WebDialog.RequestsDialogBuilder

	@out	nil
*/
int MOAIFacebookAndroid::_openRequestDialog ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	cc8* title = lua_tostring ( state, 1 );
	cc8* message = lua_tostring ( state, 2 );

	JNI_GET_JSTRING ( title, jtitle );
	JNI_GET_JSTRING ( message, jmessage );

	jclass facebook = env->FindClass(MOAI_FACEBOOK_CLASS);
	if (facebook == NULL) {

		ZLLog::Print ("MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS);
	} else {

		jmethodID openRequestDialog = env->GetStaticMethodID ( facebook, "openRequestDialog", "(Ljava/lang/String;Ljava/lang/String;)V" );
		if (openRequestDialog == NULL) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "openRequestDialog" );
		} else {

			env->CallStaticVoidMethod( facebook, openRequestDialog, jtitle, jmessage );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	sendRequest
	@text	sends a request to a specific user

	@docs	TODO

	@in		String		userId
	@out	boolean		success or error
*/
int MOAIFacebookAndroid::_sendRequest( lua_State* L ) {

	MOAILuaState state(L);
	JNI_GET_ENV (jvm, env);

	cc8* title = lua_tostring(state, 1);
	cc8* message = lua_tostring(state, 2);
	cc8* item = lua_tostring(state, 3);
	int qty = lua_tointeger(state, 4);

	JNI_GET_JSTRING (title, jtitle);
	JNI_GET_JSTRING (message, jmessage);
	JNI_GET_JSTRING (item, jitem);

	jclass facebook = env->FindClass(MOAI_FACEBOOK_CLASS);
	if (facebook == NULL) {

		ZLLog::Print ("MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS);
	} else {

		jmethodID sendRequest = env->GetStaticMethodID (facebook, "sendRequest", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
		if (sendRequest == NULL) {

			ZLLog::Print ("MOAIFacebookAndroid: Unable to find static java method %s", "sendRequest");
		} else {

			env->CallStaticVoidMethod(facebook, sendRequest, jtitle, jmessage, jitem, qty);
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAppRequests
	@text	The requests received by this person from the app making the API call.

	@docs	https://developers.facebook.com/docs/graph-api/reference/user/apprequests/

	@out	String		json result
*/
int MOAIFacebookAndroid::_getAppRequests ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID getAppRequests = env->GetStaticMethodID ( facebook, "getAppRequests", "()V" );
		if (getAppRequests == NULL) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "getAppRequests" );
		} else {

			env->CallStaticVoidMethod ( facebook, getAppRequests );
		}
	}

	return 0;
}


//----------------------------------------------------------------//
/**	@name	deleteRequest
	@text	Delete a request. Should be used after a request has been accepted, see docs under Deleting Requests
	@docs	https://developers.facebook.com/docs/howtos/requests/

	@in		string		user ID

	@out	nil // TODO
*/

int MOAIFacebookAndroid::_deleteRequest ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV  ( jvm, env );

	cc8* requestId = lua_tostring ( state, 1 );

	JNI_GET_JSTRING  ( requestId, jrequestId );

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if ( facebook == NULL ) {

		ZLLog::Print  ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID deleteRequest = env->GetStaticMethodID ( facebook, "deleteRequest", "(Ljava/lang/String;)V" );
		if  ( deleteRequest == NULL ) {

			ZLLog::Print  ( "MOAIFacebookAndroid: Unable to find static java method %s", "deleteRequest" );
		} else {

			env->CallStaticVoidMethod ( facebook, deleteRequest, jrequestId );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getUserDetails
	@text	Graph API GET request with the following params:
				picture,id,name,first_name,last_name,middle_name,gender,username,
				installed,devices,email,birthday
	@docs	https://developers.facebook.com/docs/graph-api/reference/user/

	@in		string		user ID ("me" for current user)

	@out	String		json result
*/
int MOAIFacebookAndroid::_getUserDetails ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV  ( jvm, env );

	cc8* userId = lua_tostring ( state, 1 );

	JNI_GET_JSTRING  ( userId, juserId );

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if  ( facebook == NULL ) {

		ZLLog::Print  ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID getUserDetails = env->GetStaticMethodID ( facebook, "getUserDetails", "(Ljava/lang/String;)V" );
		if ( getUserDetails == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "getUserDetails" );
		} else {

			env->CallStaticVoidMethod ( facebook, getUserDetails, juserId );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getFriends
	@text	Request to get the current user's friend list

	@docs	https://developers.facebook.com/docs/graph-api/reference/

	@out	array	friend list
*/
int MOAIFacebookAndroid::_getFriends ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if ( facebook == NULL ) {

		ZLLog::Print  ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID getFriends = env->GetStaticMethodID ( facebook, "getFriends", "()V" );
		if ( getFriends == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "getFriends" );
		} else {

			env->CallStaticVoidMethod ( facebook, getFriends );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getScores
	@text	gets APP Scores from the Facebook Scores API, see docs

	@docs	https://developers.facebook.com/docs/games/scores/

	@in		string		appId

	@out	String		json result
*/

int MOAIFacebookAndroid::_getScores ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	cc8* appid = lua_tostring ( state, 1 );

	JNI_GET_JSTRING ( appid, jappid );

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID getScores = env->GetStaticMethodID (facebook, "getScores", "(Ljava/lang/String;)V");
		if ( getScores == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "getScores" );
		} else {

			env->CallStaticVoidMethod ( facebook, getScores, jappid );
		}
	}

	return 0;
}


// TODO

//----------------------------------------------------------------//
/**	@name	sendScore
	@text	Send a score to the Scores API, see docs

	@docs	https://developers.facebook.com/docs/games/scores/

	@in		long[]		Score (should only contain one position)

	@out	nil
*/

int MOAIFacebookAndroid::_sendScore ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

	long score = lua_tonumber ( state, 1 );
	//TODO
	// JNI bug that failed sending a long integer number to Java,
	// the work around is to use an array instead.
	jlongArray scoreArray = env->NewLongArray ( 1 );
	jlong *arr = env->GetLongArrayElements ( scoreArray, NULL );
	arr [ 0 ] = score;
	env->ReleaseLongArrayElements ( scoreArray, arr, NULL );

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID sendScore = env->GetStaticMethodID ( facebook, "sendScore", "([J)V" );
		if ( sendScore == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "sendScore" );
		} else {

			env->CallStaticVoidMethod ( facebook, sendScore, scoreArray );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	graphRequest
	@text	Request to the Facebook Graph API

	@docs	https://developers.facebook.com/docs/graph-api/reference/
			https://developers.facebook.com/docs/opengraph/using-object-api/

	@in		string		path
	@in		string		verb method
	@in		luatable	parameters

	@out	String		json result
*/

int MOAIFacebookAndroid::_graphRequest( lua_State* L ) {

	MOAILuaState state(L);
	JNI_GET_ENV (jvm, env);

	cc8* path = lua_tostring(state, 1);
	cc8* method = lua_tostring(state, 2);
	jobject bundle;

    if ( state.IsType ( 3, LUA_TTABLE ) ) {
        bundle = JniUtils::bundleFromLua( L, 3 );
    }

	JNI_GET_JSTRING (path, jpath);
	JNI_GET_JSTRING (method, jmethod);

	jclass facebook = env->FindClass(MOAI_FACEBOOK_CLASS);
	if (facebook == NULL) {
		ZLLog::Print ("MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS);
	} else {
		jmethodID graphRequest = env->GetStaticMethodID (facebook, "graphRequest", "(Ljava/lang/String;Ljava/lang/String;Landroid/os/Bundle;)V");
		if (graphRequest == NULL) {
			ZLLog::Print ("MOAIFacebookAndroid: Unable to find static java method %s", "graphRequest");
		} else {
			env->CallStaticVoidMethod(facebook, graphRequest, jpath, jmethod, bundle);
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadProfilePicture

	TODO: docs
*/

int MOAIFacebookAndroid::_loadProfilePicture ( lua_State* L ) {

	MOAILuaState state ( L );
	JNI_GET_ENV  ( jvm, env );

	cc8* userId = lua_tostring ( state, 1 );
	cc8* url = lua_tostring ( state, 2 );
	bool fromCacheOnly = lua_toboolean ( state, 3 );

	JNI_GET_JSTRING ( userId, juserId );
	JNI_GET_JSTRING ( url, jurl );

	jstring ret;

	jclass facebook = env->FindClass ( MOAI_FACEBOOK_CLASS );
	if ( facebook == NULL ) {

		ZLLog::Print ( "MOAIFacebookAndroid: Unable to find java class %s", MOAI_FACEBOOK_CLASS );
	} else {

		jmethodID loadProfilePicture = env->GetStaticMethodID ( facebook, "loadProfilePicture", "(Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;" );
		if ( loadProfilePicture == NULL ) {

			ZLLog::Print ( "MOAIFacebookAndroid: Unable to find static java method %s", "loadProfilePicture" );
		} else {

			ret = ( jstring )env->CallStaticObjectMethod ( facebook, loadProfilePicture, juserId, jurl, fromCacheOnly );
		}
	}

	if ( ret == NULL ) {

		lua_pushstring ( state, "" );
	} else {

		cc8* retString = ( cc8* )env->GetStringUTFChars ( ret, 0 );
		lua_pushstring ( state, retString );
	}

	return 1;
}

int MOAIFacebookAndroid::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 idx = state.GetValue<u32> ( 1, TOTAL );

	if ( idx < TOTAL ) {
		MOAIFacebookAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}

	return 0;
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidLogin ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().notifyFBDidLogin ();
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidLogout ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().notifyFBDidLogout ();
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidRetrieveUserDetails ( JNIEnv* env, jclass obj, jstring jsonResult ) {

	if ( jsonResult == NULL ) {
		return;
	}
	MOAIFacebookAndroid::Get ().notifyFBDidRetrieveUserDetails ( ( cc8* )env->GetStringUTFChars ( jsonResult, 0 ) );
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidRetrieveFriends ( JNIEnv* env, jclass obj, jstring jsonResult ) {

	if ( jsonResult == NULL ) {
		return;
	}
	MOAIFacebookAndroid::Get ().notifyFBDidRetrieveFriends ( ( cc8* )env->GetStringUTFChars ( jsonResult, 0 ) );
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidClosePublishDialog ( JNIEnv* env, jclass obj, jboolean success ) {

	MOAIFacebookAndroid::Get ().notifyFBDidClosePublishDialog ( success );
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidCloseRequestDialog ( JNIEnv* env, jclass obj, jboolean success ) {

	MOAIFacebookAndroid::Get ().notifyFBDidCloseRequestDialog( success );
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidRetrieveScores ( JNIEnv* env, jclass obj, jstring jsonResult ) {

	if ( jsonResult == NULL ) {
		return;
	}

	const char *result = env->GetStringUTFChars ( jsonResult, 0 );
	cc8* scores = ( cc8* )result;
	MOAIFacebookAndroid::Get ().notifyFBDidRetrieveScores ( scores );
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidRetrieveAppRequests ( JNIEnv* env, jclass obj, jstring jsonResult ) {

	if ( jsonResult == NULL ) {
		return;
	}

	MOAIFacebookAndroid::Get ().notifyFBDidRetrieveAppRequests ( ( cc8* )env->GetStringUTFChars ( jsonResult, 0 ) );
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidRetrievePermissions ( JNIEnv* env, jclass obj, jstring jsonResult ) {

	if ( jsonResult == NULL ) {
		return;
	}

	MOAIFacebookAndroid::Get ().notifyFBDidRetrievePermissions ( ( cc8* )env->GetStringUTFChars ( jsonResult, 0 ) );
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidRequestPermissions ( JNIEnv* env, jclass obj ) {

	MOAIFacebookAndroid::Get ().notifyFBDidRequestPermissions ();
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidRetrieveProfilePicture ( JNIEnv* env, jclass obj, jstring userId, jstring picturePath ) {

	if ( userId == NULL ) {
		return;
	}

	if ( picturePath == NULL ) {
		return;
	}

	MOAIFacebookAndroid::Get ().notifyFBDidLoadProfilePicture ( ( cc8* )env->GetStringUTFChars ( userId, 0 ), ( cc8* )env->GetStringUTFChars ( picturePath, 0 ) );
}

extern "C" void Java_com_ziplinegames_moai_MoaiFacebook_AKUNotifyFBDidRetrieveGraphRequestData ( JNIEnv* env, jclass obj, jstring jsonResult ) {

	if ( jsonResult == NULL ) {
		return;
	}

	MOAIFacebookAndroid::Get ().notifyFBDidReceiveGraphRequestData ( ( cc8* )env->GetStringUTFChars ( jsonResult, 0 ) );
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

void MOAIFacebookAndroid::RegisterLuaClass ( MOAILuaState& state ) {
	state.SetField ( -1, "DID_LOGIN",						( u32 ) DID_LOGIN );
	state.SetField ( -1, "DID_LOGOUT",						( u32 ) DID_LOGOUT );
	state.SetField ( -1, "RETRIEVED_USER_DETAILS",			( u32 ) RETRIEVED_USER_DETAILS );
	state.SetField ( -1, "REQUEST_DIALOG_CLOSED",			( u32 ) REQUEST_DIALOG_CLOSED );
	state.SetField ( -1, "PUBLISH_DIALOG_CLOSED",			( u32 ) PUBLISH_DIALOG_CLOSED );
	state.SetField ( -1, "RETRIEVED_SCORES",				( u32 ) RETRIEVED_SCORES );
	state.SetField ( -1, "RETRIEVED_APP_REQUESTS",			( u32 ) RETRIEVED_APP_REQUESTS );
	state.SetField ( -1, "RETRIEVED_FRIENDS",				( u32 ) RETRIEVED_FRIENDS );
	state.SetField ( -1, "RETRIEVED_PERMISSIONS",			( u32 ) RETRIEVED_PERMISSIONS );
	state.SetField ( -1, "REQUESTED_PERMISSIONS",			( u32 ) REQUESTED_PERMISSIONS );
	state.SetField ( -1, "RETRIEVED_PROFILE_PICTURE",		( u32 ) RETRIEVED_PROFILE_PICTURE );
	state.SetField ( -1, "RETRIEVED_GRAPH_REQUEST_DATA",	( u32 ) RETRIEVED_GRAPH_REQUEST_DATA );

	luaL_Reg regTable[] = {
		{ "init", 					_init},
		{ "login",					_login },
		{ "logout",					_logout },
		{ "getPermissions",			_getPermissions },
		{ "requestPermissions",		_requestPermissions },
		{ "publish",				_publish },
		{ "openRequestDialog",		_openRequestDialog },
		{ "sendRequest",			_sendRequest },
		{ "getAppRequests",			_getAppRequests },
		{ "deleteRequest",			_deleteRequest },
		{ "getUserDetails",			_getUserDetails },
		{ "loadProfilePicture",		_loadProfilePicture },
		{ "getFriends",				_getFriends },
		{ "setListener",			_setListener },
		{ "sendScore",				_sendScore },
		{ "getScores",				_getScores },
		{ "graphRequest",			_graphRequest },
		{NULL, NULL}
	};

	luaL_register(state, 0, regTable);
}

void MOAIFacebookAndroid::notifyFBDidLogin() {
	MOAILuaRef& callback = this->mListeners[DID_LOGIN];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();
		state.DebugCall(0, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidLogout() {
	MOAILuaRef& callback = this->mListeners[DID_LOGOUT];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();
		state.DebugCall(0, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidRetrieveUserDetails ( cc8* jsonResult ) {
	MOAILuaRef& callback = this->mListeners [ RETRIEVED_USER_DETAILS ];

	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();

		lua_pushstring ( state, jsonResult );
		state.DebugCall ( 1, 0 );
	}
}

void MOAIFacebookAndroid::notifyFBDidRetrieveFriends(cc8* jsonResult) {
	MOAILuaRef& callback = this->mListeners[RETRIEVED_FRIENDS];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		lua_pushstring(state, jsonResult);
		state.DebugCall(1, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidClosePublishDialog(bool success) {
	MOAILuaRef& callback = this->mListeners[PUBLISH_DIALOG_CLOSED];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		lua_pushboolean(state, success);
		state.DebugCall(1, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidCloseRequestDialog(bool success) {
	MOAILuaRef& callback = this->mListeners[REQUEST_DIALOG_CLOSED];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		lua_pushboolean(state, success);
		state.DebugCall(1, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidRetrieveScores(cc8* jsonResult) {
	MOAILuaRef& callback = this->mListeners[RETRIEVED_SCORES];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		lua_pushstring(state, jsonResult);
		state.DebugCall(1, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidRetrieveAppRequests(cc8* jsonResult) {
	MOAILuaRef& callback = this->mListeners[RETRIEVED_APP_REQUESTS];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		lua_pushstring(state, jsonResult);
		state.DebugCall(1, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidRetrievePermissions(cc8* jsonResult) {
	MOAILuaRef& callback = this->mListeners[RETRIEVED_PERMISSIONS];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		lua_pushstring(state, jsonResult);
		state.DebugCall(1, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidRequestPermissions() {
	MOAILuaRef& callback = this->mListeners[REQUESTED_PERMISSIONS];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		state.DebugCall(0, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidLoadProfilePicture(cc8* userId, cc8* picturePath) {
	MOAILuaRef& callback = this->mListeners[RETRIEVED_PROFILE_PICTURE];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		lua_pushstring(state, userId);
		lua_pushstring(state, picturePath);
		state.DebugCall(2, 0);
	}
}

void MOAIFacebookAndroid::notifyFBDidReceiveGraphRequestData(cc8* jsonResult) {
	MOAILuaRef& callback = this->mListeners[RETRIEVED_GRAPH_REQUEST_DATA];

	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();

		lua_pushstring(state, jsonResult);
		state.DebugCall(1, 0);
	}
}

#endif
