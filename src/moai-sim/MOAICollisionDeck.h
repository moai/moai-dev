// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONDECK_H
#define	MOAICOLLISIONDECK_H

#include <moai-sim/MOAIDeck.h>

class MOAICollisionShape;

//================================================================//
// MOAICollisionDeck
//================================================================//
// TODO: doxygen
class MOAICollisionDeck :
	public MOAIDeck {
private:

	ZLLeanArray < ZLStrongPtr < MOAICollisionShape > >		mShapes;
	
	//----------------------------------------------------------------//
	static int		_reserveShapes			( lua_State* L );
	static int		_setBox					( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setQuad				( lua_State* L );

	//----------------------------------------------------------------//
	MOAICollisionShape&		AffirmShape						( ZLIndex idx );
	
	//----------------------------------------------------------------//
	ZLBounds				MOAIDeck_ComputeMaxAABB			();
	void					MOAIDeck_Draw					( ZLIndex idx );
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );
	
public:
	
	DECL_LUA_FACTORY ( MOAICollisionDeck )
	
	//----------------------------------------------------------------//
					MOAICollisionDeck		();
					~MOAICollisionDeck		();
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			ReserveShapes			( u32 totalShapes );
	void			SetBox					( ZLIndex idx, const ZLBox& box );
	void			SetRect					( ZLIndex idx, const ZLRect& rect );
	void			SetQuad					( ZLIndex idx, const ZLQuad& quad );
};

#endif
