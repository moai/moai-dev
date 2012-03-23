// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPPANDROID_H
#define	MOAIAPPANDROID_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAIAppAndroid
//================================================================//
class MOAIAppAndroid :
	public MOAIGlobalClass < MOAIAppAndroid, MOAILuaObject > {
private:
	
	enum {
		SESSION_START,
		SESSION_END,
		BACK_BUTTON_PRESSED,
		TOTAL,
	};
	
	MOAILuaRef	mListeners [ TOTAL ];
	
	//----------------------------------------------------------------//
	static int	_openURL					( lua_State* L );
	static int	_setListener				( lua_State* L );
	static int	_share						( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIAppAndroid )
	
				MOAIAppAndroid				();
				~MOAIAppAndroid				();
	bool		NotifyBackButtonPressed		();
	void		NotifyDidStartSession		( bool resumed );
	void		NotifyWillEndSession		();
	void		RegisterLuaClass			( MOAILuaState& state );
};

#endif