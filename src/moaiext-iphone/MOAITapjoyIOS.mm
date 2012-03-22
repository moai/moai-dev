// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_TAPJOY

#include "pch.h"

#import <UIKit/UIKit.h>

#import <moaiext-iphone/MOAITapjoyIOS.h>
#import <TapjoyConnect.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getUserId
 	@text	Gets the tapjoy user ID
 
 	@out	string userId
 */
int MOAITapjoyIOS::_getUserId ( lua_State *L ) {
	
	lua_pushstring ( L, [[TapjoyConnect getUserID] UTF8String ] );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	initVideoAds
	@text	Initializes Tapjoy to display video ads.
				
	@opt	number count			The optional number of ads to cache.
	@out	nil
*/
int MOAITapjoyIOS::_initVideoAds ( lua_State* L ) {
	
	MOAILuaState state ( L );

	[ TapjoyConnect initVideoAdWithDelegate:MOAITapjoyIOS::Get ().mVideoAdDelegate ];
	
	u32 cacheCount = state.GetValue < u32 >( 1, 0 );
	if ( cacheCount > 0 ) {
		
		printf ( "setting cache to: %d\n", cacheCount );
		[ TapjoyConnect setVideoCacheCount:cacheCount ];
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	requestTapjoyConnect
	@text	Initializes Tapjoy.
				
	@in		string	appId			Available in Tapjoy dashboard settings.
	@in		string	secretKey		Available in Tapjoy dashboard settings.
	@out	nil
*/
int MOAITapjoyIOS::_requestTapjoyConnect ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* appId = lua_tostring ( state, 1 );
	cc8* secretKey = lua_tostring ( state, 2 );
	
	NSString* ID = [[ NSString alloc ] initWithUTF8String:appId ];
	NSString* key = [[ NSString alloc ] initWithUTF8String:secretKey ];

	[ TapjoyConnect requestTapjoyConnect:ID secretKey:key ];

	[ ID release ];
	[ key release ];
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showOffers
	@text	Displays the Tapjoy marketplace.
				
	@in		nil
	@out	nil
*/
int MOAITapjoyIOS::_showOffers ( lua_State* L ) {
	
	UNUSED ( L );
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
		
	UIViewController* rootVC = [ window rootViewController ];
	if ( rootVC ) {
	
		[ TapjoyConnect showOffersWithViewController:rootVC ];
	}
		
	return 0;
}

//================================================================//
// MOAITapjoyIOS
//================================================================//

//----------------------------------------------------------------//
MOAITapjoyIOS::MOAITapjoyIOS () {

	RTTI_SINGLE ( MOAIEventSource )		
	
	mVideoAdDelegate = [ MOAITapjoyIOSVideoAdDelegate alloc ];
}

//----------------------------------------------------------------//
MOAITapjoyIOS::~MOAITapjoyIOS () {

	[ mVideoAdDelegate release ];
}

//----------------------------------------------------------------//
void MOAITapjoyIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "TAPJOY_VIDEO_AD_BEGIN", ( u32 )TAPJOY_VIDEO_AD_BEGIN );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_CLOSE", ( u32 )TAPJOY_VIDEO_AD_CLOSE );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_ERROR", ( u32 )TAPJOY_VIDEO_AD_ERROR );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_READY", ( u32 )TAPJOY_VIDEO_AD_READY );

	luaL_Reg regTable[] = {
		
		{ "getUserId",				_getUserId },
		{ "initVideoAds",			_initVideoAds },
		{ "requestTapjoyConnect",	_requestTapjoyConnect },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAITapjoyIOS >  },
		{ "showOffers",				_showOffers },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITapjoyIOS::NotifyVideoAdBegin () {	

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( TAPJOY_VIDEO_AD_BEGIN, state )) {
	
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITapjoyIOS::NotifyVideoAdClose () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( TAPJOY_VIDEO_AD_CLOSE, state )) {
	
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// MOAITapjoyIOSVideoAdDelegate
//================================================================//
@implementation MOAITapjoyIOSVideoAdDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MOAITapjoyIOSVideoAdDelegate
	//================================================================//
	
	-( void ) videoAdBegan {
	
		MOAITapjoyIOS::Get ().NotifyVideoAdBegin ();
	}
	
	-( void ) videoAdClosed {
	
		MOAITapjoyIOS::Get ().NotifyVideoAdClose ();
	}
	
@end

#endif