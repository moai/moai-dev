// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDIALOGANDROID_H
#define	MOAIDIALOGANDROID_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAIDialogAndroid
//================================================================//
class MOAIDialogAndroid :
	public MOAIGlobalClass < MOAIDialogAndroid, MOAILuaObject > {
private:
		
	enum {
        DIALOG_RESULT_POSITIVE,
        DIALOG_RESULT_NEUTRAL,
        DIALOG_RESULT_NEGATIVE,
        DIALOG_RESULT_CANCEL,
	};
	
	MOAILuaRef 		mDialogCallback;
	
	//----------------------------------------------------------------//
	static int		_showDialog					( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIDialogAndroid )
	
					MOAIDialogAndroid			();
					~MOAIDialogAndroid			();
	void			NotifyDialogDismissed		( int code );
	void			RegisterLuaClass			( MOAILuaState& state );
};

#endif