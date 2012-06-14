// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITION_H
#define	MOAIPARTITION_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLevel.h>

//================================================================//
// MOAIPartition
//================================================================//
/**	@name	MOAIPartition
	@text	Class for optimizing spatial queries against sets of primitives.
			Configure for performance; default behavior is a simple list.
	
	@const PLANE_XY
	@const PLANE_XZ
	@const PLANE_YZ
*/
class MOAIPartition :
	public virtual MOAILuaObject {
private:

	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;
	friend class MOAIProp;

	USLeanArray < MOAIPartitionLevel >	mLevels;
	MOAIPartitionCell					mEmpties;
	MOAIPartitionCell					mGlobals;
	MOAIPartitionCell					mBiggies;

	s32					mPriorityCounter;
	static const s32	PRIORITY_MASK = 0x7fffffff;

	u32 mPlaneID; // One of USBox::PLANE_XY, USBox::PLANE_XZ, USBox::PLANE_YZ

	//----------------------------------------------------------------//
	static int		_clear					( lua_State* L );
	static int		_insertProp				( lua_State* L );
	static int		_propForPoint			( lua_State* L );
	static int		_propForRay				( lua_State* L );
	static int		_propListForPoint		( lua_State* L );
	static int		_propListForRay			( lua_State* L );
	static int		_propListForRect		( lua_State* L );
	static int		_removeProp				( lua_State* L );
	static int		_reserveLevels			( lua_State* L );
	static int		_setLevel				( lua_State* L );
	static int		_setPlane				( lua_State* L );

	//----------------------------------------------------------------//
	void			AffirmPriority			( MOAIProp& prop );
	void			PrepareRebuild			();
	void			Rebuild					();
	void			UpdateProp				( MOAIProp& prop, u32 status );

public:
	
	DECL_LUA_FACTORY ( MOAIPartition )
	
	//----------------------------------------------------------------//
	void			Clear					();
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec3D& point, const USVec3D& orientation, u32 mask = 0xffffffff );
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 mask = 0xffffffff );
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec3D& point, u32 mask = 0xffffffff );
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, USBox box, u32 mask = 0xffffffff );
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USFrustum& frustum, u32 mask = 0xffffffff );
	void			InsertProp				( MOAIProp& prop );
	bool			IsEmpty					( MOAIProp& prop );
	bool			IsGlobal				( MOAIProp& prop );
					MOAIPartition			();
					~MOAIPartition			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			RemoveProp				( MOAIProp& prop );
	void			ReserveLevels			( int totalLevels );
	void			SetLevel				( int levelID, float cellSize, int width, int height );
	void			SetPlane				( u32 planeID );
};

#endif
