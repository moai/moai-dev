// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCOPE_H
#define	MOAISCOPE_H

#include <moai-core/MOAILua.h>

//================================================================//
// MOAIScope
//================================================================//
// TODO: doxygen
class MOAIScope :
	public virtual MOAILuaObject {
private:

	STLSet < MOAILuaObject* >		mObjects;
	u32								mScopeRefCount;

	//----------------------------------------------------------------//
	static int			_countRetained				( lua_State* L );
	static int			_newindex					( lua_State* L );
	static int			_purge						( lua_State* L );
	static int			_release					( lua_State* L );
	static int			_retain						( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_DecorateLuaBinding		( MOAILuaState& state );
	void				MOAILuaObject_OnPooledRemit				();

public:

	DECL_LUA_FACTORY ( MOAIScope )

	//----------------------------------------------------------------//
	void				AffirmObject				( MOAILuaObject& object );
						MOAIScope					( ZLContext& context );
						~MOAIScope					();
	void				Purge						();
	void				RemoveObject				( MOAILuaObject& object );
	void				ScopeRelease				();
	void				ScopeRetain					();
};

#endif
