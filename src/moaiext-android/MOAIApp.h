// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPP_H
#define	MOAIAPP_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAIApp
//================================================================//
class MOAIApp :
	public USGlobalClass < MOAIApp, USLuaObject > {
private:

	enum {
		SESSION_START,
		SESSION_END,
		TOTAL,
	};
	
	USLuaRef		mListeners [ TOTAL ];
	
	//----------------------------------------------------------------//
	static int		_setListener				( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIApp )
	
	//----------------------------------------------------------------//
	void		DidStartSession					();
				MOAIApp							();
				~MOAIApp						();
	void		OnInit							();
	void		RegisterLuaClass				( USLuaState& state );
	void		Reset							();
	void		WillEndSession					();
};

#endif
