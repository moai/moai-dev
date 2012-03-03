// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAINOTIFICATIONS_H
#define	MOAINOTIFICATIONS_H

#ifndef DISABLE_NOTIFICATIONS

#include <moaicore/moaicore.h>

//================================================================//
// MOAINotifications
//================================================================//
class MOAINotifications :
	public MOAIGlobalClass < MOAINotifications, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_registerForRemoteNotifications 	( lua_State* L );
	static int	_unregisterForRemoteNotifications 	( lua_State* L );
	static int	_setListener						( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAINotifications );
	
	enum {
		REMOTE_NOTIFICATION_REGISTRATION_COMPLETE,
		REMOTE_NOTIFICATION_MESSAGE_RECEIVED,
		TOTAL
	};
	
	enum {
       	REMOTE_NOTIFICATION_RESULT_REGISTERED,
		REMOTE_NOTIFICATION_RESULT_UNREGISTERED,
        REMOTE_NOTIFICATION_RESULT_ERROR_SERVICE_NOT_AVAILABLE,
        REMOTE_NOTIFICATION_RESULT_ERROR_ACCOUNT_MISSING,
        REMOTE_NOTIFICATION_RESULT_ERROR_AUTHENTICATION_FAILED,
        REMOTE_NOTIFICATION_RESULT_ERROR_TOO_MANY_REGISTRATIONS,
        REMOTE_NOTIFICATION_RESULT_ERROR_INVALID_SENDER,
        REMOTE_NOTIFICATION_RESULT_ERROR_PHONE_REGISTRATION_ERROR
	};
	
	MOAILuaRef		mListeners [ TOTAL ];
	
					MOAINotifications					();
					~MOAINotifications					();
	void			NotifyRemoteNotificationReceived	( int entries, cc8** keys, cc8** values );
	void			NotifyRemoteRegistrationComplete	( int code, cc8* registration );
	void			RegisterLuaClass					( MOAILuaState& state );
};

#endif  //DISABLE_NOTIFICATIONS

#endif  //MOAINOTIFICATIONS_H
