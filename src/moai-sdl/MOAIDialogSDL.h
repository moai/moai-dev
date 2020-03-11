// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDIALOGSDL_H
#define	MOAIDIALOGSDL_H

#include <moai-core/headers.h>
#include <SDL.h>

//================================================================//
// MOAIDialogSDL
//================================================================//
class MOAIDialogSDL :
	public ZLContextClass < MOAIDialogSDL >,
	public virtual MOAILuaObject {
private:

	SDL_Window*		mWindow;

	//----------------------------------------------------------------//
	static int		_show					( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_SINGLETON ( MOAIDialogSDL )
	
	GET_SET ( SDL_Window*, Window, mWindow )
	
	//----------------------------------------------------------------//
					MOAIDialogSDL			();
					~MOAIDialogSDL			();
};

#endif
