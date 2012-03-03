// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaiext-android/MOAIApp.h>

#include <android/log.h>

//================================================================//
// Utility macros
//================================================================//

	#define PRINT(str) \
		__android_log_write ( ANDROID_LOG_INFO, "MoaiLog", str );

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIApp::_openURL ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* url = state.GetValue < cc8* >( 1, "" );
	MOAIApp::Get ().openURLFunc ( url );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIApp::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		MOAIApp::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIApp::_share ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* prompt = state.GetValue < cc8* >( 1, "" );
	cc8* subject = state.GetValue < cc8* >( 2, "" );
	cc8* text = state.GetValue < cc8* >( 3, "" );
	
	MOAIApp::Get ().shareFunc ( prompt, subject, text );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIApp::_showDialog ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* title = state.GetValue < cc8* >( 1, "" );
	cc8* message = state.GetValue < cc8* >( 2, "" );
	cc8* positive = state.GetValue < cc8* >( 3, "" );
	cc8* neutral = state.GetValue < cc8* >( 4, "" );
	cc8* negative = state.GetValue < cc8* >( 5, "" );
	bool cancelable = state.GetValue < bool >( 6, "" );
	
	if ( state.IsType ( 7, LUA_TFUNCTION )) {
		// NOTE: This is fragile. We're storing the callback function in a global variable,
		// effectively. Invoking the showDialog method multiple times in succession can
		// therefore lead to unpredictable results. In fact, it's unknown how Android itself
		// handles multiple invocations - are they queued? On iOS, UIAlertView is LIFO and
		// new invocations supersede previous ones, but once dismissed, the system continues
		// down the alert stack... 
		MOAIApp::Get ().mDialogCallback.SetStrongRef ( state, 7 );
	}	
		
	MOAIApp::Get ().showDialogFunc ( title, message, positive, neutral, negative, cancelable );
	
	return 0;
}

//================================================================//
// MOAIApp
//================================================================//

//----------------------------------------------------------------//
MOAIApp::MOAIApp () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIApp::~MOAIApp () {
}

//----------------------------------------------------------------//
void MOAIApp::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "SESSION_START",		    			( u32 )SESSION_START );
	state.SetField ( -1, "SESSION_END",			    			( u32 )SESSION_END );
	state.SetField ( -1, "BACK_BUTTON_PRESSED",					( u32 )BACK_BUTTON_PRESSED );

	state.SetField ( -1, "DIALOG_RESULT_POSITIVE",				( u32 )DIALOG_RESULT_POSITIVE );
	state.SetField ( -1, "DIALOG_RESULT_NEUTRAL",				( u32 )DIALOG_RESULT_NEUTRAL );
	state.SetField ( -1, "DIALOG_RESULT_NEGATIVE",				( u32 )DIALOG_RESULT_NEGATIVE );
	state.SetField ( -1, "DIALOG_RESULT_CANCEL",				( u32 )DIALOG_RESULT_CANCEL );

	luaL_Reg regTable[] = {
		{ "openURL",							_openURL },
		{ "setListener",						_setListener },
		{ "showDialog",							_showDialog },
		{ "share",								_share },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIApp::DidStartSession ( bool resumed ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_START ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();

		lua_pushboolean ( state, resumed );
			
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
bool MOAIApp::NotifyBackButtonPressed () {
	MOAILuaRef& callback = this->mListeners [ BACK_BUTTON_PRESSED ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();

		state.DebugCall ( 0, 1 );

		return lua_toboolean ( state, -1 );
	} else {
		return false;
	}
}

//----------------------------------------------------------------//
void MOAIApp::NotifyDialogDismissed ( int dialogResult ) {

	if ( ! mDialogCallback.IsNil() ) {
		MOAILuaStateHandle state = mDialogCallback.GetSelf ();
		
		state.Push ( dialogResult );
		state.DebugCall ( 1, 0 );
		
		mDialogCallback.Clear();
	}
}

//----------------------------------------------------------------//
void MOAIApp::OnInit () {

}

//----------------------------------------------------------------//
void MOAIApp::Reset () {
	for ( int i = 0 ; i < TOTAL; i++ ) {
		mListeners [ i ].Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIApp::SetOpenURLFunc ( void ( *func ) ( cc8* )) {
	openURLFunc = func;
}

//----------------------------------------------------------------//
void MOAIApp::SetShowDialogFunc ( void ( *func ) ( cc8*, cc8*, cc8*, cc8*, cc8*, bool )) {
	showDialogFunc = func;
}

//----------------------------------------------------------------//
void MOAIApp::SetShareFunc ( void ( *func ) ( cc8*, cc8*, cc8* )) {
	shareFunc = func;
}

//----------------------------------------------------------------//
void MOAIApp::WillEndSession( ) {
	MOAILuaRef& callback = this->mListeners [ SESSION_END ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}
