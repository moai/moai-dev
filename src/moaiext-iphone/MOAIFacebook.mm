//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MOAIFacebook.h>

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
	
	MOAIFacebook::Get ().mFacebook = [[ Facebook alloc ] initWithAppId: [[ NSString alloc ] initWithUTF8String: appID ]
														   andDelegate: MOAIFacebook::Get ().mFBSessionDelegate ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIFacebook::_login ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( ![ MOAIFacebook::Get ().mFacebook isSessionValid ]) {
		[ MOAIFacebook::Get ().mFacebook authorize:nil ];
	}
	
	return 0;
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
// MOAIAdColony
//================================================================//

//----------------------------------------------------------------//
void MOAIFacebook::HandleOpenURL ( NSURL* url ) {
	
	[ mFacebook handleOpenURL:url ];
}

//----------------------------------------------------------------//
MOAIFacebook::MOAIFacebook () {
    
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )	
	
	mFBSessionDelegate = [ MoaiFBSessionDelegate alloc ];
}

//----------------------------------------------------------------//
MOAIFacebook::~MOAIFacebook () {
    
	[ mFBSessionDelegate release ];
}

//----------------------------------------------------------------//
void MOAIFacebook::RegisterLuaClass ( MOAILuaState& state ) {
    
	luaL_Reg regTable[] = {
		{ "getToken",				_getToken },
		{ "init",					_init },
		{ "login",					_login },
		{ "setToken",				_setToken },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIFacebook > },
		{ NULL, NULL }	
	};
    
	luaL_register( state, 0, regTable );
}

//================================================================//
// MoaiLeaderboardDelegate
//================================================================//
@implementation MoaiFBSessionDelegate

//================================================================//
#pragma mark -
#pragma mark Protocol FBSessionDelegate
//================================================================//


@end