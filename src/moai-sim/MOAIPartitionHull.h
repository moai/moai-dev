// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONHULL_H
#define	MOAIPARTITIONHULL_H

#include <moai-sim/MOAITransform.h>

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

class MOAIPartitionHull;

//================================================================//
// MOAIPartitionHull
//================================================================//
/**	@lua	MOAIPartitionHull
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	@attr	ATTR_PARTITION
*/
class MOAIPartitionHull :
	public virtual MOAITransform {
private:

	friend class MOAIPartition;
	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;

	MOAIPartition*				mPartition;
	MOAIPartitionCell*			mCell;
	
	// this is only for debug draw
	MOAIPartitionLevel*			mLevel;
	
	ZLLeanLink < MOAIPartitionHull* >	mLinkInCell;
	MOAIPartitionHull*					mNextResult;

	u32							mInterfaceMask;
	u32							mQueryMask;
	s32							mPriority;
	
	ZLBox						mWorldBounds;
	u32							mBoundsStatus;
	
	//----------------------------------------------------------------//
	static int			_getBounds					( lua_State* L );
	static int			_getDeck					( lua_State* L );
	static int			_getDims					( lua_State* L );
	static int			_getGrid					( lua_State* L );
	static int			_getIndex					( lua_State* L );
	static int			_getPartition				( lua_State* L );
	static int			_getPriority				( lua_State* L );
	static int			_getWorldBounds				( lua_State* L );
	static int			_getWorldBoundsCenter		( lua_State* L );
	static int			_inside						( lua_State* L );
	static int			_setBounds					( lua_State* L );
	static int			_setBoundsPad				( lua_State* L );
	static int			_setDeck					( lua_State* L );
	static int			_setExpandForSort			( lua_State* L );
	static int			_setFlag					( lua_State* L ); // TODO: macro
	static int			_setGrid					( lua_State* L );
	static int			_setGridScale				( lua_State* L );
	static int			_setHitGranularity			( lua_State* L );
	static int			_setIndex					( lua_State* L );
	static int			_setLayer					( lua_State* L );
	static int			_setPartition				( lua_State* L );
	static int			_setPriority				( lua_State* L );
	static int			_setQueryMask				( lua_State* L );
	static int			_setRemapper				( lua_State* L );

	//----------------------------------------------------------------//
	virtual void		MOAIPartitionHull_AddToSortBuffer			( MOAIPartitionResultBuffer& buffer, u32 key = 0 ) = 0;
	virtual u32			MOAIPartitionHull_AffirmInterfaceMask		( MOAIPartition& partition ) = 0;
	virtual void		MOAIPartitionHull_BoundsDidChange			();
	virtual u32			MOAIPartitionHull_GetModelBounds			( ZLBox& bounds ) = 0; // get the prop bounds in model space
	virtual bool		MOAIPartitionHull_Inside					( ZLVec3D vec, float pad );
	virtual bool		MOAIPartitionHull_PrepareForInsertion		( const MOAIPartition& partition );
	virtual void		MOAIPartitionHull_WasRemovedFromPartition	();

protected:

	u32										mFlags;

	ZLBox									mBoundsOverride;
	ZLVec3D									mBoundsPad;

	u32										mHitGranularity;

	//----------------------------------------------------------------//
	void				AddToSortBuffer				( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	u32					AffirmInterfaceMask			( MOAIPartition& partition );
	void				BoundsDidChange				();
	void				OnDepNodeUpdate				();
	bool				PrepareForInsertion			( const MOAIPartition& partition );
	u32					ResolveModelBounds			( ZLBox& bounds );
	void				UpdateWorldBounds			( u32 status );
	void				UpdateWorldBounds			( const ZLBox& bounds, u32 status ); // update bounds in world space
	void				WasRemovedFromPartition		();
	
public:

	DECL_ATTR_HELPER ( MOAIPartitionHull )

	static const s32 UNKNOWN_PRIORITY	= 0x80000000;
	static const int NO_SUBPRIM_ID		= 0xffffffff;
	
	enum {
		HIT_TEST_COARSE,	// no hit test will be performed; only the prop's bounds will be used
		HIT_TEST_MEDIUM,	// implementation dependent
		HIT_TEST_FINE,		// implementation dependent
	};
	
	enum {
		BOUNDS_EMPTY,
		BOUNDS_GLOBAL,
		BOUNDS_OK,
	};

	enum {
		//ATTR_INDEX,
		ATTR_PARTITION,
		TOTAL_ATTR,
	};

	enum {
		FLAGS_OVERRIDE_BOUNDS		= 0x01,
		FLAGS_PAD_BOUNDS			= 0x02,
		FLAGS_EXPAND_FOR_SORT		= 0x04,
		FLAGS_PARTITION_GLOBAL		= 0x08,
		TOTAL_FLAGS,
	};

	GET ( s32,				Priority,		mPriority )
	GET ( MOAIPartition*,	Partition,		mPartition )
	
	GET ( u32,			BoundsStatus,		mBoundsStatus )
	GET ( ZLBox,		Bounds,				mWorldBounds )
	GET ( ZLVec3D,		BoundsMax,			mWorldBounds.mMax )
	GET ( ZLVec3D,		BoundsMin,			mWorldBounds.mMin )

	//----------------------------------------------------------------//
	bool				ApplyAttrOp				( u32 attrID, MOAIAttribute& attr, u32 op );
	u32					GetModelBounds			( ZLBox& bounds ); // get the prop bounds in model space
	MOAIPartition*		GetPartitionTrait		();
	bool				GetCellRect				( ZLRect* cellRect, ZLRect* paddedRect = 0 );
	bool				Inside					( ZLVec3D vec, float pad );
	bool				InsideModelBounds		( const ZLVec3D& vec, float pad );
						MOAIPartitionHull		();
						~MOAIPartitionHull		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void				SetPartition			( MOAIPartition* partition );
};

#endif
