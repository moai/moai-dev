// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCOPE_H
#define	MOAISCOPE_H

#include <moai-core/MOAILua.h>

class MOAIPoolableObject;

//================================================================//
// MOAIScope
//================================================================//
// TODO: doxygen
class MOAIScope :
	public virtual MOAILuaObject {
private:

	STLSet < MOAIPoolableObject* >		mObjects;
	u32									mScopeRefCount;

	//----------------------------------------------------------------//
	static int			_purge						( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAIScope )

	//----------------------------------------------------------------//
	void				AddObject					( MOAIPoolableObject* object );
						MOAIScope					();
						~MOAIScope					();
	void				Purge						();
	void				ScopeRelease				();
	void				ScopeRetain					();
};

#endif
