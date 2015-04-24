// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionCell.h>
#include <moai-sim/MOAIPartitionLevel.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIProp.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	clear
	@text	Remove all props from the partition.
	
	@in		MOAIPartition self
	@out	nil
*/
int MOAIPartition::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "U" )

	self->Clear ();
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartition::_getInterfaceMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "U" )

	u32 interfaceMask = MASK_ANY;
	u32 typeID = state.GetValue < u32 >( 2, 0 );

	if ( typeID ) {
		interfaceMask = self->GetInterfaceMask ( typeID );
	}
	state.Push ( typeID );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	insertProp
	@text	Inserts a prop into the partition. A prop can only be
			in one partition at a time.
	
	@in		MOAIPartition self
	@in		MOAIProp prop
	@out	nil
*/
int MOAIPartition::_insertProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UU" )

	MOAIProp* prop = state.GetLuaObject < MOAIProp >( 2, true );
	if ( !prop ) return 0;

	self->InsertProp ( *prop );
	prop->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	propForPoint
	@text	Returns the prop with the highest priority that contains
			the given world space point.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@opt	number interfaceMask
	@opt	number queryMask
	@opt	number sortMode			One of the MOAILayer sort modes. Default value is SORT_PRIORITY_ASCENDING.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@out	MOAIProp prop			The prop under the point or nil if no prop found.
