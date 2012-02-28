//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIADCOLONY_H
#define MOAIADCOLONY_H

#include <moaicore/moaicore.h>
#import "FBConnect.h"

// Forward declare delegates
@class MoaiFBDialogDelegate;
@class MoaiFBRequestDelegate;
@class MoaiFBSessionDelegate;

//================================================================//
// MOAIAdColony
//================================================================//
class MOAIFacebook :
	public MOAIGlobalClass < MOAIFacebook, MOAILuaObject >,
	public MOAIGlobalEventSource {
private:
	
	Facebook*					mFacebook;
	MoaiFBDialogDelegate*		mFBDialogDelegate;
	MoaiFBSessionDelegate*		mFBSessionDelegate;
		
	STLString					mToken;
	STLString					mAppId;
    	
	//----------------------------------------------------------------//
	static int		_getToken						( lua_State* L );
	static int		_init							( lua_State* L );
	static int		_login							( lua_State* L );
	static int		_logout							( lua_State* L );
	static int		_postToFeed						( lua_State* L );
	static int		_sendRequest					( lua_State* L );
	static int		_setToken						( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIFacebook );
		
	enum {
		
		DIALOG_DID_CANCEL,
		DIALOG_DID_COMPLETE
	};
		
	void			DialogDidCancel					();
	void			DialogDidComplete				();
	void			HandleOpenURL					( NSURL* url );
                    MOAIFacebook					();
					~MOAIFacebook                   ();
	void			RegisterLuaClass				( MOAILuaState& state );
};

//================================================================//
// MoaiFBDialogDelegate
//================================================================//
@interface MoaiFBDialogDelegate : NSObject < FBDialogDelegate > {
@private
}
@end

//================================================================//
// MoaiFBRequestDelegate
//================================================================//
@interface MoaiFBRequestDelegate : NSObject < FBRequestDelegate > {
@private
}
@end

//================================================================//
// MoaiFBSessionDelegate
//================================================================//
@interface MoaiFBSessionDelegate : NSObject < FBSessionDelegate > {
@private
}
@end

#endif
