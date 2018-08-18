// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKPROXY_H
#define	MOAIDECKPROXY_H

#include <moai-sim/MOAIDeck.h>

//================================================================//
// MOAIDeckProxy
//================================================================//
// TODO: doxygen
class MOAIDeckProxy :
	public virtual MOAIDeck {
protected:

	MOAILuaSharedPtr < MOAIDeck >	mDeck;
	
	//----------------------------------------------------------------//
	static int				_setDeck						( lua_State* L );

	//----------------------------------------------------------------//
	ZLBounds				MOAIDeck_ComputeMaxBounds		();
	void					MOAIDeck_Draw					( u32 idx );
	ZLBounds				MOAIDeck_GetBounds				( u32 idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( u32 idx );
	bool					MOAIDeck_Overlap				( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap				( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );
	virtual u32				MOAIDeckProxy_Remap				( u32 idx );

public:
	
	//----------------------------------------------------------------//
							MOAIDeckProxy					();
							~MOAIDeckProxy					();
	void					RegisterLuaClass				( MOAILuaState& state );
	void					RegisterLuaFuncs				( MOAILuaState& state );
	void					SerializeIn						( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut					( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
