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

// right now, MOAIDeck is going to be an everything sandwich that ties all the moai-sim
// subsystems together - collision, graphics, bounds, hit tests. since MOAIDeck is the
// base, not a big deal to have a top-level interface in one place. if it becomes
// unwieldy, easy enough to factor more down the road.

//================================================================//
// MOAIDeck
//================================================================//
/**	@lua	MOAIDeck
	@text	Base class for all decks.
*/
class MOAIDeck :
	public virtual MOAILuaObject {
protected:

	friend class MOAIDeckProxy;

	enum {
		NO_CONTENT = 0xffffffff,
	};
	
	ZLBounds				mMaxBounds;
	bool					mBoundsDirty;
	
	//----------------------------------------------------------------//
	static int				_draw							( lua_State* L );
	static int				_getBounds						( lua_State* L );

	//----------------------------------------------------------------//
	void					SetBoundsDirty					();

	//----------------------------------------------------------------//
	virtual ZLBounds				MOAIDeck_ComputeMaxBounds		();
	virtual void					MOAIDeck_Draw					( ZLIndex idx );
	virtual ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	virtual MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	virtual bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	virtual bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );

public:
	
	//----------------------------------------------------------------//
	void					Draw							( ZLIndex idx );
	ZLBounds				GetBounds						();
	ZLBounds				GetBounds						( ZLIndex idx );
	MOAICollisionShape*		GetCollisionShape				( ZLIndex idx );
							MOAIDeck						();
							~MOAIDeck						();
	bool					Overlap							( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result = 0 );
	void					RegisterLuaClass				( MOAILuaState& state );
	void					RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
