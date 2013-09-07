#include "stdafx.h"
#ifndef SLEDGEINPUTHANDLER_H
#define SLEDGEINPUTHANDLER_H

#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAILua.h>

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
private:
	
	//----------------------------------------------------------------//
	static int		_classHello					( lua_State* L );
	static int		_instanceHello				( lua_State* L );
	static int		_setDeadzones				( lua_State* L );
	//static int		_getActiveControllerCount	( lua_State* L );
	static int		_quitGame ( lua_State* L );

	static SledgeInputManager* _manager;

public:
	
	DECL_LUA_SINGLETON ( SledgeInputHandler )

	//----------------------------------------------------------------//
					SledgeInputHandler	();
					~SledgeInputHandler	();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );

	static void		SetManager			( SledgeInputManager* p_manager );

};

#endif
