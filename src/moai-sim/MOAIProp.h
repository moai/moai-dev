// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROP_H
#define	MOAIPROP_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>
#include <moai-core/MOAILuaSharedPtr.h>

class MOAICellCoord;
class MOAICollisionProp;
class MOAIDeck;
class MOAIDeckRemapper;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAISurfaceSampler2D;

class MOAIProp;

//================================================================//
// MOAIProp
//================================================================//
/**	@lua	MOAIProp
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	@attr	ATTR_PARTITION
*/
class MOAIProp :
	public MOAITransform {
private:

	friend class MOAIPartition;
	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;

	MOAIPartition*				mPartition;
	MOAIPartitionCell*			mCell;
	
	// this is only for debug draw
	MOAIPartitionLevel*			mLevel;
	
	ZLLeanLink < MOAIProp* >	mLinkInCell;
	MOAIProp*					mNextResult;

	u32							mInterfaceMask;
	u32							mQueryMask;
	s32							mPriority;
	ZLBox						mWorldBounds;
	
	//----------------------------------------------------------------//
	static int			_getBounds					( lua_State* L );
	static int			_getDeck					( lua_State* L );
	static int			_getDims					( lua_State* L );
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
	static int			_setGrid					( lua_State* L );
	static int			_setGridScale				( lua_State* L );
	static int			_setIndex					( lua_State* L );
	static int			_setLayer					( lua_State* L );
	static int			_setPartition				( lua_State* L );
	static int			_setPriority				( lua_State* L );
	static int			_setQueryMask				( lua_State* L );
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

	//----------------------------------------------------------------//
	virtual u32			AffirmInterfaceMask			( MOAIPartition& partition ) = 0;
	virtual u32			OnGetModelBounds			( ZLBox& bounds ); // get the prop bounds in model space
	virtual void		OnBoundsChanged				();
	virtual void		OnRemoved					();
	virtual bool		PrepareForInsertion			( const MOAIPartition& partition );
	u32					ResolveModelBounds			( ZLBox& bounds );
	void				UpdateWorldBounds			( u32 status );
	void				UpdateWorldBounds			( const ZLBox& bounds, u32 status ); // update bounds in world space

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
	void							SetPartition			( MOAIPartition* partition );
};

#endif
