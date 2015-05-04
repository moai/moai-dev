// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

#include <moai-sim/MOAIMaterialBatch.h>

class MOAIBoundsDeck;
class MOAICellCoord;
class MOAICollisionShape;
class MOAIColor;
class MOAIGfxState;
class MOAIGrid;
class MOAIImage;
class MOAIQuadBrush;
class MOAIShader;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIDeck
//================================================================//
/**	@lua	MOAIDeck
	@text	Base class for decks.
*/
class MOAIDeck :
	public virtual MOAIMaterialBatch {
protected:

	enum {
		NO_CONTENT = 0xffffffff,
	};

	MOAILuaSharedPtr < MOAIBoundsDeck > mBoundsDeck; // bounds override

	ZLBox	mMaxBounds;
	bool	mBoundsDirty;
	
	//----------------------------------------------------------------//
	static int						_setBoundsDeck				( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLBox					ComputeMaxBounds			() = 0;
	virtual void					DrawIndex					( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	virtual ZLBox					GetItemBounds				( u32 idx ) = 0;
	void							SetBoundsDirty				();

public:
	
	//----------------------------------------------------------------//
	virtual bool					Contains					( u32 idx, const ZLVec2D& vec );
	void							Draw						( u32 idx, MOAIMaterialBatch& materials );
	void							Draw						( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	ZLBox							GetBounds					();
	ZLBox							GetBounds					( u32 idx );
	virtual void					GetCollisionShape			( MOAICollisionShape& shape );
	virtual bool					Inside						( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad );
									MOAIDeck					();
									~MOAIDeck					();
	void							RegisterLuaClass			( MOAILuaState& state );
	void							RegisterLuaFuncs			( MOAILuaState& state );
	virtual MOAIMaterialBatch&		ResolveMaterialBatch		( MOAIMaterialBatch* override );
};

#endif
