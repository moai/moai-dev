//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios/headers.h>
#import <moai-ios-facebook/MOAIFacebookIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	extendToken
 @text	Extends the life of an active token. Should be called on app resume/start.
 
 @in		nil
 @out	nil
 */
int MOAIFacebookIOS::_extendToken ( lua_State* L ) {
	UNUSED ( L );
	printf ( "extending token....\n" );
	//[ MOAIFacebookIOS::Get ().mFacebook extendAccessTokenIfNeeded ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getExpirationDate
 @text	Retrieve the Facebook login token expiration date.
 
 @in		nil
 @out	string	token expiration date
 */
int MOAIFacebookIOS::_getExpirationDate ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSDateFormatter *formatter = [[[ NSDateFormatter alloc ] init ] autorelease ];
	[ formatter setDateFormat:@"dd-MM-yyyy HH:mm:ss" ];
	
	NSString* dateString = [ formatter stringFromDate:FBSession.activeSession.accessTokenData.expirationDate ];
	lua_pushstring ( L, [ dateString UTF8String ]);
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getToken
 @text	Retrieve the Facebook login token.
 
 @in		nil
 @out	string	token
 */
int MOAIFacebookIOS::_getToken ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* token = FBSession.activeSession.accessTokenData.accessToken;
	
	if ( token ) {
		lua_pushstring ( L, [ token UTF8String ]);
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	graphRequest
	@text	Make a request on Facebook's Graph API
 
	@in		string	path
	@opt	table	parameters
	@out	nil
 */
int MOAIFacebookIOS::_graphRequest ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* httpMethod = @"GET";
	
    cc8* path = state.GetValue < cc8* >( 1, "" );
	NSString* nsPath = [[[ NSString alloc ] initWithUTF8String:path ] autorelease ];
	
    NSMutableDictionary* paramsDict = [[[ NSMutableDictionary alloc ] init ] autorelease ];
	if ( state.IsTableOrUserdata ( 2 )) {
		[ paramsDict initWithLua:state stackIndex:2 ];
		httpMethod = @"POST";
	}
	
	[ FBRequestConnection startWithGraphPath:nsPath
		parameters:paramsDict
		HTTPMethod:httpMethod
		completionHandler:^( FBRequestConnection* connection, id result, NSError* error ) {
		
			UNUSED ( connection );
			
			if ( error ) {
				 MOAIFacebookIOS::Get ().ReceivedRequestResponseFailure ();
			}
			else {
				MOAIFacebookIOS::Get ().ReceivedRequestResponse ( result );
			}
		}
	];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFacebookIOS::_handleDidBecomeActive ( lua_State* L ) {
	UNUSED ( L );
	[ FBAppCall handleDidBecomeActiveWithSession:FBSession.activeSession ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFacebookIOS::_handleOpenUrl ( lua_State* L ) {

	MOAILuaState state ( L );
	
	NSURL* url						= [ NSURL URLWithString:[ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )]];
	NSString* sourceApplication		= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 2, "" )];

	[ FBAppCall handleOpenURL:url sourceApplication:sourceApplication ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	init
 @text	Initialize Facebook.
 
 @in		string	appId			Available in Facebook developer settings.
 @out 	nil
 */
int MOAIFacebookIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	[ FBSettings setDefaultAppID:[ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )]];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	login
 @text	Prompt the user to login to Facebook.
 
 @opt	table	permissions		Optional set of required permissions. See Facebook documentation for a full list. Default is nil.
 @out 	nil
 */
int MOAIFacebookIOS::_login ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	MOAIFacebookIOS& facebook = MOAIFacebookIOS::Get ();
	
	NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
		[ paramsDict initWithLua:state stackIndex:1 ];
	}
	NSArray* paramsArray = [ paramsDict allValues ];
	
	[ FBSession openActiveSessionWithReadPermissions:paramsArray
		allowLoginUI:YES
		completionHandler:^( FBSession *session, FBSessionState sessionState, NSError *error ) {
			UNUSED ( session );
			
			switch ( sessionState ) {
				case FBSessionStateOpen:
					facebook.SessionDidLogin ();
					break;
					
				case FBSessionStateClosed:
				case FBSessionStateClosedLoginFailed:
					
					[ FBSession.activeSession closeAndClearTokenInformation ];
					facebook.DialogDidNotComplete ();
					break;
					
				default:
					break;
			}

			if ( error) {
				facebook.DialogDidNotComplete ();
			}
		}
	];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	logout
 @text	Log the user out of Facebook.
 
 @in		nil
 @out 	nil
 */
int MOAIFacebookIOS::_logout ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	[ FBSession.activeSession closeAndClearTokenInformation ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	postToFeed
 @text	Post a message to the logged in users' news feed.
 
 @in		string	link			The URL that the post links to. See Facebook documentation.
 @in		string	picture			The URL of an image to include in the post. See Facebook documentation.
 @in		string	name			The name of the link. See Facebook documentation.
 @in		string	caption			The caption of the link. See Facebook documentation.
 @in		string	description		The description of the link. See Facebook documentation.
 @out 	nil
 */
int MOAIFacebookIOS::_postToFeed ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	FBShareDialogParams *shareParams = [[[FBShareDialogParams alloc ] init ] autorelease ];
	
	shareParams.link			= [ NSURL URLWithString:[ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )]];
	shareParams.picture			= [ NSURL URLWithString:[ NSString stringWithUTF8String:state.GetValue < cc8* >( 2, "" )]];
	shareParams.name			= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 3, "" )];
	shareParams.caption			= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 4, "" )];
	shareParams.description		= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 5, "" )];

	if ([ FBDialogs canPresentShareDialogWithParams:shareParams ]){

		[ FBDialogs presentShareDialogWithParams:shareParams
			clientState:nil
			handler:^( FBAppCall* call, NSDictionary *results, NSError *error ) {
				UNUSED ( call );
				
				if ( error ) {
					MOAIFacebookIOS::Get ().DialogDidNotComplete ();
				}
				else if ( results [ @"completionGesture" ] && [ results [ @"completionGesture" ] isEqualToString:@"cancel" ]) {
					MOAIFacebookIOS::Get ().DialogDidNotComplete ();
				}
				else {
					MOAIFacebookIOS::Get ().DialogDidNotComplete ();
				}
		}];
	}
	else {

		NSDictionary *params = @{
			@"name":shareParams.name,
			@"caption":shareParams.caption,
			@"description":shareParams.description,
			@"picture":shareParams.picture,
			@"link":shareParams.link,
		};

		// Invoke the dialog
		[ FBWebDialogs presentFeedDialogModallyWithSession:nil
			parameters:params
			handler:^( FBWebDialogResult result, NSURL* resultURL, NSError* error ) {
				UNUSED ( resultURL );
				
				if (error) {
					MOAIFacebookIOS::Get ().DialogDidNotComplete ();
				}
				else {
					if ( result == FBWebDialogResultDialogNotCompleted ) {
						MOAIFacebookIOS::Get ().DialogDidNotComplete ();
					}
					else {
						MOAIFacebookIOS::Get ().DialogDidNotComplete ();
					}
				}
			}
		];
    }
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFacebookIOS::_restoreSession ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	BOOL result = [ FBSession openActiveSessionWithAllowLoginUI:NO ];
	lua_pushboolean ( L, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	sendRequest
	@text	Send an app request to the logged in users' friends.
 
	@opt	string	message			The message for the request. See Facebook documentation. Default is nil.
	@out 	nil
 */
int MOAIFacebookIOS::_sendRequest ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* message = [ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )];
	
	NSMutableDictionary* params = nil;
	
	if ( state.IsType ( 2, LUA_TTABLE )) {
		
		NSMutableDictionary* friendDict = [[[ NSMutableDictionary alloc ] init ] autorelease ];
		[ friendDict initWithLua:state stackIndex:2 ];
		NSArray* targeted = [ friendDict allValues ];
		
		if ( targeted != nil && [ targeted count ] > 0 ) {
			params = [[[ NSMutableDictionary alloc ] init ] autorelease ];
			NSString* selectIDsStr = [ targeted componentsJoinedByString:@"," ];
			[ params setObject:selectIDsStr forKey:@"suggestions" ];
		}
	}
	
    [ FBWebDialogs presentRequestsDialogModallyWithSession:nil
		message:message
		title:nil
		parameters:params
		handler:^( FBWebDialogResult result, NSURL* resultURL, NSError* error ) {
			UNUSED ( resultURL );
		
			if ( error ) {
				MOAIFacebookIOS::Get ().DialogDidNotComplete ();
			}
			else {
				if (result == FBWebDialogResultDialogCompleted) {
					MOAIFacebookIOS::Get ().DialogDidComplete ();
				} else {
					MOAIFacebookIOS::Get ().DialogDidNotComplete ();
				}
			}
		}
	];

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	sessionValid
 @text	Determine whether or not the current Facebook session is valid.
 
 @in		nil
 @out 	boolean	valid
 */
