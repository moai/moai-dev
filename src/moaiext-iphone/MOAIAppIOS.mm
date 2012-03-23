// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moaiext-iphone/MOAIAppIOS.h>
#import <moaiext-iphone/NSData+MOAILib.h>
#import <moaiext-iphone/NSDate+MOAILib.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSError+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

/*
#include <netinet/in.h>     // INET6_ADDRSTRLEN
#include <arpa/nameser.h>   // NS_MAXDNAME
#include <netdb.h>          // getaddrinfo, struct addrinfo, AI_NUMERICHOST
#include <unistd.h>         // getopt
*/

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getDirectoryInDomain
	@text	Search the platform's internal directory structure for a special directory
			as defined by the platform.
 
	@in		string domain		One of MOAIAppIOS.DOMAIN_DOCUMENTS, MOAIAppIOS.DOMAIN_APP_SUPPORT
	@out	string directory	The directory associated with the given domain.
*/
int MOAIAppIOS::_getDirectoryInDomain ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 dirCode = state.GetValue<u32>( 1, 0 ); 
	
	if( dirCode == 0 ) {
		lua_pushstring ( L, "" );
	}
	else {
	
		NSString *dir = [ NSSearchPathForDirectoriesInDomains ( dirCode, NSUserDomainMask, YES ) lastObject ];

		if ( ![[ NSFileManager defaultManager ] fileExistsAtPath:dir ]) {
			NSError *error;
			if ( ![[ NSFileManager defaultManager ] createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:&error ]) {
				NSLog ( @"Error creating directory %@: %@", dir, error );
				return 0;
			}
		}
	
		[ dir toLua:L ];
	}
	return 1;
}

//----------------------------------------------------------------//
int MOAIAppIOS::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		MOAIAppIOS::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//================================================================//
// MOAIAppIOS
//================================================================//

//----------------------------------------------------------------//
void MOAIAppIOS::AppOpenedFromURL ( NSURL* url ) {

	MOAILuaRef& callback = this->mListeners [ APP_OPENED_FROM_URL ];

	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();

		[[ url absoluteString ] toLua:state ];

		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::DidResolveHostName( NSString* hostname, cc8* ipAddress ) {

	MOAILuaRef& callback = this->mListeners [ ASYNC_NAME_RESOLVE ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		[ hostname toLua:state ];
		state.Push ( ipAddress );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::DidStartSession( bool resumed ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_START ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushboolean ( state, resumed );
			
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
MOAIAppIOS::MOAIAppIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->mReachabilityListener = [ ReachabilityListener alloc ];
	[ this->mReachabilityListener startListener ];
}

//----------------------------------------------------------------//
MOAIAppIOS::~MOAIAppIOS () {
}

//----------------------------------------------------------------//
void MOAIAppIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "APP_OPENED_FROM_URL",			( u32 )APP_OPENED_FROM_URL );
	state.SetField ( -1, "ASYNC_NAME_RESOLVE",			( u32 )ASYNC_NAME_RESOLVE );
	state.SetField ( -1, "SESSION_START",				( u32 )SESSION_START );
	state.SetField ( -1, "SESSION_END",					( u32 )SESSION_END );
	
	state.SetField ( -1, "DOMAIN_DOCUMENTS",			( u32 )DOMAIN_DOCUMENTS );
	state.SetField ( -1, "DOMAIN_APP_SUPPORT",			( u32 )DOMAIN_APP_SUPPORT );
	
	luaL_Reg regTable[] = {
		{ "getDirectoryInDomain",		_getDirectoryInDomain },
		{ "setListener",				_setListener },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAppIOS::WillEndSession( ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_END ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

