// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINOTIFICATIONS_H
#define	MOAINOTIFICATIONS_H

#ifndef DISABLE_NOTIFICATIONS

#import <moaicore/moaicore.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//================================================================//
// MOAINotificationsIOS
//================================================================//
/**	@name	MOAINotificationsIOS
	@text	Wrapper for push notificiation integration on iOS 
			devices. Exposed to lua via MOAINotifications on all
			mobile platforms.

	@const	LOCAL_NOTIFICATION_MESSAGE_RECEIVED			Event code for a local notification message receipt.
	@const	REMOTE_NOTIFICATION_REGISTRATION_COMPLETE	Event code for notification registration completion.
	@const	REMOTE_NOTIFICATION_MESSAGE_RECEIVED		Event code for a push notification message receipt.

	@const	REMOTE_NOTIFICATION_RESULT_REGISTERED		Error code for a successful notification registration.
	@const	REMOTE_NOTIFICATION_RESULT_UNREGISTERED		Error code for a successful notification deregistration.
	@const	REMOTE_NOTIFICATION_RESULT_ERROR			Error code for a failed notification registration or deregistration.
	
	@const	REMOTE_NOTIFICATION_NONE					Notification type none.
	@const	REMOTE_NOTIFICATION_BADGE					Notification type icon badges.
	@const	REMOTE_NOTIFICATION_SOUND					Notification type sound.
	@const	REMOTE_NOTIFICATION_ALERT					Notification type alerts.
*/
class MOAINotificationsIOS :
	public MOAIGlobalClass < MOAINotificationsIOS, MOAILuaObject > {
private:

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
	NSDictionary*	mAppNotificationPayload;

	//----------------------------------------------------------------//
	static int	_getAppIconBadgeNumber				( lua_State* L );
	static int	_getNotificationThatStartedApp		( lua_State* L );
	static int	_presentLocalNotification			( lua_State* L );
	static int	_registerForRemoteNotifications		( lua_State* L );
	static int	_scheduleLocalNotification			( lua_State* L );
	static int	_setAppIconBadgeNumber				( lua_State* L );
	static int	_setListener						( lua_State* L );
	static int	_unregisterForRemoteNotifications	( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAINotificationsIOS )
	
	//----------------------------------------------------------------//
				MOAINotificationsIOS				();
				~MOAINotificationsIOS				();
	void		NotifyLocalNotificationReceived		( UILocalNotification* notification );
	void		NotifyRemoteDeregistrationComplete	();
	void		NotifyRemoteNotificationReceived	( NSDictionary* notification );
	void		NotifyRemoteRegistrationComplete	( NSData* token );
	void		RegisterLuaClass					( MOAILuaState& state );
	void		SetRemoteNotificationPayload		( NSDictionary* remoteNotificationPayload );
};

#endif  //DISABLE_NOTIFICATIONS

#endif  //MOAINOTIFICATIONS_H