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
	MOAILuaSharedPtr < MOAIImage >		mHitMask;
	
	u32		mHitColorScalar;
	u32		mHitColorThreshold;

	ZLBox	mMaxBounds;
	bool	mBoundsDirty;
	
	u32		mHitGranularity;

	//----------------------------------------------------------------//
	static int				_setBoundsDeck			( lua_State* L );
	static int				_setHitGranularity		( lua_State* L );
	static int				_setHitMask				( lua_State* L );
	static int				_setHitMaskScalar		( lua_State* L );
	static int				_setHitMaskThreshold	( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLBox					ComputeMaxBounds			() = 0;
	virtual void					DrawIndex					( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	virtual ZLBox					GetItemBounds				( u32 idx ) = 0;
	void							SetBoundsDirty				();
	bool							TestHit						( float x, float y ); // in local (uv) space of the mask
	bool							TestHit						( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ); // in local (model) space of the quad

public:
	
	enum {
		HIT_TEST_COARSE,	// no hit test will be performed; only the prop's bounds will be used
		HIT_TEST_MEDIUM,	// implementation dependent
		HIT_TEST_FINE,		// implementation dependent
	};
	
	GET ( u32, HitGranularity, mHitGranularity )
	
	//----------------------------------------------------------------//
	virtual bool					Contains					( u32 idx, const ZLVec2D& vec );
	void							Draw						( u32 idx, MOAIMaterialBatch& materials );
	void							Draw						( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	ZLBox							GetBounds					();
	ZLBox							GetBounds					( u32 idx );
	virtual void					GetCollisionShape			( MOAICollisionShape& shape );
	virtual bool					Inside						( u32 idx, ZLVec3D vec, float pad );
									MOAIDeck					();
									~MOAIDeck					();
	void							RegisterLuaClass			( MOAILuaState& state );
	void							RegisterLuaFuncs			( MOAILuaState& state );
	virtual MOAIMaterialBatch*		ResolveMaterialBatch		( MOAIMaterialBatch* override );
};

#endif
