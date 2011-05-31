// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROP2D_H
#define	MOAIPROP2D_H

#include <moaicore/MOAIColor.h>
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
	public MOAIProp,
	public MOAIColor {
protected:
	
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
	
	USRect						mFrame;
	bool						mFitToFrame;
	
	USRef < MOAIShader >		mShader;
	USRef < MOAITransformBase >	mUVTransform;
	
	USColorVec					mColor;
	
	//----------------------------------------------------------------//
	static int		_getGrid			( lua_State* L );
	static int		_getIndex			( lua_State* L );
	static int		_inside				( lua_State* L );
	static int		_setDeck			( lua_State* L );
	static int		_setFrame			( lua_State* L );
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
	USColorVec		GetColorTrait			();
	USRect*			GetFrameTrait			();
	MOAIShader*		GetShaderTrait			();
	void			LoadShader				();

public:
	
	DECL_LUA_FACTORY ( MOAIProp2D )
	DECL_ATTR_HELPER ( MOAIProp2D )
	
	enum {
		ATTR_INDEX,
		TOTAL_ATTR,
	};
	
	enum {
		INHERIT_COLOR		= 0x00000004,
		INHERIT_FRAME		= 0x00000008,
		INHERIT_PARTITION	= 0x00000010,
	};
	
	
	GET_SET ( u32, Index, mIndex )
	
	//----------------------------------------------------------------//
	bool							ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
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
