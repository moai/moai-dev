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
	void					MOAIDeck_Draw					( ZLIndex idx );
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );
	virtual ZLIndex			MOAIDeckProxy_Remap				( ZLIndex idx );

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
