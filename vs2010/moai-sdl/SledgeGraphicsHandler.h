#ifndef SLEDGEGRAPHICSHANDLER_H
#define SLEDGEGRAPHICSHANDLER_H

#include <pch.h>
#include <uslscore/USAccessors.h>
#include <uslscore/USBox.h>
#include <uslscore/USColor.h>
#include <uslscore/USLeanArray.h>
#include <uslscore/USStream.h>
#include <uslscore/USStreamReader.h>
#include <uslscore/USStreamWriter.h>
#include <uslscore/STLList.h>
#include <uslscore/STLSet.h>
#include <uslscore/STLMap.h>
#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAILua.h>

#include <vector>

#include <SDL.h>
#define SDL_main main


#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

#include "UtilityTypes.h"

class SledgeGraphicsHandler : 
	public MOAIGlobalClass <SledgeGraphicsHandler, MOAILuaObject> {
private:
	static int		_getSupportedResolutions( lua_State* L );
	static int		_setResolution( lua_State* L );
	

public:
	DECL_LUA_SINGLETON (SledgeGraphicsHandler)

	//----------------------------------------------------------------//
					SledgeGraphicsHandler	();
					~SledgeGraphicsHandler	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );

	static void		SetWindow				( SDL_Window*	p_window );
	static SDL_Window* m_window;
};



#endif