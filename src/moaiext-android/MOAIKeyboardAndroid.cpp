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

//================================================================//
// MOAIKeyboardAndroid
//================================================================//
  
//----------------------------------------------------------------//
// The listeners need to be called on the event
extern "C" bool Java_com_ziplinegames_moai_MoaiKeyboard_AKUNotifyKeyEvent ( JNIEnv* env, jclass cls ) {
	MOAIKeyboardAndroid::Get ().NotifyKeyEvent();
}

extern "C" bool Java_com_ziplinegames_moai_MoaiKeyboard_AKUNotifyTextDone ( JNIEnv* env, jclass cls ) {
	MOAIKeyboardAndroid::Get ().NotifyTextDone();
}

//----------------------------------------------------------------//
int MOAIKeyboardAndroid::_hideKeyboard ( lua_State* L ) {
	JNI_GET_ENV ( jvm, env );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );

	if ( moai ) {
		jmethodID hideSoftKeyboard = env->GetStaticMethodID ( moai, "hideKeyboard", "()V" );
		if ( hideSoftKeyboard ) {
			env->CallStaticVoidMethod ( moai, hideSoftKeyboard ); 
			return 1;
		}
	}

	MOAIKeyboardAndroid::Get ().Finish ();
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
		MOAIKeyboardAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
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

}

int MOAIKeyboardAndroid::_showKeyboard ( lua_State* L ) {
  	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
	if ( moai ) {
		jmethodID showKeyboard = env->GetStaticMethodID(moai, "showKeyboard", "()V");
		if ( showKeyboard ) {
			env->CallStaticVoidMethod ( moai, showKeyboard );
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
	MOAILuaStateHandle state = callback.GetSelf ();  

	if ( callback ) {
		jclass moai = env->FindClass ( "com/ziplinegames/moai/MoaiKeyboard" );
		if ( moai ) {
			jmethodID getString = env->GetStaticMethodID ( moai, "getString", "()Ljava/lang/String;" );
			if ( getString ) {
				jstring jkeystring = ( jstring )env->CallStaticObjectMethod ( moai, getString );
				JNI_GET_CSTRING ( jkeystring, ckeystring );

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
	MOAILuaStateHandle state = callback.GetSelf ();

	state.DebugCall ( 0, 0 );
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
		{ "showKeyboard",   _showKeyboard },
		{ "hideKeyboard",   _hideKeyboard },
		{ "setText",      _setText },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

