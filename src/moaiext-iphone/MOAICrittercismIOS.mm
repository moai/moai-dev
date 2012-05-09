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
		{ "init",	_init },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

#endif
