// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <UIKit/UIKit.h>
#import <Tapjoy/Tapjoy.h>
#import <moai-ios/headers.h>
#import <moai-ios-tapjoy/MOAITapjoyIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getUserId
 @text	Gets the tapjoy user ID.
 
 @out	string userId
 */
int MOAITapjoyIOS::_getUserId ( lua_State *L ) {
	
	lua_pushstring ( L, [[ Tapjoy getUserID] UTF8String ] );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	init
 @text	Initializes Tapjoy.
 
 @in		string	appId			Available in Tapjoy dashboard settings.
 @in		string	secretKey		Available in Tapjoy dashboard settings.
 @out	nil
 */
int MOAITapjoyIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* appId = lua_tostring ( state, 1 );
	cc8* secretKey = lua_tostring ( state, 2 );
	
	NSString* ID = [[ NSString alloc ] initWithUTF8String:appId ];
	NSString* key = [[ NSString alloc ] initWithUTF8String:secretKey ];
	
	[ Tapjoy requestTapjoyConnect:ID secretKey:key ];
	
	[ ID release ];
	[ key release ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	initVideoAds
 @text	Initializes Tapjoy to display video ads.
 
 @opt	number count			The optional number of ads to cache. Default is Tapjoy dependent.
 @out	nil
 */
int MOAITapjoyIOS::_initVideoAds ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	[ Tapjoy cacheVideosWithDelegate:MOAITapjoyIOS::Get ().mVideoAdDelegate ];
	
	u32 cacheCount = state.GetValue < u32 >( 1, 0 );
	if ( cacheCount > 0 ) {
		
		printf ( "setting cache to: %d\n", cacheCount );
		[ Tapjoy setVideoCacheCount:cacheCount ];
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUserId
 @text	Sets the tapjoy user ID.
 
 @in	string userId
 @out	nil
 */
int MOAITapjoyIOS::_setUserId ( lua_State *L ) {
	
	MOAILuaState state ( L );
	cc8* uid = state.GetValue < cc8* >( 1, 0 );
	
	NSString* ID = [[ NSString alloc ] initWithUTF8String:uid ];
	[ Tapjoy setUserID:ID ];
	[ ID release ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	showOffers
 @text	Displays the Tapjoy marketplace.
 
 @out	nil
 */
int MOAITapjoyIOS::_showOffers ( lua_State* L ) {
	
	UNUSED ( L );
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	
	UIViewController* rootVC = [ window rootViewController ];
	if ( rootVC ) {
		
		[ Tapjoy showOffersWithViewController:rootVC ];
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
	
	state.SetField ( -1, "TAPJOY_VIDEO_AD_BEGIN", 								( u32 )TAPJOY_VIDEO_AD_BEGIN );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_CLOSE", 								( u32 )TAPJOY_VIDEO_AD_CLOSE );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_ERROR", 								( u32 )TAPJOY_VIDEO_AD_ERROR );
	state.SetField ( -1, "TAPJOY_VIDEO_AD_READY", 								( u32 )TAPJOY_VIDEO_AD_READY );
	
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_NO_ERROR", 						( u32 )TAPJOY_VIDEO_STATUS_NO_ERROR );
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE", 		( u32 )TAPJOY_VIDEO_STATUS_MEDIA_STORAGE_UNAVAILABLE );
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS",	( u32 )TAPJOY_VIDEO_STATUS_NETWORK_ERROR_ON_INIT_VIDEOS );
	state.SetField ( -1, "TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO", 			( u32 )TAPJOY_VIDEO_STATUS_UNABLE_TO_PLAY_VIDEO );
	
	luaL_Reg regTable [] = {
		{ "getUserId",		_getUserId },
		{ "initVideoAds",	_initVideoAds },
		{ "init",			_init },
		{ "setListener",	&MOAIGlobalEventSource::_setListener < MOAITapjoyIOS >  },
		{ "setUserId",		_setUserId },
		{ "showOffers",		_showOffers },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITapjoyIOS::NotifyVideoAdBegin () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( TAPJOY_VIDEO_AD_BEGIN, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITapjoyIOS::NotifyVideoAdClose () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( TAPJOY_VIDEO_AD_CLOSE, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// MOAITapjoyIOSVideoAdDelegate
//================================================================//
@implementation MOAITapjoyIOSVideoAdDelegate

#pragma mark -
#pragma mark Protocol MOAITapjoyIOSVideoAdDelegate

//----------------------------------------------------------------//
- ( void ) videoAdBegan {
	MOAITapjoyIOS::Get ().NotifyVideoAdBegin ();
}

//----------------------------------------------------------------//
- ( void ) videoAdClosed {
	MOAITapjoyIOS::Get ().NotifyVideoAdClose ();
}

@end
