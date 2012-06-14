// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_NOTIFICATIONS

#include "pch.h"

#import <moaiext-iphone/MOAINotificationsIOS.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSData+MOAILib.h>
#import <moaiext-iphone/NSDate+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getAppIconBadgeNumber
	@text	Get the current icon badge number.
				
	@out 	integer	count
*/
int MOAINotificationsIOS::_getAppIconBadgeNumber ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	lua_pushnumber ( state, application.applicationIconBadgeNumber );
	
	return 1;
}

//----------------------------------------------------------------//
int MOAINotificationsIOS::_localNotificationInSeconds ( lua_State* L ) {
 	MOAILuaState state ( L );
 	
	int seconds					= state.GetValue < int > ( 1, 0 );
 	
	cc8* alertBody				= state.GetValue < cc8* >( 2, 0 );
 	
 	UILocalNotification* notification = [[[ UILocalNotification alloc ] init ] autorelease ]; 	
 	notification.fireDate			= [[ NSDate date ] dateByAddingTimeInterval:seconds ]; 	
 	notification.alertBody			= [ NSString stringWithUTF8String:alertBody ];
	
	if ( state.IsType ( 3, LUA_TTABLE )) {
		
		NSMutableDictionary* userInfoDictionary = [[ NSMutableDictionary alloc ] init ];
		[ userInfoDictionary initWithLua:state stackIndex:3 ];
		notification.userInfo = userInfoDictionary;
	}
 	
	notification.applicationIconBadgeNumber	= 1;
 	
 	UIApplication* application = [ UIApplication sharedApplication ];
 	[ application scheduleLocalNotification:notification ];
 	
 	return 0;
}

