// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com


// Copyright (c) 2012 allaboutapps.at. All Rights Reserved.
//
// Contributed to the MOAI project by Jay Vaughan, allaboutapps.at
//
// This file implements the MOAIKeyboardAndroid object
// for user-input from the soft keyboard on Android
// 
// http://getmoai.com
//

#include "pch.h"

#include <jni.h>
#include <moaicore/moaicore.h>
#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIKeyboardAndroid.h>

extern JavaVM* jvm;

static jstring currentString = NULL;

//================================================================//
// MOAIKeyboardAndroid
//================================================================//

//----------------------------------------------------------------//
// The listeners need to be called on the event
extern "C" bool Java_com_ziplinegames_moai_Moai_NotifyKeyEvent ( JNIEnv* env, jclass cls, jstring key) {
    
    currentString = key;
    
    MOAIKeyboardAndroid::Get ().NotifyKeyEvent();
}


void MOAIKeyboardAndroid::NotifyKeyEvent( ) {
    //USLog::Print ( "MOAIKeyboardAndroid: within NotifyKeyEvent");
    JNI_GET_ENV ( jvm, env );
    
    MOAILuaRef& callback = this->mListeners [ EVENT_INPUT ];
    MOAILuaStateHandle state = callback.GetSelf ();  
    
    //USLog::Print ( "MOAIKeyboardAndroid: NotifyKeyEvent ");
    if ( callback ) {
        
        JNI_GET_CSTRING ( currentString, ckeystring );
        //USLog::Print("CKeyString: [%s] ", ckeystring);
        // push the start, length and string
        state.Push ( 0 );
        state.Push ( strlen(ckeystring) );
        state.Push( ckeystring );
        
        state.DebugCall ( 3, 0 );
        
        JNI_RELEASE_CSTRING ( currentString, ckeystring );
        
        /*
        //USLog::Print ( "MOAIKeyboardAndroid: NotifyKeyEvent callback ");
        jclass moai = env->FindClass ( "com/playnomi/thepicturegame/MoaiActivity" );
        
        if ( moai == NULL ) {
            USLog::Print ( "MOAIKeyboardAndroid: Unable to find java class %s", "com/playnomi/thepicturegame/MoaiActivity" );
            lua_pushnil ( state );
        } else {
            jmethodID getKeyCode = env->GetStaticMethodID ( moai, "getKeyCode", "()Ljava/lang/String;" );
            if ( getKeyCode == NULL ) {
                //USLog::Print ( "MOAIKeyboardAndroid: Unable to find static java method %s", "getKeyCode" );
            } else {
                
                jstring jkeystring = ( jstring )env->CallStaticObjectMethod ( moai, getKeyCode );
                JNI_GET_CSTRING ( jkeystring, ckeystring );
                //USLog::Print("CKeyString: [%s] ", ckeystring);
                // push the start, length and string
                state.Push ( 0 );
                state.Push ( strlen(ckeystring) );
                state.Push( ckeystring );
                
                state.DebugCall ( 3, 0 );
                
                
                JNI_RELEASE_CSTRING ( jkeystring, ckeystring );
                
            }
            
        }
        */
    }
    /*
     else
     USLog::Print ( "MOAIKeyboardAndroid: NotifyKeyEvent *NO* callback ");*/
}


// Listeners use getText to retrieve the input
//----------------------------------------------------------------//
int MOAIKeyboardAndroid::_getText ( lua_State* L ) {
    //USLog::Print ( "MOAIKeyboardAndroid: _getText ");
    MOAILuaState state ( L );
    MOAIKeyboardAndroid::Get ().PushText ( state );
    return 1;
}


//----------------------------------------------------------------//
void MOAIKeyboardAndroid::PushText ( MOAILuaState& state ) {
    //USLog::Print ( "MOAIKeyboardAndroid: within PushText");
    JNI_GET_ENV ( jvm, env );
    //USLog::Print ( "MOAIKeyboardAndroid: PushText ");
    
    jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
    
    if ( moai == NULL ) {
        //USLog::Print ( "MOAIKeyboardAndroid: Unable to find java class %s", "at/allaboutapps/test/moaikeyboard/MoaiActivity" );
        lua_pushnil ( state );
    } else {
        
        jmethodID getKeyCode = env->GetStaticMethodID ( moai, "getKeyCode", "()Ljava/lang/String;" );
        
        if ( getKeyCode == NULL ) {
            //USLog::Print ( "MOAIKeyboardAndroid: Unable to find static java method %s", "getKeyCode" );
        } else {
            jstring jkeystring = ( jstring )env->CallStaticObjectMethod ( moai, getKeyCode );
            
            const char *kc = env->GetStringUTFChars((jstring) jkeystring, NULL);
            
            JNI_GET_CSTRING ( jkeystring, keystring );
            //USLog::Print("MOAIKeyboardAndroid KeyString: [%s]", keystring);
            
            state.Push(keystring);
            JNI_RELEASE_CSTRING ( jkeystring, keystring );
        }
    }  
}


/** 
 @name _showSoftKeyboard/_hideSoftKeyboard
 @text Keyboard events
 
 @in   
 @out  int           result
 !J! AllAboutApps Modification to MOAI build host for host keyboard input
 */
