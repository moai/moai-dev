// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROP2D_H
#define	MOAIPROP2D_H

#include <moaicore/MOAIProp.h>

class MOAIDeck;
class MOAIDeckRemapper;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIShader;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIProp2D
//================================================================//
/**	@name	MOAIProp2D
	@text	Base class for 2D props.
	
	@attr	ATTR_INDEX
	
	@const	FRAME_FROM_DECK
	@const	FRAME_FROM_PARENT
	@const	FRAME_FROM_SELF
*/
class MOAIProp2D :
	public MOAIProp {
protected:
	
	enum {
		FRAME_FROM_DECK,
		FRAME_FROM_PARENT,
		FRAME_FROM_SELF,
	};
	
	enum {
		REPEAT_X	= 0x00000001,
		REPEAT_Y	= 0x00000002,
	};
	
	USRef < MOAIDeck >			mDeck;
	USRef < MOAIDeckRemapper >	mRemapper;
	u32							mIndex;
	
	USRef < MOAIGrid >			mGrid;
	u32							mRepeat;
	USVec2D						mGridScale;
	
	u32							mFrameSource;
	USRect						mFrame;
	
	USRef < MOAIShader >		mShader;
	USRef < MOAITransformBase >	mUVTransform;
	
	//----------------------------------------------------------------//
	static int		_getGrid			( lua_State* L );
	static int		_getIndex			( lua_State* L );
	static int		_inside				( lua_State* L );
	static int		_setDeck			( lua_State* L );
	static int		_setFrame			( lua_State* L );
	static int		_setFrameSource		( lua_State* L );
	static int		_setGrid			( lua_State* L );
	static int		_setGridScale		( lua_State* L );
	static int		_setIndex			( lua_State* L );
	static int		_setRemapper		( lua_State* L );
	static int		_setRepeat			( lua_State* L );
	static int		_setShader			( lua_State* L );
	static int		_setUVTransform		( lua_State* L );
	
	//----------------------------------------------------------------//
	bool			BindDeck				();
	void			GetBoundsInRect			( const USRect& rect, USCellCoord& c0, USCellCoord& c1 );
	void			GetBoundsInView			( USCellCoord& c0, USCellCoord& c1 );
	void			LoadShader				();

public:
	
	enum {
		ATTR_INDEX = MOAITransform::TOTAL_ATTR,
		TOTAL_ATTR,
	};
	
	GET_SET ( u32, Index, mIndex )
	
	DECL_LUA_FACTORY ( MOAIProp2D )
	
	//----------------------------------------------------------------//
	void							ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
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
