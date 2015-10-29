// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTANDARDDECK_H
#define	MOAISTANDARDDECK_H

#include <moai-sim/MOAIDeck.h>
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
// MOAIStandardDeck
//================================================================//
/**	@lua	MOAIStandardDeck
	@text	Base class for decks with standard capabilities (materials, bounds override).
*/
class MOAIStandardDeck :
	public virtual MOAIDeck,
	public virtual MOAIMaterialBatch {
protected:

	MOAILuaSharedPtr < MOAIBoundsDeck > mBoundsDeck; // bounds override

	ZLBox	mMaxBounds;
	bool	mBoundsDirty;
	
	//----------------------------------------------------------------//
	static int				_setBoundsDeck				( lua_State* L );

	//----------------------------------------------------------------//
	void					DrawIndex					( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	void					SetBoundsDirty				();

public:
	
	//----------------------------------------------------------------//
	bool					Contains					( u32 idx, const ZLVec2D& vec );
	void					Draw						( u32 idx, MOAIMaterialBatch& materials );
	void					Draw						( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	ZLBox					GetBounds					();
	ZLBox					GetBounds					( u32 idx );
	void					GetCollisionShape			( MOAICollisionShape& shape );
	bool					Inside						( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad );
							MOAIStandardDeck			();
							~MOAIStandardDeck			();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	MOAIMaterialBatch&		ResolveMaterialBatch		( MOAIMaterialBatch* override );
};

#endif
