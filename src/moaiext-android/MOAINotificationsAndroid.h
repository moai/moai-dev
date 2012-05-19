// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINOTIFICATIONSANDROID_H
#define	MOAINOTIFICATIONSANDROID_H

#ifndef DISABLE_NOTIFICATIONS

#include <moaicore/moaicore.h>

//================================================================//
// MOAINotificationsAndroid
//================================================================//
/**	@name	MOAINotificationsAndroid
	@text	Wrapper for push notification integration on Android 
			devices. Exposed to lua via MOAINotifications on all
			mobile platforms.

	@const	LOCAL_NOTIFICATION_MESSAGE_RECEIVED			Event code for a local notification message receipt.
	@const	REMOTE_NOTIFICATION_REGISTRATION_COMPLETE	Event code for notification registration completion.
	@const	REMOTE_NOTIFICATION_MESSAGE_RECEIVED		Event code for a push notification message receipt.

	@const	REMOTE_NOTIFICATION_RESULT_REGISTERED		Error code for a successful notification registration.
	@const	REMOTE_NOTIFICATION_RESULT_UNREGISTERED		Error code for a successful notification deregistration.
	@const	REMOTE_NOTIFICATION_RESULT_ERROR			Error code for a failed notification registration or deregistration.

	@const	REMOTE_NOTIFICATION_NONE					Notification type none. Unused.
	@const	REMOTE_NOTIFICATION_BADGE					Notification type icon badges. Unused.
	@const	REMOTE_NOTIFICATION_SOUND					Notification type sound. Unused.
	@const	REMOTE_NOTIFICATION_ALERT					Notification type alerts. Unused.
*/
class MOAINotificationsAndroid :
	public MOAIGlobalClass < MOAINotificationsAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_getAppIconBadgeNumber				( lua_State* L );
	static int	_localNotificationInSeconds			( lua_State* L );
	static cc8*	_luaParseTable 						( lua_State* L, int idx );
	static int	_registerForRemoteNotifications		( lua_State* L );
	static int	_setAppIconBadgeNumber				( lua_State* L );
	static int	_setListener						( lua_State* L );
	static int	_unregisterForRemoteNotifications	( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAINotificationsAndroid );
	
	enum {
		LOCAL_NOTIFICATION_MESSAGE_RECEIVED,
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
	
			MOAINotificationsAndroid			();
			~MOAINotificationsAndroid			();
	void	NotifyLocalNotificationReceived		( int entries, cc8** keys, cc8** values );
	void	NotifyRemoteNotificationReceived	( int entries, cc8** keys, cc8** values );
	void	NotifyRemoteRegistrationComplete	( int code, cc8* registration );
	void	RegisterLuaClass					( MOAILuaState& state );
};

#endif  //DISABLE_NOTIFICATIONS

#endif  //MOAINOTIFICATIONS_H