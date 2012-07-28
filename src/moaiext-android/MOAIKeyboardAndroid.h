// Copyright (c) 2012 allaboutapps.at. All Rights Reserved.
//
// Contributed to the MOAI project by Jay Vaughan, allaboutapps.at
//
// This file implements the MOAIKeyboardAndroid object
// for user-input from the soft keyboard on Android
// 
// http://getmoai.com
//

#ifndef MOAIKEYBOARDIOS_H
#define MOAIKEYBOARDIOS_H

#import <moaicore/moaicore.h>

//================================================================//
// MOAIKeyboardAndroid
//================================================================//
/** @name MOAIKeyboardAndroid
 @text <p>Wrapper for the native keyboard on Android.  Compatible
 with the iOS methods, albeit with JNI interjection.
 
 // !J! iOS constants don't belong here but for ref when the input-type select TODO is done:
 @const  EVENT_INPUT
 @const  EVENT_RETURN
 
 @const  AUTOCAP_ALL
 @const  AUTOCAP_NONE
 @const  AUTOCAP_SENTENCES
 @const  AUTOCAP_WORDS
 
 @const  KEYBOARD_ASCII
 @const  KEYBOARD_DECIMAL_PAD
 @const  KEYBOARD_DEFAULT
 @const  KEYBOARD_EMAIL
 @const  KEYBOARD_NUM_PAD
 @const  KEYBOARD_NUMERIC
 @const  KEYBOARD_PHONE_PAD
 @const  KEYBOARD_TWITTER
 @const  KEYBOARD_URL
 
 @const  APPEARANCE_ALERT
 @const  APPEARANCE_DEFAULT
 
 @const  RETURN_KEY_DEFAULT
 @const  RETURN_KEY_DONE
 @const  RETURN_KEY_GO
 @const  RETURN_KEY_JOIN
 @const  RETURN_KEY_NEXT
 @const  RETURN_KEY_ROUTE
 @const  RETURN_KEY_SEARCH
 @const  RETURN_KEY_SEND
 
 
 Android equivalents:
 
 Must be one or more (separated by '|') of the following constant values.
 Constant          Value Description
 none                0x00000000  There is no content type. The text is not editable.
 text                0x00000001  Just plain old text. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_NORMAL.
 textCapCharacters   0x00001001  Can be combined with text and its variations to request capitalization of all characters. Corresponds to TYPE_TEXT_FLAG_CAP_CHARACTERS.
 textCapWords        0x00002001  Can be combined with text and its variations to request capitalization of the first character of every word. Corresponds to TYPE_TEXT_FLAG_CAP_WORDS.
 textCapSentences    0x00004001  Can be combined with text and its variations to request capitalization of the first character of every sentence. Corresponds to TYPE_TEXT_FLAG_CAP_SENTENCES.
 textAutoCorrect     0x00008001  Can be combined with text and its variations to request auto-correction of text being input. Corresponds to TYPE_TEXT_FLAG_AUTO_CORRECT.
 textAutoComplete    0x00010001  Can be combined with text and its variations to specify that this field will be doing its own auto-completion and talking with the input method appropriately. Corresponds to TYPE_TEXT_FLAG_AUTO_COMPLETE.
 textMultiLine       0x00020001  Can be combined with text and its variations to allow multiple lines of text in the field. If this flag is not set, the text field will be constrained to a single line. Corresponds to TYPE_TEXT_FLAG_MULTI_LINE.
 textImeMultiLine    0x00040001  Can be combined with text and its variations to indicate that though the regular text view should not be multiple lines, the IME should provide multiple lines if it can. Corresponds to TYPE_TEXT_FLAG_IME_MULTI_LINE.
 textNoSuggestions   0x00080001  Can be combined with text and its variations to indicate that the IME should not show any dictionary-based word suggestions. Corresponds to TYPE_TEXT_FLAG_NO_SUGGESTIONS.
 textUri             0x00000011  Text that will be used as a URI. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_URI.
 textEmailAddress    0x00000021  Text that will be used as an e-mail address. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_EMAIL_ADDRESS.
 textEmailSubject    0x00000031  Text that is being supplied as the subject of an e-mail. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_EMAIL_SUBJECT.
 textShortMessage    0x00000041  Text that is the content of a short message. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_SHORT_MESSAGE.
 textLongMessage     0x00000051  Text that is the content of a long message. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_LONG_MESSAGE.
 textPersonName      0x00000061  Text that is the name of a person. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_PERSON_NAME.
 textPostalAddress   0x00000071  Text that is being supplied as a postal mailing address. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_POSTAL_ADDRESS.
 textPassword        0x00000081  Text that is a password. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_PASSWORD.
 textVisiblePassword 0x00000091  Text that is a password that should be visible. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_VISIBLE_PASSWORD.
 textWebEditText     0x000000a1  Text that is being supplied as text in a web form. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_WEB_EDIT_TEXT.
 textFilter          0x000000b1  Text that is filtering some other data. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_FILTER.
 textPhonetic        0x000000c1  Text that is for phonetic pronunciation, such as a phonetic name field in a contact entry. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_PHONETIC.
 textWebEmailAddress 0x000000d1  Text that will be used as an e-mail address on a web form. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_WEB_EMAIL_ADDRESS.
 textWebPassword     0x000000e1  Text that will be used as a password on a web form. Corresponds to TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_WEB_PASSWORD.
 number              0x00000002  A numeric only field. Corresponds to TYPE_CLASS_NUMBER | TYPE_NUMBER_VARIATION_NORMAL.
 numberSigned        0x00001002  Can be combined with number and its other options to allow a signed number. Corresponds to TYPE_CLASS_NUMBER | TYPE_NUMBER_FLAG_SIGNED.
 numberDecimal       0x00002002  Can be combined with number and its other options to allow a decimal (fractional) number. Corresponds to TYPE_CLASS_NUMBER | TYPE_NUMBER_FLAG_DECIMAL.
 numberPassword      0x00000012  A numeric password field. Corresponds to TYPE_CLASS_NUMBER | TYPE_NUMBER_VARIATION_PASSWORD.
 phone               0x00000003  For entering a phone number. Corresponds to TYPE_CLASS_PHONE.
 datetime            0x00000004  For entering a date and time. Corresponds to TYPE_CLASS_DATETIME | TYPE_DATETIME_VARIATION_NORMAL.
 date                0x00000014  For entering a date. Corresponds to TYPE_CLASS_DATETIME | TYPE_DATETIME_VARIATION_DATE.
 time                0x00000024  For entering a time. Corresponds to TYPE_CLASS_DATETIME | TYPE_DATETIME_VARIATION_TIME.
 
 Constant Value: 16843296 (0x01010220)
 
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
    void      Finish                  ();
    MOAIKeyboardAndroid     ();
    ~MOAIKeyboardAndroid    ();
    void      RegisterLuaClass        ( MOAILuaState& state );
    
    void      PushText                ( MOAILuaState& state );
    void      NotifyKeyEvent( );
    bool      FieldShouldReturn( );
    
};

#endif