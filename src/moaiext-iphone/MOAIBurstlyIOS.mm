//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef DISABLE_CHARTBOOST

#import <moaiext-iphone/MOAIBurstlyIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize ChartBoost.
	
	@in		string	appId			Available in ChartBoost dashboard settings.
	@in 	string	appSignature	Available in ChartBoost dashboard settings.
	@out 	nil
*/
int MOAIBurstlyIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	cc8* signature = lua_tostring ( state, 2 );
	
	//[[ ChartBoost sharedChartBoost ] setAppId:[ NSString stringWithUTF8String:identifier ]];
	//[[ ChartBoost sharedChartBoost ] setAppSignature:[ NSString stringWithUTF8String:signature ]];
	//[[ ChartBoost sharedChartBoost ] setDelegate:MOAIChartBoostIOS::Get ().mDelegate ];
	//[[ ChartBoost sharedChartBoost ] startSession ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadInterstitial
	@text	Request that an interstitial ad be cached for later display.
	
	@opt	string	locationId		Optional location ID.
	@out 	nil
*/
int MOAIBurstlyIOS::_loadInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	// At the moment, to keep parity with Android, don't allow locations.
	// cc8* location = lua_tostring ( state, 1 );
	// 
	// if ( location != nil ) {
	// 	
	// 	[[ ChartBoost sharedChartBoost ] cacheInterstitial:[ NSString stringWithUTF8String:location ]];
	// } else {
	// 	
		//[[ ChartBoost sharedChartBoost ] cacheInterstitial ];
	// }
			
	return 0;
}

//----------------------------------------------------------------//
int MOAIBurstlyIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		//MOAIChartBoostIOS::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showInterstitial
	@text	Request an interstitial ad display if a cached ad is available.
	
	@opt	string	locationId		Optional location ID.
	@out 	bool					True, if an ad is cached and will be displayed.
*/
int MOAIBurstlyIOS::_showInterstitial ( lua_State* L ) {
	
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
		
		//if ( [[ ChartBoost sharedChartBoost ] hasCachedInterstitial ]) {
			
		//	[[ ChartBoost sharedChartBoost ] showInterstitial ];

		//	lua_pushboolean ( state, true );
			
			return 1;
		//}
	// }
			
	lua_pushboolean ( state, false );

	return 1;
}

//================================================================//
// MOAIChartBoostIOS
//================================================================//

//----------------------------------------------------------------//
MOAIBurstlyIOS::MOAIBurstlyIOS () {

	RTTI_SINGLE ( MOAILuaObject )	

	mDelegate = [[ MoaiChartBoostDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIBurstlyIOS::~MOAIBurstlyIOS () {

	[ mDelegate release ];
}

//----------------------------------------------------------------//
void MOAIBurstlyIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "INTERSTITIAL_LOAD_FAILED",	( u32 )INTERSTITIAL_LOAD_FAILED );
	state.SetField ( -1, "INTERSTITIAL_DISMISSED", 		( u32 )INTERSTITIAL_DISMISSED );

	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ "loadInterstitial",	_loadInterstitial },
		{ "setListener",		_setListener },
		{ "showInterstitial",	_showInterstitial },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBurstlyIOS::NotifyInterstitialDismissed () {	
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_DISMISSED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBurstlyIOS::NotifyInterstitialLoadFailed () {	
	
	MOAILuaRef& callback = this->mListeners [ INTERSTITIAL_LOAD_FAILED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}
//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@implementation MoaiBurstlyDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MoaiChartBoostDelegate
	//================================================================//

	- (NSString *)publisherId
	{
		return @"TeW3mgnGxkSQXNERLognRQ";
	}

	- (NSString*)getZone
	{
		return @"0059914779078214412";
	}

	- (UIViewController*)viewControllerForModalPresentation
	{
		return self;
	}

	- (Anchor)burstlyAnchor {
		// The ad will attach to the anchor point at the top center.
		return Anchor_Top;
	}

	- (CGPoint)burstlyAnchorPoint {
		// Set anchor to top/center.
		return CGPointMake(self.view.frame.size.width / 2, 0);
	}

	- (CGFloat)defaultSessionLife {
		// A new ad will automatically be requested every 15 seconds.
		return 15.0f;
	}

	- (void)adManager:(OAIAdManager*)manager requestThrottled:(NSNumber*)time {
		NSLog(@"adManager:%p interstitialRequestThrottled:%@", manager, time);
	}

	- (void)adManager:(OAIAdManager*)manager didPrecacheAd:(NSString*)aNetwork {
		NSLog(@"adManager:%p interstitialDidPrecache:%@", manager, aNetwork);
	}

	- (void)adManager:(OAIAdManager*)manager attemptingToLoad:(NSString*)aNetwork {
		NSLog(@"adManager:%p attemptingToLoad:%@", manager, aNetwork);
	}

	- (void)adManager:(OAIAdManager*)manager didLoad:(NSString*)aNetwork {
		NSLog(@"adManager:%p didLoad:%@", manager, aNetwork);
	}

	- (void)adManager:(OAIAdManager*)manager failedToLoad:(NSString*)aNetwork {
		NSLog(@"adManager:%p failedToLoad:%@", manager, aNetwork);
	}

	- (void)adManager:(OAIAdManager*)manager didLoad:(NSString*)aNetwork isInterstitial:(BOOL)isInterstitial {
		NSLog(@"adManager:%p didLoad Interstial:%@", manager, aNetwork);
	}
	
@end

#endif