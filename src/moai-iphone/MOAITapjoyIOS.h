// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITAPJOYIOS_H
#define	MOAITAPJOYIOS_H

#ifndef DISABLE_TAPJOY

#import <Foundation/Foundation.h> 
#import <moaicore/moaicore.h>

#import <Components/TJCVideoAds/TJCVideoAdProtocol.h>

@class MOAITapjoyIOSVideoAdDelegate;

//================================================================//
// MOAITapjoyIOS
//================================================================//
/**	@name	MOAITapjoyIOS
	@text	Wrapper for Tapjoy integration on iOS devices.
			Tapjoy provides a turnkey advertising platform that 
			delivers cost-effective, high-value new users and helps 
			apps make money. Exposed to lua via MOAITapjoy on 
			all mobile platforms.

	@const	TAPJOY_VIDEO_AD_BEGIN								Event code for Tapjoy video ad playback begin.
	@const	TAPJOY_VIDEO_AD_CLOSE								Event code for Tapjoy video ad playback completion.
	@const	TAPJOY_VIDEO_AD_ERROR								Event code for Tapjoy video ad playback errors. Unused.
	@const	TAPJOY_VIDEO_AD_READY								Event code for Tapjoy video ad playback availability. Unused.

	@const	TAPJOY_VIDEO_STATUS_NO_ERROR						Error code for success. Unused.
	@const	TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE		Error code for inadequate storage for video ad. Unused.
	@const	TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS	Error code for network error. Unused.
	@const	TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO			Error code for playback error. Unused.
*/
class MOAITapjoyIOS :
	public MOAIGlobalClass < MOAITapjoyIOS, MOAIGlobalEventSource > {
private:

	MOAITapjoyIOSVideoAdDelegate* mVideoAdDelegate;

	//----------------------------------------------------------------//
	static int	_getUserId		( lua_State* L );
	static int	_initVideoAds	( lua_State* L );
	static int	_init			( lua_State* L );
	static int	_setUserId		( lua_State* L );
	static int	_showOffers		( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAITapjoyIOS );
	
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
	
			MOAITapjoyIOS		();
			~MOAITapjoyIOS		();
	void	RegisterLuaClass	( MOAILuaState& state );
	void	NotifyVideoAdBegin	();
	void	NotifyVideoAdClose	();
};

//================================================================//
// MOAITapjoyIOSDelegate
//================================================================//
@interface MOAITapjoyIOSVideoAdDelegate : NSObject < TJCVideoAdDelegate > {
@private
}
@end

#endif  //DISABLE_TAPJOY

#endif  //MOAITAPJOY_H