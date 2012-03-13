//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MOAIFacebook.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIFacebook::_getToken ( lua_State* L ) {
	MOAILuaState state ( L );
		
	MOAIFacebook::Get ().mToken = [[ MOAIFacebook::Get ().mFacebook accessToken ] UTF8String ];
	if ( !MOAIFacebook::Get ().mToken.empty ()) {
		
		lua_pushstring ( L, MOAIFacebook::Get ().mToken.c_str ());
	} else {
		
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
int MOAIFacebook::_init ( lua_State* L ) {
	MOAILuaState state ( L );
         
	cc8* appID = state.GetValue < cc8* >( 1, "" );
	MOAIFacebook::Get ().mAppId = appID;
	
	
	MOAIFacebook::Get ().mFacebook = [[ Facebook alloc ] initWithAppId: [[ NSString alloc ] initWithUTF8String: appID ]
														   andDelegate: MOAIFacebook::Get ().mFBSessionDelegate ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_login ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
		
		NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
		[ paramsDict initWithLua:state stackIndex:1 ];
		
		NSArray* paramsArray = [ paramsDict allValues ];
		
		if ( ![ MOAIFacebook::Get ().mFacebook isSessionValid ]) {
			[ MOAIFacebook::Get ().mFacebook authorize:paramsArray ];
		}
		
	} else {
		
		if ( ![ MOAIFacebook::Get ().mFacebook isSessionValid ]) {
			[ MOAIFacebook::Get ().mFacebook authorize:nil ];
		}
	}
	
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_logout ( lua_State* L ) {
	MOAILuaState state ( L );
		
	[ MOAIFacebook::Get ().mFacebook logout ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_postToFeed ( lua_State* L ) {
	MOAILuaState state ( L );
	
	NSString* link = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	NSString* pic = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 2, "" ) ];
	NSString* name = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 3, "" ) ];
	NSString* caption = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 4, "" ) ];
	NSString* desc = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 5, "" ) ];
	NSString* msg = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 6, "" ) ];
	
	NSString* appId = [[ NSString alloc ] initWithUTF8String:MOAIFacebook::Get ().mAppId.c_str() ];
	
	NSMutableDictionary* params = [ NSMutableDictionary dictionaryWithObjectsAndKeys:
									 appId, @"app_id",
									  link, @"link",
									   pic, @"picture",
									  name, @"name",
								   caption, @"caption",
									  desc, @"description",
									   msg, @"message",
										nil ];
	
	[ MOAIFacebook::Get ().mFacebook dialog:@"feed" andParams:params andDelegate:MOAIFacebook::Get ().mFBDialogDelegate ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_sendRequest ( lua_State* L ) {
	MOAILuaState state ( L );
	
	NSString* msg = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	
	NSMutableDictionary* params = [ NSMutableDictionary dictionaryWithObjectsAndKeys:
								       msg, @"message",
								        nil ];
	
	[ MOAIFacebook::Get ().mFacebook dialog:@"apprequests" andParams:params andDelegate:MOAIFacebook::Get ().mFBDialogDelegate ];
	
	return 0;
}


//----------------------------------------------------------------//
int MOAIFacebook::_sessionValid ( lua_State* L ) {
	MOAILuaState state ( L );
	
	lua_pushboolean( state, [ MOAIFacebook::Get ().mFacebook isSessionValid ]);
	
	return 1;
}

//----------------------------------------------------------------//
int MOAIFacebook::_setToken ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* token = state.GetValue < cc8* >( 1, "" );
	
	if ( token != "" ) {

		MOAIFacebook::Get ().mToken = token;
		MOAIFacebook::Get ().mFacebook.accessToken = [[ NSString alloc ] initWithUTF8String:token ];
	}
	
	return 0;
}
	
//================================================================//
// MOAIFacebook
//================================================================//

//----------------------------------------------------------------//
void MOAIFacebook::DialogDidNotComplete ( ) {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_NOT_COMPLETE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebook::DialogDidComplete ( ) {
		
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( DIALOG_DID_COMPLETE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebook::HandleOpenURL ( NSURL* url ) {
	
	[ mFacebook handleOpenURL:url ];
}

//----------------------------------------------------------------//
MOAIFacebook::MOAIFacebook () {
    
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )	
	
	mFBDialogDelegate = [ MoaiFBDialogDelegate alloc ];
	mFBSessionDelegate = [ MoaiFBSessionDelegate alloc ];
}

//----------------------------------------------------------------//
MOAIFacebook::~MOAIFacebook () {
    
	[ mFBDialogDelegate release ];
	[ mFBSessionDelegate release ];
}

//----------------------------------------------------------------//
void MOAIFacebook::RegisterLuaClass ( MOAILuaState& state ) {
    
	state.SetField ( -1, "DIALOG_DID_COMPLETE", ( u32 )DIALOG_DID_COMPLETE );
	state.SetField ( -1, "DIALOG_DID_NOT_COMPLETE", ( u32 )DIALOG_DID_NOT_COMPLETE );
	state.SetField ( -1, "SESSION_DID_LOGIN", ( u32 )SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN", ( u32 )SESSION_DID_NOT_LOGIN );
	
	luaL_Reg regTable[] = {
		{ "getToken",				_getToken },
		{ "init",					_init },
		{ "login",					_login },
		{ "logout",					_logout },
		{ "postToFeed",				_postToFeed },
		{ "sendRequest",			_sendRequest },
		{ "sessionValid",			_sessionValid },
		{ "setToken",				_setToken },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIFacebook > },
		{ NULL, NULL }	
	};
    
	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFacebook::SessionDidLogin () {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFacebook::SessionDidNotLogin () {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( SESSION_DID_NOT_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// MoaiFBDialogDelegate
//================================================================//
@implementation MoaiFBDialogDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MoaiFBDialogDelegate
	//================================================================//

	- ( void ) dialogCompleteWithUrl:(NSURL *)url {
		
		if ([ url query ]) {

			MOAIFacebook::Get ().DialogDidComplete ();
		} else {
			
			MOAIFacebook::Get ().DialogDidNotComplete ();
		}
	}

	- ( void ) dialogDidComplete: (FBDialog *)dialog {
		
		// NOT USED DUE TO A BUG IN FACEBOOK SDK
		//MOAIFacebook::Get ().DialogDidComplete ();
	}

	- ( void ) dialogDidNotComplete: (FBDialog *)dialog {
		
		MOAIFacebook::Get ().DialogDidNotComplete ();
	}

@end

//================================================================//
// MoaiFBSessionDelegate
//================================================================//
@implementation MoaiFBSessionDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol FBSessionDelegate
	//================================================================//

	- ( void ) fbDidLogin {
		
		MOAIFacebook::Get ().SessionDidLogin ();
	}

	- ( void ) fbDidNotLogin:( BOOL )cancelled {
		
		MOAIFacebook::Get ().SessionDidNotLogin ();
	}

@end