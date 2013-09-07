#include "stdafx.h"
#ifndef SLEDGECORE_H
#define SLEDGECORE_H

#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIEnvironment.h>

#include "SledgeGraphicsHandler.h"

#include "UtilityTypes.h"

class SledgeCore :
	public MOAIGlobalClass <SledgeCore, MOAILuaObject> {
private:
	static const char* const SFSMOAIEnvKeys[];

public:
	DECL_LUA_SINGLETON (SledgeCore)

					SledgeCore			();
					~SledgeCore			();
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );

#if defined(_WIN32) || defined(_WIN64)
	static void		LoadInfoXML			( const char* p_xmlfilename, MOAIEnvironment* p_env );
	static void		GetAdditionalHWInfo	( MOAIEnvironment* p_env );
#endif

private:
	static s32		DoKillConsole		( lua_State* L );
	static s32		DoSetupDirectories	( lua_State* L );
	static s32		DoQuitGame			( lua_State* L );

	static void		createDir			( const char* absolutePath );
	static s32		dirExists			( const char* absolutePath );

};



#endif