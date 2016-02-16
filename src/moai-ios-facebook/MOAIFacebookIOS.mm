//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios/headers.h>
#import <moai-ios-facebook/MOAIFacebookIOS.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getExpirationDate
 @text	Retrieve the Facebook login token expiration date.
 
 @in	nil
 @out	string	token expiration date
 */
int MOAIFacebookIOS::_getExpirationDate ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	NSDateFormatter *formatter = [[ NSDateFormatter alloc ] init ];
	[ formatter setDateFormat:@"dd-MM-yyyy HH:mm:ss" ];
	
	NSString* dateString = [ formatter stringFromDate:[[ FBSDKAccessToken currentAccessToken ] expirationDate ]];
	lua_pushstring ( L, [ dateString UTF8String ]);
	
	return 1;
}
//----------------------------------------------------------------//
/**	@lua	getToken
 @text	Retrieve the Facebook login token.
 
 @in	nil
 @out	string	token
 */
int MOAIFacebookIOS::_getToken ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	NSString* token = [[ FBSDKAccessToken currentAccessToken ] tokenString ];
	
	if ( token ) {
		lua_pushstring ( L, [ token UTF8String ]);
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getUserEmail
 @text	Retrieve the Facebook User email .
 
 @in	nil
 @out	string	User email
 */
int MOAIFacebookIOS::_getUserEmail ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	

	
	if ( MOAIFacebookIOS::Get ().mEmail != "" ) {
		lua_pushstring ( L, MOAIFacebookIOS::Get ().mEmail);
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}
//----------------------------------------------------------------//
/**	@lua	getUserID
 @text	Retrieve the Facebook User ID .
 
 @in	nil
 @out	string	User ID
 */
int MOAIFacebookIOS::_getUserID ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	NSString* ID = [[ FBSDKAccessToken currentAccessToken ] userID ];
	
	if ( ID ) {
		lua_pushstring ( L, [ ID UTF8String ]);
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}
//----------------------------------------------------------------//
/**	@lua	getUserName
 @text	Retrieve the Facebook User Name .
 
 @in	nil
 @out	string	User User Name
 */
int MOAIFacebookIOS::_getUserName ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	
	

	if ( MOAIFacebookIOS::Get ().mName != "") {
		lua_pushstring ( L, MOAIFacebookIOS::Get ().mName.c_str() );
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
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	if ( ![ FBSDKAccessToken currentAccessToken ]) {
		MOAIFacebookIOS::Get ().ReceivedRequestResponseFailure ();
		return 0;
	}
	
	BOOL post = NO;
	
    cc8* path = state.GetValue < cc8* >( 1, "" );
	NSString* nsPath = [[ NSString alloc ] initWithUTF8String:path ];
	
    NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
	if ( state.IsTableOrUserdata ( 2 )) {
		[ paramsDict initWithLua:state stackIndex:2 ];
		post = YES;
		
		if ( ![[ FBSDKAccessToken currentAccessToken ] hasGranted:@"publish_actions" ]) {
			MOAIFacebookIOS::Get ().ReceivedRequestResponseFailure ();
			return 0;
		}
	}
	
	[[[ FBSDKGraphRequest alloc] initWithGraphPath:nsPath parameters:paramsDict HTTPMethod:( post ? @"POST" : @"GET" )]
		startWithCompletionHandler:^( FBSDKGraphRequestConnection *connection, id result, NSError *error ) {
		
		UNUSED ( connection );
			
		if ( error ) {
			MOAIFacebookIOS::Get ().ReceivedRequestResponseFailure ();
		}
		else {
			MOAIFacebookIOS::Get ().ReceivedRequestResponse ( result );
			
		}
	}];
	
	return 0;
}
int MOAIFacebookIOS::_handleDidBecomeActive		( lua_State* L ) {
	UNUSED( L );
	
	[FBSDKAppEvents activateApp];
	return 0;
}
int MOAIFacebookIOS::_handleOpenUrl ( lua_State* L ) {
	
	UNUSED( L );
	return 0;
}
//----------------------------------------------------------------//
/**	@lua	init
 @text	Initialize Facebook.
 
 @in		string	appId			Available in Facebook developer settings.
 @out 	nil
 */
int MOAIFacebookIOS::_init ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	[ FBSDKSettings setAppID:[ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )]];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	login
 @text	Prompt the user to login to Facebook.
 
 @opt	table	permissions		Optional set of required permissions. See Facebook documentation for a full list. Default is nil.
 @out 	nil
 */
int MOAIFacebookIOS::_login ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
	if ( state.IsType ( 1, LUA_TTABLE )) {
		[ paramsDict initWithLua:state stackIndex:1 ];
	}
	NSArray* paramsArray = [ paramsDict allValues ];
	
	FBSDKLoginManager* login = [[ FBSDKLoginManager alloc] init ];
	[ login logInWithReadPermissions:paramsArray handler:^( FBSDKLoginManagerLoginResult *result, NSError *error ) {
	
		if ( error ) {
			self->DialogDidNotComplete ();
		}
		else if ( result.isCancelled ) {
		
			self->DialogDidNotComplete ();
		}
		else {
		
			self->SessionDidLogin ();
		
			// If you ask for multiple permissions at once, you
			// should check if specific permissions missing
			//if ([ result.grantedPermissions containsObject:@"email" ]) {
				// Do work
				
			//}
		}
	}];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	logout
	 @text	Log the user out of Facebook.
	 
	 @in	nil
	 @out 	nil
 */
