#include "stdafx.h"
#ifndef SLEDGESTEAMCHEEVOS_H
#define SLEDGESTEAMCHEEVOS_H

// Steam interop
#define _ACH_ID(id, name) { id, #id, name, "", 0, 0 }
struct Achievement_t
{
	s32			m_eAchievementID;
	const char*	m_pchAchievementID;
	char		m_rgchName[128];
	char		m_rgchDescription[256];
	bool		m_bAchieved;
	int			m_iIconImage;
};


class SledgeSteamCheevos :
	public MOAIGlobalClass <SledgeSteamCheevos, MOAILuaObject> {
private:
public:
	DECL_LUA_SINGLETON (SledgeSteamCheevos)

		SledgeSteamCheevos		();
		~SledgeSteamCheevos		();
	//------------------------------------------------------------------------//
	void		RegisterLuaClass( MOAILuaState& state );
	void		RegisterLuaFuncs( MOAILuaState& state );

	static int	_initCheevos	( lua_State* L );
	static int	_setCheevo		( lua_State* L );
}

#endif