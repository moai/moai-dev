// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com


#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIImagePickerAndroid.h>

extern JavaVM* jvm;

static lua_State* L = NULL;
static int targetRef = 0;
static int onImagePickedRef = 0;
static int onPickerDismissedRef = 0;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* MOAIImagePickerAndroid::_luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}

//================================================================//
// MOAIFacebookAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIImagePickerAndroid::MOAIImagePickerAndroid () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIImagePickerAndroid::~MOAIImagePickerAndroid () {

}


static int resizePickedImage(lua_State *L) {
    bool result=false;
    
    
    int width = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);
   
    MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass imagePicker = env->FindClass ( "com/ziplinegames/moai/MoaiImagePicker" );
    if ( imagePicker == NULL ) {
        
		USLog::Print ( "MOAIImagePicker: Unable to find java class %s", "com/ziplinegames/moai/MoaiImagePicker" );
    } else {
        
    	jmethodID resize = env->GetStaticMethodID ( imagePicker, "resizePickedImage", "(I;I)V" );
   		if ( resize == NULL ) {
            
			USLog::Print ( "MOAIImagePicker: Unable to find static java method %s", "resize" );
		} else {
            
			env->CallStaticVoidMethod ( imagePicker, resize, width, height);
		}
	}

    lua_pushboolean(L, result);
   
    return 1;
}

static int pickImageIpad(lua_State *L) {
    bool result=false;
    
    /*
    //@try {
    NSUInteger sourceType = (NSUInteger) luaL_checkinteger(L, 1);
    int pickerWidth = luaL_checkinteger(L, 4);
    int pickerHeight = luaL_checkinteger(L, 5);
    int pickerXOrigin = luaL_checkinteger(L, 6);
    int pickerYOrigin = luaL_checkinteger(L, 7);
    
    lua_pop(L,4);
    stackdump(L);
    int onPickerDismissedRef=luaL_ref(L, LUA_REGISTRYINDEX);
    int onImageReadyRef=luaL_ref(L, LUA_REGISTRYINDEX);
    int targetRef=luaL_ref(L, LUA_REGISTRYINDEX);
    
    pickerHelper = [[ImagePickerHelper alloc] initWithL: L targetRef: targetRef onImageReadyRef: onImageReadyRef onPickerDismissedRef: onPickerDismissedRef];
    result=[pickerHelper pickImageIpad:sourceType pickerWidth:pickerWidth pickerHeight:pickerHeight pickerXOrigin:pickerXOrigin pickerYOrigin:pickerYOrigin];
    //}
    //@catch (id e) {
    //    NSLog(@"pickImageIpad: Caught %@", e);
    // }
     
     */
    
    lua_pushboolean(L, result);
     
    return 1;
}

static void initWithL(lua_State *anL, int aTargetRef, int anOnImageReadyRef, int anOnPickerDismissedRef) {
    
    L = anL;
    targetRef = aTargetRef;
    onImagePickedRef = anOnImageReadyRef;
    onPickerDismissedRef = anOnPickerDismissedRef; 
    
}

static int pickImage(lua_State *L) {
    bool result=false;
    
    int sourceType = (int) luaL_checkinteger(L, 1);
    int onPickerDismissedRef=luaL_ref(L, LUA_REGISTRYINDEX);
    int onImageReadyRef=luaL_ref(L, LUA_REGISTRYINDEX);
    int targetRef=luaL_ref(L, LUA_REGISTRYINDEX);

    initWithL(L, targetRef, onImageReadyRef, onPickerDismissedRef);
    
    MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass imagePicker = env->FindClass ( "com/ziplinegames/moai/MoaiImagePicker" );
    if ( imagePicker == NULL ) {
        
		USLog::Print ( "MOAIImagePicker: Unable to find java class %s", "com/ziplinegames/moai/MoaiImagePicker" );
    } else {
        
    	jmethodID test = env->GetStaticMethodID ( imagePicker, "test", "(I)V" );
   		if ( test == NULL ) {
            
			USLog::Print ( "MOAIImagePicker: Unable to find static java method %s", "pickImage" );
		} else {
            
			env->CallStaticVoidMethod ( imagePicker, test, sourceType);
		}
	}

    
    /*
    //@try {
    NSUInteger sourceType = (NSUInteger) luaL_checkinteger(L, 1);
    int onPickerDismissedRef=luaL_ref(L, LUA_REGISTRYINDEX);
    int onImageReadyRef=luaL_ref(L, LUA_REGISTRYINDEX);
    int targetRef=luaL_ref(L, LUA_REGISTRYINDEX);
    
    pickerHelper = [[ImagePickerHelper alloc] initWithL: L targetRef: targetRef onImageReadyRef: onImageReadyRef onPickerDismissedRef: onPickerDismissedRef];
    result=[pickerHelper pickImage: sourceType];
    //}
    // @catch (id e) {
    //     NSLog(@"pickImage: Caught %@", e);
    // }
          */
    lua_pushboolean(L, result);

    return 1;
}

