// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_NOTIFICATIONS

#include "pch.h"

#import <moaiext-iphone/MOAINotifications.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAINotifications::_getAppIconBadgeNumber ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	lua_pushnumber ( state, application.applicationIconBadgeNumber );
	
	return 1;
}

//----------------------------------------------------------------//
int MOAINotifications::_registerForRemoteNotifications ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	[ application registerForRemoteNotificationTypes:( UIRemoteNotificationType )state.GetValue < u32 >( 1, ( u32 )UIRemoteNotificationTypeNone ) ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAINotifications::_setAppIconBadgeNumber ( lua_State* L ) {

	MOAILuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	application.applicationIconBadgeNumber = state.GetValue < int >( 1, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAINotifications::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAINotifications::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAINotifications::_unregisterForRemoteNotifications ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	[ application unregisterForRemoteNotifications ];

	MOAINotifications::Get ().NotifyRemoteDeregistrationComplete ();
	
	return 0;
}

//================================================================//
// MOAINotifications
//================================================================//

//----------------------------------------------------------------//
MOAINotifications::MOAINotifications () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAINotifications::~MOAINotifications () {

}

//----------------------------------------------------------------//
void MOAINotifications::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "REMOTE_NOTIFICATION_NONE",					( u32 )UIRemoteNotificationTypeNone );
	state.SetField ( -1, "REMOTE_NOTIFICATION_BADGE",					( u32 )UIRemoteNotificationTypeBadge );
	state.SetField ( -1, "REMOTE_NOTIFICATION_SOUND",					( u32 )UIRemoteNotificationTypeSound );
	state.SetField ( -1, "REMOTE_NOTIFICATION_ALERT",					( u32 )UIRemoteNotificationTypeAlert );
	
	state.SetField ( -1, "REMOTE_NOTIFICATION_REGISTRATION_COMPLETE", 	( u32 )REMOTE_NOTIFICATION_REGISTRATION_COMPLETE );
	state.SetField ( -1, "REMOTE_NOTIFICATION_MESSAGE_RECEIVED", 		( u32 )REMOTE_NOTIFICATION_MESSAGE_RECEIVED );
        
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_REGISTERED", 		( u32 )REMOTE_NOTIFICATION_RESULT_REGISTERED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_UNREGISTERED", 	( u32 )REMOTE_NOTIFICATION_RESULT_UNREGISTERED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR", 			( u32 )REMOTE_NOTIFICATION_RESULT_ERROR );
	
	luaL_Reg regTable [] = {
		{ "getAppIconBadgeNumber",				_getAppIconBadgeNumber },
		{ "registerForRemoteNotifications",		_registerForRemoteNotifications },
		{ "setAppIconBadgeNumber",				_setAppIconBadgeNumber },
		{ "setListener",						_setListener },
		{ "unregisterForRemoteNotifications",	_unregisterForRemoteNotifications },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAINotifications::NotifyRemoteNotificationReceived ( NSDictionary* notification ) {

	MOAILuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION_MESSAGE_RECEIVED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		[ notification toLua:state ];
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAINotifications::NotifyRemoteDeregistrationComplete () {

	MOAILuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION_REGISTRATION_COMPLETE ];

	if ( callback ) {
	
		MOAILuaStateHandle state = callback.GetSelf ();
	
		lua_pushinteger ( state, REMOTE_NOTIFICATION_RESULT_UNREGISTERED );
		lua_pushnil ( state );
	
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAINotifications::NotifyRemoteRegistrationComplete ( NSData* deviceToken ) {
	
	MOAILuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION_REGISTRATION_COMPLETE ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();

		if ( deviceToken != nil ) {
			
			STLString token;
			token.hex_encode ([ deviceToken bytes ], [ deviceToken length ]);
		
			lua_pushinteger ( state, REMOTE_NOTIFICATION_RESULT_REGISTERED );
			lua_pushstring ( state, token );
		} else {
			
			lua_pushinteger ( state, REMOTE_NOTIFICATION_RESULT_ERROR );
			lua_pushnil ( state );
		}
		
		state.DebugCall ( 2, 0 );
	}
}

#endif