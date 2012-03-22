// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITAPJOY_H
#define	MOAITAPJOY_H

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

	@const	TAPJOY_VIDEO_AD_BEGIN		Event code for Tapjoy video ad start.
	@const	TAPJOY_VIDEO_AD_CLOSE		Event code for Tapjoy video ad completion.
*/
class MOAITapjoyIOS :
	public MOAIGlobalClass < MOAITapjoyIOS, MOAIGlobalEventSource > {
private:

	MOAITapjoyIOSVideoAdDelegate* mVideoAdDelegate;

	//----------------------------------------------------------------//
	static int	_getUserId				( lua_State* L );
	static int	_initVideoAds			( lua_State* L );
	static int	_requestTapjoyConnect	( lua_State* L );
	static int	_showOffers				( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAITapjoyIOS );
	
	enum {
		TAPJOY_VIDEO_AD_BEGIN,
		TAPJOY_VIDEO_AD_CLOSE,
		TAPJOY_VIDEO_AD_ERROR, // UNUSED
		TAPJOY_VIDEO_AD_READY, // UNUSED
		TOTAL
	};

					MOAITapjoyIOS		();
					~MOAITapjoyIOS		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			NotifyVideoAdBegin	();
	void			NotifyVideoAdClose	();
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