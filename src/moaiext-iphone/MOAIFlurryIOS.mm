// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#import <moaiext-iphone/MOAIFlurryIOS.h>
#import <moaiext-iphone/NSData+MOAILib.h>
#import <moaiext-iphone/NSDate+MOAILib.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSError+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

#import "Flurry.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	startSession
 @text	Start the Flurry session.
 
 @in	string apikey		The api key from Flurry.
 @out	nil
 */
int MOAIFlurryIOS::_startSession( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	cc8* api = state.GetValue < cc8* >( 1, "" );
	
	if ( api && api [ 0 ] != '\0' ) {
		NSString * str = [ NSString stringWithUTF8String:api ];
		[ Flurry startSession:str ];
		//[ Flurry setSessionReportsOnCloseEnabled:YES ];
		//[ Flurry setSessionReportsOnPauseEnabled:YES ];
		
		printf("Start Flurry Session: '%s'\n", [str UTF8String]);
	}
	return 0;
}

/**	@name	logEvent
 @text	Log an event to Flurry.
 
 @in	string eventName	The name of the event to send to Flurry
 @in	table params		(optional)The params to send to Flurry with the event or nil if not required.
 @out	nil
 */
int MOAIFlurryIOS::_logEvent( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	NSString* eventName = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	
	if ( eventName == NULL )
		return 0;
    
	if ( state.IsType ( 2, LUA_TTABLE )) {
		NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
		[ paramsDict initWithLua:state stackIndex:2 ];
		[ Flurry logEvent:eventName withParameters:paramsDict ];
	}
	else {
        printf("Log Flurry Event: '%s'\n", [eventName UTF8String]);
		[ Flurry logEvent:eventName ];
	}
    
	return 0;
}

/**	@name	logTimedEvent
 @text	Start a timed event.
 
 @in	string eventName	The name of the event to send to Flurry
 @in	table params		(optional)The params to send to Flurry with the event or nil if not required.
 @out	nil
 */
int MOAIFlurryIOS::_logTimedEvent( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	NSString* eventName = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	
	if ( eventName == NULL )
		return 0;
	
	if ( state.IsType ( 2, LUA_TTABLE )) {
		NSMutableDictionary* paramsDict = [[ NSMutableDictionary alloc ] init ];
		[ paramsDict initWithLua:state stackIndex:2 ];
		[ Flurry logEvent:eventName withParameters:paramsDict timed:YES ];
	}
	else {
		[ Flurry logEvent:eventName timed:YES ];
	}
    
	return 0;
}

/**	@name	endTimedEvent
 @text	End a timed event.
 
 @in	string eventName	The name of the event to send to Flurry
 @out	nil
 */
int MOAIFlurryIOS::_endTimedEvent( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	NSString* eventName = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	
	if ( eventName == NULL )
		return 0;
	
	[ Flurry endTimedEvent:eventName withParameters:nil ];
    
	return 0;
}

//================================================================//
// MOAIFlurryIOS
//================================================================//

//----------------------------------------------------------------//
MOAIFlurryIOS::MOAIFlurryIOS () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
}

//----------------------------------------------------------------//
MOAIFlurryIOS::~MOAIFlurryIOS () {
	
}

//----------------------------------------------------------------//
void MOAIFlurryIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	//printf("register Flurry '%s'\n", "test");
	
	luaL_Reg regTable [] = {
		{ "startSession",		_startSession },
		{ "logEvent",			_logEvent },
		{ "logTimedEvent",		_logTimedEvent },
		{ "endTimedEvent",		_endTimedEvent },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}