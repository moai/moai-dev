// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com


#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIKeyboardAndroid.h>

extern JavaVM* jvm;

static lua_State* L = NULL;
static int targetRef = 0;
static int onImagePickedRef = 0;
static int onPickerDismissedRef = 0;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* MOAIKeyboardAndroid::_luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}

//================================================================//
// MOAIKeyboardIOS
//================================================================//

//----------------------------------------------------------------//
void MOAIKeyboardAndroid::Finish () {
   
    /*
	if ( this->mTextField ) {
		[ this->mTextField resignFirstResponder ];
		[ this->mTextField removeFromSuperview ];
		
		id delegate = [ this->mTextField delegate ];
		[ this->mTextField setDelegate:0 ];
		[ delegate release ];
		
		[ this->mTextField release ];
		this->mTextField = 0;
	}
     */
}

//----------------------------------------------------------------//
MOAIKeyboardAndroid::MOAIKeyboardAndroid () {
    
	RTTI_SINGLE ( MOAIGlobalEventSource )
}

//----------------------------------------------------------------//
MOAIKeyboardAndroid::~MOAIKeyboardAndroid () {
    
	this->Finish ();
    
}

//----------------------------------------------------------------//
void MOAIKeyboardAndroid::PushText ( MOAILuaState& state ) {
    
	//if ( this->mTextField ) {
	//	state.Push ([[ this->mTextField text ] UTF8String ]);
	//}
	//else {
	//	state.Push ();
	//}
}

static void initWithL(lua_State *anL, int aTargetRef, int anOnImageReadyRef, int anOnPickerDismissedRef) {
    
    L = anL;
    targetRef = aTargetRef;
    onImagePickedRef = anOnImageReadyRef;
    onPickerDismissedRef = anOnPickerDismissedRef; 
    
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getText
 @text	Returns the text currently being edited (or nil if no editing).
 
 @out	string text
 
 */
int MOAIKeyboardAndroid::_getText ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIKeyboardAndroid::Get ().PushText ( state );
	return 1;
}


int MOAIKeyboardAndroid::_showKeyboard ( lua_State* L ) {
    bool result=false;
    
    MOAILuaState state ( L );
	
	cc8* text			= state.GetValue < cc8* >( 1, "" );
	int type			= state.GetValue < int >( 2, KEYBOARD_DEFAULT );
	int returnKey		= state.GetValue < int >( 3, RETURN_KEY_DEFAULT );
	bool secure			= state.GetValue < bool >( 4, false );
	int autocap			= state.GetValue < int >( 5, AUTOCAP_NONE );
	int appearance		= state.GetValue < int >( 6, APPEARANCE_DEFAULT );
	
	//MOAIKeyboardAndroid::Get ().ShowKeyboard ( text, type, returnKey, secure, autocap, appearance );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass keyboard = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
    if ( keyboard == NULL ) {
        
		USLog::Print ( "MOAIImagePicker: Unable to find java class %s", "com/ziplinegames/moai/MoaiKeyboard" );
    } else {
        
    	jmethodID show = env->GetStaticMethodID ( keyboard, "showKeyboard", "(I)V" );
   		if ( show == NULL ) {
            
			USLog::Print ( "MOAIImagePicker: Unable to find static java method %s", "showKeyboard" );
		} else {
            
			env->CallStaticVoidMethod ( keyboard, show, 0);
		}
	}
    
    lua_pushboolean(L, result);
    
	return 0;
    
}


