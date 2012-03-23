// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDIALOGIOS_H
#define	MOAIDIALOGIOS_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>

//================================================================//
// MOAIDialogIOS
//================================================================//
/**	@name	MOAIDialogIOS
	@text	Interface to display a dialog box.
	
	@const	DIALOG_RESULT_POSITIVE
	@const	DIALOG_RESULT_NEUTRAL
	@const	DIALOG_RESULT_NEGATIVE
	@const	DIALOG_RESULT_CANCEL
*/
class MOAIDialogIOS :
	public MOAIGlobalClass < MOAIDialogIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int		_showDialog					( lua_State* L );

public:
	
	// MUST be public - accessed by LuaAlertView.
	enum {
        DIALOG_RESULT_POSITIVE,
        DIALOG_RESULT_NEUTRAL,
        DIALOG_RESULT_NEGATIVE,
        DIALOG_RESULT_CANCEL,
	};
	
	DECL_LUA_SINGLETON ( MOAIDialogIOS )
	
	//----------------------------------------------------------------//
				MOAIDialogIOS					();
				~MOAIDialogIOS					();
	void		RegisterLuaClass				( MOAILuaState& state );
};

#endif
