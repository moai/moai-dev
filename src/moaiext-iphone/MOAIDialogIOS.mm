// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moaiext-iphone/MOAIDialogIOS.h>

//================================================================//
// LuaAlertView
//================================================================//
@interface LuaAlertView : UIAlertView < UIAlertViewDelegate > {
@public
	int positiveButtonIndex;
	int neutralButtonIndex;
	int negativeButtonIndex;
	MOAILuaRef callback;
};

- ( id ) initWithTitle:( NSString * )title message:( NSString * )message cancelButtonTitle:( NSString * )cancelButtonTitle;

@end

//================================================================//
// LuaAlertView
//================================================================//
@implementation LuaAlertView

	//----------------------------------------------------------------//
	-( id ) initWithTitle:( NSString* )title message:( NSString* )message cancelButtonTitle:( NSString* )cancelButtonTitle {
		positiveButtonIndex = -1;
		neutralButtonIndex = -1;
		negativeButtonIndex = -1;

		return [ super initWithTitle:title message:message delegate:self cancelButtonTitle:cancelButtonTitle otherButtonTitles:nil ];
	}

	//----------------------------------------------------------------//
	-( void ) alertView:( UIAlertView* )alertView didDismissWithButtonIndex:( NSInteger )buttonIndex {
		UNUSED ( alertView );
		
		if ( self->callback ) {
			MOAILuaStateHandle state = self->callback.GetSelf ();
			
			int dialogResult = -1;
			if ( buttonIndex == positiveButtonIndex ) {
				dialogResult = MOAIDialogIOS::DIALOG_RESULT_POSITIVE;
			}
			else if ( buttonIndex == neutralButtonIndex ) {
				dialogResult = MOAIDialogIOS::DIALOG_RESULT_NEUTRAL;
			}
			else if ( buttonIndex == negativeButtonIndex ) {
				dialogResult = MOAIDialogIOS::DIALOG_RESULT_NEGATIVE;
			}
			else if ( buttonIndex == [ alertView cancelButtonIndex ] ) {
				dialogResult = MOAIDialogIOS::DIALOG_RESULT_CANCEL;
			}

			state.Push ( dialogResult );
			state.DebugCall ( 1, 0 );
		}
		
		[ self release ];
	}

@end

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/** @name	showDialog
	@text	Display a modal style dialog box with one or more buttons, including a
			cancel button. This will not halt execution (this function returns immediately),
			so if you need to respond to the user's selection, pass a callback.

	@in		string title		The title of the dialog box.
	@in		string message		The message to display.
	@in		string positive		The title of the positive button.
	@in		string neutral		The title of the neutral button.
	@in		string negative		The title of the negative button.
	@in		bool cancelable		The title of the cancelable button.
	@in		function callback	The function that will receive a DIALOG_RESULT indicating which button was pressed.
 
*/
int MOAIDialogIOS::_showDialog( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* title = state.GetValue < cc8* >( 1, "" );
	cc8* message = state.GetValue < cc8* >( 2, "" );
	cc8* positive = state.GetValue < cc8* >( 3, "" );
	cc8* neutral = state.GetValue < cc8* >( 4, "" );
	cc8* negative = state.GetValue < cc8* >( 5, "" );
	bool cancelable = state.GetValue < bool >( 6, "" );

	LuaAlertView* alert = [[ LuaAlertView alloc ]
						   initWithTitle:[ NSString stringWithUTF8String:title ] 
						   message:[ NSString stringWithUTF8String:message ]
						   cancelButtonTitle:(( cancelable ) ? @"Cancel" : nil )];
	
	if ( state.IsType ( 7, LUA_TFUNCTION )) {
		alert->callback.SetStrongRef ( state, 7 );
	}	
	
	if ( positive != nil ) {
		alert->positiveButtonIndex = [ alert addButtonWithTitle:[ NSString stringWithUTF8String:positive ]];
	}

	if ( neutral != nil ) {
		alert->neutralButtonIndex = [ alert addButtonWithTitle:[ NSString stringWithUTF8String:neutral ]];
	}

	if ( negative != nil ) {
		alert->negativeButtonIndex = [ alert addButtonWithTitle:[ NSString stringWithUTF8String:negative ]];
	}
		
	// Keep this alive until pressed.
	[ alert retain ];
	[ alert show ];
	
	return 0;
}

//================================================================//
// MOAIDialogIOS
//================================================================//

//----------------------------------------------------------------//
MOAIDialogIOS::MOAIDialogIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDialogIOS::~MOAIDialogIOS () {
}

//----------------------------------------------------------------//
void MOAIDialogIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DIALOG_RESULT_POSITIVE", 		( u32 )DIALOG_RESULT_POSITIVE );
	state.SetField ( -1, "DIALOG_RESULT_NEUTRAL", 		( u32 )DIALOG_RESULT_NEUTRAL );
	state.SetField ( -1, "DIALOG_RESULT_NEGATIVE", 		( u32 )DIALOG_RESULT_NEGATIVE );
	state.SetField ( -1, "DIALOG_RESULT_CANCEL", 		( u32 )DIALOG_RESULT_CANCEL );
	
	luaL_Reg regTable[] = {
		{ "showDialog",							_showDialog },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
