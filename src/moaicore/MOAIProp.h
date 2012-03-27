// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROP_H
#define	MOAIPROP_H

#include <moaicore/MOAIBlendMode.h>
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIRenderable.h>
#include <moaicore/MOAITransform.h>

class MOAICellCoord;
class MOAICollisionShape;
class MOAIDeck;
class MOAIDeckRemapper;
class MOAIGfxState;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAIShader;
class MOAISurfaceSampler2D;
class MOAITextureBase;

//================================================================//
// MOAIProp
//================================================================//
/**	@name	MOAIProp
	@text	Base class for props.
	
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
	
	@const	DEPTH_TEST_DISABLE
	@const	DEPTH_TEST_NEVER
	@const	DEPTH_TEST_LESS
	@const	DEPTH_TEST_EQUAL
	@const	DEPTH_TEST_LESS_EQUAL
	@const	DEPTH_TEST_GREATER
	@const	DEPTH_TEST_NOTEQUAL
	@const	DEPTH_TEST_GREATER_EQUAL
	@const	DEPTH_TEST_ALWAYS
	
	@const	CULL_NONE
	@const	CULL_ALL
	@const	CULL_BACK
	@const	CULL_FRONT
*/
class MOAIProp :
	public MOAITransform,
	public MOAIColor,
	public MOAIRenderable {
private:

	friend class MOAIPartition;
	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;

	MOAIPartition*				mPartition;
	MOAIPartitionCell*			mCell;
	
	// this is only for debug draw
	MOAIPartitionLevel*			mLayer;
	
	USLeanLink < MOAIProp* >	mLinkInCell;
	MOAIProp*					mNextResult;

	u32				mMask;
	USBox			mBounds;
	s32				mPriority;
	
	//----------------------------------------------------------------//
	static int		_getBounds			( lua_State* L );
	static int		_getGrid			( lua_State* L );
	static int		_getIndex			( lua_State* L );
	static int		_getPriority		( lua_State* L );
	static int		_inside				( lua_State* L );
	static int		_setBlendMode		( lua_State* L );
	static int		_setCullMode		( lua_State* L );
	static int		_setDeck			( lua_State* L );
	static int		_setDepthMask		( lua_State* L );
	static int		_setDepthTest		( lua_State* L );
	static int		_setExpandForSort	( lua_State* L );
	static int		_setFrame			( lua_State* L );
	static int		_setGrid			( lua_State* L );
	static int		_setGridScale		( lua_State* L );
	static int		_setIndex			( lua_State* L );
	static int		_setParent			( lua_State* L );
	static int		_setPriority		( lua_State* L );
	static int		_setRemapper		( lua_State* L );
	static int		_setShader			( lua_State* L );
	static int		_setTexture			( lua_State* L );
	static int		_setUVTransform		( lua_State* L );
	static int		_setVisible			( lua_State* L );

protected:

	MOAILuaSharedPtr < MOAIDeck >			mDeck;
	MOAILuaSharedPtr < MOAIDeckRemapper >	mRemapper;
	u32										mIndex;
	
	MOAILuaSharedPtr < MOAIGrid >			mGrid;
	USVec2D									mGridScale;
	
	MOAILuaSharedPtr < MOAIShader >			mShader;
	MOAILuaSharedPtr < MOAIGfxState >		mTexture;
	MOAILuaSharedPtr < MOAITransformBase >	mUVTransform;
	
	USBox						mFrame;
	bool						mFitToFrame;
	
	int							mCullMode;
	int							mDepthTest;
	bool						mDepthMask;
	MOAIBlendMode				mBlendMode;
	bool						mVisible;
	
	bool						mExpandForSort;

	//----------------------------------------------------------------//
	virtual u32		GetDeckBounds			( USBox& bounds ); // get the deck bounds in model space
	u32				GetFrame				( USBox& bounds );
	void			GetGridBoundsInView		( MOAICellCoord& c0, MOAICellCoord& c1 );
	u32				GetPropBounds			( USBox& bounds ); // get the prop bounds in model space
	void			LoadGfxState			();
	void			UpdateBounds			( u32 status );
	void			UpdateBounds			( const USBox& bounds, u32 status );

public:

	DECL_LUA_FACTORY ( MOAIProp )
	DECL_ATTR_HELPER ( MOAIProp )

	static const s32 UNKNOWN_PRIORITY	= 0x80000000;
	static const int NO_SUBPRIM_ID		= 0xffffffff;

	enum {
		BOUNDS_EMPTY,
		BOUNDS_GLOBAL,
		BOUNDS_OK,
	};

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

	enum {
		CAN_DRAW					= 1 << 0x00,
		CAN_DRAW_DEBUG				= 1 << 0x01,
		CAN_GATHER_SURFACES			= 1 << 0x02,
	};

	GET_SET ( u32, Index, mIndex )
	GET_SET ( u32, Mask, mMask )
	GET ( s32, Priority, mPriority )
	GET ( MOAIPartition*, Partition, mPartition )
	
	GET ( MOAIDeck*, Deck, mDeck )
	GET ( MOAIDeckRemapper*, Remapper, mRemapper )
	GET ( USBox, Bounds, mBounds )

	//----------------------------------------------------------------//
	bool				ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	virtual void		Draw					( int subPrimID, bool reload );
	virtual void		DrawDebug				( int subPrimID );
	virtual void		ExpandForSort			( MOAIPartitionResultBuffer& buffer );
	virtual void		GatherSurfaces			( MOAISurfaceSampler2D& sampler );
	MOAIPartition*		GetPartitionTrait		();
	bool				GetCellRect				( USRect* cellRect, USRect* paddedRect = 0 );
	virtual void		GetCollisionShape		( MOAICollisionShape& shape );
	virtual bool		Inside					( USVec3D vec, float pad );
						MOAIProp				();
	virtual				~MOAIProp				();
	void				OnDepNodeUpdate			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Render					();
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void				SetPartition			( MOAIPartition* partition );
};

#endif
