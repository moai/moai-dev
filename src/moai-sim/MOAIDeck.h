// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK_H
#define	MOAIDECK_H

class MOAIBoundsDeck;
class MOAICellCoord;
class MOAICollisionShape;
class MOAIColor;
class MOAIGrid;
class MOAIImage;
class MOAIMaterialBatch;
class MOAIQuadBrush;
class MOAIShader;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIDeck
//================================================================//
/**	@lua	MOAIDeck
	@text	Base class for all decks.
*/
class MOAIDeck :
	public virtual MOAILuaObject {
protected:

	enum {
		NO_CONTENT = 0xffffffff,
	};
	
	//----------------------------------------------------------------//
	static int						_getBounds					( lua_State* L );
	static int						_subdivideRect				( lua_State* L );

public:
	
	//----------------------------------------------------------------//
	virtual ZLBox					ComputeMaxBounds			() = 0;
	virtual ZLBox					GetItemBounds				( u32 idx ) = 0;
	virtual bool					Contains					( u32 idx, const ZLVec2D& vec ) = 0;
	virtual void					Draw						( u32 idx, MOAIMaterialBatch& materials );
	virtual void					Draw						( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	virtual void					DrawIndex					( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) = 0;
	virtual ZLBox					GetBounds					() = 0;
	virtual ZLBox					GetBounds					( u32 idx ) = 0;
	virtual void					GetCollisionShape			( MOAICollisionShape& shape ) = 0;
	virtual bool					Inside						( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad ) = 0;
									MOAIDeck					();
									~MOAIDeck					();
	void							RegisterLuaClass			( MOAILuaState& state );
	void							RegisterLuaFuncs			( MOAILuaState& state );
	virtual MOAIMaterialBatch&		ResolveMaterialBatch		( MOAIMaterialBatch* override ) = 0;
};

#endif
