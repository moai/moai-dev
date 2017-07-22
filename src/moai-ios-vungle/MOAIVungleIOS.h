//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAIVUNGLEIOS_H
#define	MOAIVUNGLEIOS_H

#include <moai-core/headers.h>
#import <VungleSDK/VungleSDK.h>

@class MOAIVungleDelegate;

//================================================================//
// MOAIVungleIOS
//================================================================//
class MOAIVungleIOS :
	public MOAIGlobalClass < MOAIVungleIOS, MOAIGlobalEventSource > {
private:

	MOAIVungleDelegate*		mDelegate;
	
	//----------------------------------------------------------------//
	static int		_displayAdvert				( lua_State* L );
	static int		_init						( lua_State* L );
	static int		_isVideoAvailable			( lua_State* L );
	
public:
		
	enum {
		AD_VIEWED,
	};

	DECL_LUA_SINGLETON ( MOAIVungleIOS );

	//----------------------------------------------------------------//
						MOAIVungleIOS				();
						~MOAIVungleIOS				();
	MOAIVungleDelegate*	GetVungleDelegate			() const;
	void				NotifyMoviePlayed			( bool playedFull, NSString* placementID );
	void				RegisterLuaClass			( MOAILuaState& state );
};

//================================================================//
// MOAIVungleDelegate
//================================================================//
@interface MOAIVungleDelegate : NSObject < VungleSDKDelegate > {
@private
}
@end

#endif  //MOAIVUNGLEIOS_H
