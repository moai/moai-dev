// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINOTIFICATIONS_H
#define	MOAINOTIFICATIONS_H

#ifndef DISABLE_NOTIFICATIONS

#import <moaicore/moaicore.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//================================================================//
// MOAINotifications
//================================================================//
class MOAINotifications :
	public MOAIGlobalClass < MOAINotifications, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_getAppIconBadgeNumber				( lua_State* L );
	static int	_registerForRemoteNotifications		( lua_State* L );
	static int	_setAppIconBadgeNumber				( lua_State* L );
	static int	_setListener						( lua_State* L );
	static int	_unregisterForRemoteNotifications	( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAINotifications )
	
	enum {
		REMOTE_NOTIFICATION_REGISTRATION_COMPLETE,
		REMOTE_NOTIFICATION_MESSAGE_RECEIVED,
		TOTAL
	};

	enum {
   		REMOTE_NOTIFICATION_RESULT_REGISTERED,
		REMOTE_NOTIFICATION_RESULT_UNREGISTERED,
    	REMOTE_NOTIFICATION_RESULT_ERROR,
	};
	
	MOAILuaRef		mListeners [ TOTAL ];
	
	//----------------------------------------------------------------//
				MOAINotifications					();
				~MOAINotifications					();
	void		NotifyRemoteDeregistrationComplete	();
	void		NotifyRemoteNotificationReceived	( NSDictionary* notification );
	void		NotifyRemoteRegistrationComplete	( NSData* token );
	void		RegisterLuaClass					( MOAILuaState& state );
};

#endif  //DISABLE_NOTIFICATIONS

#endif  //MOAINOTIFICATIONS_H