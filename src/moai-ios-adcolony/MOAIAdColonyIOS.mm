//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios-adcolony/MOAIAdColonyIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize AdColony.
	
	@in		string	appId			Available in AdColony dashboard settings.
	@in 	table	zones			A list of zones to configure. Available in AdColony dashboard settings.
	@in		bool	verbose			Enable AdColony verbose logging. Default value is false.
	@out 	nil
*/
int MOAIAdColonyIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
	cc8* appID		= state.GetValue < cc8* >( 1, "" );
	bool verbose	= state.GetValue < bool >( 3, false );

	NSMutableArray* zones = [[[ NSMutableArray alloc ] init ] autorelease ];
	if ( state.IsType ( 2, LUA_TTABLE )) {
		for ( int key = 1; ; ++key ) {
			state.GetField ( 2, key );
			cc8* value = state.GetValue < cc8* >( -1, 0 );
			lua_pop ( state, 1 );
			
			if ( value ) {
				[ zones addObject:[ NSString stringWithUTF8String:value ]];
			}
			else {
				break;
			}	
		}
	}
    
	[ AdColony configureWithAppID:[ NSString stringWithUTF8String:appID ] zoneIDs:zones delegate:MOAIAdColonyIOS::Get ().mAdColonyDelegate logging:verbose ];
    
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	playVideo
	@text	Play an AdColony video ad.
	
	@in 	string	zone			The zone from which to play a video ad.
	@opt	bool	prompt			Determines whether the user is asked whether they want to play a video ad or not. Default is true.
	@opt	bool	confirm			Determines whether the user is presented with a confirmation dialog after video ad playback completes. Default is true.
	@out 	nil
*/
int MOAIAdColonyIOS::_playVideo ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* zone = lua_tostring ( state, 1 );
	
	bool prompt = state.GetValue < bool >( 2, true );
	bool confirmation = state.GetValue < bool >( 3, true );
	
	[ AdColony playVideoAdForZone:[ NSString stringWithUTF8String:zone ] withDelegate:MOAIAdColonyIOS::Get ().mAdColonyDelegate withV4VCPrePopup:prompt andV4VCPostPopup:confirmation ];

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	videoReadyForZone
	@text	Check the readiness of a video ad for a given zone.
	
	@in 	string	zone			The zone from which to check for a video ad.
	@out 	bool					True, if a video ad is ready to play.
*/
int MOAIAdColonyIOS::_videoReadyForZone ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	cc8* zone = lua_tostring ( state, 1 );
	
	int result = [ AdColony zoneStatusForZone:[ NSString stringWithUTF8String:zone ]];
	bool ready = result == ADCOLONY_ZONE_STATUS_ACTIVE ? true : false;
	
	lua_pushboolean ( L, ready );
	
	return 1;
}
	
//================================================================//
// MOAIAdColonyIOS
//================================================================//

//----------------------------------------------------------------//
MOAIAdColonyIOS::MOAIAdColonyIOS () {
    
	RTTI_SINGLE ( MOAILuaObject )	
	
	this->mAdColonyDelegate = [[ MOAIAdColonyIOSDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIAdColonyIOS::~MOAIAdColonyIOS () {
    
	[ this->mAdColonyDelegate release ];
}

//----------------------------------------------------------------//
void MOAIAdColonyIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "VIDEO_STARTED",		( u32 )VIDEO_STARTED );
	state.SetField ( -1, "VIDEO_SHOWN",			( u32 )VIDEO_SHOWN );
	state.SetField ( -1, "VIDEO_FAILED",		( u32 )VIDEO_FAILED );
    
	luaL_Reg regTable [] = {
		{ "getListener",		&MOAIGlobalEventSource::_getListener < MOAIAdColonyIOS > },
		{ "init",				_init },
		{ "playVideo",			_playVideo },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIAdColonyIOS > },
		{ "videoReadyForZone",	_videoReadyForZone },
		{ NULL, NULL }	
	};
    
	luaL_register( state, 0, regTable );
}

//================================================================//
// MOAIAdColonyIOSDelegate
//================================================================//
@implementation MOAIAdColonyIOSDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol AdColonyAdDelegate
	//================================================================//

	//----------------------------------------------------------------//
	- ( void ) onAdColonyAdStartedInZone:( NSString* )zoneID {
		UNUSED ( zoneID );
		MOAIAdColonyIOS::Get ().InvokeListener (( u32 )MOAIAdColonyIOS::VIDEO_STARTED );
	}

	//----------------------------------------------------------------//
	- ( void ) onAdColonyAdAttemptFinished:( BOOL )shown inZone:( NSString* )zoneID {
		UNUSED ( zoneID );
		u32 eventID = shown ? MOAIAdColonyIOS::VIDEO_SHOWN : MOAIAdColonyIOS::VIDEO_FAILED;
		MOAIAdColonyIOS::Get ().InvokeListener (( u32 )eventID );
	}

@end
