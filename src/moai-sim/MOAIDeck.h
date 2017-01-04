// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
	
	MOAILuaSharedPtr < MOAIBoundsDeck > mBoundsDeck; // bounds override

	ZLBox	mMaxBounds;
	bool	mBoundsDirty;
	
	//----------------------------------------------------------------//
	static int				_draw							( lua_State* L );
	static int				_getBounds						( lua_State* L );
	static int				_setBoundsDeck					( lua_State* L );

	// right now, MOAIDeck is going to be an everything sandwich that ties all the moai-sim
	// subsystems together - collision, graphics, bounds, hit tests. since MOAIDeck is the
	// base, not a big deal to have a top-level interface in one place. if it becomes
	// unwieldy, easy enough to factor more down the road.

	//----------------------------------------------------------------//
	virtual MOAICollisionShape*			MOAIDeck_GetCollisionShape		( u32 idx );

public:
	
	//----------------------------------------------------------------//
	virtual ZLBox			ComputeMaxBounds				();
	virtual bool			Contains						( u32 idx, const ZLVec2D& vec );
	virtual void			Draw							( u32 idx, MOAIMaterialBatch* materials );
	virtual void			Draw							( u32 idx, MOAIMaterialBatch* materials, ZLVec3D offset, ZLVec3D scale );
	virtual void			DrawIndex						( u32 idx, MOAIMaterialBatch* materials, ZLVec3D offset, ZLVec3D scale );
	virtual ZLBox			GetBounds						();
	virtual ZLBox			GetBounds						( u32 idx );
	MOAICollisionShape*		GetCollisionShape				( u32 idx );
	virtual ZLBox			GetItemBounds					( u32 idx );
	virtual bool			Inside							( u32 idx, MOAIMaterialBatch* materials, u32 granularity, ZLVec3D vec, float pad ) = 0;
							MOAIDeck						();
							~MOAIDeck						();
	void					RegisterLuaClass				( MOAILuaState& state );
	void					RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
