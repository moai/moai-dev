// Copyright (c) 2012 allaboutapps.at. All Rights Reserved.
//
// Contributed to the MOAI project by Jay Vaughan, allaboutapps.at
//
// This file implements the MOAIKeyboardAndroid object
// for user-input from the soft keyboard on Android
// 
// http://getmoai.com
//

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>
#include <moai-core/headers.h>
#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIKeyboardAndroid.h>

extern JavaVM* jvm;

//================================================================//
// MOAIKeyboardAndroid
//================================================================//
  
//----------------------------------------------------------------//
// The listeners need to be called on the event
extern "C" void Java_com_ziplinegames_moai_MoaiKeyboard_AKUNotifyKeyEvent ( JNIEnv* env, jclass cls ) {
	MOAIKeyboardAndroid::Get ().NotifyKeyEvent();
}

extern "C" void Java_com_ziplinegames_moai_MoaiKeyboard_AKUNotifyTextDone ( JNIEnv* env, jclass cls ) {
	MOAIKeyboardAndroid::Get ().NotifyTextDone();
}

//----------------------------------------------------------------//
int MOAIKeyboardAndroid::_hideKeyboard ( lua_State* L ) {
	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );

	if ( moai ) {
        jmethodID hideSoftKeyboard = env->GetStaticMethodID ( moai, "hideKeyboard", "()V" );
		if ( hideSoftKeyboard ) {
            env->CallStaticVoidMethod ( moai, hideSoftKeyboard );
            // !hiq-max - max@dynlab.at !: pushing "false", because old code returned the value from call to nonexisting
            // function "bool hideKeyboard()", signature is "void hideKeyboard()"
            // void was casted to bool and returned to the lua vm
            // this worked up until now, wrong function lookup and call caused segfault
            lua_pushboolean ( state, false );
			return 1;
		}
	}

	MOAIKeyboardAndroid::Get ().Finish ();

	return 0;
}

// Listeners use getText to retrieve the input
//----------------------------------------------------------------//
int MOAIKeyboardAndroid::_getText ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIKeyboardAndroid::Get ().PushText ( state );
	return 1;
}

//----------------------------------------------------------------//
int MOAIKeyboardAndroid::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		MOAIKeyboardAndroid::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}

	return 0;
}

int MOAIKeyboardAndroid::_setText ( lua_State* L ) { 

	MOAILuaState state ( L ); 

	cc8* text = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env ); 
	JNI_GET_JSTRING ( text, jtext );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
	if ( moai ) {
		jmethodID setText = env->GetStaticMethodID ( moai, "setText", "(Ljava/lang/String;)V" );
		if ( setText ) {
			env->CallStaticVoidMethod ( moai, setText, jtext ); 
			return 1;
		}
	}
	return 0;
}

int MOAIKeyboardAndroid::_showKeyboard ( lua_State* L ) {
    return _showTextKeyboard( L );
}

int MOAIKeyboardAndroid::_showTextKeyboard( lua_State* L ) {
    return _showKeyboardHelper( "showTextKeyboard" );
}

int MOAIKeyboardAndroid::_showNumberKeyboard( lua_State* L ) {
    return _showKeyboardHelper( "showNumberKeyboard" );
}

int MOAIKeyboardAndroid::_showDateTimeKeyboard( lua_State* L ) {
    return _showKeyboardHelper( "showDateTimeKeyboard" );
}

int MOAIKeyboardAndroid::_showPhoneKeyboard( lua_State* L ) {
    return _showKeyboardHelper( "showPhoneKeyboard" );
}

int MOAIKeyboardAndroid::_showKeyboardHelper( const char* j_func ) {
    JNI_GET_ENV ( jvm, env );

    jclass t_class = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
    if ( t_class ) {
        jmethodID t_func = env->GetStaticMethodID(t_class, j_func, "()V");
        if ( t_func ) {
            env->CallStaticVoidMethod ( t_class, t_func );
        }
    }
    return 0;
}

//================================================================//
// MOAIKeyboardAndroid
//================================================================//

//----------------------------------------------------------------//
void MOAIKeyboardAndroid::Finish () {
}

//----------------------------------------------------------------//
MOAIKeyboardAndroid::MOAIKeyboardAndroid () {
	RTTI_SINGLE ( MOAILuaObject ) 
}

//----------------------------------------------------------------//
MOAIKeyboardAndroid::~MOAIKeyboardAndroid () {
	this->Finish ();
}

//----------------------------------------------------------------//
void MOAIKeyboardAndroid::NotifyKeyEvent ( ) {
	JNI_GET_ENV ( jvm, env );

	MOAILuaRef& callback = this->mListeners [ EVENT_INPUT ];
	if ( callback ) {
		
		jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
		if ( moai ) {
			jmethodID getString = env->GetStaticMethodID ( moai, "getString", "()Ljava/lang/String;" );
			if ( getString ) {
				jstring jkeystring = ( jstring )env->CallStaticObjectMethod ( moai, getString );
				JNI_GET_CSTRING ( jkeystring, ckeystring );
				
				MOAIScopedLuaState state = callback.GetSelf ();  

				// push the start, length and string
				state.Push ( 0 );
				state.Push ( strlen(ckeystring) );
				state.Push( ckeystring );

				state.DebugCall ( 3, 0 );

				JNI_RELEASE_CSTRING ( jkeystring, ckeystring );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIKeyboardAndroid::NotifyTextDone ( ) {
	JNI_GET_ENV ( jvm, env );

	MOAILuaRef& callback = this->mListeners [ EVENT_RETURN ];
	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIKeyboardAndroid::PushText ( MOAILuaState& state ) {
	JNI_GET_ENV ( jvm, env );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );

	if ( moai ) {
		jmethodID getString = env->GetStaticMethodID ( moai, "getString", "()Ljava/lang/String;" );
		if ( getString ) {
			jstring jkeystring = ( jstring )env->CallStaticObjectMethod ( moai, getString );

			const char *kc = env->GetStringUTFChars((jstring) jkeystring, NULL);

			JNI_GET_CSTRING ( jkeystring, keystring );

			state.Push(keystring);
			JNI_RELEASE_CSTRING ( jkeystring, keystring );
		}
	}  
}

//----------------------------------------------------------------//
void MOAIKeyboardAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "EVENT_INPUT",         ( u32 )EVENT_INPUT );
	state.SetField ( -1, "EVENT_RETURN",        ( u32 )EVENT_RETURN );

	luaL_Reg regTable [] = {
		{ "getText",        _getText },
		{ "setListener",    _setListener  },

		{ "showKeyboard",           _showKeyboard },
        { "showTextKeyboard",       _showTextKeyboard },
        { "showNumberKeyboard",     _showNumberKeyboard },
        { "showDateTimeKeyboard",   _showDateTimeKeyboard },
        { "showPhoneKeyboard",      _showPhoneKeyboard },

        { "hideKeyboard",   _hideKeyboard },
		{ "setText",      _setText },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

