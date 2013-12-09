#include "stdafx.h"
#include "SledgeSteamCheevos.h"

SledgeSteamCheevos::SledgeSteamCheevos()
{
	// register all classes SledgeInput derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )

		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( SledgeInput )
	RTTI_END

}

SledgeSteamCheevos::~SledgeSteamCheevos()
{
}

void SledgeSteamCheevos::RegisterLuaClass(MOAILuaState& state)
{
	luaL_reg regTable[] = {
		{ "setAchievement", _setCheevo },
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

void SledgeSteamCheevos::RegisterLuaFuncs(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

/**
 *	Init cheevos.
 *
 *	@param	cheevotable	Table of integer / string cheevo pairs.
 *	@return	name		Local name.
 */
int SledgeSteamCheevos::_initCheevos( lua_State* L )
{


	// tell lua that it's all okay
	return 0;
}

/**
 *	Attempt to set an achievement; dispatches an async call to Steam.
 *	
 *	@param	id			string identifier of the achievement you want to set.
 *	@return	success		a boolean value representing if the call succeeded or
 *						not.
 */
int SledgeSteamCheevos::_setCheevo( lua_State* L )
{
	MOAI_LUA_SETUP ( SledgeInput, "UT" );

	// @todo	some cheevo stuff: none of the below is going to work until
	//			steam is hooked up



	// tell lua that we returned nothing.
	return 0;
}