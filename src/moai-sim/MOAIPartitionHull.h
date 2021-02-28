// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONHULL_H
#define	MOAIPARTITIONHULL_H

#include <moai-sim/MOAIAbstractPickable.h>

class ZLGridCoord;
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
	public virtual MOAINode,
	public virtual MOAIAbstractPickable {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIPartition;
	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;

	MOAIPartition*				mPartition;
	MOAIPartitionCell*			mCell;
	
	// this is only for debug draw
	MOAIPartitionLevel*			mLevel;
	
	ZLLeanLink < MOAIPartitionHull* >	mLinkInCell;

	u32					mQueryMask;
	s32					mPriority;
	
	ZLBounds			mWorldBounds;

	//----------------------------------------------------------------//
	static int			_getPartition				( lua_State* L );
	static int			_getPriority				( lua_State* L );
	static int			_getWorldBounds				( lua_State* L );
	static int			_getWorldBoundsCenter		( lua_State* L );
	static int			_setPartition				( lua_State* L );
	static int			_setPriority				( lua_State* L );
	static int			_setQueryMask				( lua_State* L );

	//----------------------------------------------------------------//
	void				AddToSortBuffer				( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	u32					AffirmInterfaceMask			( MOAIPartition& partition );
	void				BoundsDidChange				();
	bool				PrepareForInsertion			( const MOAIPartition& partition );
	void				WasRemovedFromPartition		();

	//----------------------------------------------------------------//
	void				_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIPickResult		MOAIAbstractPickable_PickByPoint			( ZLVec3D loc );
	MOAIPickResult		MOAIAbstractPickable_PickByRay				( ZLVec3D loc, ZLVec3D normal );
	bool				MOAINode_ApplyAttrOp						( ZLAttrID attrID, ZLAttribute& attr, u32 op );

	//----------------------------------------------------------------//
	virtual void		MOAIPartitionHull_AddToSortBuffer			( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	virtual void		MOAIPartitionHull_BoundsDidChange			();
//	virtual bool		MOAIPartitionHull_Inside					( ZLVec3D vec, float pad );
	virtual bool		MOAIPartitionHull_PrepareForInsertion		( const MOAIPartition& partition );
	virtual void		MOAIPartitionHull_WasRemovedFromPartition	();

	//----------------------------------------------------------------//
	inline bool AcceptQuery ( const MOAIPartitionHull* ignore, ZLTypeID typeID, u32 queryMask ) {
		return (( this != ignore ) && ( this->mQueryMask & queryMask ) && this->IsType ( typeID ));
	}

public:

	DECL_ATTR_HELPER ( MOAIPartitionHull )

	static const s32 UNKNOWN_PRIORITY	= 0x80000000;
	static const int NO_SUBPRIM_ID		= 0xffffffff;
	
	enum {
		ATTR_PARTITION,
		ATTR_WORLD_BOUNDS_TRAIT,
		TOTAL_ATTR,
	};

	GET ( s32,					Priority,				mPriority )
	GET ( MOAIPartition*,		Partition,				mPartition )
	GET ( ZLBounds,				WorldBounds,			mWorldBounds )

	//----------------------------------------------------------------//
	MOAIPartition*		GetPartitionTrait		();
	bool				GetCellRect				( ZLRect* cellRect, ZLRect* paddedRect = 0 );
						MOAIPartitionHull		( ZLContext& context );
						~MOAIPartitionHull		();
	void				SetPartition			( MOAIPartition* partition );
	void				UpdateWorldBounds		( const ZLBounds& bounds );
};

#endif
