// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITAPJOY_H
#define	MOAITAPJOY_H

#ifndef DISABLE_TAPJOY

#include <moaicore/moaicore.h>

//================================================================//
// MOAITapjoy
//================================================================//
class MOAITapjoy :
	public MOAIGlobalClass < MOAITapjoy, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int		_getUserId				( lua_State* L );
	static int		_initVideoAds			( lua_State* L );
	static int		_requestTapjoyConnect	( lua_State* L );
	static int		_setListener			( lua_State* L );
	static int		_showOffers				( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAITapjoy );
	
	enum {
		TAPJOY_VIDEO_AD_READY,
		TAPJOY_VIDEO_AD_ERROR,
		TAPJOY_VIDEO_AD_CLOSE,
		TOTAL
	};

	enum {
        TAPJOY_VIDEO_STATUS_NO_ERROR,
        TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE,
        TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS,
        TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO,
	};
	
	MOAILuaRef		mListeners [ TOTAL ];

					MOAITapjoy						();
					~MOAITapjoy						();
	void			RegisterLuaClass				( MOAILuaState& state );
	void			NotifyVideoAdReady				();
	void			NotifyVideoAdError				( int code );
	void			NotifyVideoAdClose				();
};

#endif  //DISABLE_TAPJOY

#endif  //MOAITAPJOY_H
