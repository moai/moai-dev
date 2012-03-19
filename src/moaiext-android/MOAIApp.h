// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPP_H
#define	MOAIAPP_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAIApp
//================================================================//
class MOAIApp :
	public MOAIGlobalClass < MOAIApp, MOAILuaObject > {
private:
	
	//----------------------------------------------------------------//
	static int	_openURL					( lua_State* L );
	static int	_setListener				( lua_State* L );
	static int	_share						( lua_State* L );
	static int	_showDialog					( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIApp )

	enum {
		SESSION_START,
		SESSION_END,
		BACK_BUTTON_PRESSED,
		TOTAL,
	};
		
	enum {
        DIALOG_RESULT_POSITIVE,
        DIALOG_RESULT_NEUTRAL,
        DIALOG_RESULT_NEGATIVE,
        DIALOG_RESULT_CANCEL,
	};
	
	MOAILuaRef 		mDialogCallback;
	MOAILuaRef		mListeners [ TOTAL ];
	
					MOAIApp					();
					~MOAIApp				();
	bool			NotifyBackButtonPressed	();
	void			NotifyDialogDismissed	( int code );
	void			NotifyDidStartSession	( bool resumed );
	void			NotifyWillEndSession	();
	void			RegisterLuaClass		( MOAILuaState& state );
};

#endif