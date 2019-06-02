// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITION_H
#define	MOAIPARTITION_H

#include <moai-sim/MOAIPartitionCell.h>
#include <moai-sim/MOAIPartitionLevel.h>

//================================================================//
// MOAIPartition
//================================================================//
/**	@lua	MOAIPartition
	@text	Class for optimizing spatial queries against sets of primitives.
			Configure for performance; default behavior is a simple list.
	
	@const PLANE_XY
	@const PLANE_XZ
	@const PLANE_YZ
*/
class MOAIPartition :
	public virtual MOAILuaObject {
private:

	static const u32 MASK_ANY = 0xffffffff;

	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;
	friend class MOAIPartitionHull;

	ZLLeanArray < MOAIPartitionLevel >	mLevels;
	MOAIPartitionCell					mEmpties;
	MOAIPartitionCell					mGlobals;
	MOAIPartitionCell					mBiggies;

	s32					mPriorityCounter;
	static const s32	PRIORITY_MASK = 0x7fffffff;

	u32 mPlaneID; // One of ZLBox::PLANE_XY, ZLBox::PLANE_XZ, ZLBox::PLANE_YZ

	//----------------------------------------------------------------//
	static int		_clear					( lua_State* L );
	static int		_hullForPoint			( lua_State* L );
	static int		_hullForRay				( lua_State* L );
	static int		_hullList				( lua_State* L );
	static int		_hullListForPoint		( lua_State* L );
	static int		_hullListForRay			( lua_State* L );
	static int		_hullListForRect		( lua_State* L );
	static int		_reserveLevels			( lua_State* L );
	static int		_setLevel				( lua_State* L );
	static int		_setPlane				( lua_State* L );

	//----------------------------------------------------------------//
	void			AffirmPriority			( MOAIPartitionHull& hull );
	void			PrepareRebuild			();
	void			Rebuild					();
	void			UpdateHull				( MOAIPartitionHull& hull );

	//----------------------------------------------------------------//
	virtual void	MOAIPartition_DrawDebugBack		();
	virtual void	MOAIPartition_DrawDebugFront	();
	virtual void	MOAIPartition_OnInsertHull		( MOAIPartitionHull& hull );
	virtual void	MOAIPartition_OnRemoveHull		( MOAIPartitionHull& hull );
	virtual void	MOAIPartition_OnUpdateHull		( MOAIPartitionHull& hull );

public:
	
	DECL_LUA_FACTORY ( MOAIPartition )
	
	//----------------------------------------------------------------//
	void			Clear					();
	void			DrawDebugBack			();
	void			DrawDebugFront			();
	u32				GatherHulls				( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, ZLTypeID typeID, u32 mask = MASK_ANY );
	u32				GatherHulls				( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, const ZLVec3D& point, ZLTypeID typeID, u32 mask = MASK_ANY );
	u32				GatherHulls				( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, const ZLVec3D& point, const ZLVec3D& orientation, ZLTypeID typeID, u32 mask = MASK_ANY );
	u32				GatherHulls				( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, ZLRect rect, ZLTypeID typeID, u32 mask = MASK_ANY );
	u32				GatherHulls				( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, ZLBox box, ZLTypeID typeID, u32 mask = MASK_ANY );
	u32				GatherHulls				( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, const ZLFrustum& frustum, ZLTypeID typeID, u32 mask = MASK_ANY );
	void			InsertHull				( MOAIPartitionHull& hull );
	bool			IsEmpty					( MOAIPartitionHull& hull );
	bool			IsGlobal				( MOAIPartitionHull& hull );
					MOAIPartition			();
					~MOAIPartition			();
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			RemoveHull				( MOAIPartitionHull& hull );
	void			ReserveLevels			( ZLSize totalLevels );
	void			SetLevel				( ZLIndex levelID, float cellSize, int width, int height );
	void			SetPlane				( u32 planeID );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	u32 AffirmInterfaceMask () {
		return this->AffirmInterfaceMask ( ZLType::GetID < TYPE >());
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	u32 GetInterfaceMask () const {
		return this->GetInterfaceMask ( ZLType::GetID < TYPE >());
	}
};

#endif
