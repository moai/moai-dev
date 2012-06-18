// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIKEYBOARDANDROID_H
#define	MOAIKEYBOARDANDROID_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAIFacebookAndroid
//================================================================//
/**	@name	MOAIFacebookAndroid
	@text	Wrapper for Facebook integration on Android devices.
			Facebook provides social integration for sharing on
			www.facebook.com. Exposed to lua via MOAIFacebook on 
			all mobile platforms.

	@const	DIALOG_DID_COMPLETE			Event code for a successfully completed Facebook dialog.
	@const	DIALOG_DID_NOT_COMPLETE		Event code for a failed (or canceled) Facebook dialog.
	@const	SESSION_DID_LOGIN			Event code for a successfully completed Facebook login.
	@const	SESSION_DID_NOT_LOGIN		Event code for a failed (or canceled) Facebook login.
*/
class MOAIKeyboardAndroid :
	public MOAIGlobalClass < MOAIKeyboardAndroid, MOAIGlobalEventSource > {
private:
        
    	enum {
            AUTOCAP_ALL				= 0,
            AUTOCAP_NONE			= 1,
            AUTOCAP_SENTENCES		= 2,
            AUTOCAP_WORDS			= 3,
            
            KEYBOARD_ASCII			= 4,
            KEYBOARD_DECIMAL_PAD	= 5,
            KEYBOARD_DEFAULT		= 6,
            KEYBOARD_EMAIL			= 7,
            KEYBOARD_NUM_PAD		= 8,
            KEYBOARD_NUMERIC		= 9,
            KEYBOARD_PHONE_PAD		= 10,
            KEYBOARD_TWITTER		= 11,
            KEYBOARD_URL			= 12,
            
            APPEARANCE_ALERT		= 13,
            APPEARANCE_DEFAULT		= 14,
            
            RETURN_KEY_DEFAULT		= 15,
            RETURN_KEY_DONE			= 16,
            RETURN_KEY_GO			= 17,
            RETURN_KEY_JOIN			= 18,
            RETURN_KEY_NEXT			= 19,
            RETURN_KEY_ROUTE		= 20,
            RETURN_KEY_SEARCH		= 21,
            RETURN_KEY_SEND			= 22,
        };    
        

	//----------------------------------------------------------------//
	//static int	_getToken		( lua_State* L );
	//static int	_init			( lua_State* L );
	//static int	_login			( lua_State* L );
	//static int	_logout			( lua_State* L );
	static cc8*	_luaParseTable 	( lua_State* L, int idx );

    //----------------------------------------------------------------//    
    static int		_getText				( lua_State* L );
    static int		_showKeyboard			( lua_State* L );

    //----------------------------------------------------------------//
    
    void                showKeyboard			( cc8* text, int type, int returnKey, bool secure, int autocap, int appearance );   
        
        
public:	

	DECL_LUA_SINGLETON ( MOAIKeyboardAndroid );

        enum {
            EVENT_INPUT,
            EVENT_RETURN,
            
        };
		
	//MOAILuaRef		mListeners [ TOTAL ];
	
        void			Finish					();
        MOAIKeyboardAndroid		();
        ~MOAIKeyboardAndroid		();
        void			PushText				( MOAILuaState& state );
        void			RegisterLuaClass		( MOAILuaState& state );
};

#endif  //MOAIFACEBOOK_H