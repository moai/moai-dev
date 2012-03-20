// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFACEBOOK_H
#define	MOAIFACEBOOK_H

#ifndef DISABLE_FACEBOOK

#include <moaicore/moaicore.h>

//================================================================//
// MOAIFacebook
//================================================================//
class MOAIFacebook :
	public MOAIGlobalClass < MOAIFacebook, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_getToken		( lua_State* L );
	static int	_init			( lua_State* L );
	static int	_login			( lua_State* L );
	static int	_logout			( lua_State* L );
	static cc8*	_luaParseTable 	( lua_State* L, int idx );
	static int	_postToFeed		( lua_State* L );
	static int	_sendRequest	( lua_State* L );
	static int	_sessionValid	( lua_State* L );
	static int	_setListener 	( lua_State* L );
	static int	_setToken	 	( lua_State* L );

public:	

	DECL_LUA_SINGLETON ( MOAIFacebook );

	enum {
		DIALOG_DID_COMPLETE,
		DIALOG_DID_NOT_COMPLETE,
		SESSION_DID_LOGIN,
		SESSION_DID_NOT_LOGIN,
		TOTAL,
	};
	
	enum {
        DIALOG_RESULT_SUCCESS,
        DIALOG_RESULT_CANCEL,
        DIALOG_RESULT_ERROR,
	};
		
	MOAILuaRef		mListeners [ TOTAL ];
	
					MOAIFacebook			();
					~MOAIFacebook			();
	void 			NotifyLoginComplete		( int code );
	void 			NotifyDialogComplete	( int code );
	void			RegisterLuaClass		( MOAILuaState& state );
};

#endif  //DISABLE_FACEBOOK

#endif  //MOAIFACEBOOK_H