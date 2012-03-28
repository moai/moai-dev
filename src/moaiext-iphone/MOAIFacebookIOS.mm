//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MOAIFacebookIOS.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getToken
	@text	Retrieve the Facebook login token.
				
	@out	string	token
*/
int MOAIFacebookIOS::_getToken ( lua_State* L ) {
	
	MOAILuaState state ( L );
		
	MOAIFacebookIOS::Get ().mToken = [[ MOAIFacebookIOS::Get ().mFacebook accessToken ] UTF8String ];
	if ( !MOAIFacebookIOS::Get ().mToken.empty ()) {
		
		lua_pushstring ( L, MOAIFacebookIOS::Get ().mToken.c_str ());
	} else {
		
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Facebook.
				
	@in		string	appId			Available in Facebook developer settings.
	@out 	nil
*/
int MOAIFacebookIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
         
	cc8* appID = state.GetValue < cc8* >( 1, "" );
	MOAIFacebookIOS::Get ().mAppId = appID;
	
	MOAIFacebookIOS::Get ().mFacebook = [[ Facebook alloc ] initWithAppId: [[ NSString alloc ] initWithUTF8String: appID ] andDelegate: MOAIFacebookIOS::Get ().mFBSessionDelegate ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	login
	@text	Prompt the user to login to Facebook.
				
	@opt	table	permissions		Optional set of required permissions. See Facebook documentation for a full list. Default is nil.
	@out 	nil
*/
int MOAIFacebookIOS::_login ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
		
		NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
		[ paramsDict initWithLua:state stackIndex:1 ];
		
		NSArray* paramsArray = [ paramsDict allValues ];
		
		if ( ![ MOAIFacebookIOS::Get ().mFacebook isSessionValid ]) {
			
			[ MOAIFacebookIOS::Get ().mFacebook authorize:paramsArray ];
		}
		
	} else {
		
		if ( ![ MOAIFacebookIOS::Get ().mFacebook isSessionValid ]) {
			
			[ MOAIFacebookIOS::Get ().mFacebook authorize:nil ];
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	logout
	@text	Log the user out of Facebook.
				
	@out 	nil
*/
int MOAIFacebookIOS::_logout ( lua_State* L ) {
	
	MOAILuaState state ( L );
		
	[ MOAIFacebookIOS::Get ().mFacebook logout ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	postToFeed
	@text	Post a message to the logged in users' news feed.
				
	@in		string	link			The URL that the post links to. See Facebook documentation.
	@in		string	picture			The URL of an image to include in the post. See Facebook documentation.
	@in		string	name			The name of the link. See Facebook documentation.
	@in		string	caption			The caption of the link. See Facebook documentation.
	@in		string	description		The description of the link. See Facebook documentation.
	@in		string	message			The message for the post. See Facebook documentation.
	@out 	nil
*/
int MOAIFacebookIOS::_postToFeed ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* link = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	NSString* pic = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 2, "" ) ];
	NSString* name = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 3, "" ) ];
	NSString* caption = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 4, "" ) ];
	NSString* desc = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 5, "" ) ];
	NSString* msg = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 6, "" ) ];
	
	NSString* appId = [[ NSString alloc ] initWithUTF8String:MOAIFacebookIOS::Get ().mAppId.c_str() ];
	
	NSMutableDictionary* params = [ NSMutableDictionary dictionaryWithObjectsAndKeys:
									 appId, @"app_id",
									  link, @"link",
									   pic, @"picture",
									  name, @"name",
								   caption, @"caption",
									  desc, @"description",
									   msg, @"message",
										nil ];
	
	[ MOAIFacebookIOS::Get ().mFacebook dialog:@"feed" andParams:params andDelegate:MOAIFacebookIOS::Get ().mFBDialogDelegate ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	sendRequest
	@text	Send an app request to the logged in users' friends.
				
	@opt	string	message			The message for the request. See Facebook documentation. Default is nil.
	@out 	nil
*/
int MOAIFacebookIOS::_sendRequest ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* msg = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	
	NSMutableDictionary* params = [ NSMutableDictionary dictionaryWithObjectsAndKeys: msg, @"message", nil ];
	
	[ MOAIFacebookIOS::Get ().mFacebook dialog:@"apprequests" andParams:params andDelegate:MOAIFacebookIOS::Get ().mFBDialogDelegate ];
	
	return 0;
}


