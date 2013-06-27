// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGOOGLEPLAYSERVICESANDROID_H
#define	MOAIGOOGLEPLAYSERVICESANDROID_H

#ifndef DISABLE_PLAYSERVICES

#include <moaicore/moaicore.h>

//================================================================//
// MOAIGooglePlayServicesAndroid
//================================================================//
/**	@name	MOAIGooglePlayServicesAndroid
	@text	Wrapper for Google Play services.
*/
class MOAIGooglePlayServicesAndroid :
	public MOAIGlobalClass < MOAIGooglePlayServicesAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int			_connect				( lua_State* L );
	static int			_isConnected			( lua_State* L );
	static int			_setListener			( lua_State* L );
	static int			_showAchievements		( lua_State* L );
	static int			_showLeaderboard		( lua_State* L );
	static int			_submitScore			( lua_State* L );
	static int			_unlockAchievement		( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIGooglePlayServicesAndroid );

	enum {
		CONNECTION_COMPLETE,
		TOTAL,
	};

	MOAILuaRef		mListeners [ TOTAL ];

			MOAIGooglePlayServicesAndroid		();
			~MOAIGooglePlayServicesAndroid		();
	void 	NotifyConnectionComplete			();
	void	RegisterLuaClass					( MOAILuaState& state );
};

#endif  //DISABLE_PLAYSERVICES

#endif  //MOAIGOOGLEPLAYSERVICESANDROID_H
