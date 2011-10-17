// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROP2D_H
#define	MOAIPROP2D_H

#include <moaicore/MOAIBlendMode.h>
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIProp.h>

class MOAICellCoord;
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
	
	@const	BLEND_NORMAL
	@const	BLEND_ADD
	@const	BLEND_MULTIPLY
	
	@const	GL_ONE
	@const	GL_ZERO
	@const	GL_DST_ALPHA
	@const	GL_DST_COLOR
	@const	GL_SRC_COLOR
	@const	GL_ONE_MINUS_DST_ALPHA
	@const	GL_ONE_MINUS_DST_COLOR
	@const	GL_ONE_MINUS_SRC_ALPHA
	@const	GL_ONE_MINUS_SRC_COLOR
	@const	GL_SRC_ALPHA
	@const	GL_SRC_ALPHA_SATURATE
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
	
	MOAIBlendMode				mBlendMode;
	bool						mVisible;
	
	//----------------------------------------------------------------//
	static int		_getGrid			( lua_State* L );
	static int		_getIndex			( lua_State* L );
	static int		_getRect			( lua_State* L );
	static int		_inside				( lua_State* L );
	static int		_setBlendMode		( lua_State* L );
	static int		_setDeck			( lua_State* L );
	static int		_setFrame			( lua_State* L );
	static int		_setGrid			( lua_State* L );
	static int		_setGridScale		( lua_State* L );
	static int		_setIndex			( lua_State* L );
	static int		_setRemapper		( lua_State* L );
	static int		_setRepeat			( lua_State* L );
	static int		_setShader			( lua_State* L );
	static int		_setUVTransform		( lua_State* L );
	static int		_setVisible			( lua_State* L );
	
	//----------------------------------------------------------------//
	bool				BindDeck				();
	void				GetBoundsInRect			( const USRect& rect, MOAICellCoord& c0, MOAICellCoord& c1 );
	void				GetBoundsInView			( MOAICellCoord& c0, MOAICellCoord& c1 );
	void				LoadShader				();

public:
	
	DECL_LUA_FACTORY ( MOAIProp2D )
	DECL_ATTR_HELPER ( MOAIProp2D )
	
	enum {
		ATTR_INDEX,
		ATTR_PARTITION,
		ATTR_SHADER,
		ATTR_BLEND_MODE,
		ATTR_VISIBLE,
		
		INHERIT_FRAME,
		FRAME_TRAIT,
		
		TOTAL_ATTR,
	};
	
	GET_SET ( u32, Index, mIndex )
	
	//----------------------------------------------------------------//
	bool							ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	virtual void					Draw					();
	virtual void					DrawDebug				();
	virtual void					GatherSurfaces			( MOAISurfaceSampler2D& sampler );
	virtual u32						GetLocalFrame			( USRect& frame );
	bool							Inside					( USVec2D vec, float pad );
	virtual MOAIOverlapPrim2D*		IsOverlapPrim2D			();
									MOAIProp2D				();
									~MOAIProp2D				();
	void							OnDepNodeUpdate			();
	void							RegisterLuaClass		( USLuaState& state );
	void							RegisterLuaFuncs		( USLuaState& state );
	void							SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void							SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
};

#endif
