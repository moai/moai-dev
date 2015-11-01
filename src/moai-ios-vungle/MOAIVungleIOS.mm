//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <vunglepub/vunglepub.h>
#import <moai-ios-vungle/MOAIVungleIOS.h>

//================================================================//
// MOAIVungleDelegate
//================================================================//
@interface MOAIVungleDelegate : NSObject < VGVunglePubDelegate > {
@private
}
@end

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	allowAutoRotate
	@text	Set to true to keep your users from rotating the device from landscape to portrait to properly view the video ad
 
	@in		bool autorotate 
	@out 	nil
*/
int	MOAIVungleIOS::_allowAutoRotate	( lua_State* L ) {
	MOAILuaState state ( L );

	bool autorotate = lua_toboolean( state, 1 );
	[ VGVunglePub allowAutoRotate:autorotate ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	cacheSizeGet
	@text	Detects if an add has been downloaded
 
	@out 	number current video cache size
*/
int	MOAIVungleIOS::_cacheSizeGet ( lua_State* L ) {
	MOAILuaState state ( L );

	int cacheSize = ( int )[ VGVunglePub cacheSizeGet ];
	lua_pushnumber ( state, cacheSize );
	return 1;
	
}

//----------------------------------------------------------------//
/**	@lua	cacheSizeSet
	@text	Detects if an add has been downloaded
 
	@in		number size size in megabytes to set the video cache to
	@out 	nil
*/
int	MOAIVungleIOS::_cacheSizeSet ( lua_State* L ) {
	MOAILuaState state ( L );
	
	int cacheSize = lua_tonumber( state, 1 );
	[ VGVunglePub cacheSizeSet:cacheSize ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int	MOAIVungleIOS::_displayAdvert ( lua_State* L ) {
	MOAILuaState state ( L );
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
	
	bool incentivised	= state.GetValue < bool >( 1, true );
	
	if ( incentivised ) {
		bool showClose = state.GetValue < bool >( 2, false );
		[ VGVunglePub playIncentivizedAd:rootVC animated:TRUE showClose:showClose userTag:nil ];
	}
	else {
		[ VGVunglePub playModalAd:rootVC animated:TRUE ];
	}
	return 0;
}
	
//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize Vungle.
	
	@in		string	appId			Available in Vungle dashboard settings.
	@opt	bool	landscape		True to dispaly video ads in landscape mode
	@out 	nil
*/
int MOAIVungleIOS::_init ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	
	if ( identifier ) {
		
		bool landscape = state.GetValue < bool >( 2, true );
		VGUserData*  data  = [ VGUserData defaultUserData ];
		
		if ( landscape ) {
			data.adOrientation = VGAdOrientationLandscape;
		}
		else {
			data.adOrientation = VGAdOrientationPortrait;
		}
		
		data.locationEnabled = TRUE;
		
		// start vungle publisher library
		[ VGVunglePub setDelegate: ( VGVungleDelegate ) MOAIVungleIOS::Get ().mDelegate ];
		[ VGVunglePub startWithPubAppID:[ NSString stringWithUTF8String:identifier ] userData:data ];
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isVideoAvailable
	@text	Detects if an add has been downloaded
 
	@out 	bool	True, if an ad is cached and will be displayed.
 */
int MOAIVungleIOS::_isVideoAvailable ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool isAdAvailable = [ VGVunglePub adIsAvailable ];
	lua_pushboolean ( state, isAdAvailable );
	return 1;
}

//================================================================//
// MOAIVungleIOS
//================================================================//

//----------------------------------------------------------------//
MOAIVungleIOS::MOAIVungleIOS () :
	mWatchedAd ( false ) {

	RTTI_SINGLE ( MOAILuaObject )
	
	mDelegate = [[ MOAIVungleDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIVungleIOS::~MOAIVungleIOS () {
	[ mDelegate release ];
}

//----------------------------------------------------------------//
void MOAIVungleIOS::NotifyMoviePlayed () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( AD_VIEWED, state )) {
		state.Push ( this->mWatchedAd );
		state.DebugCall ( 1, 0 );
	}
	this->mWatchedAd = false;
}

//----------------------------------------------------------------//
void MOAIVungleIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "AD_VIEWED", ( u32 )AD_VIEWED );

	luaL_Reg regTable [] = {
		{ "allowAutoRotate",	_allowAutoRotate },
		{ "cacheSizeGet",		_cacheSizeGet },
		{ "cacheSizeSet",		_cacheSizeSet },
		{ "displayAdvert",		_displayAdvert },
		{ "getListener",		&MOAIGlobalEventSource::_getListener < MOAIVungleIOS > },
		{ "init",				_init },
		{ "isVideoAvailable",	_isVideoAvailable },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIVungleIOS > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVungleIOS::WatchedAd ( bool playedFull ) {
	this->mWatchedAd = playedFull;
}

//================================================================//
// MOAIVungleDelegate
//================================================================//
@implementation MOAIVungleDelegate

	//----------------------------------------------------------------//
	- ( void ) vungleMoviePlayed:( VGPlayData * ) playData {
		bool playedFull = [ playData playedFull ];
		MOAIVungleIOS::Get ().WatchedAd ( playedFull );
	}

	//----------------------------------------------------------------//
	- ( void ) vungleViewDidDisappear:( UIViewController * ) viewController {
		UNUSED ( viewController );
		MOAIVungleIOS::Get ().NotifyMoviePlayed ();
	}

@end
