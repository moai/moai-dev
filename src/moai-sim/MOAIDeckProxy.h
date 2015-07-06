// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
	static int						_setDeck					( lua_State* L );

public:
	
	//----------------------------------------------------------------//
	ZLBox							ComputeMaxBounds			();
	bool							Contains					( u32 idx, const ZLVec2D& vec );
	void							DrawIndex					( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	ZLBox							GetBounds					();
	ZLBox							GetBounds					( u32 idx );
	void							GetCollisionShape			( MOAICollisionShape& shape );
	ZLBox							GetItemBounds				( u32 idx );
	bool							Inside						( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad );
									MOAIDeckProxy				();
									~MOAIDeckProxy				();
	void							RegisterLuaClass			( MOAILuaState& state );
	void							RegisterLuaFuncs			( MOAILuaState& state );
	virtual MOAIMaterialBatch&		ResolveMaterialBatch		( MOAIMaterialBatch* override );
};

#endif
