// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPP_H
#define	MOAIAPP_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>

//================================================================//
// MOAIApp
//================================================================//
class MOAIApp :
	public USGlobalClass < MOAIApp, USLuaObject > {
private:

	UIApplication*	mApplication;

	USLuaRef	mOnRemoteNotification;

	//----------------------------------------------------------------//
	static int		_getAppIconBadgeNumber					( lua_State* L );
	static int		_registerForRemoteNotifications			( lua_State* L );
	static int		_scheduleLocalNotification				( lua_State* L );
	static int		_setAppIconBadgeNumber					( lua_State* L );
	static int		_setRemoteNotificationCallback			( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIApp )
	
	enum {
		ERROR,
		DID_REGISTER,
		LOCAL_NOTIFICATION,
		REMOTE_NOTIFICATION,
	};
	
	SET ( UIApplication*, Application, mApplication )
	
	//----------------------------------------------------------------//
	void		DidFailToRegisterForRemoteNotificationsWithError	( NSError* error );
	void		DidReceiveLocalNotification							( UILocalNotification* notification );
	void		DidReceiveRemoteNotification						( NSDictionary* userInfo );
	void		DidRegisterForRemoteNotificationsWithDeviceToken	( NSData* deviceToken );
				MOAIApp												();
				~MOAIApp											();
	void		OnInit												();
	void		RegisterLuaClass									( USLuaState& state );
};

#endif
