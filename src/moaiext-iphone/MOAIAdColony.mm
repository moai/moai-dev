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
					 withDelegate:nil /* add takeover delegate */
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
MOAIAdColony::MOAIAdColony () : mAdColonyDelegate ( 0 ){
    
	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIAdColony::~MOAIAdColony () {
    
}

//----------------------------------------------------------------//
void MOAIAdColony::RegisterLuaClass ( MOAILuaState& state ) {
    
	luaL_Reg regTable[] = {
		{ "initAdColony",				_initAdColony },
		{ "playVideo",					_playVideo },
		{ "videoReadyForZone",			_videoReadyForZone },
		{ NULL, NULL }	
	};
    
	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAdColony::SetAdColonyDelegate ( id < AdColonyDelegate > delegate ) {
	
	mAdColonyDelegate = delegate;
}