//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIADCOLONY_H
#define MOAIADCOLONY_H

#include <moaicore/moaicore.h>
#import "FBConnect.h"

// Forward declare delegate
@class MoaiFBSessionDelegate;

//================================================================//
// MOAIAdColony
//================================================================//
class MOAIFacebook :
	public MOAIGlobalClass < MOAIFacebook, MOAILuaObject >,
	public MOAIGlobalEventSource {
private:
	
	Facebook*					mFacebook;
	MoaiFBSessionDelegate*		mFBSessionDelegate;
		
	STLString					mToken;
    	
	//----------------------------------------------------------------//
	static int		_getToken						( lua_State* L );
	static int		_init							( lua_State* L );
	static int		_login							( lua_State* L );
	static int		_setToken						( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIFacebook );
	
	void			HandleOpenURL					( NSURL* url );
                    MOAIFacebook					();
					~MOAIFacebook                   ();
	void			RegisterLuaClass				( MOAILuaState& state );
};

//================================================================//
// MoaiLeaderboardDelegate
//================================================================//
@interface MoaiFBSessionDelegate : NSObject < FBSessionDelegate > {
@private
}
@end

#endif
