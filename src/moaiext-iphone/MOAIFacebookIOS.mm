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
/**	@name	extendToken
 @text	Extends the life of an active token. Should be called on app resume/start.
 
 @in		nil
 @out	nil
 */
int MOAIFacebookIOS::_extendToken ( lua_State* L ) {
	
	printf ( "extending token....\n" );
	[ MOAIFacebookIOS::Get ().mFacebook extendAccessTokenIfNeeded ];
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getExpirationDate
 @text	Retrieve the Facebook login token expiration date.
 
 @in		nil
 @out	string	token expiration date
 */
int MOAIFacebookIOS::_getExpirationDate ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSDateFormatter *formatter = [[ NSDateFormatter alloc ] init ];
	[ formatter setDateFormat:@"dd-MM-yyyy HH:mm:ss" ];
	NSString *dateString = [ formatter stringFromDate:[ MOAIFacebookIOS::Get ().mFacebook expirationDate ]];
	[ formatter release ];
	
	MOAIFacebookIOS::Get ().mExpirationDate = [ dateString UTF8String ];
	if ( !MOAIFacebookIOS::Get ().mExpirationDate.empty ()) {
		lua_pushstring ( L, MOAIFacebookIOS::Get ().mExpirationDate.c_str ());
	} else {
		
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getToken
 @text	Retrieve the Facebook login token.
 
 @in		nil
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
/**	@name	graphRequest
 @text	Make a request on Facebook's Graph API
 
 @in		string	path
 @out	nil
 */
int MOAIFacebookIOS::_graphRequest ( lua_State* L ) {
	
	MOAILuaState state ( L );
	cc8* path = state.GetValue < cc8* >( 1, "" );
	NSString* nsPath = [[[ NSString alloc ] initWithUTF8String:path ] autorelease ];
	[ MOAIFacebookIOS::Get ().mFacebook requestWithGraphPath:nsPath andDelegate:MOAIFacebookIOS::Get ().mFBRequestDelegate ];	
	
	return 0;
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
 
 @in		nil
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
	
	if ( state.IsType ( 2, LUA_TTABLE )) {
		
		NSMutableDictionary* friendDict = [[ NSMutableDictionary alloc ] init ];		
		[ friendDict initWithLua:state stackIndex:2 ];
		NSArray* targeted = [ friendDict allValues ];
		
		
		if ( targeted != nil && [ targeted count ] > 0 ) {
			NSString* selectIDsStr = [ targeted componentsJoinedByString:@","];
			[ params setObject:selectIDsStr forKey:@"suggestions" ];
		}		
	}	
	
	[ MOAIFacebookIOS::Get ().mFacebook dialog:@"apprequests" andParams:params andDelegate:MOAIFacebookIOS::Get ().mFBDialogDelegate ];
	
	return 0;
}


//----------------------------------------------------------------//
/**	@name	sessionValid
 @text	Determine whether or not the current Facebook session is valid.
 
 @in		nil
 @out 	boolean	valid
 */
int MOAIFacebookIOS::_sessionValid ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	lua_pushboolean ( state, [ MOAIFacebookIOS::Get ().mFacebook isSessionValid ]);
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setExpirationDate
 @text	Set the Facebook login token expiration date.
 
 @in		string	expirationDate			The login token expiration date. See Facebook documentation.
 @out 	nil
 */
int MOAIFacebookIOS::_setExpirationDate ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* expirationDate = state.GetValue < cc8* >( 1, "" );
	
	NSString *dateString = [[ NSString alloc ] initWithUTF8String:expirationDate ];
	NSDateFormatter *formatter = [[ NSDateFormatter alloc ] init ];
	[ formatter setDateFormat:@"dd-MM-yyyy HH:mm:ss" ];
	NSDate *date = [ formatter dateFromString:dateString ];
	
	MOAIFacebookIOS::Get ().mExpirationDate = expirationDate;
	MOAIFacebookIOS::Get ().mFacebook.expirationDate = date;
	
	[ dateString release ];
	[ formatter release ];
	
	return 0;
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
	
	mFBSessionDelegate = [[ MOAIFacebookIOSSessionDelegate alloc ] init];
	mFBDialogDelegate = [[ MOAIFacebookIOSDialogDelegate alloc ] init ];
	mFBRequestDelegate = [[ MOAIFacebookIOSRequestDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIFacebookIOS::~MOAIFacebookIOS () {
    
	[ mFBSessionDelegate release ];	
	[ mFBDialogDelegate release ];
	[ mFBRequestDelegate release ];
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::RegisterLuaClass ( MOAILuaState& state ) {
    
	state.SetField ( -1, "DIALOG_DID_COMPLETE", 	( u32 )DIALOG_DID_COMPLETE );
	state.SetField ( -1, "DIALOG_DID_NOT_COMPLETE",	( u32 )DIALOG_DID_NOT_COMPLETE );
	state.SetField ( -1, "REQUEST_RESPONSE", 		( u32 )REQUEST_RESPONSE );
	state.SetField ( -1, "SESSION_DID_LOGIN", 		( u32 )SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN", 	( u32 )SESSION_DID_NOT_LOGIN );
	state.SetField ( -1, "SESSION_EXTENDED",		( u32 )SESSION_EXTENDED );
	
	luaL_Reg regTable[] = {
		{ "extendToken",			_extendToken },
		{ "getExpirationDate",		_getExpirationDate },
		{ "getToken",				_getToken },
		{ "graphRequest",			_graphRequest },
		{ "init",					_init },
		{ "login",					_login },
		{ "logout",					_logout },
		{ "postToFeed",				_postToFeed },
		{ "sendRequest",			_sendRequest },
		{ "sessionValid",			_sessionValid },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIFacebookIOS > },
		{ "setToken",				_setToken },
		{ "setExpirationDate",		_setExpirationDate },
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
void MOAIFacebookIOS::ReceivedRequestResponse ( NSData * response ) {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( REQUEST_RESPONSE, state )) {

		[response toLua:state];
		state.DebugCall ( 1, 0 );
	}
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

//----------------------------------------------------------------//
void MOAIFacebookIOS::SessionExtended ( cc8* token, cc8* expDate ) {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_EXTENDED, state )) {
		
		state.Push ( token );		
		state.Push ( expDate );
		state.DebugCall ( 2, 0 );
	}
}

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
	
	NSDateFormatter *formatter = [[ NSDateFormatter alloc ] init ];
	[ formatter setDateFormat:@"dd-MM-yyyy HH:mm:ss" ];
	NSString *dateString = [ formatter stringFromDate:expiresAt ];
	[ formatter release ];
	
	MOAIFacebookIOS::Get ().SessionExtended( [ accessToken UTF8String ], [ dateString UTF8String ]);
}

- (void) fbDidLogout {
}

- (void) fbSessionInvalidated {
}

@end

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
// MOAIFacebookIOSRequestDelegate
//================================================================//
@implementation MOAIFacebookIOSRequestDelegate

//================================================================//
#pragma mark -
#pragma mark Protocol MOAIFacebookIOSRequestDelegate
//================================================================//

- ( void )request:( FBRequest* )request didLoadRawResponse:( NSData * )response {
	
	MOAIFacebookIOS::Get ().ReceivedRequestResponse ( response );
}

@end

