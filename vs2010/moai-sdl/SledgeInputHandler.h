// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef SLEDGEINPUTHANDLER_H
#define SLEDGEINPUTHANDLER_H

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


#include <SDL.h>
#define SDL_main main

//#include <moaicore/MOAILuaObject.h>

#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

#include "UtilityTypes.h"
#include "SledgeInputManager.h"


//================================================================//
// SledgeInputHandler
//================================================================//
/**	@name	SledgeInputHandler
	@text	Lua wrapper for SledgeInputManager.
*/
class SledgeInputHandler :
	public MOAIGlobalClass <SledgeInputHandler, MOAILuaObject> {
	//public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int		_classHello			( lua_State* L );
	static int		_instanceHello		( lua_State* L );
	static int		_setDeadzones		( lua_State* L );

	static SledgeInputManager* _manager;

public:
	
	DECL_LUA_SINGLETON ( SledgeInputHandler )
	//DECL_LUA_FACTORY ( SledgeInputHandler )

	//----------------------------------------------------------------//
					SledgeInputHandler	();
					~SledgeInputHandler	();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );

	static void		SetManager			( SledgeInputManager* p_manager );
};

#endif
