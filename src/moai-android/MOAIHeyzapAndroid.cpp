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
/** @name   init
    @text   initialize heyzap
    
    @out    nil
*/
int MOAIHeyzapAndroid::_start ( lua_State* L ) {
    
    MOAILuaState state ( L );

    JNI_GET_ENV ( jvm, env );

    jclass heyzap = env->FindClass ( "com/ziplinegames/moai/MoaiHeyzap" );
    if ( heyzap == NULL ) {

        ZLLog::Print ( "MOAIHeyzapAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiHeyzap" );
    } else {

        jmethodID start = env->GetStaticMethodID ( heyzap, "start", "()V" );
        if ( start == NULL ) {

            ZLLog::Print ( "MOAIHeyzapAndroid: Unable to find static java method %s", "start" );
        } else {

            env->CallStaticVoidMethod ( heyzap, start );
        }
    }
            
    return 0;
}

//----------------------------------------------------------------//
/** @name   loadInterstitial
    @text   Request that an interstitial ad be cached for later display.
    
    @opt    string  tag     Optional tag.
    @out    nil
*/
int MOAIHeyzapAndroid::_loadInterstitial ( lua_State* L ) {
    
    MOAILuaState state ( L );

    cc8* tag = lua_tostring( state, 1 );

    JNI_GET_ENV ( jvm, env );
    
    JNI_GET_JSTRING ( tag, jtag );

    jclass heyzap = env->FindClass ( "com/ziplinegames/moai/MoaiHeyzap" );
    if ( heyzap == NULL ) {

        ZLLog::Print ( "MOAIHeyzapAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiHeyzap" );
    } else {

        jmethodID loadInterstitial = env->GetStaticMethodID ( heyzap, "loadInterstitial", "(Ljava/lang/String;)V" );
        if ( loadInterstitial == NULL ) {

            ZLLog::Print ( "MOAIHeyzapAndroid: Unable to find static java method %s", "loadInterstitial" );
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
        
        ZLLog::Print ( "_setListener" );
        MOAIHeyzapAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
    }
    
    return 0;
}

//----------------------------------------------------------------//
/** @name   showInterstitial
    @text   Request an interstitial ad display if a cached ad is available.
    
    @opt    string  tag     Optional tag
    @out    nil
*/
int MOAIHeyzapAndroid::_showInterstitial ( lua_State* L ) {
    
    MOAILuaState state ( L );

    cc8* tag = lua_tostring( state, 1 );

    JNI_GET_ENV ( jvm, env );
    
    JNI_GET_JSTRING ( tag, jtag );

    jclass heyzap = env->FindClass ( "com/ziplinegames/moai/MoaiHeyzap" );
    if ( heyzap == NULL ) {

        ZLLog::Print ( "MOAIHeyzapAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiHeyzap" );
    } else {

        jmethodID showInterstitial = env->GetStaticMethodID ( heyzap, "showInterstitial", "(Ljava/lang/String;)V" );
        if ( showInterstitial == NULL ) {

            ZLLog::Print ( "MOAIHeyzapAndroid: Unable to find static java method %s", "showInterstitial" );
        } else {

            env->CallStaticVoidMethod ( heyzap, showInterstitial, jtag );
        }
    }

    return 0;
}

//----------------------------------------------------------------//
/** @name   isInterstitialAvailable
    @text   Determine whether an interstitial is available.

    @out    boolean isAvailable
*/
int MOAIHeyzapAndroid::_isInterstitialAvailable ( lua_State* L ) {
    
    MOAILuaState state ( L );

    cc8* tag = lua_tostring( state, 1 );

    JNI_GET_ENV ( jvm, env );
    
    JNI_GET_JSTRING ( tag, jtag );
    
    jclass heyzap = env->FindClass ( "com/ziplinegames/moai/MoaiHeyzap" );
    if ( heyzap == NULL ) {

        ZLLog::Print ( "MOAIHeyzapAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiHeyzap" );
    } else {

        jmethodID isInterstitialAvailable = env->GetStaticMethodID ( heyzap, "isInterstitialAvailable", "(Ljava/lang/String;)Z" );
        if ( isInterstitialAvailable == NULL ) {

            ZLLog::Print ( "MOAIHeyzapAndroid: Unable to find static java method %s", "isInterstitialAvailable" );
        } else {

            jboolean isAvailable = ( jboolean )env->CallStaticBooleanMethod ( heyzap, isInterstitialAvailable, jtag );    

            lua_pushboolean ( state, isAvailable );

            return 1;       
        }
    }

    return 0;
}

//================================================================//
// MOAIHeyzapAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIHeyzapAndroid::MOAIHeyzapAndroid () {

    RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIHeyzapAndroid::~MOAIHeyzapAndroid () {

}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::RegisterLuaClass ( MOAILuaState& state ) {

    state.SetField ( -1, "INTERSTITIAL_AVAILABLE",      ( u32 )INTERSTITIAL_AVAILABLE );
    state.SetField ( -1, "INTERSTITIAL_FETCH_FAILED",   ( u32 )INTERSTITIAL_FETCH_FAILED );
    state.SetField ( -1, "INTERSTITIAL_SHOW",           ( u32 )INTERSTITIAL_SHOW );
    state.SetField ( -1, "INTERSTITIAL_SHOW_FAILED",    ( u32 )INTERSTITIAL_SHOW_FAILED );
    state.SetField ( -1, "INTERSTITIAL_CLICKED",        ( u32 )INTERSTITIAL_CLICKED );
    state.SetField ( -1, "INTERSTITIAL_HIDE",           ( u32 )INTERSTITIAL_HIDE );
    state.SetField ( -1, "INTERSTITIAL_AUDIO_STARTED",  ( u32 )INTERSTITIAL_AUDIO_STARTED );
    state.SetField ( -1, "INTERSTITIAL_AUDIO_FINISHED", ( u32 )INTERSTITIAL_AUDIO_FINISHED );

    luaL_Reg regTable [] = {
        { "start",                      _start },
        { "loadInterstitial",           _loadInterstitial },
        { "setListener",                _setListener },
        { "showInterstitial",           _showInterstitial },
        { "isInterstitialAvailable",    _isInterstitialAvailable },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialFetchFailed () {  
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_FETCH_FAILED ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialAvailable () {   
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_AVAILABLE ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialShow () {   
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_SHOW ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialHide () {   
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_HIDE ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialShowFailed () {   
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_SHOW_FAILED ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialClicked () {   
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_CLICKED ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialAudioStarted () {   
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_AUDIO_STARTED ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//----------------------------------------------------------------//
void MOAIHeyzapAndroid::NotifyInterstitialAudioFinished () {   
    
    MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_AUDIO_FINISHED ];
    
    if ( callback ) {
        
        MOAIScopedLuaState state = callback.GetSelf ();
        
        state.DebugCall ( 0, 0 );
    }
}

//================================================================//
// Heyzap JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialFetchFailed ( JNIEnv* env, jclass obj ) {

    ZLLog::Print ( "Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialFetchFailed" );
    MOAIHeyzapAndroid::Get ().NotifyInterstitialFetchFailed ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialAvailable ( JNIEnv* env, jclass obj ) {

    ZLLog::Print ( "Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialAvailable" );
    MOAIHeyzapAndroid::Get ().NotifyInterstitialAvailable ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialShow ( JNIEnv* env, jclass obj ) {

    ZLLog::Print ( "Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialShow" );
    MOAIHeyzapAndroid::Get ().NotifyInterstitialShow ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialHide ( JNIEnv* env, jclass obj ) {

    ZLLog::Print ( "Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialHide" );
    MOAIHeyzapAndroid::Get ().NotifyInterstitialHide ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialShowFailed ( JNIEnv* env, jclass obj ) {

    ZLLog::Print ( "Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialShowFailed" );
    MOAIHeyzapAndroid::Get ().NotifyInterstitialShowFailed ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialClicked ( JNIEnv* env, jclass obj ) {

    ZLLog::Print ( "Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialClicked" );
    MOAIHeyzapAndroid::Get ().NotifyInterstitialClicked ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialAudioStarted ( JNIEnv* env, jclass obj ) {

    ZLLog::Print ( "Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialAudioStarted" );
    MOAIHeyzapAndroid::Get ().NotifyInterstitialAudioStarted ();
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialAudioFinished ( JNIEnv* env, jclass obj ) {

    ZLLog::Print ( "Java_com_ziplinegames_moai_MoaiHeyzap_AKUNotifyHeyzapInterstitialAudioFinished" );
    MOAIHeyzapAndroid::Get ().NotifyInterstitialAudioFinished ();
}

#endif