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
	static int		_getToken			( lua_State* L );
	static int		_init				( lua_State* L );
	static int		_login				( lua_State* L );
	static int		_logout				( lua_State* L );
	static int		_postToFeed			( lua_State* L );
	static int		_sessionValid 		( lua_State* L );
	static int		_setListener 		( lua_State* L );
	static int		_setToken	 		( lua_State* L );
	
	
public:	
	enum {
		DIALOG_DID_CANCEL,
		DIALOG_DID_COMPLETE,
		SESSION_DID_LOGIN,
		SESSION_DID_NOT_LOGIN,
		TOTAL_LISTENERS
	};
	
	MOAILuaRef		mListeners [ TOTAL_LISTENERS ];
	
	DECL_LUA_SINGLETON ( MOAIFacebook );

					MOAIFacebook					();
					~MOAIFacebook					();
	void 			NotifyFacebookDialog 			( int );
	void 			NotifyFacebookLogin 			( int );
	void			RegisterLuaClass				( MOAILuaState& state );
	
	static cc8* 	_luaParseTable 					( lua_State* L, int idx );
	
	
};

#endif  //DISABLE_FACEBOOK

#endif  //MOAIFACEBOOK_H
