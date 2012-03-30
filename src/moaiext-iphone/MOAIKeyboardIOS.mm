// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moaiext-iphone/MOAIKeyboardIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	showKeyboard
	@text	Show a native dialog to the user.
				
	@out 	nil
*/
int MOAIKeyboardIOS::_showKeyboard ( lua_State* L ) {
	
	UNUSED ( L );
	
	return 0;
}

//================================================================//
// MOAIKeyboardIOS
//================================================================//

//----------------------------------------------------------------//
MOAIKeyboardIOS::MOAIKeyboardIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIKeyboardIOS::~MOAIKeyboardIOS () {

	[ this->mTextField release ];
}

//----------------------------------------------------------------//
void MOAIKeyboardIOS::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "showKeyboard",	_showKeyboard },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIKeyboardIOS::ShowKeyboard () {

	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	
	CGRect frame = CGRectMake ( 0, 0, 320, 24 );
	this->mTextField = [[ UITextField alloc ] initWithFrame:frame ];
	[ window addSubview:this->mTextField ];
	[ this->mTextField becomeFirstResponder ];
}