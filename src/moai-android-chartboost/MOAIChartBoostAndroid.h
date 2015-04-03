// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICHARTBOOSTANDROID_H
#define	MOAICHARTBOOSTANDROID_H

#include <moai-core/headers.h>
#include <moai-android/JniUtils.h>

//================================================================//
// MOAIChartBoostAndroid
//================================================================//
class MOAIChartBoostAndroid :
	public MOAIGlobalClass < MOAIChartBoostAndroid, MOAIGlobalEventSource >,
	public JniUtils {
private:
	
		//----------------------------------------------------------------//
		static int	_cacheInterstitial		( lua_State* L );
		static int	_hasCachedInterstitial	( lua_State* L );
		static int	_init	 				( lua_State* L );
		static int	_showInterstitial 		( lua_State* L );

public:

		DECL_LUA_SINGLETON ( MOAIChartBoostAndroid );

		enum {
			INTERSTITIAL_LOAD_FAILED,
			INTERSTITIAL_DISMISSED,
		};

		//----------------------------------------------------------------//
						MOAIChartBoostAndroid			();
						~MOAIChartBoostAndroid			();
		void 			NotifyInterstitialDismissed		();
		void 			NotifyInterstitialLoadFailed	();
		void			RegisterLuaClass				( MOAILuaState& state );
};

#endif  //MOAICHARTBOOST_H
