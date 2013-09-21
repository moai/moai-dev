//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef DISABLE_VUNGLE

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <vunglepub/vunglepub.h>
#import <moai-iphone/MOAIVungleIOS.h>

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
/**	@name	adIsAvailable
	@text	Detects if an add has been downloaded
 
	@out 	bool	True, if an ad is cached and will be displayed.
 */
int MOAIVungleIOS::_adIsAvailable ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool isAdAvailable = [ VGVunglePub adIsAvailable ];
	
	lua_pushboolean ( state, isAdAvailable );
	
	return 1;
}

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

	int cacheSize = [ VGVunglePub cacheSizeGet ];
	
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
/**	@name	playModalAd
	@text	Play a video ad
 
	@out 	nil
*/
int	MOAIVungleIOS::_playModalAd	( lua_State* L ) {
	MOAILuaState state ( L );
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
	
	[ VGVunglePub playModalAd:rootVC animated:TRUE ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	playIncentivizedAd
	@text	Plays an incentivised ad. You can use the listeners to determine if the full movie was watched by the user.
 
	@out 	nil
*/
int	MOAIVungleIOS::_playIncentivizedAd ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool showClose = lua_toboolean( state, 1 );
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
	
	[ VGVunglePub playIncentivizedAd:rootVC animated:TRUE showClose:showClose userTag:nil ];
	
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
int MOAIVungleIOS::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIVungleIOS::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}
	
	return 0;
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
	
	MOAILuaRef& callback = this->mListeners [ MOVIE_PLAYED ];
	
	if ( callback ) {
		
		MOAIScopedLuaState state = callback.GetSelf ();
		
		lua_pushboolean ( state, playedFull );
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIVungleIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "MOVIE_PLAYED",		( u32 )MOVIE_PLAYED );
	state.SetField ( -1, "STATUS_UPDATE",		( u32 )STATUS_UPDATE );
	state.SetField ( -1, "VIEW_DID_DISAPPEAR", 	( u32 )VIEW_DID_DISAPPEAR );
	state.SetField ( -1, "VIEW_WILL_APPEAR",	( u32 )VIEW_WILL_APPEAR );

	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ "adIsAvailable",		_adIsAvailable },
		{ "allowAutoRotate",	_allowAutoRotate },
		{ "cacheSizeGet",		_cacheSizeGet },
		{ "cacheSizeSet",		_cacheSizeSet },
		{ "playModalAd",		_playModalAd },
		{ "playIncentivizedAd", _playIncentivizedAd },
		{ "setListener",		_setListener },
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
		
	}

	- ( void ) vungleViewDidDisappear:( UIViewController * ) viewController {
		
	}

	- ( void ) vungleViewWillAppear:( UIViewController* ) viewController {
		
	}

@end

#endif