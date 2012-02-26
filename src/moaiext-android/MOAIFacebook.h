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
	static int		_login				( lua_State* L );
	static int		_setListener 		( lua_State* L );
	
	
public:	
	enum {
		FACEBOOK_LOGIN_LISTENER,
		TOTAL_LISTENERS
	};
	
	MOAILuaRef		mListeners [ TOTAL_LISTENERS ];
	
	DECL_LUA_SINGLETON ( MOAIFacebook );

					MOAIFacebook					();
					~MOAIFacebook					();
	void 			NotifyFacebookLogin 			( int );
	void			RegisterLuaClass				( MOAILuaState& state );
	
	
};

#endif  //DISABLE_FACEBOOK

#endif  //MOAIFACEBOOK_H