*/
int MOAIPartition::_propForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	ZLVec3D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	vec.mZ = state.GetValue < float >( 4, 0.0f );

	u32 interfaceMask	= state.GetValue < u32 >( 5, MASK_ANY );
	u32 queryMask		= state.GetValue < u32 >( 6, MASK_ANY );

	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
	
	u32 total = self->GatherProps ( buffer, 0, vec, interfaceMask, queryMask );
	if ( total ) {
		
		buffer.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
		
		u32 sortMode = state.GetValue < u32 >( 7, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
		float xScale = state.GetValue < float >( 8, 0.0f );
		float yScale = state.GetValue < float >( 9, 0.0f );
		float zScale = state.GetValue < float >( 10, 0.0f );
		float priorityScale = state.GetValue < float >( 11, 1.0f );
		
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		
		MOAIProp* prop = buffer.FindBest ();
		if ( prop ) {
			prop->PushLuaUserdata ( state );
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	propForRay
	@text	Returns the prop closest to the camera that intersects the given ray
	 
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@in		number xdirection
	@in		number ydirection
	@in		number zdirection
	@opt	number interfaceMask
	@opt	number queryMask
	@out	MOAIProp prop		The prop under the point in order of depth or nil if no prop found.
*/
int MOAIPartition::_propForRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )
	
	ZLVec3D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	vec.mZ = state.GetValue < float >( 4, 0.0f );
	
	ZLVec3D direction;
	direction.mX = state.GetValue < float >( 5, 0.0f );
	direction.mY = state.GetValue < float >( 6, 0.0f );
	direction.mZ = state.GetValue < float >( 7, 0.0f );
	direction.Norm ();
	
	u32 interfaceMask	= state.GetValue < u32 >( 8, MASK_ANY );
	u32 queryMask		= state.GetValue < u32 >( 9, MASK_ANY );
	
	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
	
	u32 total = self->GatherProps ( buffer, 0, vec, direction, interfaceMask, queryMask );

	if ( total ) {
		
		MOAIProp* prop = buffer.FindBest ();
		if ( prop ) {
			prop->PushLuaUserdata ( state );
			return 1;
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	propList
	@text	Returns all props.
	
	@in		MOAIPartition self
	@opt	number sortMode			One of the MOAILayer sort modes. Default value is SORT_NONE.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number interfaceMask
	@opt	number queryMask
	@out	... props				The props pushed onto the stack.
*/
int MOAIPartition::_propList ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "U" )

	u32 sortMode = state.GetValue < u32 >( 2, MOAIPartitionResultBuffer::SORT_NONE );
	float xScale = state.GetValue < float >( 3, 0.0f );
	float yScale = state.GetValue < float >( 4, 0.0f );
	float zScale = state.GetValue < float >( 5, 0.0f );
	float priorityScale = state.GetValue < float >( 6, 1.0f );		
	u32 interfaceMask	= state.GetValue < u32 >( 7, MASK_ANY );
	u32 queryMask		= state.GetValue < u32 >( 8, MASK_ANY );

	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();

	u32 total = self->GatherProps ( buffer, 0, interfaceMask, queryMask );
	if ( total ) {
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushProps ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	propListForPoint
	@text	Returns all props under a given world space point.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@opt	number sortMode			One of the MOAILayer sort modes. Default value is SORT_NONE.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number interfaceMask
	@opt	number queryMask
	@out	... props				The props under the point, all pushed onto the stack.
*/
int MOAIPartition::_propListForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	ZLVec3D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	vec.mZ = state.GetValue < float >( 4, 0.0f );

	u32 sortMode = state.GetValue < u32 >( 5, MOAIPartitionResultBuffer::SORT_NONE );
	float xScale = state.GetValue < float >( 6, 0.0f );
	float yScale = state.GetValue < float >( 7, 0.0f );
	float zScale = state.GetValue < float >( 8, 0.0f );
	float priorityScale = state.GetValue < float >( 9, 1.0f );		
	u32 interfaceMask	= state.GetValue < u32 >( 10, MASK_ANY );
	u32 queryMask		= state.GetValue < u32 >( 11, MASK_ANY );

	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();

	u32 total = self->GatherProps ( buffer, 0, vec, interfaceMask, queryMask );
	if ( total ) {
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushProps ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	propListForRay
	@text	Returns all props that intersect the given ray.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@in		number xdirection
	@in		number ydirection
	@in		number zdirection
	@opt	number sortMode			One of the MOAILayer sort modes. Default value is SORT_KEY_ASCENDING.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number interfaceMask
	@opt	number queryMask
	@out	... props				The props under the point in order of depth, all pushed onto the stack.
*/
int MOAIPartition::_propListForRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	ZLVec3D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	vec.mZ = state.GetValue < float >( 4, 0.0f );
	
	ZLVec3D direction;
	direction.mX = state.GetValue < float >( 5, 0.0f );
	direction.mY = state.GetValue < float >( 6, 0.0f );
	direction.mZ = state.GetValue < float >( 7, 0.0f );
	direction.Norm ();
	
	u32 sortMode = state.GetValue < u32 >( 8, MOAIPartitionResultBuffer::SORT_KEY_ASCENDING );
	float xScale = state.GetValue < float >( 9, 0.0f );
	float yScale = state.GetValue < float >( 10, 0.0f );
	float zScale = state.GetValue < float >( 11, 0.0f );
	float priorityScale = state.GetValue < float >( 12, 1.0f );		
	u32 interfaceMask	= state.GetValue < u32 >( 13, MASK_ANY );
	u32 queryMask		= state.GetValue < u32 >( 14, MASK_ANY );
	
	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
	
	u32 total = self->GatherProps ( buffer, 0, vec, direction, interfaceMask, queryMask );

	if ( total ) {
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushProps ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	propListForRect
	@text	Returns all props under a given world space rect.
	
	@in		MOAIPartition self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number sortMode			One of the MOAILayer sort modes. Default value is SORT_NONE.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number interfaceMask
	@opt	number queryMask
	@out	... props				The props under the rect, all pushed onto the stack.
*/
int MOAIPartition::_propListForRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNNNN" )
	
	ZLBox box;
	
	box.mMin.mX = state.GetValue < float >( 2, 0.0f );
	box.mMin.mY = state.GetValue < float >( 3, 0.0f );
	box.mMin.mZ = 0.0f;
	
	box.mMax.mX = state.GetValue < float >( 4, 0.0f );
	box.mMax.mY = state.GetValue < float >( 5, 0.0f );
	box.mMax.mZ = 0.0f;
	
	u32 sortMode = state.GetValue < u32 >( 6, MOAIPartitionResultBuffer::SORT_NONE );
	float xScale = state.GetValue < float >( 7, 0.0f );
	float yScale = state.GetValue < float >( 8, 0.0f );
	float zScale = state.GetValue < float >( 9, 0.0f );
	float priorityScale = state.GetValue < float >( 10, 1.0f );	
	u32 interfaceMask	= state.GetValue < u32 >( 11, MASK_ANY );
	u32 queryMask		= state.GetValue < u32 >( 12, MASK_ANY );
	
	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
	
	u32 total = self->GatherProps ( buffer, 0, box, interfaceMask, queryMask );
	if ( total ) {
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushProps ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	removeProp
	@text	Removes a prop from the partition.
	
	@in		MOAIPartition self
	@in		MOAIProp prop
	@out	nil
*/
int MOAIPartition::_removeProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UU" )

	MOAIProp* prop = state.GetLuaObject < MOAIProp >( 2, true );
	if ( !prop ) return 0;

	self->RemoveProp ( *prop );
	prop->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveLevels
	@text	Reserves a stack of levels in the partition. Levels must be
			initialized with setLevel (). This will trigger a full rebuild
			of the partition if it contains any props.
	
	@in		MOAIPartition self
	@in		number nLevels
	@out	nil
*/
int MOAIPartition::_reserveLevels ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UN" )

	u32 totalLevels = state.GetValue < u32 >( 2, 0 );
	
	self->ReserveLevels ( totalLevels );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setLevel
	@text	Initializes a level previously created by reserveLevels ().
			This will trigger a full rebuild of the partition if it contains any props.
			Each level is a loose grid. Props of a given size may be placed by
			the system into any level with cells large enough to accommodate them.
			The dimensions of a level control how many cells the level contains.
			If an object goes off of the edge of a level, it will wrap around
			to the other side. It is possible to model a quad tree by initializing
			levels correctly, but for some simulations better structures
			may be possible.
	
	@in		MOAIPartition self
	@in		number levelID
	@in		number cellSize		Dimensions of the level's cells.
	@in		number xCells		Width of level in cells.
	@in		number yCells		Height of level in cells.
	@out	nil
*/
int MOAIPartition::_setLevel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNNNN" )

	u32 levelID		= state.GetValue < u32 >( 2, 1 ) - 1;
	float cellSize	= state.GetValue < float >( 3, 1.0f );
	u32 width		= state.GetValue < u32 >( 4, 0 );
	u32 height		= state.GetValue < u32 >( 5, 0 );

	self->SetLevel ( levelID, cellSize, width, height );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPlane
	@text	Selects the plane the partition will use. If this is different
			from the current plane then all non-global props will be redistributed.
			Redistribution works by moving all props to the 'empties' cell and then
			scheduling them all for a dep node update (which refreshes the prop's
			bounds and may also flag it as global).
	
	@in		MOAIPartition self
	@in		number planeID		One of MOAIPartition::PLANE_XY, MOAIPartition::PLANE_XZ, MOAIPartition::PLANE_YZ. Default value is MOAIPartition::PLANE_XY.
	@out	nil
*/
int MOAIPartition::_setPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "U" )

	u32 planeID	= state.GetValue < u32 >( 2, ZLBox::PLANE_XY );
	self->SetPlane ( planeID );

	return 0;
}

//================================================================//
// MOAIPartition
//================================================================//

//----------------------------------------------------------------//
u32 MOAIPartition::AffirmInterfaceMask ( u32 typeID ) {

	u32 mask = this->GetInterfaceMask ( typeID );
	if ( mask == 0 ) {
		u32 top = this->mInterfaceIDs.Size ();
		if ( top < INTERFACE_MASK_BITS ) {
			this->mInterfaceIDs.Grow ( top + 1 );
			this->mInterfaceIDs [ top ] = typeID;
			return 1 << top;
		}
	}
	return mask;
}

//----------------------------------------------------------------//
void MOAIPartition::AffirmPriority ( MOAIProp& prop ) {

	if ( prop.mPriority == MOAIProp::UNKNOWN_PRIORITY ) {
		prop.mPriority = this->mPriorityCounter++;
		this->mPriorityCounter = this->mPriorityCounter & PRIORITY_MASK;
	}
}

//----------------------------------------------------------------//
void MOAIPartition::Clear () {

	u32 totalLevels = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].Clear ();
	}
	this->mBiggies.Clear ();
	this->mGlobals.Clear ();
	this->mEmpties.Clear ();
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 interfaceMask, u32 queryMask ) {
	
	results.Reset ();
	
	u32 totalLevels = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, interfaceMask, queryMask );
	}
	this->mBiggies.GatherProps ( results, ignore, interfaceMask, queryMask );
	this->mGlobals.GatherProps ( results, ignore, interfaceMask, queryMask );
	this->mEmpties.GatherProps ( results, ignore, interfaceMask, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask, u32 queryMask ) {
	
	results.Reset ();
	
	u32 totalLevels = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, point, orientation, interfaceMask, queryMask );
	}
	this->mBiggies.GatherProps ( results, ignore, point, orientation, interfaceMask, queryMask );
	this->mGlobals.GatherProps ( results, ignore, point, orientation, interfaceMask, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLVec3D& point, u32 interfaceMask, u32 queryMask ) {
	
	results.Reset ();
	
	u32 totalLevels = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, point, this->mPlaneID, interfaceMask, queryMask );
	}
	this->mBiggies.GatherProps ( results, ignore, point, interfaceMask, queryMask );
	this->mGlobals.GatherProps ( results, ignore, interfaceMask, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, ZLBox box, u32 interfaceMask, u32 queryMask ) {
	
	results.Reset ();
	box.Bless ();
	
	u32 totalLevels = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, box, this->mPlaneID, interfaceMask, queryMask );
	}
	this->mBiggies.GatherProps ( results, ignore, box, interfaceMask, queryMask );
	this->mGlobals.GatherProps ( results, ignore, interfaceMask, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLFrustum& frustum, u32 interfaceMask, u32 queryMask ) {
	
	results.Reset ();
	
	u32 totalLevels = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, frustum, this->mPlaneID, interfaceMask, queryMask );
	}
	this->mBiggies.GatherProps ( results, ignore, frustum, interfaceMask, queryMask );
	this->mGlobals.GatherProps ( results, ignore, interfaceMask, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GetInterfaceMask ( u32 typeID ) const {

	u32 total = this->mInterfaceIDs.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		if ( this->mInterfaceIDs [ i ] == typeID ) {
			return 1 << i;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIPartition::InsertProp ( MOAIProp& prop ) {
	
	if ( prop.mPartition == this ) return;
	
	this->LuaRetain ( &prop );
	
	if ( prop.mPartition ) {
		prop.mPartition->RemoveProp ( prop );
	}
	
	u32 interfaceMask = prop.AffirmInterfaceMask ( *this );
	if ( interfaceMask ) {
	
		if ( prop.mPriority == MOAIProp::UNKNOWN_PRIORITY ) {
			prop.mPriority = this->mPriorityCounter++;
			this->mPriorityCounter = this->mPriorityCounter & PRIORITY_MASK;
		}
		
		this->mEmpties.InsertProp ( prop );
		this->AffirmPriority ( prop );
		
		prop.mPartition = this;
		prop.mInterfaceMask = interfaceMask;
		prop.ScheduleUpdate ();
		
		this->OnPropInserted ( prop );
	}
}

//----------------------------------------------------------------//
bool MOAIPartition::IsEmpty ( MOAIProp& prop ) {
	return prop.mCell == &this->mEmpties;
}

//----------------------------------------------------------------//
bool MOAIPartition::IsGlobal ( MOAIProp& prop ) {
	return prop.mCell == &this->mGlobals;
}

//----------------------------------------------------------------//
MOAIPartition::MOAIPartition () :
	mPriorityCounter ( 0 ),
	mPlaneID ( ZLBox::PLANE_XY ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPartition::~MOAIPartition () {
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIPartition::OnPropInserted ( MOAIProp& prop ) {
	UNUSED ( prop );
}

//----------------------------------------------------------------//	
void MOAIPartition::OnPropRemoved ( MOAIProp& prop ) {
	UNUSED ( prop );
}

//----------------------------------------------------------------//
void MOAIPartition::OnPropUpdated ( MOAIProp& prop ) {
	UNUSED ( prop );
}

//----------------------------------------------------------------//
// This moves all props to the 'empties' cell
void MOAIPartition::PrepareRebuild () {

	u32 totalLevels = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].ExtractProps ( this->mEmpties, 0 );
	}
	this->mBiggies.ExtractProps ( this->mEmpties, 0 );
	this->mGlobals.ExtractProps ( this->mEmpties, 0 );
}

//----------------------------------------------------------------//
// This schedules all props in the 'empties' cell for an update
void MOAIPartition::Rebuild () {

	this->mEmpties.ScheduleProps ();
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "PLANE_XY",	( u32 )ZLBox::PLANE_XY );
	state.SetField ( -1, "PLANE_XZ",	( u32 )ZLBox::PLANE_XZ );
	state.SetField ( -1, "PLANE_YZ",	( u32 )ZLBox::PLANE_YZ );
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "clear",						_clear },
		{ "getInterfaceMask",			_getInterfaceMask },
		{ "insertProp",					_insertProp },
		{ "propForPoint",				_propForPoint },
		{ "propForRay",					_propForRay },
		{ "propList",					_propList },
		{ "propListForPoint",			_propListForPoint },
		{ "propListForRay",				_propListForRay },
		{ "propListForRect",			_propListForRect },
		{ "removeProp",					_removeProp },
		{ "reserveLevels",				_reserveLevels },
		{ "setLevel",					_setLevel },
		{ "setPlane",					_setPlane },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPartition::RemoveProp ( MOAIProp& prop ) {

	if ( prop.mPartition != this ) return;
	
	if ( prop.mCell ) {
		prop.mCell->RemoveProp ( prop );
	}

	prop.mPartition = 0;
	prop.mInterfaceMask = 0;
	this->OnPropRemoved ( prop );

	this->LuaRelease ( &prop );
}

//----------------------------------------------------------------//
void MOAIPartition::ReserveLevels ( int totalLevels ) {

	this->PrepareRebuild ();
	this->mLevels.Init ( totalLevels );
	this->Rebuild ();
}

//----------------------------------------------------------------//
void MOAIPartition::SetLevel ( int levelID, float cellSize, int width, int height ) {

	this->PrepareRebuild ();
	this->mLevels [ levelID ].Init ( cellSize, width, height );
	this->Rebuild ();
}

//----------------------------------------------------------------//
void MOAIPartition::UpdateProp ( MOAIProp& prop, u32 status ) {

	// clear out the level; level will be re-calculated below
	// also: prop.mLevel is *only* for debug drawing 
	prop.mLevel = 0;

	// status is not 'OK' so prop is either global or empty
	if ( status != MOAIProp::BOUNDS_OK ) {
		
		if ( status == MOAIProp::BOUNDS_GLOBAL ) {
			this->mGlobals.InsertProp ( prop );
		}
		else {
			this->mEmpties.InsertProp ( prop );
		}
		return;
	}

	ZLRect rect = prop.mWorldBounds.GetRect ( this->mPlaneID );

	float width = rect.Width ();
	float height = rect.Height ();
	float cellSize = ( width > height ) ? width : height;
	
	if ( cellSize > 0.0f ) {
		
		MOAIPartitionLevel* level = 0;
		
		u32 totalLevels = this->mLevels.Size ();
		for ( u32 i = 0; i < totalLevels; ++i ) {
			
			MOAIPartitionLevel* testLevel = &this->mLevels [ i ];
			
			if ( cellSize <= testLevel->mCellSize ) {
				if (( level == 0 ) || ( testLevel->mCellSize  < level->mCellSize )) {
					level = testLevel;
				}
			}
		}
		
		if ( level ) {
			// level prop
			level->PlaceProp ( prop );
			prop.mLevel = level;
		}
		else {
			// biggie prop - has dimension but too big to fit in any level
			this->mBiggies.InsertProp ( prop );
		}
	}
	else {
		// empty prop
		this->mEmpties.InsertProp ( prop );
	}
}

//----------------------------------------------------------------//
void MOAIPartition::SetPlane ( u32 planeID ) {

	if ( this->mPlaneID == planeID ) return;
	this->PrepareRebuild ();
	this->mPlaneID = planeID;
	this->Rebuild ();
}
