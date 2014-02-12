// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_HEYZAP

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIHeyzapAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/** @name   start
    @text   Initialize Heyzap.
    
    @out    nil
*/
int MOAIHeyzapAndroid::_start ( lua_State* L ) {
    
    MOAILuaState state ( L );

    JNI_GET_ENV ( jvm, env );

    jclass heyzap = env->FindClass ( "com/ziplinegames/moai/MoaiHeyzap" );
    if ( heyzap == NULL ) {

        ZLLog::Print ( "MoaiHeyzapAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiHeyzap" );
    } else {

        jmethodID start = env->GetStaticMethodID ( heyzap, "start", "()V" );
        if ( start == NULL ) {

            ZLLog::Print ( "MoaiHeyzapAndroid: Unable to find static java method %s", "start" );
        } else {

            env->CallStaticVoidMethod ( heyzap, start );                
        }
    }
            
    return 0;
}

//----------------------------------------------------------------//
/** @name   loadInterstitial
    @text   Request that an interstitial ad be cached for later display.
    
    @opt    string  tag      Optional tag.
    @out    nil
*/
int MOAIHeyzapAndroid::_loadInterstitial ( lua_State* L ) {
    
    MOAILuaState state ( L );

    cc8* tag = lua_tostring( state, 1 );

    JNI_GET_ENV ( jvm, env );
    
    JNI_GET_JSTRING ( tag, jtag );

    jclass heyzap = env->FindClass ( "com/ziplinegames/moai/MoaiHeyzap" );
    if ( heyzap == NULL ) {

        ZLLog::Print ( "MoaiHeyzapAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiHeyzap" );
    } else {

        jmethodID loadInterstitial = env->GetStaticMethodID ( heyzap, "loadInterstitial", "(Ljava/lang/String;)V" );
        if ( loadInterstitial == NULL ) {

            ZLLog::Print ( "MoaiHeyzapAndroid: Unable to find static java method %s", "loadInterstitial" );
        } else {

            env->CallStaticVoidMethod ( heyzap, loadInterstitial, jtag );          
        }
    }
            
    return 0;
}

//----------------------------------------------------------------//
int MOAIHeyzapAndroid::_setListener ( lua_State* L ) {
    
    MOAILuaState state ( L );
    
    u32 idx = state.GetValue < u32 >( 1, TOTAL );

    if ( idx < TOTAL ) {
        
        MOAIChartBoostAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
    }
    
    return 0;
}

//----------------------------------------------------------------//
/** @name   showInterstitial
    @text   Request an interstitial ad display if a cached ad is available.
    
    @opt    string  tag      Optional tag.
    @out    nil
*/
int MOAIHeyzapAndroid::_showInterstitial ( lua_State* L ) {
    
    MOAILuaState state ( L );

    cc8* tag = lua_tostring( state, 1 );

    JNI_GET_ENV ( jvm, env );
    
    JNI_GET_JSTRING ( location, jtag );

    jclass heyzap = env->FindClass ( "com/ziplinegames/moai/MoaiHeyzap" );
    if ( heyzap == NULL ) {

        ZLLog::Print ( "MoaiHeyzapAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiHeyzap" );
    } else {

        jmethodID showInterstitial = env->GetStaticMethodID ( heyzap, "showInterstitial", "(Ljava/lang/String;)V" );
        if ( showInterstitial == NULL ) {

            ZLLog::Print ( "MoaiHeyzapAndroid: Unable to find static java method %s", "showInterstitial" );
        } else {

            env->CallStaticVoidMethod ( heyzap, showInterstitial, jtag );              
        }
    }

    return 0;
}

//----------------------------------------------------------------//
/** @name   interstitialIsAvailable
    @text   Checks to see if an interstitial is available.

    @out    boolean isAvailable
*/
int MOAIHeyzapAndroid::_interstitialIsAvailable ( lua_State* L ) {
    
    MOAILuaState state ( L );

    cc8* tag = lua_tostring( state, 1 );

    JNI_GET_ENV ( jvm, env );
    
    JNI_GET_JSTRING ( location, jtag );
    
    jclass heyzap = env->FindClass ( "com/ziplinegames/moai/MoaiHeyzap" );
    if ( heyzap == NULL ) {

        ZLLog::Print ( "MoaiHeyzapAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiHeyzap" );
    } else {

        jmethodID interstitialIsAvailable = env->GetStaticMethodID ( heyzap, "interstitialIsAvailable", "()Z" );
        if ( interstitialIsAvailable == NULL ) {

            ZLLog::Print ( "MoaiHeyzapAndroid: Unable to find static java method %s", "interstitialIsAvailable" );
        } else {

            jboolean isAvailable = ( jboolean )env->CallStaticBooleanMethod ( heyzap, interstitialIsAvailable, jtag );    

            lua_pushboolean ( state, isAvailable );

            return 1;       
        }
    }

    return 0;
}

//================================================================//
// MoaiHeyzapAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIHeyzapAndroid::MoaiHeyzapAndroid () {

    RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIHeyzapAndroid::~MoaiHeyzapAndroid () {

}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::RegisterLuaClass ( MOAILuaState& state ) {

    state.SetField ( -1, "INTERSTITIAL_LOAD_FAILED",    ( u32 )INTERSTITIAL_LOAD_FAILED );
    state.SetField ( -1, "INTERSTITIAL_DISMISSED",      ( u32 )INTERSTITIAL_DISMISSED );

    luaL_Reg regTable [] = {
        { "start",                   _start },
        { "loadInterstitial",        _loadInterstitial },
        { "setListener",             _setListener },
        { "showInterstitial",        _showInterstitial },
        { "interstitialIsAvailable", _interstitialIsAvailable },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialDismissed () {    
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_DISMISSED ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialLoadFailed () {   
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_LOAD_FAILED ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//================================================================//
// Heyzap JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialDismissed ( JNIEnv* env, jclass obj ) {

    MOAIHeyzapAndroid::Get ().NotifyInterstitialDismissed ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialLoadFailed ( JNIEnv* env, jclass obj ) {

    MOAIHeyzapAndroid::Get ().NotifyInterstitialLoadFailed ();
}

#endif