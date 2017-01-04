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
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( u32 idx );
	
	ZLBox					GetItemBounds					( u32 idx );
	
public:
	
	DECL_LUA_FACTORY ( MOAICollisionDeck )
	
	//----------------------------------------------------------------//
	void			DrawIndex				( u32 idx, MOAIMaterialBatch* materials, ZLVec3D offset, ZLVec3D scale );
	bool			Inside					( u32 idx, MOAIMaterialBatch* materials, u32 granularity, ZLVec3D vec, float pad );
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
