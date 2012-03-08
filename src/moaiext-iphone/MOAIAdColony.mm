//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MOAIAdColony.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAdColony::_initAdColony ( lua_State* L ) {
	MOAILuaState state ( L );
     
	[ AdColony initAdColonyWithDelegate:MOAIAdColony::Get ().mAdColonyDelegate ];    
    
	return 0;
}

//----------------------------------------------------------------//
int MOAIAdColony::_playVideo ( lua_State* L ) {
	MOAILuaState state ( L );
	
	int zone = state.GetValue < int >( 1, 1 );
	bool prePopup = state.GetValue < bool >( 2, true );
	bool postPopup = state.GetValue < bool >( 3, true );
	
	[ AdColony playVideoAdForSlot:zone 
					 withDelegate:MOAIAdColony::Get ().mTakeoverDelegate
				 withV4VCPrePopup:prePopup 
  				 andV4VCPostPopup:postPopup ];
}

//----------------------------------------------------------------//
int MOAIAdColony::_videoReadyForZone ( lua_State *L ) {
	MOAILuaState state ( L );
	
	int zone = state.GetValue < int >( 1, 0 );
	
	int result = [ AdColony zoneStatusForSlot:zone ];
	if ( result == ADCOLONY_ZONE_STATUS_ACTIVE ) {
		
		lua_pushboolean( L, true );
	}
	else {
		
		lua_pushboolean( L, false );
	}
	
	return 1;
}
	
//================================================================//
// MOAIAdColony
//================================================================//

//----------------------------------------------------------------//
void MOAIAdColony::FireZoneListenerEvent (int event, cc8* zone ) {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( event, state )) {
		
		state.Push ( zone );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
MOAIAdColony::MOAIAdColony () : mAdColonyDelegate ( 0 ){
    
	RTTI_SINGLE ( MOAILuaObject )	
	
	mTakeoverDelegate = [ MoaiAdColonyTakeoverDelegate alloc ];
}

//----------------------------------------------------------------//
MOAIAdColony::~MOAIAdColony () {
    
	[ mTakeoverDelegate release ];
}

//----------------------------------------------------------------//
void MOAIAdColony::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "VIDEO_BEGAN_IN_ZONE", ( u32 )VIDEO_BEGAN_IN_ZONE );
	state.SetField ( -1, "VIDEO_ENDED_IN_ZONE", ( u32 )VIDEO_ENDED_IN_ZONE );
	state.SetField ( -1, "VIDEO_FAILED_IN_ZONE", ( u32 )VIDEO_FAILED_IN_ZONE );
	state.SetField ( -1, "VIDEO_PAUSED_IN_ZONE", ( u32 )VIDEO_PAUSED_IN_ZONE );
	state.SetField ( -1, "VIDEO_RESUMED_IN_ZONE", ( u32 )VIDEO_RESUMED_IN_ZONE );
    
	luaL_Reg regTable[] = {
		{ "initAdColony",				_initAdColony },
		{ "playVideo",					_playVideo },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIAdColony > },
		{ "videoReadyForZone",			_videoReadyForZone },
		{ NULL, NULL }	
	};
    
	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAdColony::SetAdColonyDelegate ( id < AdColonyDelegate > delegate ) {
	
	mAdColonyDelegate = delegate;
}

//================================================================//
// MoaiAdColonyTakeoverDelegate
//================================================================//
@implementation MoaiAdColonyTakeoverDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol AdColonyTakeoverDelegate
	//================================================================//

	-(void)adColonyTakeoverBeganForZone:(NSString *)zone {

		MOAIAdColony::Get ().FireZoneListenerEvent (
			MOAIAdColony::Get ().VIDEO_BEGAN_IN_ZONE, [ zone UTF8String ]);
	}

	-(void)adColonyTakeoverEndedForZone:(NSString *)zone withVC:(BOOL)withVirtualCurrencyAward {
		
		MOAIAdColony::Get ().FireZoneListenerEvent (
			MOAIAdColony::Get ().VIDEO_ENDED_IN_ZONE, [ zone UTF8String ]);
	}

	-(void)adColonyVideoAdNotServedForZone:(NSString *)zone {
		
		MOAIAdColony::Get ().FireZoneListenerEvent (
			MOAIAdColony::Get ().VIDEO_FAILED_IN_ZONE, [ zone UTF8String ]);
	}

	-(void)adColonyVideoAdPausedInZone:(NSString *)zone {
		
		MOAIAdColony::Get ().FireZoneListenerEvent (
			MOAIAdColony::Get ().VIDEO_PAUSED_IN_ZONE, [ zone UTF8String ]);
	}

	-(void)adColonyVideoAdResumedInZone:(NSString *)zone {
		
		MOAIAdColony::Get ().FireZoneListenerEvent (
			MOAIAdColony::Get ().VIDEO_RESUMED_IN_ZONE, [ zone UTF8String ]);
	}

@end