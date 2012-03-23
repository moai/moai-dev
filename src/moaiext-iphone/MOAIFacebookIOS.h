//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIFACEBOOK_H
#define MOAIFACEBOOK_H

#ifndef DISABLE_FACEBOOK

#import <Foundation/Foundation.h> 
#import <moaicore/moaicore.h>

#import "FBConnect.h"

@class MOAIFacebookIOSDialogDelegate;
@class MOAIFacebookIOSSessionDelegate;

//================================================================//
// MOAIFacebookIOS
//================================================================//
/**	@name	MOAIFacebookIOS
	@text	Wrapper for Facebook integration on iOS devices.
			Facebook provides social integration for sharing on
			www.facebook.com. Exposed to lua via MOAIFacebook on 
			all mobile platforms.

	@const	DIALOG_DID_COMPLETE			Event code for a successfully completed Facebook dialog.
	@const	DIALOG_DID_NOT_COMPLETE		Event code for a failed (or canceled) Facebook dialog.
	@const	SESSION_DID_LOGIN			Event code for a successfully completed Facebook login.
	@const	SESSION_DID_NOT_LOGIN		Event code for a failed (or canceled) Facebook login.
*/
class MOAIFacebookIOS :
	public MOAIGlobalClass < MOAIFacebookIOS, MOAILuaObject >,
	public MOAIGlobalEventSource {
private:
	
	Facebook*						mFacebook;
	MOAIFacebookIOSDialogDelegate*	mFBDialogDelegate;
	MOAIFacebookIOSSessionDelegate*	mFBSessionDelegate;
		
	STLString					mToken;
	STLString					mAppId;
    	
	//----------------------------------------------------------------//
	static int	_getToken		( lua_State* L );
	static int	_init			( lua_State* L );
	static int	_login			( lua_State* L );
	static int	_logout			( lua_State* L );
	static int	_postToFeed		( lua_State* L );
	static int	_sendRequest	( lua_State* L );
	static int	_sessionValid	( lua_State* L );
	static int	_setToken		( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIFacebookIOS );
		
	enum {
		DIALOG_DID_COMPLETE,
		DIALOG_DID_NOT_COMPLETE,
		SESSION_DID_LOGIN,
		SESSION_DID_NOT_LOGIN
	};
		
    		MOAIFacebookIOS			();
			~MOAIFacebookIOS		();
	void	DialogDidNotComplete	();
	void	DialogDidComplete		();
	void	HandleOpenURL			( NSURL* url );
	void	RegisterLuaClass		( MOAILuaState& state );
	void	SessionDidLogin			();
	void	SessionDidNotLogin		();
};

//================================================================//
// MOAIFacebookIOSDialogDelegate
//================================================================//
@interface MOAIFacebookIOSDialogDelegate : NSObject < FBDialogDelegate > {
@private
}
@end

//================================================================//
// MOAIFacebookIOSSessionDelegate
//================================================================//
@interface MOAIFacebookIOSSessionDelegate : NSObject < FBSessionDelegate > {
@private
}
@end

#endif  //DISABLE_FACEBOOK

#endif // MOAIFACEBOOK_H