// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICHARTBOOSTANDROID_H
#define	MOAICHARTBOOSTANDROID_H

#ifndef DISABLE_CHARTBOOST

#include <moaicore/moaicore.h>

//================================================================//
// MOAIChartBoostAndroid
//================================================================//
class MOAIChartBoostAndroid :
	public MOAIGlobalClass < MOAIChartBoostAndroid, MOAILuaObject > {
private:
		//----------------------------------------------------------------//
		static int	_init 				( lua_State* L );
		static int	_loadInterstitial 	( lua_State* L );
		static int	_setListener		( lua_State* L );
		static int	_showInterstitial 	( lua_State* L );

	public:

		DECL_LUA_SINGLETON ( MOAIChartBoostAndroid );

		enum {
			INTERSTITIAL_LOAD_FAILED,
			INTERSTITIAL_DISMISSED,
			TOTAL
		};

		MOAILuaRef		mListeners [ TOTAL ];

						MOAIChartBoostAndroid			();
						~MOAIChartBoostAndroid			();
		void 			NotifyInterstitialDismissed		();
		void 			NotifyInterstitialLoadFailed	();
		void			RegisterLuaClass				( MOAILuaState& state );
};

#endif  //DISABLE_CHARTBOOST

#endif  //MOAICHARTBOOST_H
