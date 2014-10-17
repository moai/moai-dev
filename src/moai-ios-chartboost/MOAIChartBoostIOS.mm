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
/**	@lua	cacheInterstitial
	@text	Request that an interstitial ad be cached for later display.
	
	@out 	nil
*/
int MOAIChartBoostIOS::_cacheInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );
	[[ Chartboost sharedChartboost ] cacheInterstitial ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hasCachedInterstitial
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
/**	@lua	init
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
/**	@lua	showInterstitial
	@text	Request an interstitial ad display if a cached ad is available.
	
	@out 	bool					True, if an ad is cached and will be displayed.
*/
int MOAIChartBoostIOS::_showInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	if ( [[ Chartboost sharedChartboost ] hasCachedInterstitial ]) {
		[[ Chartboost sharedChartboost ] showInterstitial ];
		lua_pushboolean ( state, true );
		return 1;
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
	- ( BOOL ) shouldDisplayInterstitial:( CBLocation )location {
		UNUSED ( location );
		return YES;
	}

	//----------------------------------------------------------------//
	- ( void ) didDismissInterstitial:( CBLocation )location {
		UNUSED ( location );
		MOAIChartBoostIOS::Get ().InvokeListener ( MOAIChartBoostIOS::INTERSTITIAL_DISMISSED );
	}

	//----------------------------------------------------------------//
	- ( BOOL ) shouldDisplayMoreApps:( UIView * )moreAppsView {
		UNUSED ( moreAppsView );
		return NO;
	}

	//----------------------------------------------------------------//
	-( BOOL ) shouldRequestInterstitialsInFirstSession {
		return NO;
	}

@end
