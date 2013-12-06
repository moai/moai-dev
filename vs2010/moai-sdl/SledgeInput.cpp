#include "stdafx.h"
#include "SledgeInput.h"

SledgeInput::SledgeInput()
{
	// register all classes SledgeInput derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )

		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( SledgeInput )
	RTTI_END

}

SledgeInput::~SledgeInput()
{
}

void SledgeInput::RegisterLuaClass(MOAILuaState& state)
{
	luaL_reg regTable[] = {
		{ "getNameForKeybScancode", _getNameForKeybScancode },
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

void SledgeInput::RegisterLuaFuncs(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

/**
 *	Get the local name for the specified SDL scancode (numeric).
 *
 *	@param	scancode	SDL keyboard scancode.
 *	@return	name		Local name.
 */
int SledgeInput::_getNameForKeybScancode( lua_State* L )
{
	MOAI_LUA_SETUP ( SledgeInput, "UN" );

	// get the scancode
	SDL_Scancode sc = (SDL_Scancode)(state.GetValue<int>(2, 0));

	// get the name
	const char* name = SDL_GetScancodeName(sc);

	// ...okay, let's see how this works.
	lua_pushstring(L, name);

	// tell lua that we actually returned a thing.
	return 1;
}