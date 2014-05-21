// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROP_H
#define	MOAIPROP_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>

class MOAICellCoord;
class MOAICollisionFacet;
class MOAIDeck;
class MOAIDeckRemapper;
class MOAIGfxState;
class MOAIGraphicsProp;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAIScissorRect;
class MOAIShader;
class MOAISurfaceSampler2D;
class MOAITextureBase;

class MOAIProp;

//================================================================//
// MOAIFacet
//================================================================//
class MOAIFacet :
	public virtual MOAILuaObject {
protected:

	friend class MOAIProp;
	
	MOAIProp* mProp;

	//----------------------------------------------------------------//
	virtual void	OnAttach		( MOAIProp& prop );
	virtual void	OnDetach		( MOAIProp& prop );

public:

	//----------------------------------------------------------------//
	MOAIProp&		GetProp			();
					MOAIFacet		();
					~MOAIFacet		();
};

//================================================================//
// MOAIProp
//================================================================//
/**	@name	MOAIProp
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	@attr	ATTR_PARTITION
*/
class MOAIProp :
	public MOAITransform {
public:

	static const u32 UNKNOWN_FACET = 0xffffffff;

	enum {
		COLLISION_FACET,
		TOTAL_FACETS,
	};

private:

	friend class MOAIPartition;
	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;

	MOAIPartition*				mPartition;
	MOAIPartitionCell*			mCell;
	
	// this is only for debug draw
	MOAIPartitionLevel*			mLayer;
	
	ZLLeanLink < MOAIProp* >	mLinkInCell;
	MOAIProp*					mNextResult;

	u32							mMask;
	s32							mPriority;
	ZLBox						mWorldBounds;
	
	//----------------------------------------------------------------//
	static int			_getBounds					( lua_State* L );
	static int			_getDeck					( lua_State* L );
	static int			_getDims					( lua_State* L );
	static int			_getFacet					( lua_State* L );
	static int			_getGrid					( lua_State* L );
	static int			_getIndex					( lua_State* L );
	static int			_getPriority				( lua_State* L );
	static int			_getWorldBounds				( lua_State* L );
	static int			_getWorldBoundsCenter		( lua_State* L );
	static int			_inside						( lua_State* L );
	static int			_setBounds					( lua_State* L );
	static int			_setBoundsPad				( lua_State* L );
	static int			_setDeck					( lua_State* L );
	static int			_setExpandForSort			( lua_State* L );
	static int			_setFacet					( lua_State* L );
	static int			_setGrid					( lua_State* L );
	static int			_setGridScale				( lua_State* L );
	static int			_setIndex					( lua_State* L );
	static int			_setLayer					( lua_State* L );
	static int			_setPartition				( lua_State* L );
	static int			_setPriority				( lua_State* L );
	static int			_setRemapper				( lua_State* L );

protected:

	u32										mFlags;

	MOAILuaSharedPtr < MOAIDeck >			mDeck;
	MOAILuaSharedPtr < MOAIDeckRemapper >	mRemapper;
	u32										mIndex;
	
	MOAILuaSharedPtr < MOAIGrid >			mGrid;
	ZLVec2D									mGridScale;

	ZLBox									mBoundsOverride;
	ZLVec3D									mBoundsPad;

	MOAIFacet*								mFacets [ TOTAL_FACETS ];

	//----------------------------------------------------------------//
	virtual u32			OnGetModelBounds		( ZLBox& bounds ); // get the prop bounds in model space
	MOAIFacet*			ReplaceFacet			( MOAIFacet* oldFacet, MOAIFacet* newFacet );
	u32					ResolveModelBounds		( ZLBox& bounds );
	void				UpdateWorldBounds		( u32 status );
	void				UpdateWorldBounds		( const ZLBox& bounds, u32 status ); // update bounds in world space

public:

	DECL_ATTR_HELPER ( MOAIProp )

	static const s32 UNKNOWN_PRIORITY	= 0x80000000;
	static const int NO_SUBPRIM_ID		= 0xffffffff;
	
	enum {
		BOUNDS_EMPTY,
		BOUNDS_GLOBAL,
		BOUNDS_OK,
	};

	enum {
		CAN_DRAW					= 0x01,
		CAN_DRAW_DEBUG				= 0x02,
		CAN_GATHER_SURFACES			= 0x04,
		CAN_OVERLAP					= 0x08,
	};

	enum {
		ATTR_INDEX,
		ATTR_PARTITION,
		TOTAL_ATTR,
	};

	enum {
		FLAGS_OVERRIDE_BOUNDS		= 0x01,
		FLAGS_PAD_BOUNDS			= 0x02,
		FLAGS_EXPAND_FOR_SORT		= 0x04,
	};

	GET_SET ( u32, Index, mIndex )
	GET_SET ( u32, Mask, mMask )
	GET ( s32, Priority, mPriority )
	GET ( MOAIPartition*, Partition, mPartition )
	
	GET ( MOAIDeck*, Deck, mDeck )
	GET ( MOAIDeckRemapper*, Remapper, mRemapper )
	GET ( ZLBox, Bounds, mWorldBounds )
	GET ( ZLVec3D, BoundsMax, mWorldBounds.mMax )
	GET ( ZLVec3D, BoundsMin, mWorldBounds.mMin )

	//----------------------------------------------------------------//
	void							AddToSortBuffer			( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	bool							ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	virtual void					Draw					( int subPrimID, float lod );
	virtual void					DrawDebug				( int subPrimID, float lod );
	void							GatherSurfaces			( MOAISurfaceSampler2D& sampler );
	MOAICollisionFacet*				GetCollisionFacet		();
	MOAIFacet*						GetFacet				( u32 facetID );
	virtual MOAIGraphicsProp*		GetGraphicsProp			();
	void							GetGridBoundsInView		( MOAICellCoord& c0, MOAICellCoord& c1 ); // TODO: this shoudln't be here
	u32								GetModelBounds			( ZLBox& bounds );
	MOAIPartition*					GetPartitionTrait		();
	bool							GetCellRect				( ZLRect* cellRect, ZLRect* paddedRect = 0 );
	virtual bool					Inside					( ZLVec3D vec, float pad );
									MOAIProp				();
	virtual							~MOAIProp				();
	void							OnDepNodeUpdate			();
	void							RegisterLuaClass		( MOAILuaState& state );
	void							RegisterLuaFuncs		( MOAILuaState& state );
	void							SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void							SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void							SetFacet				( u32 facetID, MOAIFacet* facet );
	void							SetPartition			( MOAIPartition* partition );
};

#endif
