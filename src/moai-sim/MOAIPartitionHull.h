// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONHULL_H
#define	MOAIPARTITIONHULL_H

#include <moai-sim/MOAITransform.h>

class MOAICellCoord;
class MOAICollisionProp;
class MOAIDeck;
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
	public virtual MOAINode {
protected:

	friend class MOAIPartition;
	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;

	MOAIPartition*				mPartition;
	MOAIPartitionCell*			mCell;
	
	// this is only for debug draw
	MOAIPartitionLevel*			mLevel;
	
	ZLLeanLink < MOAIPartitionHull* >	mLinkInCell;
	MOAIPartitionHull*					mNextResult;

	u32					mQueryMask;
	s32					mPriority;
	
	ZLBounds			mWorldBounds;
	ZLPrism				mWorldPrism;

	//----------------------------------------------------------------//
	static int			_getPartition				( lua_State* L );
	static int			_getPriority				( lua_State* L );
	static int			_getWorldBounds				( lua_State* L );
	static int			_getWorldBoundsCenter		( lua_State* L );
	static int			_inside						( lua_State* L );
	static int			_setHitGranularity			( lua_State* L );
	static int			_setPartition				( lua_State* L );
	static int			_setPriority				( lua_State* L );
	static int			_setQueryMask				( lua_State* L );

	//----------------------------------------------------------------//
	virtual void		MOAIPartitionHull_AddToSortBuffer			( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	virtual void		MOAIPartitionHull_BoundsDidChange			();
	virtual bool		MOAIPartitionHull_Inside					( ZLVec3D vec, float pad );
	virtual bool		MOAIPartitionHull_PrepareForInsertion		( const MOAIPartition& partition );
	virtual void		MOAIPartitionHull_WasRemovedFromPartition	();

protected:

	u32					mHitGranularity;

	//----------------------------------------------------------------//
	void				AddToSortBuffer				( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	u32					AffirmInterfaceMask			( MOAIPartition& partition );
	void				BoundsDidChange				();
	bool				PrepareForInsertion			( const MOAIPartition& partition );
	void				WasRemovedFromPartition		();
	
	//----------------------------------------------------------------//
	bool				MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	
	//----------------------------------------------------------------//
	inline bool AcceptQuery ( const MOAIPartitionHull* ignore, ZLTypeID typeID, u32 queryMask ) {
		return (( this != ignore ) && ( this->mQueryMask & queryMask ) && this->IsType ( typeID ));
	}
	
public:

	DECL_ATTR_HELPER ( MOAIPartitionHull )

	static const s32 UNKNOWN_PRIORITY	= 0x80000000;
	static const int NO_SUBPRIM_ID		= 0xffffffff;
	
	// these are implementation dependent; use them as hints
	enum {
		HIT_TEST_COARSE,	// object bounds in world space
		HIT_TEST_MEDIUM,	// individual geometry elements in model space
		HIT_TEST_FINE,		// pixel-level granularity
	};

	enum {
		ATTR_PARTITION,
		ATTR_WORLD_BOUNDS_TRAIT,
		TOTAL_ATTR,
	};

	GET ( s32,				Priority,				mPriority )
	GET ( MOAIPartition*,	Partition,				mPartition )
	GET ( ZLBounds,			WorldBounds,			mWorldBounds )

	//----------------------------------------------------------------//
	MOAIPartition*		GetPartitionTrait		();
	bool				GetCellRect				( ZLRect* cellRect, ZLRect* paddedRect = 0 );
	bool				Inside					( ZLVec3D vec, float pad );
						MOAIPartitionHull		();
						~MOAIPartitionHull		();
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void				SetPartition			( MOAIPartition* partition );
	void				UpdateWorldBounds		( const ZLBounds& bounds );
	void				UpdateWorldBounds		( const ZLPrism& prism );
};

#endif
