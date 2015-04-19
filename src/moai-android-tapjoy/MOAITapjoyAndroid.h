// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITAPJOYANDROID_H
#define	MOAITAPJOYANDROID_H


#include <moai-core/headers.h>
#include <moai-android/JniUtils.h>

//================================================================//
// MOAITapjoyAndroid
//================================================================//
// TODO: doxygen
class MOAITapjoyAndroid :
	public MOAIGlobalClass < MOAITapjoyAndroid, MOAIGlobalEventSource >,
	public JniUtils {
private:

	//----------------------------------------------------------------//
	static int	_init				( lua_State* L );
	static int	_setUserId			( lua_State* L );
	static int	_showOffers			( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAITapjoyAndroid );

	enum {
		TAPJOY_VIDEO_AD_START,
		TAPJOY_VIDEO_AD_COMPLETE,
		TAPJOY_VIDEO_AD_ERROR,
		TAPJOY_OFFERS_RESPONSE,
		TAPJOY_OFFERS_RESPONSE_FAILED,
    };

	enum {
        TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE		= 1,
        TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS	= 2,
        TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO			= 3,
	};

	//----------------------------------------------------------------//
			MOAITapjoyAndroid	();
			~MOAITapjoyAndroid	();
	void	RegisterLuaClass	( MOAILuaState& state );
};


#endif  //MOAITAPJOY_H