int MOAIFacebookIOS::_sessionValid ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	lua_pushboolean ( state, [ FBSession.activeSession isOpen ]);
	return 1;
}
	
//================================================================//
// MOAIFacebookIOS
//================================================================//

//----------------------------------------------------------------//
MOAIFacebookIOS::MOAIFacebookIOS () {
    
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )
}

//----------------------------------------------------------------//
MOAIFacebookIOS::~MOAIFacebookIOS () {
    
	[ FBSession.activeSession close ];
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::RegisterLuaClass ( MOAILuaState& state ) {
    
	state.SetField ( -1, "DIALOG_DID_COMPLETE", 	( u32 )DIALOG_DID_COMPLETE );
	state.SetField ( -1, "DIALOG_DID_NOT_COMPLETE",	( u32 )DIALOG_DID_NOT_COMPLETE );
	state.SetField ( -1, "REQUEST_RESPONSE", 		( u32 )REQUEST_RESPONSE );
	state.SetField ( -1, "REQUEST_RESPONSE_FAILED", ( u32 )REQUEST_RESPONSE_FAILED );
	state.SetField ( -1, "SESSION_DID_LOGIN", 		( u32 )SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN", 	( u32 )SESSION_DID_NOT_LOGIN );
	state.SetField ( -1, "SESSION_EXTENDED",		( u32 )SESSION_EXTENDED );
	
	luaL_Reg regTable[] = {
		{ "extendToken",				_extendToken },
		{ "getExpirationDate",			_getExpirationDate },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIFacebookIOS > },
		{ "getToken",					_getToken },
		{ "graphRequest",				_graphRequest },
		{ "handleDidBecomeActive",		_handleDidBecomeActive },
		{ "handleOpenUrl",				_handleOpenUrl },
		{ "init",						_init },
		{ "login",						_login },
		{ "logout",						_logout },
		{ "postToFeed",					_postToFeed },
		{ "restoreSession",				_restoreSession },
		{ "sendRequest",				_sendRequest },
		{ "sessionValid",				_sessionValid },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIFacebookIOS > },
		{ NULL, NULL }	
	};
    
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::DialogDidNotComplete ( ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_NOT_COMPLETE, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::DialogDidComplete () {
		
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_COMPLETE, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::ReceivedRequestResponse ( id result ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ([ result isKindOfClass:[ NSString class ]]) {
		[(( NSString* )result ) toLua:state ];
	}
	else if ([ result isKindOfClass:[ NSData class ]]) {
		[(( NSData* )result ) toLua:state ];
	}
	
	if ( this->PushListener ( REQUEST_RESPONSE, state )) {
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::ReceivedRequestResponseFailure () {
    MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( REQUEST_RESPONSE_FAILED, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::SessionDidLogin () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_LOGIN, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::SessionDidNotLogin () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_NOT_LOGIN, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebookIOS::SessionExtended ( cc8* token, cc8* expDate ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_EXTENDED, state )) {
		state.Push ( token );		
		state.Push ( expDate );
		state.DebugCall ( 2, 0 );
	}
}
