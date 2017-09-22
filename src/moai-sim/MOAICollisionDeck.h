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
	MOAICollisionShape&		AffirmShape						( u32 idx );
	
	//----------------------------------------------------------------//
	ZLBounds				MOAIDeck_ComputeMaxBounds		();
	void					MOAIDeck_Draw					( u32 idx );
	ZLBounds				MOAIDeck_GetBounds				( u32 idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( u32 idx );
	bool					MOAIDeck_Overlap				( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap				( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );
	
public:
	
	DECL_LUA_FACTORY ( MOAICollisionDeck )
	
	//----------------------------------------------------------------//
					MOAICollisionDeck		();
					~MOAICollisionDeck		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveShapes			( u32 totalShapes );
	void			SetBox					( u32 idx, const ZLBox& box );
	void			SetRect					( u32 idx, const ZLRect& rect );
	void			SetQuad					( u32 idx, const ZLQuad& quad );
};

#endif
