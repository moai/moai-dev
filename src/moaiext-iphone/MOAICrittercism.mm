// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_CRITTERCISM

#include "pch.h"

#import <UIKit/UIKit.h>

#import <moaiext-iphone/MOAICrittercism.h>
#import <Crittercism.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAICrittercism::_init ( lua_State* L ) {
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

	// Releasing these strings causes Crittercism to crash. 
	// [ ID relase ];
	// [ key relase ];
	// [ secret relase ];
		
	return 0;
}

//================================================================//
// MOAICrittercism
//================================================================//

//----------------------------------------------------------------//
MOAICrittercism::MOAICrittercism () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAICrittercism::~MOAICrittercism () {

}

//----------------------------------------------------------------//
void MOAICrittercism::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "init",	_init },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

#endif
