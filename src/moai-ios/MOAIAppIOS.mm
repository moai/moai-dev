// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#import <moai-ios/MOAIAppIOS.h>
#import <moai-ios/NSData+MOAILib.h>
#import <moai-ios/NSDate+MOAILib.h>
#import <moai-ios/NSDictionary+MOAILib.h>
#import <moai-ios/NSError+MOAILib.h>
#import <moai-ios/NSString+MOAILib.h>

#import <ifaddrs.h>
#import <arpa/inet.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getDirectoryInDomain
	@text	Search the platform's internal directory structure for 
			a special directory as defined by the platform.
 
	@in		string domain		The domain to search.
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

				lua_pushstring ( L, "" );

				return 1;
			}
		}
	
		[ dir toLua:L ];
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getInterfaceOrientation
 @text	Get the current orientation of the user interface
 
 @in	nil
 @out	number Interface orientation
 */
int MOAIAppIOS::_getInterfaceOrientation ( lua_State* L ) {
	
	MOAILuaState state ( L );

	UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;

	lua_pushnumber ( state, orientation );

	return 1;
}

//----------------------------------------------------------------//
int MOAIAppIOS::_getIPAddress ( lua_State* L ) {

	MOAILuaState state ( L );
	
	struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    NSString *wifiAddress = nil;
    NSString *cellAddress = nil;
	
    // retrieve the current interfaces - returns 0 on success
    if( !getifaddrs ( &interfaces )) {
		
        // Loop through linked list of interfaces
        temp_addr = interfaces;
		
        while ( temp_addr != NULL ) {
			
            sa_family_t sa_type = temp_addr->ifa_addr->sa_family;
			
            if ( sa_type == AF_INET || sa_type == AF_INET6 ) {
				
                NSString *name = [ NSString stringWithUTF8String:temp_addr->ifa_name ];
                NSString *addr = [ NSString stringWithUTF8String:inet_ntoa ((( struct sockaddr_in * )temp_addr->ifa_addr )->sin_addr )]; // pdp_ip0
				
                if([ name isEqualToString:@"en0" ]) {
					
                    // Interface is the wifi connection on the iPhone
                    wifiAddress = addr;
					
                } else if([ name isEqualToString:@"pdp_ip0" ]) {
					
					// Interface is the cell connection on the iPhone
					cellAddress = addr;
				}
            }
            temp_addr = temp_addr->ifa_next;
        }
        // Free memory
        freeifaddrs(interfaces);
    }
    NSString *addr = wifiAddress ? wifiAddress : cellAddress;
	addr = addr ? addr : @"0.0.0.0";
	
	lua_pushstring ( L, [ addr UTF8String ]);
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getUTCTime
 @text	Get the current UTC time in seconds
 
 @in	nil
 @out	num UTC time in seconds
 */
int MOAIAppIOS::_getUTCTime ( lua_State* L ) {
	
	MOAILuaState state ( L );

	lua_pushnumber ( state, [[ NSDate date ] timeIntervalSince1970 ]);

	return 1;
}

//----------------------------------------------------------------//
int MOAIAppIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIAppIOS::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}
	
	return 0;
}

//================================================================//
// MOAIAppIOS
//================================================================//

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

	state.SetField ( -1, "DID_BECOME_ACTIVE",	( u32 )DID_BECOME_ACTIVE );
	state.SetField ( -1, "OPEN_URL",			( u32 )OPEN_URL );
	state.SetField ( -1, "WILL_RESIGN_ACTIVE",	( u32 )WILL_RESIGN_ACTIVE );
	state.SetField ( -1, "WILL_TERMINATE",		( u32 )WILL_TERMINATE );
	
	state.SetField ( -1, "DOMAIN_DOCUMENTS",	( u32 )DOMAIN_DOCUMENTS );
	state.SetField ( -1, "DOMAIN_APP_SUPPORT",	( u32 )DOMAIN_APP_SUPPORT );
	state.SetField ( -1, "DOMAIN_CACHES",		( u32 )DOMAIN_CACHES );
	
	state.SetField ( -1, "INTERFACE_ORIENTATION_PORTRAIT",				( u32 )INTERFACE_ORIENTATION_PORTRAIT );
	state.SetField ( -1, "INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN",	( u32 )INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN );
	state.SetField ( -1, "INTERFACE_ORIENTATION_LANDSCAPE_LEFT",		( u32 )INTERFACE_ORIENTATION_LANDSCAPE_LEFT );
	state.SetField ( -1, "INTERFACE_ORIENTATION_LANDSCAPE_RIGHT",		( u32 )INTERFACE_ORIENTATION_LANDSCAPE_RIGHT );

	luaL_Reg regTable [] = {
		{ "getDirectoryInDomain",		_getDirectoryInDomain },
		{ "getInterfaceOrientation",	_getInterfaceOrientation },
		{ "getIPAddress",				_getIPAddress },
		{ "getUTCTime",					_getUTCTime },
		//{ "sendMail",					_sendMail },
		{ "setListener",				_setListener },
		//{ "takeCamera",					_takeCamera },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAppIOS::DidBecomeActive () {

	MOAILuaRef& callback = this->mListeners [ DID_BECOME_ACTIVE ];
	
	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::OpenUrl ( NSURL* url, NSString* sourceApplication ) {

	MOAILuaRef& callback = this->mListeners [ OPEN_URL ];

	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		[[ url absoluteString ] toLua:state ];
		[ sourceApplication toLua:state ];
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::WillResignActive () {

	MOAILuaRef& callback = this->mListeners [ WILL_RESIGN_ACTIVE ];
	
	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::WillTerminate () {

	MOAILuaRef& callback = this->mListeners [ WILL_TERMINATE ];
	
	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

