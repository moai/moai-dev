// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moaiext-iphone/MOAIDialogIOS.h>

//================================================================//
// LuaAlertView
//================================================================//
@implementation LuaAlertView

	//----------------------------------------------------------------//
	- ( id ) initWithTitle:( NSString* )title message:( NSString* )message cancelButtonTitle:( NSString* )cancelButtonTitle {

		positiveButtonIndex = -1;
		neutralButtonIndex = -1;
		negativeButtonIndex = -1;

		return [ super initWithTitle:title message:message delegate:self cancelButtonTitle:cancelButtonTitle otherButtonTitles:nil ];
	}

	//----------------------------------------------------------------//
	- ( void ) alertView:( UIAlertView* )alertView didDismissWithButtonIndex:( NSInteger )buttonIndex {
		
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
/**	@name	showDialog
	@text	Show a native dialog to the user.
				
	@in		string		title			The title of the dialog box. Can be nil.
	@in		string		message			The message to show the user. Can be nil.
	@in		string		positive		The text for the positive response dialog button. Can be nil.
	@in		string		neutral			The text for the neutral response dialog button. Can be nil.
	@in		string		negative		The text for the negative response dialog button. Can be nil.
	@in		bool		cancelable		Specifies whether or not the dialog is cancelable
	@opt	function	callback		A function to callback when the dialog is dismissed. Default is nil.
	@out 	nil
*/
int MOAIDialogIOS::_showDialog ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* title = state.GetValue < cc8* >( 1, "" );
	cc8* message = state.GetValue < cc8* >( 2, "" );
	cc8* positive = state.GetValue < cc8* >( 3, nil );
	cc8* neutral = state.GetValue < cc8* >( 4, nil );
	cc8* negative = state.GetValue < cc8* >( 5, nil );
	bool cancelable = state.GetValue < bool >( 6, false );

	LuaAlertView* alert = [[ LuaAlertView alloc ] initWithTitle:[ NSString stringWithUTF8String:title ] message:[ NSString stringWithUTF8String:message ] cancelButtonTitle:(( cancelable ) ? @"Cancel" : nil )];
	
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

	state.SetField ( -1, "DIALOG_RESULT_POSITIVE",	( u32 )DIALOG_RESULT_POSITIVE );
	state.SetField ( -1, "DIALOG_RESULT_NEUTRAL", 	( u32 )DIALOG_RESULT_NEUTRAL );
	state.SetField ( -1, "DIALOG_RESULT_NEGATIVE",	( u32 )DIALOG_RESULT_NEGATIVE );
	state.SetField ( -1, "DIALOG_RESULT_CANCEL", 	( u32 )DIALOG_RESULT_CANCEL );
	
	luaL_Reg regTable [] = {
		{ "showDialog",	_showDialog },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}