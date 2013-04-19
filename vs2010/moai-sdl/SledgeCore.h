#ifndef SLEDGECORE_H
#define SLEDGECORE_H

#include <uslscore/pch.h>
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
#include <moaicore/MOAIEnvironment.h>


//#include <io.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <errno.h>


#include <SDL.h>
#define SDL_main main


#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

#include "UtilityTypes.h"

class SledgeCore :
	public MOAIGlobalClass <SledgeCore, MOAILuaObject> {
private:
	static int		_setupDirectories	( lua_State* L );
	static int		_quitGame			( lua_State* L );

	static void		createDir			( const char* absolutePath );
	static int		dirExists			( const char* absolutePath );

public:
	DECL_LUA_SINGLETON (SledgeCore)

	//----------------------------------------------------------------//
					SledgeCore			();
					~SledgeCore			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};



#endif