int MOAIKeyboardAndroid::_hideKeyboard ( lua_State* L ) {
    JNI_GET_ENV ( jvm, env );
    //USLog::Print ( "MOAIKeyboardAndroid: _hideSoftKeyboard ");
    
    // TODO: Add keyboard type (numpad, numeric, alpha, etc.) support
    jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
    
    if ( moai == NULL ) {
        USLog::Print ( "MOAIKeyboardAndroid: Unable to find java class %s", "com/playnomi/thepicturegame/MoaiActivity" );
    } else {
        jmethodID hideSoftKeyboard = env->GetStaticMethodID ( moai, "hideSoftKeyboard", "()V" );
        if ( hideSoftKeyboard == NULL ) {
            //USLog::Print ( "MOAIKeyboardAndroid: Unable to find static java method %s", "hideSoftKeyboard" );
        } else {
            env->CallStaticVoidMethod ( moai, hideSoftKeyboard ); 
            return 1;
        }
    }
    
    // !J!
    MOAIKeyboardAndroid::Get ().Finish ();
    
}

int MOAIKeyboardAndroid::_showKeyboard ( lua_State* L ) {
    //USLog::Print ( "MOAIKeyboardAndroid: _showSoftKeyboard ");
    JNI_GET_ENV ( jvm, env );
    //USLog::Print ( "MOAIKeyboardAndroid: _showSoftKeyboard ");
    
    // s.s. later
    //MOAILuaState state ( L );
	
	//cc8* text			= state.GetValue < cc8* >( 1, "" );
	//int type			= state.GetValue < int >( 2, KEYBOARD_DEFAULT );
	//int returnKey		= state.GetValue < int >( 3, RETURN_KEY_DEFAULT );
	//bool secure			= state.GetValue < bool >( 4, false );
	//int autocap			= state.GetValue < int >( 5, AUTOCAP_NONE );
	//int appearance		= state.GetValue < int >( 6, APPEARANCE_DEFAULT );
    
    
    jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
    
    
    if ( moai == NULL ) {
        USLog::Print ( "MOAIKeyboardAndroid: Unable to find java class %s", "com/playnomi/thepicturegame/MoaiActivity" );
    } else {
        jmethodID showSoftKeyboard = env->GetStaticMethodID ( moai, "showSoftKeyboard", "()V" );
        if ( showSoftKeyboard == NULL ) {
            USLog::Print ( "MOAIKeyboardAndroid: Unable to find static java method %s", "showSoftKeyboard" );
        } else {
            env->CallStaticVoidMethod ( moai, showSoftKeyboard ); 
            
            return 1;
        }
    }
}

int MOAIKeyboardAndroid::_setText ( lua_State* L ) { 
    
    MOAILuaState state ( L ); 
    
    cc8* text = lua_tostring ( state, 1 );
    
    JNI_GET_ENV ( jvm, env ); 
    JNI_GET_JSTRING ( text, jtext );
    
    jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
    if ( moai == NULL ) {
        USLog::Print ( "MOAIKeyboardAndroid: Unable to find java class %s", "com/playnomi/thepicturegame/MoaiActivity" );
    } else {
        jmethodID setText = env->GetStaticMethodID ( moai, "setText", "(Ljava/lang/String;)V" );
        if ( setText == NULL ) {
            USLog::Print ( "MOAIKeyboardAndroid: Unable to find static java method %s", "setText" );
        } else {
            env->CallStaticVoidMethod ( moai, setText, jtext ); 
            
            return 1;
        }
    }
    
}


//----------------------------------------------------------------//
void MOAIKeyboardAndroid::Finish () {
    //USLog::Print ( "MOAIKeyboardAndroid: Finish ");
}

//================================================================//
// MOAIKeyboardAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIKeyboardAndroid::MOAIKeyboardAndroid () {
    
    RTTI_SINGLE ( MOAILuaObject ) 
}

//----------------------------------------------------------------//
MOAIKeyboardAndroid::~MOAIKeyboardAndroid () {
    this->Finish ();
    
}

//----------------------------------------------------------------//
int MOAIKeyboardAndroid::_setListener ( lua_State* L ) {
    //  USLog::Print ( "MOAIKeyboardAndroid: setListener " );  
    MOAILuaState state ( L );
    
    u32 idx = state.GetValue < u32 >( 1, TOTAL );
    //  USLog::Print ( "MOAIKeyboardAndroid: setListener " );
    
    if ( idx < TOTAL ) {
        //USLog::Print ( "MOAIKeyboardAndroid: Set state for %d", idx );
        MOAIKeyboardAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
    }
    
    return 0;
}

//----------------------------------------------------------------//
void MOAIKeyboardAndroid::RegisterLuaClass ( MOAILuaState& state ) {
    
    state.SetField ( -1, "EVENT_INPUT",         ( u32 )EVENT_INPUT );
    state.SetField ( -1, "EVENT_RETURN",        ( u32 )EVENT_RETURN );
    
    luaL_Reg regTable [] = {
        { "getText",        _getText },
        { "setListener",    _setListener  },
        { "showKeyboard",   _showKeyboard },
        { "hideKeyboard",   _hideKeyboard },
        { "setText",      _setText },
        { NULL, NULL }
    };
    
    luaL_register ( state, 0, regTable );
}

