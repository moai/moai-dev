// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moai-iphone/MOAIBrowserIOS.h>
#import <moai-iphone/NSDictionary+MOAILib.h>
#import <moai-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	canOpenURL
 @text	Return true if iOS will handle the passed URL.
 
 @in	string url
 @out	boolean
 */
int MOAIBrowserIOS::_canOpenURL ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* url = state.GetValue < cc8* >( 1, "" );

	if ( url && url [ 0 ] != '\0' ) {
		NSString *requestString = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		NSURL * myURL = [[NSURL alloc] initWithString:[requestString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
		lua_pushboolean (state, [[ UIApplication sharedApplication ]
								 canOpenURL:myURL]);
		[myURL release];
		return 1;
	}

	lua_pushnil( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	openURL
	@text	Open the native device web browser at the specified URL.
 
	@in		string url
	@out	nil
*/
int MOAIBrowserIOS::_openURL ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* url = state.GetValue < cc8* >( 1, "" );
	
	if ( url && url [ 0 ] != '\0' ) {
		NSString *requestString = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		NSURL * myURL = [[NSURL alloc] initWithString:[requestString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
		[[ UIApplication sharedApplication ] openURL:myURL];
		
		[myURL release];
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	openURLWithParams
	@text	Open the native device web browser at the specified URL
			with the specified list of query string parameters.
 
	@in		string url
	@in		table params
	@out	nil
*/
int MOAIBrowserIOS::_openURLWithParams ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* baseURL = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	NSMutableDictionary* params = [[ NSMutableDictionary alloc ] initWithCapacity:5 ];
	[ params initWithLua: state stackIndex: 2 ];
	
	if ( baseURL == NULL || params == NULL ) return 0;
	
	NSURL* parsedURL = [ NSURL URLWithString: baseURL ];
	NSString* urlQueryPrefix = parsedURL.query ? @"&" : @"?";
	
	NSMutableArray* paramPairs = [ NSMutableArray array ];
	for ( NSString* key in [ params keyEnumerator ] ) {
		
		NSString* escapedValue = ( NSString* )CFURLCreateStringByAddingPercentEscapes( NULL, ( CFStringRef )[ params objectForKey: key ], NULL, ( CFStringRef )@"!*'();:@&=+$,/?%#[]", kCFStringEncodingUTF8 );
		[ paramPairs addObject:[ NSString stringWithFormat: @"%@=%@", key, escapedValue ]];
		[ escapedValue release ];
	}
	
	NSString* urlQuery = [ paramPairs componentsJoinedByString: @"&" ];
		
	[[ UIApplication sharedApplication ] openURL:[ NSURL URLWithString:[ NSString stringWithFormat: @"%@%@%@", baseURL, urlQueryPrefix, urlQuery ]]];	

	return 0;
}

//================================================================//
// MOAIBrowserIOS
//================================================================//

//----------------------------------------------------------------//
MOAIBrowserIOS::MOAIBrowserIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIBrowserIOS::~MOAIBrowserIOS () {
}

//----------------------------------------------------------------//
void MOAIBrowserIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "canOpenURL",			_canOpenURL },
		{ "openURL",			_openURL },
		{ "openURLWithParams",	_openURLWithParams },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
