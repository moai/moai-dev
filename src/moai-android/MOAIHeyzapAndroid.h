// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHEYZAPANDROID_H
#define MOAIHEYZAPANDROID_H

#ifndef DISABLE_HEYZAP

#include <moai-core/headers.h>

//================================================================//
// MOAIHeyzapAndroid
//================================================================//
class MOAIHeyzapAndroid :
    public MOAIGlobalClass < MOAIHeyzapAndroid, MOAILuaObject > {
private:
        //----------------------------------------------------------------//
        static int  _init                    ( lua_State* L );
        static int  _loadInterstitial        ( lua_State* L );
        static int  _setListener             ( lua_State* L );
        static int  _showInterstitial        ( lua_State* L );
        static int  _interstitialIsAvailable ( lua_State* L );

public:

        DECL_LUA_SINGLETON ( MOAIHeyzapAndroid );

        enum {
            INTERSTITIAL_AVAILABLE,
            INTERSTITIAL_FETCH_FAILED,
            INTERSTITIAL_SHOW,
            INTERSTITIAL_SHOW_FAILED,
            INTERSTITIAL_LOAD_FAILED,
            INTERSTITIAL_CLICKED,
            INTERSTITIAL_HIDE,
            INTERSTITIAL_DISMISSED,
            TOTAL
        };

        MOAILuaRef      mListeners [ TOTAL ];

                        MOAIHeyzapAndroid                ();
                        ~MOAIHeyzapAndroid               ();
        void            NotifyInterstitialDismissed      ();
        void            NotifyInterstitialFetchFailed    ();
        void            NotifyInterstitialAvailable      ();
        void            NotifyInterstitialShow           ();
        void            NotifyInterstitialHide           ();
        void            NotifyInterstitialShowFailed     ();
        void            NotifyInterstitialClicked        ();

        void            RegisterLuaClass                ( MOAILuaState& state );
};

#endif  //DISABLE_HEYZAP

#endif  //MOAIHEYZAP_H
