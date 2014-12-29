// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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

	static const u32 INTERFACE_MASK_BITS = 32;
	static const u32 MASK_ANY = 0xffffffff;

	friend class MOAIPartitionCell;
	friend class MOAIPartitionLevel;
	friend class MOAIProp;

	ZLLeanArray < MOAIPartitionLevel >	mLevels;
	MOAIPartitionCell					mEmpties;
	MOAIPartitionCell					mGlobals;
	MOAIPartitionCell					mBiggies;

	ZLLeanArray < u32 >					mInterfaceIDs; // array if ZLTypeIDs for supported interfaces

	s32					mPriorityCounter;
	static const s32	PRIORITY_MASK = 0x7fffffff;

	u32 mPlaneID; // One of ZLBox::PLANE_XY, ZLBox::PLANE_XZ, ZLBox::PLANE_YZ

	//----------------------------------------------------------------//
	static int		_clear					( lua_State* L );
	static int		_getInterfaceMask		( lua_State* L );
	static int		_insertProp				( lua_State* L );
	static int		_propForPoint			( lua_State* L );
	static int		_propForRay				( lua_State* L );
	static int		_propList				( lua_State* L );
	static int		_propListForPoint		( lua_State* L );
	static int		_propListForRay			( lua_State* L );
	static int		_propListForRect		( lua_State* L );
	static int		_removeProp				( lua_State* L );
	static int		_reserveLevels			( lua_State* L );
	static int		_setLevel				( lua_State* L );
	static int		_setPlane				( lua_State* L );

	//----------------------------------------------------------------//
	u32				AffirmInterfaceMask		( u32 typeID );
	void			AffirmPriority			( MOAIProp& prop );
	u32				GetInterfaceMask		( u32 typeID ) const;
	virtual void	OnPropInserted			( MOAIProp& prop );	
	virtual void	OnPropRemoved			( MOAIProp& prop );	
	virtual void	OnPropUpdated			( MOAIProp& prop );
	void			PrepareRebuild			();
	void			Rebuild					();
	void			UpdateProp				( MOAIProp& prop, u32 status );

public:
	
	DECL_LUA_FACTORY ( MOAIPartition )
	
	//----------------------------------------------------------------//
	void			Clear					();
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask = MASK_ANY, u32 mask = MASK_ANY );
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 interfaceMask = MASK_ANY, u32 mask = MASK_ANY );
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLVec3D& point, u32 interfaceMask = MASK_ANY, u32 mask = MASK_ANY );
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, ZLBox box, u32 interfaceMask = MASK_ANY, u32 mask = MASK_ANY );
	u32				GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLFrustum& frustum, u32 interfaceMask = MASK_ANY, u32 mask = MASK_ANY );
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
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	u32 AffirmInterfaceMask () {
		return this->AffirmInterfaceMask ( ZLTypeID < TYPE >::GetID ());
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	u32 GetInterfaceMask () const {
		return this->GetInterfaceMask ( ZLTypeID < TYPE >::GetID ());
	}
};

#endif
