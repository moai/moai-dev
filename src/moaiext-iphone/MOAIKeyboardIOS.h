// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIKEYBOARDIOS_H
#define	MOAIKEYBOARDIOS_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>

//================================================================//
// MOAIKeyboardIOS
//================================================================//
/**	@name	MOAIKeyboardIOS
	@text	Wrapper for the native keyboard.
*/
class MOAIKeyboardIOS :
	public MOAIGlobalClass < MOAIKeyboardIOS, MOAILuaObject > {
private:

	UITextField*	mTextField;

	//----------------------------------------------------------------//
	static int	_showKeyboard	( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIKeyboardIOS )
	
				MOAIKeyboardIOS			();
				~MOAIKeyboardIOS		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		ShowKeyboard			();
};

#endif
