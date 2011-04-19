// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-iphone/MOAIApp.h>
#include <moai-iphone/NSDate+MOAILib.h>
#include <moai-iphone/NSDictionary+MOAILib.h>
#include <moai-iphone/NSError+MOAILib.h>
#include <moai-iphone/NSString+MOAILib.h>

#define UILOCALNOTIFICATION_USER_INFO_KEY	@"userInfo"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getAppIconBadgeNumber
	@text	Return the value of the counter shown on the app icon badge.
	
	@in		MOAIApp self
	@out	number badgeNumber
*/
int MOAIApp::_getAppIconBadgeNumber ( lua_State* L ) {
	USLuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	lua_pushnumber ( state, application.applicationIconBadgeNumber );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	registerForRemoteNotifications
	@text	Return the app for remote notifications.
	
	@in		MOAIApp self
	@opt	number notificationTypes	Combination of MOAIApp.REMOTE_NOTIFICATION_BADGE,
										MOAIApp.REMOTE_NOTIFICATION_SOUND, MOAIApp.REMOTE_NOTIFICATION_ALERT.
										Default value is MOAIApp.REMOTE_NOTIFICATION_NONE.
	@out	nil
*/
int MOAIApp::_registerForRemoteNotifications ( lua_State* L ) {
	USLuaState state ( L );
	
	u32 types = state.GetValue < u32 >( 1, ( u32 )UIRemoteNotificationTypeNone );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	[ application registerForRemoteNotificationTypes:( UIRemoteNotificationType )types ];
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: test me
int MOAIApp::_scheduleLocalNotification ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* userInfo				= state.GetValue < cc8* >( 1, "" );
	cc8* fireDate				= state.GetValue < cc8* >( 2, "" ); // ISO8601
	cc8* timeZone				= state.GetValue < cc8* >( 3, 0 );
	
	cc8* alertAction			= state.GetValue < cc8* >( 5, 0 );
	cc8* alertBody				= state.GetValue < cc8* >( 6, 0 );
	bool hasAction				= state.GetValue < bool >( 4, true );
	cc8* alertLaunchImage		= state.GetValue < cc8* >( 7, 0 );
	
	int appIconBadgeNumber		= state.GetValue < int >( 8, 0 );
	cc8* soundName				= state.GetValue < cc8* >( 9, 0 );
	
	UILocalNotification* notification = [[[ NSNotification alloc ] init ] autorelease ];
	
	notification.fireDate			= [ NSDate dateFromISO8601String:[ NSString stringWithUTF8String:fireDate ]];
	notification.timeZone			= [ NSString stringWithUTF8String:timeZone ];
	
	notification.alertBody			= [ NSString stringWithUTF8String:alertAction ];
	notification.alertAction		= [ NSString stringWithUTF8String:alertBody ];
	notification.hasAction			= hasAction;
	notification.alertLaunchImage	= [ NSString stringWithUTF8String:alertLaunchImage ];
	
	notification.applicationIconBadgeNumber	= appIconBadgeNumber;
	notification.soundName					= [ NSString stringWithUTF8String:soundName ];
	
	NSMutableDictionary* userInfoDictionary = [[[ NSMutableDictionary alloc ] init ] autorelease ];
	[ userInfoDictionary setObject:[ NSString stringWithUTF8String:userInfo ] forKey:UILOCALNOTIFICATION_USER_INFO_KEY ];
	notification.userInfo = userInfoDictionary;
	
	UIApplication* application = [ UIApplication sharedApplication ];
	[ application scheduleLocalNotification:notification ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAppIconBadgeNumber
	@text	Set (or clears) the value of the counter shown on the app icon badge.
	
	@in		MOAIApp self
	@opt	number badgeNumber		Default value is 0.
	@out	nil
*/
int MOAIApp::_setAppIconBadgeNumber ( lua_State* L ) {
	USLuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	application.applicationIconBadgeNumber = state.GetValue < int >( 1, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setListener
	@text	Set a callback to handle events of a type.
	
	@in		MOAIApp self
	@in		number event		One of MOAIApp.ERROR, MOAIApp.DID_REGISTER or MOAIApp.REMOTE_NOTIFICATION.
	@opt	function handler
	@out	nil
*/
int MOAIApp::_setListener ( lua_State* L ) {
	USLuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		MOAIApp::Get ().mListeners [ idx ].SetRef ( state, 2, false );
	}
	
	return 0;
}

//================================================================//
// MOAIApp
//================================================================//

//----------------------------------------------------------------//
void MOAIApp::DidFailToRegisterForRemoteNotificationsWithError ( NSError* error ) {
	
	USLuaRef& callback = this->mListeners [ ERROR ];
	
	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		
		[ error toLua:state ];
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::DidReceiveLocalNotification ( UILocalNotification* notification ) {

	USLuaRef& callback = this->mListeners [ LOCAL_NOTIFICATION ];
	
	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		
		NSDictionary* userInfo = notification.userInfo;
		if ( userInfo ) {
		
			NSString* userInfoString = [ userInfo objectForKey:UILOCALNOTIFICATION_USER_INFO_KEY ];
			if ( userInfoString ) {
				[ userInfoString toLua:state ];
			}
		}
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::DidReceiveRemoteNotification ( NSDictionary* userInfo ) {

	USLuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION ];
	
	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		
		[ userInfo toLua:state ];
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::DidRegisterForRemoteNotificationsWithDeviceToken	( NSData* deviceToken ) {
	
	USLuaRef& callback = this->mListeners [ DID_REGISTER ];
	
	if ( callback ) {
		
		STLString tokenStr;
		tokenStr.hex_encode ([ deviceToken bytes ], [ deviceToken length ]);
			
		USLuaStateHandle state = callback.GetSelf ();
		lua_pushstring ( state, tokenStr );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
MOAIApp::MOAIApp () {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIApp::~MOAIApp () {
}

//----------------------------------------------------------------//
void MOAIApp::OnInit () {
}

//----------------------------------------------------------------//
void MOAIApp::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "REMOTE_NOTIFICATION_NONE",	( u32 )UIRemoteNotificationTypeNone );
	state.SetField ( -1, "REMOTE_NOTIFICATION_BADGE",	( u32 )UIRemoteNotificationTypeBadge );
	state.SetField ( -1, "REMOTE_NOTIFICATION_SOUND",	( u32 )UIRemoteNotificationTypeSound );
	state.SetField ( -1, "REMOTE_NOTIFICATION_ALERT",	( u32 )UIRemoteNotificationTypeAlert );
	
	state.SetField ( -1, "ERROR",				( u32 )ERROR );
	state.SetField ( -1, "DID_REGISTER",		( u32 )DID_REGISTER );
	//state.SetField ( -1, "LOCAL_NOTIFICATION",	( u32 )LOCAL_NOTIFICATION );
	state.SetField ( -1, "REMOTE_NOTIFICATION",	( u32 )REMOTE_NOTIFICATION );

	luaL_Reg regTable[] = {
		{ "getAppIconBadgeNumber",				_getAppIconBadgeNumber },
		{ "registerForRemoteNotifications",		_registerForRemoteNotifications },
		//{ "scheduleLocalNotification",			_scheduleLocalNotification },
		{ "setAppIconBadgeNumber",				_setAppIconBadgeNumber },
		{ "setListener",						_setListener },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
