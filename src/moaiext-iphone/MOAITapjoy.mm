// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_TAPJOY

#include "pch.h"

#import <UIKit/UIKit.h>

#import <moaiext-iphone/MOAITapjoy.h>
#import <TapjoyConnect.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	initVideoAds
	@text	Initializes TapjoyConnect interface for video ads. Optionally
			provide a number of how many to cache.
				
	@opt	number countToCache
	@out	nil
*/
int MOAITapjoy::_initVideoAds ( lua_State* L ) {
	USLuaState state ( L );

	[ TapjoyConnect initVideoAdWithDelegate:MOAITapjoy::Get ().mVideoAdDelegate ];
	
	u32 cacheCount = state.GetValue < u32 >( 1, 0 );
	if ( cacheCount > 0 ) {
		
		printf ( "setting cache to: %d\n", cacheCount );
		[ TapjoyConnect setVideoCacheCount:cacheCount ];
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	requestTapjoyConnect
	@text	Initializes the TapjoyConnect interface
				
	@in		string	appId
	@in		string	secretKey
	@out	nil
*/
int MOAITapjoy::_requestTapjoyConnect ( lua_State* L ) {
	USLuaState state ( L );

	cc8* appId = lua_tostring ( state, 1 );
	cc8* secretKey = lua_tostring ( state, 2 );
	
	NSString* ID = [[ NSString alloc ] initWithUTF8String:appId ];
	NSString* key = [[ NSString alloc ] initWithUTF8String:secretKey ];
	[ TapjoyConnect requestTapjoyConnect:ID secretKey:key ];
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showOffers
	@text	Displays the Tapjoy marketplace
				
	@in		nil
	@out	nil
*/
int MOAITapjoy::_showOffers ( lua_State* L ) {
		
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
		
	UIViewController* rootVC = [ window rootViewController ];
	if ( rootVC ) {
	
		[ TapjoyConnect showOffersWithViewController:rootVC ];
	}
		
	return 0;
}

//================================================================//
// MOAITapjoy
//================================================================//

//----------------------------------------------------------------//
MOAITapjoy::MOAITapjoy () {

	RTTI_SINGLE ( MOAIEventSource )		
	
	mVideoAdDelegate = [ MoaiTapjoyVideoAdDelegate alloc ];
}

//----------------------------------------------------------------//
MOAITapjoy::~MOAITapjoy () {

	[ mVideoAdDelegate release ];
}

//----------------------------------------------------------------//
void MOAITapjoy::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "TAPJOY_VIDEO_AD_BEGIN", ( u32 )TAPJOY_VIDEO_AD_BEGIN );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_CLOSE", ( u32 )TAPJOY_VIDEO_AD_CLOSE );

	luaL_Reg regTable[] = {
	
		{ "initVideoAds",				_initVideoAds },
		{ "requestTapjoyConnect",		_requestTapjoyConnect },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAITapjoy >  },
		{ "showOffers",					_showOffers },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITapjoy::SendVideoAdBeginEvent () {	

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	
	if ( this->PushListener ( TAPJOY_VIDEO_AD_BEGIN, state )) {
	
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITapjoy::SendVideoAdCloseEvent () {

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	
	if ( this->PushListener ( TAPJOY_VIDEO_AD_CLOSE, state )) {
	
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// MoaiTapjoyVideoAdDelegate
//================================================================//
@implementation MoaiTapjoyVideoAdDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol TJCVideoAdDelegate
	//================================================================//
	
	-( void ) videoAdBegan {
	}
	
	-( void ) videoAdClosed {
	}
	
@end

#endif //ifndef DISABLE_TAPJOY