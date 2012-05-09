// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDIALOGANDROID_H
#define	MOAIDIALOGANDROID_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAIDialogAndroid
//================================================================//
/**	@name	MOAIDialogAndroid
	@text	Wrapper for a simple native dialog implementation on 
			Android devices. Exposed to lua via MOAIDialog on all 
			mobile platforms.

	@const	DIALOG_RESULT_POSITIVE	Result code when the dialog is dismissed by pressing the positive button.
	@const	DIALOG_RESULT_NEUTRAL	Result code when the dialog is dismissed by pressing the neutral button.
	@const	DIALOG_RESULT_NEGATIVE	Result code when the dialog is dismissed by pressing the negative button.
	@const	DIALOG_RESULT_CANCEL	Result code when the dialog is dismissed by pressing the cancel button.
*/
class MOAIDialogAndroid :
	public MOAIGlobalClass < MOAIDialogAndroid, MOAILuaObject > {
private:

	MOAILuaRef	mDialogCallback;
	
	//----------------------------------------------------------------//
	static int	_showDialog	( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIDialogAndroid )

	enum {
		DIALOG_RESULT_POSITIVE,
		DIALOG_RESULT_NEUTRAL,
		DIALOG_RESULT_NEGATIVE,
		DIALOG_RESULT_CANCEL,
	};
	
			MOAIDialogAndroid		();
			~MOAIDialogAndroid		();
	void	NotifyDialogDismissed	( int code );
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif