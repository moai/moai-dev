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

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaSharedPtr < MOAIDeck >	mDeck;
	
	//----------------------------------------------------------------//
	static int				_setDeck						( lua_State* L );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds				MOAIDeck_ComputeMaxAABB			();
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
};

#endif
