//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIFACEBOOKIOS_H
#define MOAIFACEBOOKIOS_H

#import <Foundation/Foundation.h> 
#import <moai-core/headers.h>    

@class MOAIFacebookIOSDialogDelegate;
@class MOAIFacebookIOSRequestDelegate;
@class MOAIFacebookIOSSessionDelegate;

//================================================================//
// MOAIFacebookIOS
//================================================================//
/**	@lua	MOAIFacebookIOS
	@text	Wrapper for Facebook integration on iOS devices.
			Facebook provides social integration for sharing on
			www.facebook.com. Exposed to Lua via MOAIFacebook on 
			all mobile platforms.

	@const	DIALOG_DID_COMPLETE			Event code for a successfully completed Facebook dialog.
	@const	DIALOG_DID_NOT_COMPLETE		Event code for a failed (or canceled) Facebook dialog.
	@const	SESSION_DID_LOGIN			Event code for a successfully completed Facebook login.
	@const	SESSION_DID_NOT_LOGIN		Event code for a failed (or canceled) Facebook login.
	@const	REQUEST_RESPONSE			Event code for graph request responses.
	@const	REQUEST_RESPONSE_FAILED		Event code for failed graph request responses.
*/
class MOAIFacebookIOS :
	public ZLContextClass < MOAIFacebookIOS, MOAILuaObject >,
	public MOAIGlobalEventSource {
private:
	
	//----------------------------------------------------------------//
	static int		_getExpirationDate			( lua_State* L );
	static int		_getUserEmail				( lua_State* L );
	static int		_getUserID					( lua_State* L );
	static int		_getUserName				( lua_State* L );
	static int		_getToken					( lua_State* L );
	static int		_graphRequest				( lua_State* L );
	static int		_handleDidBecomeActive		( lua_State* L );
	static int      _handleOpenUrl				( lua_State* L );
	static int		_init						( lua_State* L );
	static int		_login						( lua_State* L );
	static int		_logout						( lua_State* L );
	static int		_postToFeed					( lua_State* L );
	static int		_restoreSession				( lua_State* L );
	static int		_sendRequest				( lua_State* L );
	static int		_sessionValid				( lua_State* L );
	
		
	STLString			mName;
	STLString			mEmail;
public:
    
	DECL_LUA_SINGLETON ( MOAIFacebookIOS );
	
	enum {
		DIALOG_DID_COMPLETE,
		DIALOG_DID_NOT_COMPLETE,
		REQUEST_RESPONSE,
		REQUEST_RESPONSE_FAILED,
		SESSION_DID_LOGIN,
		SESSION_DID_NOT_LOGIN,
		SESSION_EXTENDED
	};
	
	//----------------------------------------------------------------//
	void		DialogDidNotComplete	        ();
	void		DialogDidComplete		        ();
				MOAIFacebookIOS			        ();
				~MOAIFacebookIOS		        ();
	void		RegisterLuaClass		        ( MOAILuaState& state );
	void		ReceivedRequestResponse	        ( id result );
	void		ReceivedRequestResponseFailure	();
	void		SessionDidLogin			        ();
	void		SessionDidNotLogin		        ();
	void		SessionExtended			        ( cc8* token, cc8* expDate );
};

#endif // MOAIFACEBOOK_H
