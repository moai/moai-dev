// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONWORLD_H
#define	MOAICOLLISIONWORLD_H

#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIProp.h>

//================================================================//
// MOAICollisionWorld
//================================================================//
class MOAICollisionWorld :
	public MOAIAction {
private:

	MOAILuaSharedPtr < MOAIPartition >		mPartition;

	//----------------------------------------------------------------//
	static int		_clear					( lua_State* L );
	static int		_getPartition			( lua_State* L );
	static int		_insertProp				( lua_State* L );
	static int		_removeProp				( lua_State* L );
	static int		_setPartition			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			AffirmPartition			();

public:
	
	DECL_LUA_FACTORY ( MOAICollisionWorld )
	
	//----------------------------------------------------------------//
					MOAICollisionWorld		();
					~MOAICollisionWorld		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
