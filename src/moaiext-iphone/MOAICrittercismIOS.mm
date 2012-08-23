// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_CRITTERCISM

#include "pch.h"

#import <UIKit/UIKit.h>

#import <moaiext-iphone/MOAICrittercismIOS.h>

#import <Crittercism.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Crittercism.
	
	@in		string appId			Available in Crittercism dashboard settings.
	@in		string appKey			Available in Crittercism dashboard settings.
	@in		string appSecret		Available in Crittercism dashboard settings.
	@out	nil
*/
int MOAICrittercismIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* appId = lua_tostring ( state, 1 );
	cc8* appKey = lua_tostring ( state, 2 );
	cc8* appSecret = lua_tostring ( state, 3 );

	NSString* ID = [[ NSString alloc ] initWithUTF8String:appId ];
	NSString* key = [[ NSString alloc ] initWithUTF8String:appKey ];
	NSString* secret = [[ NSString alloc ] initWithUTF8String:appSecret ];
		
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
		
	[ Crittercism initWithAppID: ID andKey: key andSecret: secret andMainViewController: rootVC ];

	[ ID release ];
	[ key release ];
	[ secret release ];
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	forceException
	@text	Forces a crash and crittercism report, can be used for internal errors or debugging crittercism
 
	@out	nil 
 */
int	MOAICrittercismIOS::_forceException ( lua_State* L ) {
	MOAILuaState state ( L );
	
	[NSException raise:NSInvalidArgumentException format:@"Moai Force Exception"];
}

//----------------------------------------------------------------//
/**	@name	leaveBreadcrumb
	@text	Leave a breadcrumb (log statement) to trace execution.
	
	@in		string breadcrumb		A string describing the code location.
	@out	nil
*/
int MOAICrittercismIOS::_leaveBreadcrumb ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* appBreadcrumb = lua_tostring ( state, 1 );

	NSString* breadcrumb = [[ NSString alloc ] initWithUTF8String:appBreadcrumb ];
		
	[ Crittercism leaveBreadcrumb:breadcrumb ];

	[ breadcrumb release ];

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUser
 @text	Sets the username
 
 @in	string username
 @out	nil
*/
int MOAICrittercismIOS::_setUser ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* username = lua_tostring ( state, 1 );
	NSString* name = [[ NSString alloc ] initWithUTF8String:username ];
	[ Crittercism setUsername:name ];
	[ name release ];
	
	return 0;
}


//================================================================//
// MOAICrittercismIOS
//================================================================//

//----------------------------------------------------------------//
MOAICrittercismIOS::MOAICrittercismIOS () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAICrittercismIOS::~MOAICrittercismIOS () {

}

//----------------------------------------------------------------//
void MOAICrittercismIOS::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "init",				_init },
		{ "forceException",		_forceException },
		{ "leaveBreadcrumb",	_leaveBreadcrumb },
		{ "setUser",			_setUser },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

#endif
