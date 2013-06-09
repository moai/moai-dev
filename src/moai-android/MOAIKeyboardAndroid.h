// Copyright (c) 2012 allaboutapps.at. All Rights Reserved.
//
// Contributed to the MOAI project by Jay Vaughan, allaboutapps.at
//
// This file implements the MOAIKeyboardAndroid object
// for user-input from the soft keyboard on Android
// 
// http://getmoai.com
//

#ifndef MOAIKEYBOARDANDROID_H
#define MOAIKEYBOARDANDROID_H

#import <moaicore/moaicore.h>

//================================================================//
// MOAIKeyboardAndroid
//================================================================//
/** @name MOAIKeyboardAndroid
  @text <p>Wrapper for the native keyboard on Android.  Compatible
  with the iOS methods, albeit with JNI interjection.
*/

class MOAIKeyboardAndroid :
	public MOAIGlobalClass < MOAIKeyboardAndroid, MOAIGlobalEventSource > {

private:

	//----------------------------------------------------------------//
	static int    _getText        ( lua_State* L );
	static int    _showKeyboard   ( lua_State* L );
	static int    _hideKeyboard   ( lua_State* L );
	static int    _setListener    ( lua_State* L );
	static int    _setText        ( lua_State* L );

	//----------------------------------------------------------------//
	void      ShowKeyboard      ( cc8* text, int type, int returnKey, bool secure, int autocap, int appearance );

public:
  
	DECL_LUA_SINGLETON ( MOAIKeyboardAndroid )
  
	enum {
		EVENT_INPUT,
		EVENT_RETURN,
		TOTAL
	};
  
	MOAILuaRef  mListeners [ TOTAL ];

	//----------------------------------------------------------------//
	void      Finish                ();
	          MOAIKeyboardAndroid   ();
	          ~MOAIKeyboardAndroid  ();
	void      RegisterLuaClass      ( MOAILuaState& state );

	void      PushText              ( MOAILuaState& state );
	void      NotifyKeyEvent		();
	void      NotifyTextDone		();

};

#endif
