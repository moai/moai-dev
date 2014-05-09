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
// MoaiChartBoostDelegate
//================================================================//
@interface MoaiVungleDelegate : NSObject < VGVunglePubDelegate > {
@private
}
@end

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	allowAutoRotate
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
/**	@name	cacheSizeGet
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
/**	@name	cacheSizeSet
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
// TODO: doxygen
int	MOAIVungleIOS::_displayAdvert ( lua_State* L ) {
	MOAILuaState state ( L );
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
	
	bool incentivised	= lua_toboolean( state, 1 );
	
	if ( incentivised ) {
		bool showClose		= lua_toboolean( state, 2 );
		[ VGVunglePub playIncentivizedAd:rootVC animated:TRUE showClose:showClose userTag:nil ];
	}
	else {
		[ VGVunglePub playModalAd:rootVC animated:TRUE ];
	}
	return 0;
}
	
//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Vungle.
	
	@in		string	appId			Available in Vungle dashboard settings.
	@opt	bool	landscape		True to dispaly video ads in landscape mode
	@out 	nil
*/
int MOAIVungleIOS::_init ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	
	if ( identifier ) {
		
		bool landscape = lua_toboolean( state, 2 );

		VGUserData*  data  = [VGUserData defaultUserData];
		
		// set up config data
		// AJV TODO add age/gender
		//data.age             = 36;
	   // data.gender          = VGGenderFemale;
		
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
/**	@name	isVideoAvailable
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
MOAIVungleIOS::MOAIVungleIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	mDelegate = [[ MoaiVungleDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIVungleIOS::~MOAIVungleIOS () {
	[ mDelegate release ];
}

//----------------------------------------------------------------//
void MOAIVungleIOS::NotifyMoviePlayed ( bool playedFull ) {	
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( MOVIE_PLAYED, state )) {
		state.Push ( playedFull );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIVungleIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "MOVIE_PLAYED",		( u32 )MOVIE_PLAYED );

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

//================================================================//
// MoaiVungleDelegate
//================================================================//
@implementation MoaiVungleDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MoaiVungleDelegate
	//====================================================0============//

	- ( void ) vungleMoviePlayed:( VGPlayData * ) playData {
		bool playedFull = [ playData playedFull ];
		MOAIVungleIOS::Get ().NotifyMoviePlayed ( playedFull );
	}

	- ( void ) vungleStatusUpdate:( VGStatusData * ) statusData {
		UNUSED ( statusData );
	}

	- ( void ) vungleViewDidDisappear:( UIViewController * ) viewController {
		UNUSED ( viewController );
	}

	- ( void ) vungleViewWillAppear:( UIViewController* ) viewController {
		UNUSED ( viewController );
	}

@end
