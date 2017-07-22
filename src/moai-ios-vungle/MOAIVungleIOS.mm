//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moai-ios-vungle/MOAIVungleIOS.h>

//================================================================//
// MOAIVungleDelegate
//================================================================//
@implementation MOAIVungleDelegate

//----------------------------------------------------------------//
- ( void ) vungleSDKDidInitialize {
	std::cout << "Successfully initialize" << std::endl;
}

//----------------------------------------------------------------//
- ( void ) vungleSDKFailedToInitializeWithError: ( NSError * ) error {
	std::cout << "Failed to init" << std::endl;
}

//----------------------------------------------------------------//
- ( void ) vungleWillCloseAdWithViewInfo: ( VungleViewInfo * ) info placementID: ( NSString * ) placementID {
	bool playedFull = [ info completedView ];
	MOAIVungleIOS::Get ().NotifyMoviePlayed ( playedFull, placementID );
}

@end

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int	MOAIVungleIOS::_displayAdvert ( lua_State* L ) {
	MOAILuaState state ( L );
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
	
	cc8* placementIDChar = lua_tostring ( state, 1 );
	NSString* placementID = [ NSString stringWithCString: placementIDChar encoding: [ NSString defaultCStringEncoding ]];
	
	VungleSDK* sdk = [ VungleSDK sharedSDK ];
	NSError *error;
	[ sdk playAd: rootVC options: nil placementID: placementID error: &error ];
	if ( error ) {
		NSLog ( @"Error encountered playing ad: %@", error );
	}
	return 0;
}
	
//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize Vungle.
	
	@in		string	appId			Available in Vungle dashboard settings.
	@in		string 	placementID	A list of placement IDs.
	@out 	nil
*/
int MOAIVungleIOS::_init ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* appIDChar = lua_tostring ( state, 1 );
	cc8* placementIDChar = lua_tostring ( state, 2 );
	
	NSString* appID = [ NSString stringWithCString: appIDChar encoding: [ NSString defaultCStringEncoding ]];
	NSString* placementID = [ NSString stringWithCString: placementIDChar encoding: [ NSString defaultCStringEncoding ]];

	NSError* error = nil;
	VungleSDK* sdk = [ VungleSDK sharedSDK ];
	NSArray* placementIDsArray = @[ placementID ];
	[ sdk startWithAppId: appID placements: placementIDsArray error: &error ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isVideoAvailable
	@text	Detects if an add has been downloaded
 
	@in		string	placementID			Placement ID.
	@out 	bool	True, if an ad is cached and will be displayed.
 */
int MOAIVungleIOS::_isVideoAvailable ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* placementIDChar = lua_tostring ( state, 1 );
	NSString* placementID = [ NSString stringWithCString: placementIDChar encoding: [ NSString defaultCStringEncoding ]];
	
	VungleSDK* sdk = [ VungleSDK sharedSDK ];
	bool isAdAvailable = [ sdk isAdCachedForPlacementID: placementID ];
	lua_pushboolean ( state, isAdAvailable );
	return 1;
}

//================================================================//
// MOAIVungleIOS
//================================================================//

//----------------------------------------------------------------//
MOAIVungleIOS::MOAIVungleIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	mDelegate = [[ MOAIVungleDelegate alloc ] init ];
	[[ VungleSDK sharedSDK ] setDelegate: mDelegate ];
}

//----------------------------------------------------------------//
MOAIVungleIOS::~MOAIVungleIOS () {
	
	[[ VungleSDK sharedSDK ] setDelegate: nil ];
	[ mDelegate release ];
}

//----------------------------------------------------------------//
MOAIVungleDelegate*	MOAIVungleIOS::GetVungleDelegate () const {
	return mDelegate;
}

//----------------------------------------------------------------//
void MOAIVungleIOS::NotifyMoviePlayed ( bool playedFull, NSString* placementID ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( AD_VIEWED, state )) {
		state.Push ( playedFull );
		state.Push ([ placementID UTF8String ]);
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIVungleIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "AD_VIEWED", ( u32 )AD_VIEWED );

	luaL_Reg regTable [] = {
		{ "displayAdvert",		_displayAdvert },
		{ "getListener",		&MOAIGlobalEventSource::_getListener < MOAIVungleIOS > },
		{ "init",				_init },
		{ "isVideoAvailable",	_isVideoAvailable },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIVungleIOS > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
