// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moaiext-iphone/MOAISafariIOS.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	openURL
	@text	Open the native device web browser at the specified URL.
 
	@in		string url
	@out	nil
*/
int MOAISafariIOS::_openURL ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* url = state.GetValue < cc8* >( 1, "" );
	
	if ( url && url [ 0 ] != '\0' ) {
		
		[[ UIApplication sharedApplication ] openURL:[ NSURL URLWithString:[ NSString stringWithFormat: @"%s", url ]]];
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
int MOAISafariIOS::_openURLWithParams ( lua_State* L ) {
	
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
// MOAISafariIOS
//================================================================//

//----------------------------------------------------------------//
MOAISafariIOS::MOAISafariIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAISafariIOS::~MOAISafariIOS () {
}

//----------------------------------------------------------------//
void MOAISafariIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "openURL",			_openURL },
		{ "openURLWithParams",	_openURLWithParams },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}