int MOAIFacebookIOS::_logout ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	[ FBSDKAccessToken setCurrentAccessToken:nil ];
	return 0;
}

int MOAIFacebookIOS::_restoreSession ( lua_State* L ) {
	UNUSED( L );
	if ( [ FBSDKAccessToken currentAccessToken ]) {
		MOAIFacebookIOS::Get ().SessionDidLogin ();
	}
	
	return 1;
}
//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int MOAIFacebookIOS::_postToFeed ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
//	FBSDKShareLinkContent *content = [[FBSDKShareLinkContent alloc] init];
//	content.contentURL = [NSURL URLWithString:@"https://developers.facebook.com"];
//	
//	content.contentURL				= [ NSURL URLWithString:[ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )]];
//	content.imageURL				= [ NSURL URLWithString:[ NSString stringWithUTF8String:state.GetValue < cc8* >( 2, "" )]];
//	content.contentTitle			= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 3, "" )];
//	content.contentDescription		= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 4, "" )];
//
//	[ FBDialogs presentShareDialogWithParams:shareParams
//		clientState:nil
//		handler:^( FBAppCall* call, NSDictionary *results, NSError *error ) {
//			UNUSED ( call );
//			
//			if ( error ) {
//				MOAIFacebookIOS::Get ().DialogDidNotComplete ();
//			}
//			else if ( results [ @"completionGesture" ] && [ results [ @"completionGesture" ] isEqualToString:@"cancel" ]) {
//				MOAIFacebookIOS::Get ().DialogDidNotComplete ();
//			}
//			else {
//				MOAIFacebookIOS::Get ().DialogDidNotComplete ();
//			}
//	}];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	sendRequest
	@text	Send an app request to the logged in users' friends.
 
	@opt	string	message			The message for the request. See Facebook documentation. Default is nil.
	@out 	nil
 */
int MOAIFacebookIOS::_sendRequest ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
//	NSString* message = [ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )];
//	
//	NSMutableDictionary* params = nil;
//	
//	if ( state.IsType ( 2, LUA_TTABLE )) {
//		
//		NSMutableDictionary* friendDict = [[[ NSMutableDictionary alloc ] init ] autorelease ];
//		[ friendDict initWithLua:state stackIndex:2 ];
//		NSArray* targeted = [ friendDict allValues ];
//		
//		if ( targeted != nil && [ targeted count ] > 0 ) {
//			params = [[[ NSMutableDictionary alloc ] init ] autorelease ];
//			NSString* selectIDsStr = [ targeted componentsJoinedByString:@"," ];
//			[ params setObject:selectIDsStr forKey:@"suggestions" ];
//		}
//	}
//	
//    [ FBWebDialogs presentRequestsDialogModallyWithSession:nil
//		message:message
//		title:nil
//		parameters:params
//		handler:^( FBWebDialogResult result, NSURL* resultURL, NSError* error ) {
//			UNUSED ( resultURL );
//		
//			if ( error ) {
//				MOAIFacebookIOS::Get ().DialogDidNotComplete ();
//			}
//			else {
//				if (result == FBWebDialogResultDialogCompleted) {
//					MOAIFacebookIOS::Get ().DialogDidComplete ();
//				} else {
//					MOAIFacebookIOS::Get ().DialogDidNotComplete ();
//				}
//			}
//		}
//	];

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	sessionValid
 @text	Determine whether or not the current Facebook session is valid.
 
 @in		nil
 @out 	boolean	valid
 */
int MOAIFacebookIOS::_sessionValid ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFacebookIOS, "" )
	
	lua_pushboolean ( state, [ FBSDKAccessToken currentAccessToken ] != nil );
	return 1;
}
	
//================================================================//
// MOAIFacebookIOS
//================================================================//

//----------------------------------------------------------------//
MOAIFacebookIOS::MOAIFacebookIOS ():
	mName(""),mEmail(0) {
    
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	
}

//----------------------------------------------------------------//
MOAIFacebookIOS::~MOAIFacebookIOS () {
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
		{ "getExpirationDate",			_getExpirationDate },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIFacebookIOS > },
		{ "getToken",					_getToken },
		{ "getUserID",					_getUserID },
		{ "getUserName",				_getUserName },
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
void MOAIFacebookIOS::DialogDidNotComplete () {
	
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
	
	if ([FBSDKAccessToken currentAccessToken]) {
		[[[FBSDKGraphRequest alloc] initWithGraphPath:@"me" parameters:@{@"fields": @"email,name"}]
		 startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
			 UNUSED ( connection );
			 if (!error) {
				 
				 this->mName.clear();
				 if( [ result[ @"name" ]  UTF8String ] )
				 this->mName.write( [ result[ @"name" ]  UTF8String ]);
				 this->mEmail.clear();
				 if( [ result[ @"email" ]  UTF8String ] )
					 this->mEmail.write( [ result[ @"email" ]  UTF8String ] );
			 }
		 }];
		
		
	}
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
