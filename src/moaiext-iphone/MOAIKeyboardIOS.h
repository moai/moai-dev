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
	
	@const	EVENT_INPUT
	@const	EVENT_RETURN
	
	@const	AUTOCAP_ALL
	@const	AUTOCAP_NONE
	@const	AUTOCAP_SENTENCES
	@const	AUTOCAP_WORDS
	
	@const	KEYBOARD_ASCII
	@const	KEYBOARD_DECIMAL_PAD
	@const	KEYBOARD_DEFAULT
	@const	KEYBOARD_EMAIL
	@const	KEYBOARD_NUM_PAD
	@const	KEYBOARD_NUMERIC
	@const	KEYBOARD_PHONE_PAD
	@const	KEYBOARD_TWITTER
	@const	KEYBOARD_URL
	
	@const	APPEARANCE_ALERT
	@const	APPEARANCE_DEFAULT
	
	@const	RETURN_KEY_DEFAULT
	@const	RETURN_KEY_DONE
	@const	RETURN_KEY_GO
	@const	RETURN_KEY_JOIN
	@const	RETURN_KEY_NEXT
	@const	RETURN_KEY_ROUTE
	@const	RETURN_KEY_SEARCH
	@const	RETURN_KEY_SEND
*/
class MOAIKeyboardIOS :
	public MOAIGlobalClass < MOAIKeyboardIOS, MOAIGlobalEventSource > {
private:

	enum {
		AUTOCAP_ALL				= UITextAutocapitalizationTypeAllCharacters,
		AUTOCAP_NONE			= UITextAutocapitalizationTypeNone,
		AUTOCAP_SENTENCES		= UITextAutocapitalizationTypeSentences,
		AUTOCAP_WORDS			= UITextAutocapitalizationTypeWords,

		KEYBOARD_ASCII			= UIKeyboardTypeASCIICapable,
		KEYBOARD_DECIMAL_PAD	= UIKeyboardTypeDecimalPad,
		KEYBOARD_DEFAULT		= UIKeyboardTypeDefault,
		KEYBOARD_EMAIL			= UIKeyboardTypeEmailAddress,
		KEYBOARD_NUM_PAD		= UIKeyboardTypeNumberPad,
		KEYBOARD_NUMERIC		= UIKeyboardTypeNumbersAndPunctuation,
		KEYBOARD_PHONE_PAD		= UIKeyboardTypePhonePad,
		KEYBOARD_TWITTER		= UIKeyboardTypeTwitter,
		KEYBOARD_URL			= UIKeyboardTypeURL,
		
		APPEARANCE_ALERT		= UIKeyboardAppearanceAlert,
		APPEARANCE_DEFAULT		= UIKeyboardAppearanceDefault,

		RETURN_KEY_DEFAULT		= UIReturnKeyDefault,
		RETURN_KEY_DONE			= UIReturnKeyDone,
		RETURN_KEY_GO			= UIReturnKeyGo,
		RETURN_KEY_JOIN			= UIReturnKeyJoin,
		RETURN_KEY_NEXT			= UIReturnKeyNext,
		RETURN_KEY_ROUTE		= UIReturnKeyRoute,
		RETURN_KEY_SEARCH		= UIReturnKeySearch,
		RETURN_KEY_SEND			= UIReturnKeySend,
	};

	UITextField*	mTextField;

	//----------------------------------------------------------------//
	static int		_getText				( lua_State* L );
	static int		_showKeyboard			( lua_State* L );

	//----------------------------------------------------------------//
	void			ShowKeyboard			( cc8* text, int type, int returnKey, bool secure, int autocap, int appearance );

public:
	
	DECL_LUA_SINGLETON ( MOAIKeyboardIOS )
	
	enum {
		EVENT_INPUT,
		EVENT_RETURN,
	};
	
	//----------------------------------------------------------------//
	void			Finish					();
					MOAIKeyboardIOS			();
					~MOAIKeyboardIOS		();
	void			PushText				( MOAILuaState& state );
	void			RegisterLuaClass		( MOAILuaState& state );
	
};

#endif
