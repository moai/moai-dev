// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITAPJOY_H
#define	MOAITAPJOY_H

#ifndef DISABLE_TAPJOY

#import <Foundation/Foundation.h> 
#import <moaicore/moaicore.h>
#import <Components/TJCVideoAds/TJCVideoAdProtocol.h>

@class MoaiTapjoyVideoAdDelegate;

//================================================================//
// MOAITapjoy
//================================================================//
/**	@name	MOAITapjoy
	@text	Wrapper for iOS Tapjoy functionality

	@const	TAPJOY_VIDEO_AD_BEGIN		Event code for Tapjoy video ads beginning.
	@const	TAPJOY_VIDEO_AD_CLOSE		Event code for Tapjoy video ads closing.
*/

class MOAITapjoy :
	public USGlobalClass < MOAITapjoy, MOAIGlobalEventSource > {
private:

	MoaiTapjoyVideoAdDelegate* mVideoAdDelegate;

	//----------------------------------------------------------------//
	static int		_initVideoAds					( lua_State* L );
	static int		_requestTapjoyConnect			( lua_State* L );
	static int		_showOffers						( lua_State* L );	
	
public:

	DECL_LUA_SINGLETON ( MOAITapjoy );
	
	enum {
		
		TAPJOY_VIDEO_AD_BEGIN,
		TAPJOY_VIDEO_AD_CLOSE
	};

					MOAITapjoy						();
					~MOAITapjoy						();
	void			RegisterLuaClass				( USLuaState& state );
	void			SendVideoAdBeginEvent			();
	void			SendVideoAdCloseEvent			();
};

//================================================================//
// MoaiTapjoyDelegate
//================================================================//
@interface MoaiTapjoyVideoAdDelegate : NSObject < TJCVideoAdDelegate > {
@private
}
@end

#endif  //DISABLE_TAPJOY

#endif  //MOAITAPJOY_H
