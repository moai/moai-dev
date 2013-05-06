// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moaiext-iphone/MOAIKeyboardIOS.h>

//================================================================//
// MOAIGameCenterIOSLeaderboardDelegate
//================================================================//
@interface MOAITextFieldDelegate : NSObject < UITextFieldDelegate > {
@private

	NSRange		mRange;
}

	//----------------------------------------------------------------//
	-( void )	onChanged					:( NSString* )string;
	-( BOOL )	textField					:( UITextField* )textField shouldChangeCharactersInRange:( NSRange )range replacementString:( NSString* )string;
	-( BOOL )	textFieldShouldReturn		:( UITextField* )textField;

@end

@implementation MOAITextFieldDelegate

	//----------------------------------------------------------------//
	-( void ) onChanged :( NSString* )string {
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		MOAIKeyboardIOS& keyboard = MOAIKeyboardIOS::Get ();
		
		if ( keyboard.PushListener ( MOAIKeyboardIOS::EVENT_INPUT, state )) {
			state.Push ( mRange.location );
			state.Push ( mRange.length );
			state.Push ([ string UTF8String ]);
			state.DebugCall ( 3, 0 );
		}
	}

	//----------------------------------------------------------------//
	-( BOOL ) textField :( UITextField* )textField shouldChangeCharactersInRange:( NSRange )range replacementString:( NSString* )string {
		UNUSED ( textField );
		
		mRange = range;
		[ self performSelector:@selector(onChanged:) withObject:string afterDelay:0.0f ];
		
		return YES;
	}
	
	//----------------------------------------------------------------//
	-( BOOL ) textFieldShouldReturn :( UITextField* )textField {
		UNUSED ( textField );
		
		BOOL result = YES;
		
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		MOAIKeyboardIOS& keyboard = MOAIKeyboardIOS::Get ();
		
		if ( keyboard.PushListener ( MOAIKeyboardIOS::EVENT_RETURN, state )) {
			state.DebugCall ( 0, 1 );
			result = state.GetValue < bool >( -1, true );
		}
		
		if ( result ) {
			keyboard.Finish ();
		}
		
		return result;
	}

@end

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getText
	@text	Returns the text currently being edited (or nil if no editing).
	
	@out	string text

*/
int MOAIKeyboardIOS::_getText ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIKeyboardIOS::Get ().PushText ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	showKeyboard
	@text	Show the native software keyboard to the use.

	@opt	string text			Text to edit (if any). Default value is ''.
	@opt	number type			One of MOAIKeyboardIOS.KEYBOARD_ASCII, MOAIKeyboardIOS.KEYBOARD_DECIMAL_PAD, MOAIKeyboardIOS.KEYBOARD_DEFAULT,
								MOAIKeyboardIOS.KEYBOARD_EMAIL, MOAIKeyboardIOS.KEYBOARD_NUM_PAD, MOAIKeyboardIOS.KEYBOARD_NUMERIC,
								MOAIKeyboardIOS.KEYBOARD_PHONE_PAD, MOAIKeyboardIOS.KEYBOARD_TWITTER, MOAIKeyboardIOS.KEYBOARD_URL.
								Default value is MOAIKeyboardIOS.KEYBOARD_DEFAULT.
	@opt	number returnKey	One of MOAIKeyboardIOS.RETURN_KEY_DEFAULT, MOAIKeyboardIOS.RETURN_KEY_DONE, MOAIKeyboardIOS.RETURN_KEY_GO,
								MOAIKeyboardIOS.RETURN_KEY_JOIN, MOAIKeyboardIOS.RETURN_KEY_NEXT, MOAIKeyboardIOS.RETURN_KEY_ROUTE,
								MOAIKeyboardIOS.RETURN_KEY_SEARCH, MOAIKeyboardIOS.RETURN_KEY_SEND.
								Default value is MOAIKeyboardIOS.RETURN_KEY_DEFAULT.
	@opt	boolean secure		Default value is false.
	@opt	number autocap		One of MOAIKeyboardIOS.AUTOCAP_ALL, MOAIKeyboardIOS.AUTOCAP_NONE, MOAIKeyboardIOS.AUTOCAP_SENTENCES,
								MOAIKeyboardIOS.AUTOCAP_WORDS. Default value is MOAIKeyboardIOS.AUTOCAP_NONE.
	@opt	number appearance	One of MOAIKeyboardIOS.AUTOCAP_ALL, MOAIKeyboardIOS.AUTOCAP_NONE, MOAIKeyboardIOS.AUTOCAP_SENTENCES,
								MOAIKeyboardIOS.AUTOCAP_WORDS. Default value is MOAIKeyboardIOS.AUTOCAP_NONE.
	@out 	nil
*/
int MOAIKeyboardIOS::_showKeyboard ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* text			= state.GetValue < cc8* >( 1, "" );
	int type			= state.GetValue < int >( 2, KEYBOARD_DEFAULT );
	int returnKey		= state.GetValue < int >( 3, RETURN_KEY_DEFAULT );
	bool secure			= state.GetValue < bool >( 4, false );
	int autocap			= state.GetValue < int >( 5, AUTOCAP_NONE );
	int appearance		= state.GetValue < int >( 6, APPEARANCE_DEFAULT );
	
	MOAIKeyboardIOS::Get ().ShowKeyboard ( text, type, returnKey, secure, autocap, appearance );
	
	return 0;
}

//================================================================//
// MOAIKeyboardIOS
//================================================================//

