#include "stdafx.h"

#ifndef SLEDGEINPUT_H
#define SLEDGEINPUT_H


#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAILua.h>

// need a way to map "virtual keys" to actual... keys.


//============================================================================//
// SledgeInput
//============================================================================//
class SledgeInput :
	public MOAIGlobalClass <SledgeInput, MOAILuaObject> {
private:
public:
	DECL_LUA_SINGLETON ( SledgeInput )
			
					SledgeInput				();
					~SledgeInput			();
	//----------------------------------------------------------------//
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			AKUInit					();

	//----------------------------------------------------------------//
	static int		_getNameForKeybScancode	( lua_State* L );
};


#endif