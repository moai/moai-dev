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
	@text	<p>Wrapper for the native keyboard. This is a first pass
			of the keyboard functionality and is missing some
			important features (such as spell check) that will be
			added in the near future. We also need to trap the
			keyboard notifications that tell us the position and
			dimensions of the keyboard.</p>
			
			<p>The decision to divorce the keyboard from the text
			input field was deliberate. We're not ready (and may never
			be ready) to own a binding to a full set of native UI
			widgets. In future releases we'll give better was to
			connect the software keyboard to a MOAITextBox. This will
			give better integration of editable text fields with
			Moai. Even though this is more work up front, it means
			it will be easier in the long run to keep editable text
			synchronized with everything else in a scene.</p>
			
			<p>The other short term limitation we face is complex
			text layout for languages such as Arabic. Since we aren't
			displaying native text fields, input is limited to what
			MOAITextBox can display. Support for complex text layout
			is something we want to handle down the road, but until
			then we're going to be limited.</p>
			
			<p>If the keyboard (as written) doesn't meet your needs,
			if should be straightforward to adapt it into a native
			text field class that does. You'd need to change it from
			a singleton to a factory/instance type and add some API to
			position it on the screen, but otherwise most of the
			callbacks are already handled.</p>
			
	
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
