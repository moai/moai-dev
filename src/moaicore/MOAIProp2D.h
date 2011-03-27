// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROP2D_H
#define	MOAIPROP2D_H

#include <moaicore/MOAITransform2D.h>
#include <moaicore/MOAIPartition.h>

class MOAIDeck2D;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIShader;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIProp2D
//================================================================//
/**	@brief Base class for primitives.
*/
class MOAIProp2D :
	public MOAITransform2D,
	public USPartitionPrim < MOAIProp2D > {
protected:
	
	enum {
		FRAME_FROM_SOURCE,
		FRAME_FROM_PARENT,
		FRAME_FROM_DEFINE,
	};
	
	enum {
		SCALE_TO_FRAME,
		STRETCH_TO_FRAME,
	};
	
	enum {
		REPEAT_X	= 0x00000001,
		REPEAT_Y	= 0x00000002,
	};
	
	USRef < MOAIDeck2D >	mDeck;
	u32						mIndex;
	USRef < MOAIGrid >		mGrid;
	u32						mRepeat;
	
	u32				mFrameSource;
	
	USRect			mFrame;
	int				mPriority;
	
	USRef < MOAIShader >		mShader;
	USRef < MOAITransform2D >	mUVTransform;
	
	//----------------------------------------------------------------//
	static int		_getGrid			( lua_State* L );
	static int		_getIndex			( lua_State* L );
	static int		_getPriority		( lua_State* L );
	static int		_inside				( lua_State* L );
	static int		_setDeck			( lua_State* L );
	static int		_setFrame			( lua_State* L );
	static int		_setFrameSource		( lua_State* L );
	static int		_setGrid			( lua_State* L );
	static int		_setIndex			( lua_State* L );
	static int		_setPriority		( lua_State* L );
	static int		_setRepeat			( lua_State* L );
	static int		_setShader			( lua_State* L );
	static int		_setUVTransform		( lua_State* L );
	
	//----------------------------------------------------------------//
	bool			BindDeck				();
	void			GetBoundsInRect			( const USRect& rect, USTileCoord& c0, USTileCoord& c1 );
	void			GetBoundsInView			( USTileCoord& c0, USTileCoord& c1 );
	void			LoadShader				();

public:
	
	enum {
		FRAME_EMPTY,
		FRAME_GLOBAL,
		FRAME_OK,
	};
	
	enum {
		ATTR_INDEX = MOAITransform2D::TOTAL_ATTR,
		TOTAL_ATTR,
	};
	
	GET_SET ( int, Priority, mPriority )
	GET_SET ( u32, Index, mIndex )
	
	DECL_LUA_DATA ( MOAIProp2D )
	
	//----------------------------------------------------------------//
	void							ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32								CountAttributes			();
	virtual void					Draw					();
	virtual void					DrawDebug				();
	virtual void					GatherSurfaces			( MOAISurfaceSampler2D& sampler );
	virtual u32						GetLocalFrame			( USRect& frame );
	bool							Inside					( USVec2D vec );
	virtual MOAIOverlapPrim2D*		IsOverlapPrim2D			();
									MOAIProp2D				();
									~MOAIProp2D				();
	void							OnDepNodeUpdate			();
	void							RegisterLuaClass		( USLuaState& state );
	void							RegisterLuaFuncs		( USLuaState& state );
	void							SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void							SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	STLString						ToString				();
};

#endif
