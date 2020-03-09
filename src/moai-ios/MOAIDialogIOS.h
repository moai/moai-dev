// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDIALOGIOS_H
#define	MOAIDIALOGIOS_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moai-core/headers.h>

//================================================================//
// MOAIDialogIOS
//================================================================//
/**	@lua	MOAIDialogIOS
	@text	Wrapper for a simple native dialog implementation on 
			iOS devices. Exposed to Lua via MOAIDialog on all 
			mobile platforms.

	@const	DIALOG_RESULT_POSITIVE	Result code when the dialog is dismissed by pressing the positive button.
	@const	DIALOG_RESULT_NEUTRAL	Result code when the dialog is dismissed by pressing the neutral button.
	@const	DIALOG_RESULT_NEGATIVE	Result code when the dialog is dismissed by pressing the negative button.
	@const	DIALOG_RESULT_CANCEL	Result code when the dialog is dismissed by pressing the cancel button.
*/
class MOAIDialogIOS :
	public ZLContextClass < MOAIDialogIOS >,
	public virtual MOAILuaObject {
private:

	//----------------------------------------------------------------//
	static int	_showDialog	( lua_State* L );

	//----------------------------------------------------------------//
	void	MOAILuaObject_RegisterLuaClass	( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_SINGLETON ( MOAIDialogIOS )
	
	enum {
        DIALOG_RESULT_POSITIVE,
        DIALOG_RESULT_NEUTRAL,
        DIALOG_RESULT_NEGATIVE,
        DIALOG_RESULT_CANCEL,
	};
	
			MOAIDialogIOS		();
			~MOAIDialogIOS		();
};

@interface LuaAlertView : UIAlertView < UIAlertViewDelegate > {
@public
	int						positiveButtonIndex;
	int						neutralButtonIndex;
	int						negativeButtonIndex;
	MOAILuaStrongRef		callback;
};

- ( id ) initWithTitle:( NSString * )title message:( NSString * )message cancelButtonTitle:( NSString * )cancelButtonTitle;

@end

#endif
