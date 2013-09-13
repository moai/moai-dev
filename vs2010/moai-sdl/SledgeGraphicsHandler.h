#include "stdafx.h"
#ifndef SLEDGEGRAPHICSHANDLER_H
#define SLEDGEGRAPHICSHANDLER_H

#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAILua.h>

#include "UtilityTypes.h"

/**
 * Graphics functionality helper class; accessible from Lua as
 * SledgeGraphicsHandler.
 *
 * @author	Jetha Chan
 * @date	14/09/2013
 */
class SledgeGraphicsHandler : 
	public MOAIGlobalClass <SledgeGraphicsHandler, MOAILuaObject> {
private:
	static SDL_Window*	m_window;

public:
	DECL_LUA_SINGLETON (SledgeGraphicsHandler)

							SledgeGraphicsHandler	(  );
							~SledgeGraphicsHandler	(  );
	//----------------------------------------------------------------//
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	static void				SetWindowHandle				( SDL_Window*	p_window );	
	static ScreenEnvInfo	GetScreenEnvInfo		(  );

private:
	static s32				DoGetScreenList			( lua_State* L );
	static s32				DoGetResolutionInfo		( lua_State* L );
	static s32				DoSetResolution			( lua_State* L );

};



#endif