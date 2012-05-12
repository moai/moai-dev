// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITAPJOYANDROID_H
#define	MOAITAPJOYANDROID_H

#ifndef DISABLE_TAPJOY

#include <moaicore/moaicore.h>

//================================================================//
// MOAITapjoyAndroid
//================================================================//
/**	@name	MOAITapjoyAndroid
	@text	Wrapper for Tapjoy integration on Android devices.
			Tapjoy provides a turnkey advertising platform that 
			delivers cost-effective, high-value new users and helps 
			apps make money. Exposed to lua via MOAITapjoy on 
			all mobile platforms.

	@const	TAPJOY_VIDEO_AD_BEGIN								Event code for Tapjoy video ad playback begin. Unused.
	@const	TAPJOY_VIDEO_AD_CLOSE								Event code for Tapjoy video ad playback completion.
	@const	TAPJOY_VIDEO_AD_ERROR								Event code for Tapjoy video ad playback errors.
	@const	TAPJOY_VIDEO_AD_READY								Event code for Tapjoy video ad playback availability.

	@const	TAPJOY_VIDEO_STATUS_NO_ERROR						Error code for success.
	@const	TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE		Error code for inadequate storage for video ad.
	@const	TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS	Error code for network error.
	@const	TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO			Error code for playback error.
*/
class MOAITapjoyAndroid :
	public MOAIGlobalClass < MOAITapjoyAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_getUserId		( lua_State* L );
	static int	_initVideoAds	( lua_State* L );
	static int	_init			( lua_State* L );
	static int	_setListener	( lua_State* L );
	static int	_showOffers		( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAITapjoyAndroid );
	
	enum {
		TAPJOY_VIDEO_AD_BEGIN,
		TAPJOY_VIDEO_AD_CLOSE,
		TAPJOY_VIDEO_AD_ERROR,
		TAPJOY_VIDEO_AD_READY,
		TOTAL
	};

	enum {
        TAPJOY_VIDEO_STATUS_NO_ERROR,
        TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE,
        TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS,
        TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO,
	};
	
	MOAILuaRef		mListeners [ TOTAL ];

			MOAITapjoyAndroid	();
			~MOAITapjoyAndroid	();
	void	NotifyVideoAdReady	();
	void	NotifyVideoAdError	( int code );
	void	NotifyVideoAdClose	();
	void	RegisterLuaClass	( MOAILuaState& state );
};

#endif  //DISABLE_TAPJOY

#endif  //MOAITAPJOY_H