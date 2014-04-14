//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios-chartboost/MOAIChartBoostIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	cacheInterstitial
	@text	Request that an interstitial ad be cached for later display.
	
	@opt	string	locationId		Optional location ID.
	@out 	nil
*/
int MOAIChartBoostIOS::_cacheInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );
	cc8* location = lua_tostring ( state, 1 );

	if ( location != nil ) {
		[[ Chartboost sharedChartboost ] cacheInterstitial:[ NSString stringWithUTF8String:location ]];
	}
	else {
		[[ Chartboost sharedChartboost ] cacheInterstitial ];
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	hasCachedInterstitial
 @text	returns whether a cached ad is available
 
 @out 	bool	True, if an ad is cached.
 */
int MOAIChartBoostIOS::_hasCachedInterstitial ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool isAdAvailable = [[ Chartboost sharedChartboost ] hasCachedInterstitial ];
	
	lua_pushboolean ( state, isAdAvailable );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize ChartBoost.
	
	@in		string	appId			Available in ChartBoost dashboard settings.
	@in 	string	appSignature	Available in ChartBoost dashboard settings.
	@out 	nil
*/
int MOAIChartBoostIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	cc8* signature = lua_tostring ( state, 2 );
	
	[[ Chartboost sharedChartboost ] setAppId:[ NSString stringWithUTF8String:identifier ]];
	[[ Chartboost sharedChartboost ] setAppSignature:[ NSString stringWithUTF8String:signature ]];
	[[ Chartboost sharedChartboost ] setDelegate:MOAIChartBoostIOS::Get ().mDelegate ];
	[[ Chartboost sharedChartboost ] startSession ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showInterstitial
	@text	Request an interstitial ad display if a cached ad is available.
	
	@opt	string	locationId		Optional location ID.
	@out 	bool					True, if an ad is cached and will be displayed.
*/
int MOAIChartBoostIOS::_showInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );
	
	if ( location != nil ) {
		if ([[ Chartboost sharedChartboost ] hasCachedInterstitial:[ NSString stringWithUTF8String:location ]]) {
				[[ Chartboost sharedChartboost ] showInterstitial:[ NSString stringWithUTF8String:location ]];
				lua_pushboolean ( state, true );
				return 1;
		}
	}
	else {
		if ( [[ Chartboost sharedChartboost ] hasCachedInterstitial ]) {
			[[ Chartboost sharedChartboost ] showInterstitial ];
			lua_pushboolean ( state, true );
			return 1;
		}
	}
	
	lua_pushboolean ( state, false );
	return 1;
}

//================================================================//
// MOAIChartBoostIOS
//================================================================//

//----------------------------------------------------------------//
MOAIChartBoostIOS::MOAIChartBoostIOS () {

	RTTI_SINGLE ( MOAILuaObject )	

	mDelegate = [[ MoaiChartBoostDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIChartBoostIOS::~MOAIChartBoostIOS () {

	[ mDelegate release ];
}

//----------------------------------------------------------------//
void MOAIChartBoostIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "INTERSTITIAL_LOAD_FAILED",	( u32 )INTERSTITIAL_LOAD_FAILED );
	state.SetField ( -1, "INTERSTITIAL_DISMISSED", 		( u32 )INTERSTITIAL_DISMISSED );

	luaL_Reg regTable [] = {
		{ "cacheInterstitial",		_cacheInterstitial },
		{ "getListener",			&MOAIGlobalEventSource::_getListener < MOAIChartBoostIOS > },
		{ "hasCachedInterstitial",	_hasCachedInterstitial },
		{ "init",					_init },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAIChartBoostIOS > },
		{ "showInterstitial",		_showInterstitial },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@implementation MoaiChartBoostDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MoaiChartBoostDelegate
	//================================================================//

	//----------------------------------------------------------------//
	- ( BOOL ) shouldRequestInterstitial {
		return YES;
	}

	//----------------------------------------------------------------//
	- ( void ) didFailToLoadInterstitial {
		MOAIChartBoostIOS::Get ().InvokeListener ( MOAIChartBoostIOS::INTERSTITIAL_LOAD_FAILED );
	}

	//----------------------------------------------------------------//
	- ( BOOL ) shouldDisplayInterstitial:( UIView * )interstitialView {
		UNUSED ( interstitialView );
		return YES;
	}

	//----------------------------------------------------------------//
	- ( void ) didDismissInterstitial:( UIView * )interstitialView {
		UNUSED ( interstitialView );
		MOAIChartBoostIOS::Get ().InvokeListener ( MOAIChartBoostIOS::INTERSTITIAL_DISMISSED );
	}

	//----------------------------------------------------------------//
	- ( BOOL ) shouldDisplayMoreApps:( UIView * )moreAppsView {
		UNUSED ( moreAppsView );
		return NO;
	}
	
@end
