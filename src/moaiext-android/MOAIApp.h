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
	
	MOAILuaRef		mListeners [ TOTAL ];
	MOAILuaRef 		mDialogCallback;
	
	//----------------------------------------------------------------//
	static int	_openURL					( lua_State* L );
	static int	_setListener				( lua_State* L );
	static int	_share						( lua_State* L );
	static int	_showDialog					( lua_State* L );

	void ( *openURLFunc ) 						( cc8* );
	void ( *shareFunc )							( cc8*, cc8*, cc8* );
	void ( *showDialogFunc )					( cc8*, cc8*, cc8*, cc8*, cc8*, bool );

public:
	
	DECL_LUA_SINGLETON ( MOAIApp )
	
	//----------------------------------------------------------------//
				MOAIApp							();
				~MOAIApp						();
	void		DidStartSession					( bool resumed );
	bool		NotifyBackButtonPressed			();
	void		NotifyDialogDismissed			( int code );
	void		OnInit							();
	void		RegisterLuaClass				( MOAILuaState& state );
	void		Reset							();
	void		SetOpenURLFunc					( void ( *func ) ( cc8* ));
	void		SetShareFunc					( void ( *func ) ( cc8*, cc8*, cc8* ));
	void		SetShowDialogFunc				( void ( *func ) ( cc8*, cc8*, cc8*, cc8*, cc8*, bool ));
	void		WillEndSession					();
};

#endif
