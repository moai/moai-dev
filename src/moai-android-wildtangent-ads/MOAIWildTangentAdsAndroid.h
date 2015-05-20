// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIWILDTANGENTADSANDROID_H
#define	MOAIWILDTANGENTADSANDROID_H

#include <moai-core/headers.h>
#include <moai-android/JniUtils.h>

//================================================================//
// MOAIGooglePlayServicesAndroid
//================================================================//
/**	@lua	MOAIGooglePlayServicesAndroid
	@text	Wrapper for Google Play services.
*/
class MOAIWildTangentAdsAndroid :
	public MOAIGlobalClass < MOAIWildTangentAdsAndroid, MOAIGlobalEventSource >,
	public JniUtils {
private:

		//----------------------------------------------------------------//
		static int	_cacheInterstitial		( lua_State* L );
		static int	_hasCachedInterstitial	( lua_State* L );
		static int	_init	 				( lua_State* L );
		static int	_showInterstitial 		( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIWildTangentAdsAndroid );

	enum {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
	};

			MOAIWildTangentAdsAndroid		();
			~MOAIWildTangentAdsAndroid		();
	void 	NotifyInterstitialDismissed		();
	void 	NotifyInterstitialLoadFailed	();
	void	RegisterLuaClass				( MOAILuaState& state );
};

#endif  //MOAIWILDTANGENTADSANDROID_H
