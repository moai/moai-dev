//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef DISABLE_CHARTBOOST

#import <moaiext-iphone/MOAIChartBoostIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
//----------------------------------------------------------------//
/**	@name	hasCachedInterstitial
 @text	returns whether a cached ad is available
 
 @out 	bool	True, if an ad is cached.
 */
int MOAIChartBoostIOS::_hasCachedInterstitial ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool isAdAvailable = [[ ChartBoost sharedChartBoost ] hasCachedInterstitial ];
	
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
	
	[[ ChartBoost sharedChartBoost ] setAppId:[ NSString stringWithUTF8String:identifier ]];
	[[ ChartBoost sharedChartBoost ] setAppSignature:[ NSString stringWithUTF8String:signature ]];
	[[ ChartBoost sharedChartBoost ] setDelegate:MOAIChartBoostIOS::Get ().mDelegate ];
	[[ ChartBoost sharedChartBoost ] startSession ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadInterstitial
	@text	Request that an interstitial ad be cached for later display.
	
	@opt	string	locationId		Optional location ID.
	@out 	nil
*/
int MOAIChartBoostIOS::_loadInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	// At the moment, to keep parity with Android, don't allow locations.
	// cc8* location = lua_tostring ( state, 1 );
	// 
	// if ( location != nil ) {
	// 	
	// 	[[ ChartBoost sharedChartBoost ] cacheInterstitial:[ NSString stringWithUTF8String:location ]];
	// } else {
	// 	
		[[ ChartBoost sharedChartBoost ] cacheInterstitial ];
	// }
			
	return 0;
}

//----------------------------------------------------------------//
int MOAIChartBoostIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIChartBoostIOS::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
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

	// At the moment, to keep parity with Android, don't allow locations.
	// cc8* location = lua_tostring ( state, 1 );
	// 
	// if ( location != nil ) {
	// 	
	// 	if ([[ ChartBoost sharedChartBoost ] hasCachedInterstitial:[ NSString stringWithUTF8String:location ]]) {
	// 		
	// 		[[ ChartBoost sharedChartBoost ] showInterstitial:[ NSString stringWithUTF8String:location ]];
	// 		
	// 		lua_pushboolean ( state, true );
	// 		
	// 		return 1;
	// 	}
	// } else {
		
		if ( [[ ChartBoost sharedChartBoost ] hasCachedInterstitial ]) {
			
			[[ ChartBoost sharedChartBoost ] showInterstitial ];

			lua_pushboolean ( state, true );
			
			return 1;
		}
	// }
			
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
		{ "hasCachedInterstitial",	_hasCachedInterstitial },
		{ "init",					_init },
		{ "loadInterstitial",		_loadInterstitial },
		{ "setListener",			_setListener },
		{ "showInterstitial",		_showInterstitial },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIChartBoostIOS::NotifyInterstitialDismissed () {	
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_DISMISSED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIChartBoostIOS::NotifyInterstitialLoadFailed () {	
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_LOAD_FAILED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}
//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@implementation MoaiChartBoostDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MoaiChartBoostDelegate
	//================================================================//

	- ( BOOL ) shouldRequestInterstitial {
		
		return YES;
	}

	- ( void ) didFailToLoadInterstitial {
		
		MOAIChartBoostIOS::Get ().NotifyInterstitialLoadFailed ();
	}

	- ( BOOL ) shouldDisplayInterstitial:( UIView * )interstitialView {
		
		return YES;
	}

	- ( void ) didDismissInterstitial:( UIView * )interstitialView {
		
		MOAIChartBoostIOS::Get ().NotifyInterstitialDismissed ();
	}

	- ( BOOL ) shouldDisplayMoreApps:( UIView * )moreAppsView {
		
		return NO;
	}
	
@end

#endif