//----------------------------------------------------------------//
void MOAIKeyboardAndroid::RegisterLuaClass ( MOAILuaState& state ) {

    state.SetField ( -1, "EVENT_INPUT",					( u32 )EVENT_INPUT );
	state.SetField ( -1, "EVENT_RETURN",				( u32 )EVENT_RETURN );
    
	state.SetField ( -1, "AUTOCAP_ALL",					( u32 )AUTOCAP_ALL );
	state.SetField ( -1, "AUTOCAP_NONE",				( u32 )AUTOCAP_NONE );
	state.SetField ( -1, "AUTOCAP_SENTENCES",			( u32 )AUTOCAP_SENTENCES );
	state.SetField ( -1, "AUTOCAP_WORDS",				( u32 )AUTOCAP_WORDS );
	
	state.SetField ( -1, "KEYBOARD_ASCII",				( u32 )KEYBOARD_ASCII );
	state.SetField ( -1, "KEYBOARD_DECIMAL_PAD",		( u32 )KEYBOARD_DECIMAL_PAD );
	state.SetField ( -1, "KEYBOARD_DEFAULT",			( u32 )KEYBOARD_DEFAULT );
	state.SetField ( -1, "KEYBOARD_EMAIL",				( u32 )KEYBOARD_EMAIL );
	state.SetField ( -1, "KEYBOARD_NUM_PAD",			( u32 )KEYBOARD_NUM_PAD );
	state.SetField ( -1, "KEYBOARD_NUMERIC",			( u32 )KEYBOARD_NUMERIC );
	state.SetField ( -1, "KEYBOARD_PHONE_PAD",			( u32 )KEYBOARD_PHONE_PAD );
	state.SetField ( -1, "KEYBOARD_TWITTER",			( u32 )KEYBOARD_TWITTER );
	state.SetField ( -1, "KEYBOARD_URL",				( u32 )KEYBOARD_URL );
    
	state.SetField ( -1, "APPEARANCE_ALERT",			( u32 )APPEARANCE_ALERT );
	state.SetField ( -1, "APPEARANCE_DEFAULT",			( u32 )APPEARANCE_DEFAULT );
    
	state.SetField ( -1, "RETURN_KEY_DEFAULT",			( u32 )RETURN_KEY_DEFAULT );
	state.SetField ( -1, "RETURN_KEY_DONE",				( u32 )RETURN_KEY_DONE );
	state.SetField ( -1, "RETURN_KEY_GO",				( u32 )RETURN_KEY_GO );
	state.SetField ( -1, "RETURN_KEY_JOIN",				( u32 )RETURN_KEY_JOIN );
	state.SetField ( -1, "RETURN_KEY_NEXT",				( u32 )RETURN_KEY_NEXT );
	state.SetField ( -1, "RETURN_KEY_ROUTE",			( u32 )RETURN_KEY_ROUTE );
	state.SetField ( -1, "RETURN_KEY_SEARCH",			( u32 )RETURN_KEY_SEARCH );
	state.SetField ( -1, "RETURN_KEY_SEND",				( u32 )RETURN_KEY_SEND );
    

	
	luaL_Reg regTable [] = {
		{ "getText",			_getText },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIKeyboardAndroid > },
		{ "showKeyboard",		_showKeyboard },
		{ NULL, NULL }
        
	};

	luaL_register ( state, 0, regTable );
}


//================================================================//
// Keyboard JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiKeyboard_AKUOnImageReady ( JNIEnv* env, jclass obj, jstring imageFile ) {


    
   
	//MOAIImagePickerAndroid::Get ().NotifyLoginComplete ( code );
    //printf("imageFile: '%s'\n", [imageFile UTF8String]);
	const char* nativeImageFileString = env->GetStringUTFChars(imageFile, 0);
    
    printf("on image ready: %s", "test");   
    
    //lua_rawgeti(L, LUA_REGISTRYINDEX, onImagePickedRef);
    //lua_rawgeti(L, LUA_REGISTRYINDEX, targetRef);
    //lua_pushstring(L, nativeImageFileString);
    //lua_call(L, 2, 0);
    
    // Tidy up by removing the temporary file
    //[[NSFileManager defaultManager] removeItemAtPath:imageFile error: nil];
    
    // We're done, free up the static Picker instance
    //[picker release];
    //picker=nil;
    
    //env->ReleaseStringUTFChars(imageFile, nativeImageFileString);
    
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiKeyboard_AKUKeyPressed ( JNIEnv* env, jclass obj, jstring letter ) {

	//MOAIImagePickerAndroid::Get ().NotifyDialogComplete ( code );
    //printf("on picker dismissed: %s",(withImage)?"true":"false");	
	
    const char* nativeLetterString = env->GetStringUTFChars(letter, 0);
    
	//lua_rawgeti(L, LUA_REGISTRYINDEX, onInputRef);
    //lua_rawgeti(L, LUA_REGISTRYINDEX, targetRef);
    //lua_pushstring(L, nativeLetterString);
    //lua_call(L, 2, 0);
    
    //if (!withImage) {
        // We're done here free up our static Picker instance
        //[picker release];
        //picker=nil;
    //}
     
    
}