//----------------------------------------------------------------//
/**	@name	registerForRemoteNotifications
	@text	Register to receive remote notifications.
			
	@in		integer	types			A mask of requested notification types. See Apple documentation.
	@out 	nil
*/
int MOAINotificationsIOS::_registerForRemoteNotifications ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	[ application registerForRemoteNotificationTypes:( UIRemoteNotificationType )state.GetValue < u32 >( 1, ( u32 )UIRemoteNotificationTypeNone ) ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAppIconBadgeNumber
	@text	Set the current icon badge number.
			
	@in		integer	count			The count to set on the icon badge.
	@out 	nil
*/
int MOAINotificationsIOS::_setAppIconBadgeNumber ( lua_State* L ) {

	MOAILuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	application.applicationIconBadgeNumber = state.GetValue < int >( 1, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAINotificationsIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAINotificationsIOS::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	unregisterForRemoteNotifications
	@text	Dregister for remote notifications.
			
	@out 	nil
*/
int MOAINotificationsIOS::_unregisterForRemoteNotifications ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	[ application unregisterForRemoteNotifications ];

	MOAINotificationsIOS::Get ().NotifyRemoteDeregistrationComplete ();
	
	return 0;
}

//================================================================//
// MOAINotificationsIOS
//================================================================//

//----------------------------------------------------------------//
MOAINotificationsIOS::MOAINotificationsIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAINotificationsIOS::~MOAINotificationsIOS () {

}

//----------------------------------------------------------------//
void MOAINotificationsIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "REMOTE_NOTIFICATION_NONE",					( u32 )UIRemoteNotificationTypeNone );
	state.SetField ( -1, "REMOTE_NOTIFICATION_BADGE",					( u32 )UIRemoteNotificationTypeBadge );
	state.SetField ( -1, "REMOTE_NOTIFICATION_SOUND",					( u32 )UIRemoteNotificationTypeSound );
	state.SetField ( -1, "REMOTE_NOTIFICATION_ALERT",					( u32 )UIRemoteNotificationTypeAlert );

	state.SetField ( -1, "LOCAL_NOTIFICATION_MESSAGE_RECEIVED", 		( u32 )LOCAL_NOTIFICATION_MESSAGE_RECEIVED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_REGISTRATION_COMPLETE", 	( u32 )REMOTE_NOTIFICATION_REGISTRATION_COMPLETE );
	state.SetField ( -1, "REMOTE_NOTIFICATION_MESSAGE_RECEIVED", 		( u32 )REMOTE_NOTIFICATION_MESSAGE_RECEIVED );
        
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_REGISTERED", 		( u32 )REMOTE_NOTIFICATION_RESULT_REGISTERED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_UNREGISTERED", 	( u32 )REMOTE_NOTIFICATION_RESULT_UNREGISTERED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR", 			( u32 )REMOTE_NOTIFICATION_RESULT_ERROR );
	
	luaL_Reg regTable [] = {
		{ "getAppIconBadgeNumber",				_getAppIconBadgeNumber },
		{ "localNotificationInSeconds",			_localNotificationInSeconds },
		{ "registerForRemoteNotifications",		_registerForRemoteNotifications },
		{ "setAppIconBadgeNumber",				_setAppIconBadgeNumber },
		{ "setListener",						_setListener },
		{ "unregisterForRemoteNotifications",	_unregisterForRemoteNotifications },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
//----------------------------------------------------------------//
void MOAINotificationsIOS::NotifyLocalNotificationReceived ( UILocalNotification* notification ) {
 
 	MOAILuaRef& callback = this->mListeners [ LOCAL_NOTIFICATION_MESSAGE_RECEIVED ];
 	
 	if ( callback ) {
 		MOAILuaStateHandle state = callback.GetSelf ();
 		
 		NSDictionary* userInfo = notification.userInfo;
 		if ( userInfo ) {

			[ userInfo toLua:state ];
 		}
 		
 		state.DebugCall ( 1, 0 );
 	}
}

//----------------------------------------------------------------//
void MOAINotificationsIOS::NotifyRemoteNotificationReceived ( NSDictionary* notification ) {

	MOAILuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION_MESSAGE_RECEIVED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		[ notification toLua:state ];
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAINotificationsIOS::NotifyRemoteDeregistrationComplete () {

	MOAILuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION_REGISTRATION_COMPLETE ];

	if ( callback ) {
	
		MOAILuaStateHandle state = callback.GetSelf ();
	
		lua_pushinteger ( state, REMOTE_NOTIFICATION_RESULT_UNREGISTERED );
		lua_pushnil ( state );
	
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAINotificationsIOS::NotifyRemoteRegistrationComplete ( NSData* deviceToken ) {
	
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

// int MOAINotificationsIOS::_presentLocalNotification ( lua_State* L ) {
// 	MOAILuaState state ( L );
// 	
// 	cc8* alertBody					  = state.GetValue < cc8* >( 1, "" );
// 	cc8* alertAction				  = state.GetValue < cc8* >( 2, "" );
// 	
// 	UILocalNotification* notification = [[[ UILocalNotification alloc ] init ] autorelease ];
// 	if ( notification ) {
// 	
// 		notification.alertBody			  = [ NSString stringWithUTF8String:alertBody ];
// 		notification.alertAction		  = [ NSString stringWithUTF8String:alertAction ];
// 		
// 		UIApplication* application		  = [ UIApplication sharedApplication ];
// 		[ application presentLocalNotificationNow:notification ];
// 	}
// 	
// 	return 0;
// }

//----------------------------------------------------------------//
// int MOAINotificationsIOS::_scheduleLocalNotification ( lua_State* L ) {
// 	MOAILuaState state ( L );
// 	
// 	cc8* userInfo				= state.GetValue < cc8* >( 1, "" );
// 	cc8* fireDate				= state.GetValue < cc8* >( 2, "" ); // ISO8601
// 	cc8* timeZone				= state.GetValue < cc8* >( 3, 0 );
// 	
// 	cc8* alertAction			= state.GetValue < cc8* >( 5, 0 );
// 	cc8* alertBody				= state.GetValue < cc8* >( 6, 0 );
// 	bool hasAction				= state.GetValue < bool >( 4, true );
// 	cc8* alertLaunchImage		= state.GetValue < cc8* >( 7, 0 );
// 	
// 	int appIconBadgeNumber		= state.GetValue < int >( 8, 0 );
// 	cc8* soundName				= state.GetValue < cc8* >( 9, 0 );
// 	
// 	UILocalNotification* notification = [[[ UILocalNotification alloc ] init ] autorelease ];
// 	
// 	notification.fireDate			= [ NSDate dateFromISO8601String:[ NSString stringWithUTF8String:fireDate ]];
// 	notification.timeZone			= [ NSString stringWithUTF8String:timeZone ];
// 	
// 	notification.alertBody			= [ NSString stringWithUTF8String:alertBody ];
// 	notification.alertAction		= [ NSString stringWithUTF8String:alertAction ];	
// 	notification.hasAction			= hasAction;
// 	notification.alertLaunchImage	= [ NSString stringWithUTF8String:alertLaunchImage ];
// 	
// 	notification.applicationIconBadgeNumber	= appIconBadgeNumber;
// 	notification.soundName					= [ NSString stringWithUTF8String:soundName ];
// 	
// 	NSMutableDictionary* userInfoDictionary = [[[ NSMutableDictionary alloc ] init ] autorelease ];
// 	[ userInfoDictionary setObject:[ NSString stringWithUTF8String:userInfo ] forKey:UILOCALNOTIFICATION_USER_INFO_KEY ];
// 	notification.userInfo = userInfoDictionary;
// 	
// 	UIApplication* application = [ UIApplication sharedApplication ];
// 	[ application scheduleLocalNotification:notification ];
// 	
// 	return 0;
// }

#endif