//----------------------------------------------------------------//
/**	@name	sessionValid
	@text	Determine whether or not the current Facebook session is valid.
				
	@out 	boolean	valid
*/
int MOAIFacebookIOS::_sessionValid ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	lua_pushboolean ( state, [ MOAIFacebookIOS::Get ().mFacebook isSessionValid ]);
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setToken
	@text	Set the Facebook login token.
			
	@in		string	token			The login token. See Facebook documentation.
	@out 	nil
*/
int MOAIFacebookIOS::_setToken ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* token = state.GetValue < cc8* >( 1, "" );
	
	MOAIFacebookIOS::Get ().mToken = token;
	MOAIFacebookIOS::Get ().mFacebook.accessToken = [[ NSString alloc ] initWithUTF8String:token ];
	
	return 0;
}
	
//================================================================//
// MOAIFacebookIOS
//================================================================//

//----------------------------------------------------------------//
MOAIFacebookIOS::MOAIFacebookIOS () {
    
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )	
	
	mFBDialogDelegate = [[ MOAIFacebookIOSDialogDelegate alloc ] init ];
	mFBSessionDelegate = [[ MOAIFacebookIOSSessionDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIFacebookIOS::~MOAIFacebookIOS () {
    
	[ mFBDialogDelegate release ];
	[ mFBSessionDelegate release ];
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::RegisterLuaClass ( MOAILuaState& state ) {
    
	state.SetField ( -1, "DIALOG_DID_COMPLETE", 	( u32 )DIALOG_DID_COMPLETE );
	state.SetField ( -1, "DIALOG_DID_NOT_COMPLETE",	( u32 )DIALOG_DID_NOT_COMPLETE );
	state.SetField ( -1, "SESSION_DID_LOGIN", 		( u32 )SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN", 	( u32 )SESSION_DID_NOT_LOGIN );
	
	luaL_Reg regTable[] = {
		{ "getToken",		_getToken },
		{ "init",			_init },
		{ "login",			_login },
		{ "logout",			_logout },
		{ "postToFeed",		_postToFeed },
		{ "sendRequest",	_sendRequest },
		{ "sessionValid",	_sessionValid },
		{ "setListener",	&MOAIGlobalEventSource::_setListener < MOAIFacebookIOS > },
		{ "setToken",		_setToken },
		{ NULL, NULL }	
	};
    
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::DialogDidNotComplete ( ) {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_NOT_COMPLETE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::DialogDidComplete ( ) {
		
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_COMPLETE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::HandleOpenURL ( NSURL* url ) {
	
	[ mFacebook handleOpenURL:url ];
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::SessionDidLogin () {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::SessionDidNotLogin () {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_NOT_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// MOAIFacebookIOSDialogDelegate
//================================================================//
@implementation MOAIFacebookIOSDialogDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MOAIFacebookIOSDialogDelegate
	//================================================================//

	- ( void ) dialogCompleteWithUrl:(NSURL *)url {
		
		if ([ url query ]) {

			MOAIFacebookIOS::Get ().DialogDidComplete ();
		} else {
			
			MOAIFacebookIOS::Get ().DialogDidNotComplete ();
		}
	}

	- ( void ) dialogDidComplete: (FBDialog *)dialog {
		
		UNUSED ( dialog );
		
		// NOT USED DUE TO A BUG IN FACEBOOK SDK
		//MOAIFacebookIOS::Get ().DialogDidComplete ();
	}

	- ( void ) dialogDidNotComplete: (FBDialog *)dialog {
		
		UNUSED ( dialog );

		MOAIFacebookIOS::Get ().DialogDidNotComplete ();
	}

@end

//================================================================//
// MOAIFacebookIOSSessionDelegate
//================================================================//
@implementation MOAIFacebookIOSSessionDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MOAIFacebookIOSSessionDelegate
	//================================================================//

	- ( void ) fbDidLogin {
		
		MOAIFacebookIOS::Get ().SessionDidLogin ();
	}

	- ( void ) fbDidNotLogin:( BOOL )cancelled {
		
		UNUSED ( cancelled );
		
		MOAIFacebookIOS::Get ().SessionDidNotLogin ();
	}

	- (void) fbDidExtendToken:( NSString* )accessToken expiresAt:( NSDate* )expiresAt {
		
		UNUSED ( accessToken );
		UNUSED ( expiresAt );
	}

	- (void) fbDidLogout {
		
	}

	- (void) fbSessionInvalidated {
		
	}
	
@end