static int isSourceTypeAvailable(lua_State *L) {
    //NSUInteger sourceType = (NSUInteger) luaL_checkinteger(L, 1);
    //bool result=[UIImagePickerController isSourceTypeAvailable:sourceType];
    bool result = true;
    lua_pushboolean(L, result);
    return 1;
}



//----------------------------------------------------------------//
void MOAIImagePickerAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "SOURCE_TYPE_CAMERA",		( u32 ) SOURCE_TYPE_CAMERA );
	state.SetField ( -1, "SOURCE_TYPE_PHOTO_LIBRARY",	( u32 ) SOURCE_TYPE_PHOTO_LIBRARY );
	state.SetField ( -1, "SOURCE_TYPE_SAVED_PHOTOS_ALBUM",		( u32 ) SOURCE_TYPE_SAVED_PHOTOS_ALBUM );
	
	luaL_Reg regTable [] = {
		{ "resizePickedImage",		resizePickedImage },
		{ "pickImageIpad",			pickImageIpad },
		{ "pickImage",              pickImage },
		{ "isSourceTypeAvailable",	isSourceTypeAvailable},
		{ NULL, NULL }	

	};

	luaL_register ( state, 0, regTable );
}


//----------------------------------------------------------------//
//void MOAIImagePickerAndroid::NotifyLoginComplete ( int code ) {

    /*
	MOAILuaRef& callback = this->mListeners [ SESSION_DID_NOT_LOGIN ];
	if ( code == DIALOG_RESULT_SUCCESS ) {
		
		callback = this->mListeners [ SESSION_DID_LOGIN ];
	}

	if ( callback ) {
	
		MOAILuaStateHandle state = callback.GetSelf ();
	
		state.DebugCall ( 0, 0 );
	}
     */
//}

//----------------------------------------------------------------//
//void MOAIImagePickerAndroid::NotifyDialogComplete ( int code ) {
	
    /*
	MOAILuaRef& callback = this->mListeners [ DIALOG_DID_NOT_COMPLETE ];
	if ( code == DIALOG_RESULT_SUCCESS ) {
		
		callback = this->mListeners [ DIALOG_DID_COMPLETE ];
	}

	if ( callback ) {
	
		MOAILuaStateHandle state = callback.GetSelf ();
	
		state.DebugCall ( 0, 0 );
	}
     */
//}


//================================================================//
// Facebook JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiImagePicker_AKUOnImageReady ( JNIEnv* env, jclass obj, jstring imageFile ) {


    
   
	//MOAIImagePickerAndroid::Get ().NotifyLoginComplete ( code );
    //printf("imageFile: '%s'\n", [imageFile UTF8String]);
	const char* nativeImageFileString = env->GetStringUTFChars(imageFile, 0);
    
    printf("on image ready: %s", "test");   
    
    lua_rawgeti(L, LUA_REGISTRYINDEX, onImagePickedRef);
    lua_rawgeti(L, LUA_REGISTRYINDEX, targetRef);
    lua_pushstring(L, nativeImageFileString);
    lua_call(L, 2, 0);
    
    // Tidy up by removing the temporary file
    //[[NSFileManager defaultManager] removeItemAtPath:imageFile error: nil];
    
    // We're done, free up the static Picker instance
    //[picker release];
    //picker=nil;
    
    //env->ReleaseStringUTFChars(imageFile, nativeImageFileString);
    
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiImagePicker_AKUOnPickerDismissed ( JNIEnv* env, jclass obj, jboolean withImage ) {

	//MOAIImagePickerAndroid::Get ().NotifyDialogComplete ( code );
    printf("on picker dismissed: %s",(withImage)?"true":"false");	
	
    
	lua_rawgeti(L, LUA_REGISTRYINDEX, onPickerDismissedRef);
    lua_rawgeti(L, LUA_REGISTRYINDEX, targetRef);
    lua_pushboolean(L, withImage);
    lua_call(L, 2, 0);
    
    if (!withImage) {
        // We're done here free up our static Picker instance
        //[picker release];
        //picker=nil;
    }
     
    
}

