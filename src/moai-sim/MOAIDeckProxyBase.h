// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKPROXY_H
#define	MOAIDECKPROXY_H

#include <moai-sim/MOAIDeck.h>

//================================================================//
// MOAIDeckProxyBase
//================================================================//
// TODO: doxygen
class MOAIDeckProxyBase :
	public virtual MOAIDeck {
protected:

	MOAILuaSharedPtr < MOAIDeck >	mDeck;
	
	//----------------------------------------------------------------//
	static int				_setDeck						( lua_State* L );

	//----------------------------------------------------------------//
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( u32 idx );

public:
	
	//----------------------------------------------------------------//
							MOAIDeckProxyBase				();
							~MOAIDeckProxyBase				();
	void					RegisterLuaClass				( MOAILuaState& state );
	void					RegisterLuaFuncs				( MOAILuaState& state );
	void					SerializeIn						( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut					( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
