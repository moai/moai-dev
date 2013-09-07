#include "stdafx.h"
#ifndef SLEDGEGRAPHICSHANDLER_H
#define SLEDGEGRAPHICSHANDLER_H

#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAILua.h>

#include "UtilityTypes.h"

struct ScreenEnvInfo
{
	int screenDim[2];
	int screenCount;
	bool retina;
};

class SledgeGraphicsHandler : 
	public MOAIGlobalClass <SledgeGraphicsHandler, MOAILuaObject> {
private:
	static int		_getSupportedResolutions( lua_State* L );
	static int		_setResolution( lua_State* L );
		static int		_getScreenList( lua_State* L );

public:
	DECL_LUA_SINGLETON (SledgeGraphicsHandler)

	//----------------------------------------------------------------//
					SledgeGraphicsHandler	();
					~SledgeGraphicsHandler	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );

	static void		SetWindow				( SDL_Window*	p_window );
	static SDL_Window* m_window;

	static ScreenEnvInfo GetScreenEnvInfo (  );
};



#endif