//----------------------------------------------------------------//
void MOAIKeyboardIOS::Finish () {

	if ( this->mTextField ) {
		[ this->mTextField resignFirstResponder ];
		[ this->mTextField removeFromSuperview ];
		
		id delegate = [ this->mTextField delegate ];
		[ this->mTextField setDelegate:0 ];
		[ delegate release ];
		
		[ this->mTextField release ];
		this->mTextField = 0;
	}
}

//----------------------------------------------------------------//
MOAIKeyboardIOS::MOAIKeyboardIOS () :
	mTextField ( 0 ) {

	RTTI_SINGLE ( MOAIGlobalEventSource )
}

//----------------------------------------------------------------//
MOAIKeyboardIOS::~MOAIKeyboardIOS () {

	this->Finish ();
}

//----------------------------------------------------------------//
void MOAIKeyboardIOS::PushText ( MOAILuaState& state ) {

	if ( this->mTextField ) {
		state.Push ([[ this->mTextField text ] UTF8String ]);
	}
	else {
		state.Push ();
	}
}

//----------------------------------------------------------------//
void MOAIKeyboardIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "EVENT_INPUT",					( u32 )EVENT_INPUT );
	state.SetField ( -1, "EVENT_RETURN",				( u32 )EVENT_RETURN );

	state.SetField ( -1, "AUTOCAP_ALL",					( u32 )AUTOCAP_ALL );
	state.SetField ( -1, "AUTOCAP_NONE",				( u32 )AUTOCAP_NONE );
	state.SetField ( -1, "AUTOCAP_SENTENCES",			( u32 )AUTOCAP_SENTENCES );
	state.SetField ( -1, "AUTOCAP_WORDS",				( u32 )AUTOCAP_WORDS );
	
	state.SetField ( -1, "KEYBOARD_ASCII",				( u32 )KEYBOARD_ASCII );
	state.SetField ( -1, "KEYBOARD_DECIMAL_PAD",		( u32 )KEYBOARD_DECIMAL_PAD );
	state.SetField ( -1, "KEYBOARD_DEFAULT",			( u32 )KEYBOARD_DEFAULT );
	state.SetField ( -1, "KEYBOARD_EMAIL",				( u32 )KEYBOARD_EMAIL );
	state.SetField ( -1, "KEYBOARD_NUM_PAD",			( u32 )KEYBOARD_NUM_PAD );
	state.SetField ( -1, "KEYBOARD_NUMERIC",			( u32 )KEYBOARD_NUMERIC );
	state.SetField ( -1, "KEYBOARD_PHONE_PAD",			( u32 )KEYBOARD_PHONE_PAD );
	state.SetField ( -1, "KEYBOARD_TWITTER",			( u32 )KEYBOARD_TWITTER );
	state.SetField ( -1, "KEYBOARD_URL",				( u32 )KEYBOARD_URL );

	state.SetField ( -1, "APPEARANCE_ALERT",			( u32 )APPEARANCE_ALERT );
	state.SetField ( -1, "APPEARANCE_DEFAULT",			( u32 )APPEARANCE_DEFAULT );

	state.SetField ( -1, "RETURN_KEY_DEFAULT",			( u32 )RETURN_KEY_DEFAULT );
	state.SetField ( -1, "RETURN_KEY_DONE",				( u32 )RETURN_KEY_DONE );
	state.SetField ( -1, "RETURN_KEY_GO",				( u32 )RETURN_KEY_GO );
	state.SetField ( -1, "RETURN_KEY_JOIN",				( u32 )RETURN_KEY_JOIN );
	state.SetField ( -1, "RETURN_KEY_NEXT",				( u32 )RETURN_KEY_NEXT );
	state.SetField ( -1, "RETURN_KEY_ROUTE",			( u32 )RETURN_KEY_ROUTE );
	state.SetField ( -1, "RETURN_KEY_SEARCH",			( u32 )RETURN_KEY_SEARCH );
	state.SetField ( -1, "RETURN_KEY_SEND",				( u32 )RETURN_KEY_SEND );

	luaL_Reg regTable [] = {
		{ "getText",			_getText },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIKeyboardIOS > },
		{ "showKeyboard",		_showKeyboard },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIKeyboardIOS::ShowKeyboard ( cc8* text, int type, int returnKey, bool secure, int autocap, int appearance ) {
	
	if ( !this->mTextField ) {
		UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
		
		CGRect frame = CGRectMake ( 0, 0, 320, 24 );
		this->mTextField = [[ UITextField alloc ] initWithFrame:frame ];
		[ this->mTextField setDelegate:[[ MOAITextFieldDelegate alloc ] init ]];
		
		[ window addSubview:this->mTextField ];
	}
	
	[ this->mTextField setHidden:YES ];
	[ this->mTextField setText:[ NSString stringWithUTF8String:text ]];
	
	[ this->mTextField setAutocapitalizationType:( UITextAutocapitalizationType )autocap ];
	[ this->mTextField setAutocorrectionType:UITextAutocorrectionTypeNo ];
	[ this->mTextField setSpellCheckingType:UITextSpellCheckingTypeNo ];
	[ this->mTextField setEnablesReturnKeyAutomatically:NO ];
	[ this->mTextField setKeyboardAppearance:( UIKeyboardAppearance )appearance ];
	[ this->mTextField setKeyboardType:( UIKeyboardType )type ];
	[ this->mTextField setReturnKeyType:( UIReturnKeyType )returnKey ];
	[ this->mTextField setSecureTextEntry:secure ];

	[ this->mTextField becomeFirstResponder ];
}