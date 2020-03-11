// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionCell.h>
#include <moai-sim/MOAIPartitionLevel.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIPartitionHull.h>

//================================================================//
// lua
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
/**	@lua	hullForPoint
	@text	Returns the hull with the highest priority that contains
			the given world space point.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@opt	number sortMode			One of the MOAIPartitionViewLayer sort modes. Default value is SORT_PRIORITY_ASCENDING.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number typeID
	@opt	number queryMask
	@out	MOAIPartitionHull hull			The hull under the point or nil if no hull found.
*/
int MOAIPartition::_hullForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	ZLVec3D vec;
	vec.mX					= state.GetValue < float >( 2, 0.0f );
	vec.mY					= state.GetValue < float >( 3, 0.0f );
	vec.mZ					= state.GetValue < float >( 4, 0.0f );

	u32 sortMode			= state.GetValue < u32 >( 5, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
	float xScale			= state.GetValue < float >( 6, 0.0f );
	float yScale			= state.GetValue < float >( 7, 0.0f );
	float zScale			= state.GetValue < float >( 8, 0.0f );
	float priorityScale		= state.GetValue < float >( 9, 1.0f );

	ZLTypeID typeID			= state.GetValue < ZLTypeID >( 10, ZLType::GetID < MOAIPartitionHull >());
	u32 queryMask			= state.GetValue < u32 >( 11, MASK_ANY );

	MOAIScopedPartitionResultBufferHandle scopedBufferHandle = MOAIPartitionResultMgr::Get ().GetBufferHandle ();
	MOAIPartitionResultBuffer& buffer = scopedBufferHandle;
	
	u32 total = self->GatherHulls ( buffer, 0, vec, typeID, queryMask );
	if ( total ) {
		
		// this just swaps the internal buffer pointers so we can access the results
		buffer.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
		
		// generate the sort keys, but don't actually sort
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		
		// since we're just looking for one hull, do a one-pass traversal to find the best result
		MOAIPartitionHull* hull = buffer.FindBest ();
		if ( hull ) {
			hull->PushLuaUserdata ( state );
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hullForRay
	@text	Returns the first hull that intersects the given ray
	 
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@in		number xdirection
	@in		number ydirection
	@in		number zdirection
	@opt	number typeID
	@opt	number queryMask
	@out	MOAIPartitionHull hull		The hull under the point in order of depth or nil if no hull found.
*/
int MOAIPartition::_hullForRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )
	
	ZLVec3D vec;
	vec.mX					= state.GetValue < float >( 2, 0.0f );
	vec.mY 					= state.GetValue < float >( 3, 0.0f );
	vec.mZ 					= state.GetValue < float >( 4, 0.0f );
	
	ZLVec3D direction;
	direction.mX 			= state.GetValue < float >( 5, 0.0f );
	direction.mY 			= state.GetValue < float >( 6, 0.0f );
	direction.mZ 			= state.GetValue < float >( 7, 0.0f );
	direction.Norm ();
	
	ZLTypeID typeID			= state.GetValue < ZLTypeID >( 8, ZLType::GetID < MOAIPartitionHull >());
	u32 queryMask			= state.GetValue < u32 >( 9, MASK_ANY );
	
	MOAIScopedPartitionResultBufferHandle scopedBufferHandle = MOAIPartitionResultMgr::Get ().GetBufferHandle ();
	MOAIPartitionResultBuffer& buffer = scopedBufferHandle;
	
	u32 total = self->GatherHulls ( buffer, 0, vec, direction, typeID, queryMask );

	if ( total ) {
		
		MOAIPartitionHull* hull = buffer.FindBest ();
		if ( hull ) {
			hull->PushLuaUserdata ( state );
			return 1;
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hullList
	@text	Returns all props.
	
	@in		MOAIPartition self
	@opt	number sortMode			One of the MOAIPartitionViewLayer sort modes. Default value is SORT_NONE.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number typeID
	@opt	number queryMask
	@out	... props				The props pushed onto the stack.
*/
int MOAIPartition::_hullList ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "U" )

	u32 sortMode 			= state.GetValue < u32 >( 2, MOAIPartitionResultBuffer::SORT_NONE );
	float xScale 			= state.GetValue < float >( 3, 0.0f );
	float yScale 			= state.GetValue < float >( 4, 0.0f );
	float zScale 			= state.GetValue < float >( 5, 0.0f );
	float priorityScale 	= state.GetValue < float >( 6, 1.0f );
	ZLTypeID typeID			= state.GetValue < ZLTypeID >( 7, ZLType::GetID < MOAIPartitionHull >());
	u32 queryMask			= state.GetValue < u32 >( 8, MASK_ANY );

	MOAIScopedPartitionResultBufferHandle scopedBufferHandle = MOAIPartitionResultMgr::Get ().GetBufferHandle ();
	MOAIPartitionResultBuffer& buffer = scopedBufferHandle;

	u32 total = self->GatherHulls ( buffer, 0, typeID, queryMask );
	if ( total ) {
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushHulls ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hullListForPoint
	@text	Returns all props under a given world space point.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@opt	number sortMode			One of the MOAIPartitionViewLayer sort modes. Default value is SORT_NONE.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number typeID
	@opt	number queryMask
	@out	... props				The props under the point, all pushed onto the stack.
*/
int MOAIPartition::_hullListForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	ZLVec3D vec;
	vec.mX 					= state.GetValue < float >( 2, 0.0f );
	vec.mY 					= state.GetValue < float >( 3, 0.0f );
	vec.mZ 					= state.GetValue < float >( 4, 0.0f );

	u32 sortMode 			= state.GetValue < u32 >( 5, MOAIPartitionResultBuffer::SORT_NONE );
	float xScale 			= state.GetValue < float >( 6, 0.0f );
	float yScale 			= state.GetValue < float >( 7, 0.0f );
	float zScale 			= state.GetValue < float >( 8, 0.0f );
	float priorityScale 	= state.GetValue < float >( 9, 1.0f );
	ZLTypeID typeID			= state.GetValue < ZLTypeID >( 10, ZLType::GetID < MOAIPartitionHull >());
	u32 queryMask			= state.GetValue < u32 >( 11, MASK_ANY );

	MOAIScopedPartitionResultBufferHandle scopedBufferHandle = MOAIPartitionResultMgr::Get ().GetBufferHandle ();
	MOAIPartitionResultBuffer& buffer = scopedBufferHandle;

	u32 total = self->GatherHulls ( buffer, 0, vec, typeID, queryMask );
	if ( total ) {
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushHulls ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hullListForRay
	@text	Returns all props that intersect the given ray.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@in		number xdirection
	@in		number ydirection
	@in		number zdirection
	@opt	number sortMode			One of the MOAIPartitionViewLayer sort modes. Default value is SORT_KEY_ASCENDING.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number typeID
	@opt	number queryMask
	@out	... props				The props under the point in order of depth, all pushed onto the stack.
*/
int MOAIPartition::_hullListForRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	ZLVec3D vec;
	vec.mX 					= state.GetValue < float >( 2, 0.0f );
	vec.mY 					= state.GetValue < float >( 3, 0.0f );
	vec.mZ 					= state.GetValue < float >( 4, 0.0f );
	
	ZLVec3D direction;
	direction.mX 			= state.GetValue < float >( 5, 0.0f );
	direction.mY 			= state.GetValue < float >( 6, 0.0f );
	direction.mZ 			= state.GetValue < float >( 7, 0.0f );
	direction.Norm ();
	
	u32 sortMode 			= state.GetValue < u32 >( 8, MOAIPartitionResultBuffer::SORT_KEY_ASCENDING );
	float xScale 			= state.GetValue < float >( 9, 0.0f );
	float yScale 			= state.GetValue < float >( 10, 0.0f );
	float zScale 			= state.GetValue < float >( 11, 0.0f );
	float priorityScale 	= state.GetValue < float >( 12, 1.0f );
	ZLTypeID typeID			= state.GetValue < ZLTypeID >( 13, ZLType::GetID < MOAIPartitionHull >());
	u32 queryMask			= state.GetValue < u32 >( 14, MASK_ANY );
	
	MOAIScopedPartitionResultBufferHandle scopedBufferHandle = MOAIPartitionResultMgr::Get ().GetBufferHandle ();
	MOAIPartitionResultBuffer& buffer = scopedBufferHandle;
	
	u32 total = self->GatherHulls ( buffer, 0, vec, direction, typeID, queryMask );

	if ( total ) {
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushHulls ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hullListForRect
	@text	Returns all props under a given world space rect.
	
	@in		MOAIPartition self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number sortMode			One of the MOAIPartitionViewLayer sort modes. Default value is SORT_NONE.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@opt	number typeID
	@opt	number queryMask
	@out	... props				The props under the rect, all pushed onto the stack.
*/
int MOAIPartition::_hullListForRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNNNN" )
	
	ZLRect rect;
	
	rect.mXMin 				= state.GetValue < float >( 2, 0.0f );
	rect.mYMin 				= state.GetValue < float >( 3, 0.0f );
	
	rect.mXMax 				= state.GetValue < float >( 4, 0.0f );
	rect.mYMax 				= state.GetValue < float >( 5, 0.0f );
	
	u32 sortMode 			= state.GetValue < u32 >( 6, MOAIPartitionResultBuffer::SORT_NONE );
	float xScale 			= state.GetValue < float >( 7, 0.0f );
	float yScale 			= state.GetValue < float >( 8, 0.0f );
	float zScale			= state.GetValue < float >( 9, 0.0f );
	float priorityScale		= state.GetValue < float >( 10, 1.0f );
	ZLTypeID typeID			= state.GetValue < ZLTypeID >( 11, ZLType::GetID < MOAIPartitionHull >());
	u32 queryMask			= state.GetValue < u32 >( 12, MASK_ANY );
	
	MOAIScopedPartitionResultBufferHandle scopedBufferHandle = MOAIPartitionResultMgr::Get ().GetBufferHandle ();
	MOAIPartitionResultBuffer& buffer = scopedBufferHandle;
	
	u32 total = self->GatherHulls ( buffer, 0, rect, typeID, queryMask );
	if ( total ) {
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushHulls ( L );
		return total;
	}
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

	ZLIndex levelID		= state.GetValue < MOAILuaIndex >( 2, 0 );
	float cellSize		= state.GetValue < float >( 3, 1.0f );
	u32 width			= state.GetValue < u32 >( 4, 0 );
	u32 height			= state.GetValue < u32 >( 5, 0 );

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
void MOAIPartition::AffirmPriority ( MOAIPartitionHull& hull ) {

	if ( hull.mPriority == MOAIPartitionHull::UNKNOWN_PRIORITY ) {
		hull.mPriority = this->mPriorityCounter++;
		this->mPriorityCounter = this->mPriorityCounter & PRIORITY_MASK;
	}
}

//----------------------------------------------------------------//
void MOAIPartition::Clear () {

	ZLSize totalLevels = this->mLevels.Size ();
	for ( ZLIndex i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].Clear ();
	}
	this->mBiggies.Clear ();
	this->mGlobals.Clear ();
	this->mEmpties.Clear ();
}

//----------------------------------------------------------------//
void MOAIPartition::DrawDebugBack () {

	this->MOAIPartition_DrawDebugBack ();
}

//----------------------------------------------------------------//
void MOAIPartition::DrawDebugFront () {

	this->MOAIPartition_DrawDebugFront ();
}


//----------------------------------------------------------------//
u32 MOAIPartition::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, ZLTypeID typeID, u32 queryMask ) {
	
	results.Reset ();
	
	ZLSize totalLevels = this->mLevels.Size ();
	for ( ZLIndex i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherHulls ( results, ignoreProp, typeID, queryMask );
	}
	this->mBiggies.GatherHulls ( results, ignoreProp, typeID, queryMask );
	this->mGlobals.GatherHulls ( results, ignoreProp, typeID, queryMask );
	this->mEmpties.GatherHulls ( results, ignoreProp, typeID, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, const ZLVec3D& point, ZLTypeID typeID, u32 queryMask ) {
	
	results.Reset ();
	
	ZLSize totalLevels = this->mLevels.Size ();
	for ( ZLIndex i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherHulls ( results, ignoreProp, point, this->mPlaneID, typeID, queryMask );
	}
	this->mBiggies.GatherHulls ( results, ignoreProp, point, typeID, queryMask );
	this->mGlobals.GatherHulls ( results, ignoreProp, typeID, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, const ZLVec3D& point, const ZLVec3D& orientation, ZLTypeID typeID, u32 queryMask ) {
	
	results.Reset ();
	
	ZLSize totalLevels = this->mLevels.Size ();
	for ( ZLIndex i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherHulls ( results, ignoreProp, point, orientation, typeID, queryMask );
	}
	this->mBiggies.GatherHulls ( results, ignoreProp, point, orientation, typeID, queryMask );
	this->mGlobals.GatherHulls ( results, ignoreProp, point, orientation, typeID, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, ZLRect rect, ZLTypeID typeID, u32 queryMask ) {
	
	results.Reset ();
	rect.Bless ();
	
	ZLSize totalLevels = this->mLevels.Size ();
	for ( ZLIndex i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherHulls ( results, ignoreProp, rect, typeID, queryMask );
	}
	this->mBiggies.GatherHulls ( results, ignoreProp, rect, typeID, queryMask );
	this->mGlobals.GatherHulls ( results, ignoreProp, typeID, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, ZLBox box, ZLTypeID typeID, u32 queryMask ) {
	
	results.Reset ();
	box.Bless ();
	
	ZLSize totalLevels = this->mLevels.Size ();
	for ( ZLIndex i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherHulls ( results, ignoreProp, box, this->mPlaneID, typeID, queryMask );
	}
	this->mBiggies.GatherHulls ( results, ignoreProp, box, typeID, queryMask );
	this->mGlobals.GatherHulls ( results, ignoreProp, typeID, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, const ZLFrustum& frustum, ZLTypeID typeID, u32 queryMask ) {
	
	results.Reset ();
	
	ZLSize totalLevels = this->mLevels.Size ();
	for ( ZLIndex i = 0; i < totalLevels; ++i ) {
		this->mLevels [ i ].GatherHulls ( results, ignoreProp, frustum, this->mPlaneID, typeID, queryMask );
	}
	this->mBiggies.GatherHulls ( results, ignoreProp, frustum, typeID, queryMask );
	this->mGlobals.GatherHulls ( results, ignoreProp, typeID, queryMask );
	
	return results.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
}

//----------------------------------------------------------------//
void MOAIPartition::InsertHull ( MOAIPartitionHull& hull ) {
	
	if ( hull.mPartition == this ) return;
	
	this->LuaRetain ( &hull );
	
	if ( hull.mPartition ) {
		hull.mPartition->RemoveHull ( hull );
	}
	
	if ( hull.mPriority == MOAIPartitionHull::UNKNOWN_PRIORITY ) {
		hull.mPriority = this->mPriorityCounter++;
		this->mPriorityCounter = this->mPriorityCounter & PRIORITY_MASK;
	}
	
	this->mEmpties.InsertHull ( hull );
	this->AffirmPriority ( hull );
	
	hull.mPartition = this;
	hull.ScheduleUpdate ();
	
	this->MOAIPartition_OnInsertHull ( hull );
}

//----------------------------------------------------------------//
bool MOAIPartition::IsEmpty ( MOAIPartitionHull& hull ) {
	return hull.mCell == &this->mEmpties;
}

//----------------------------------------------------------------//
bool MOAIPartition::IsGlobal ( MOAIPartitionHull& hull ) {
	return hull.mCell == &this->mGlobals;
}

//----------------------------------------------------------------//
MOAIPartition::MOAIPartition () :
	mPriorityCounter ( 0 ),
	mPlaneID ( ZLBox::PLANE_XY ) {
	
	RTTI_BEGIN ( MOAIPartition )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIPartition >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPartition::~MOAIPartition () {
	this->Clear ();
}

//----------------------------------------------------------------//
// This moves all props to the 'empties' cell
void MOAIPartition::PrepareRebuild () {

	ZLSize totalLevels = this->mLevels.Size ();
	for ( ZLIndex i = 0; i < totalLevels; ++i ) {
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
void MOAIPartition::RemoveHull ( MOAIPartitionHull& hull ) {

	if ( hull.mPartition != this ) return;
	
	if ( hull.mCell ) {
		hull.mCell->RemoveHull ( hull );
	}

	hull.mPartition = 0;
	this->MOAIPartition_OnRemoveHull ( hull );

	this->LuaRelease ( &hull );
}

//----------------------------------------------------------------//
void MOAIPartition::ReserveLevels ( ZLSize totalLevels ) {

	this->PrepareRebuild ();
	this->mLevels.Init ( totalLevels );
	this->Rebuild ();
}

//----------------------------------------------------------------//
void MOAIPartition::SetLevel ( ZLIndex levelID, float cellSize, int width, int height ) {

	this->PrepareRebuild ();
	this->mLevels [ levelID ].Init ( cellSize, width, height );
	this->Rebuild ();
}

//----------------------------------------------------------------//
void MOAIPartition::UpdateHull ( MOAIPartitionHull& hull ) {

	// clear out the level; level will be re-calculated below
	// also: hull.mLevel is *only* for debug drawing 
	hull.mLevel = 0;
	
	u32 status = hull.mWorldBounds.mStatus;

	// status is not 'OK' so hull is either global or empty
	if ( status == ZLBounds::ZL_BOUNDS_GLOBAL ) {
		this->mGlobals.InsertHull ( hull );
		return;
	}
	
	if ( status == ZLBounds::ZL_BOUNDS_EMPTY ) {
		this->mEmpties.InsertHull ( hull );
		return;
	}

	ZLRect rect = hull.mWorldBounds.mAABB.GetRect ( this->mPlaneID );

	float width = rect.Width ();
	float height = rect.Height ();
	float cellSize = ( width > height ) ? width : height;
	
	if ( cellSize > 0.0f ) {
		
		MOAIPartitionLevel* level = 0;
		
		ZLSize totalLevels = this->mLevels.Size ();
		for ( ZLIndex i = 0; i < totalLevels; ++i ) {
			
			MOAIPartitionLevel* testLevel = &this->mLevels [ i ];
			
			if ( cellSize <= testLevel->mCellSize ) {
				if (( level == 0 ) || ( testLevel->mCellSize  < level->mCellSize )) {
					level = testLevel;
				}
			}
		}
		
		if ( level ) {
			// level hull
			level->PlaceHull ( hull );
			hull.mLevel = level;
		}
		else {
			// biggie hull - has dimension but too big to fit in any level
			this->mBiggies.InsertHull ( hull );
		}
	}
	else {
		// empty hull
		this->mEmpties.InsertHull ( hull );
	}
}

//----------------------------------------------------------------//
void MOAIPartition::SetPlane ( u32 planeID ) {

	if ( this->mPlaneID == planeID ) return;
	this->PrepareRebuild ();
	this->mPlaneID = planeID;
	this->Rebuild ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPartition::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	state.SetField ( -1, "PLANE_XY",	( u32 )ZLBox::PLANE_XY );
	state.SetField ( -1, "PLANE_XZ",	( u32 )ZLBox::PLANE_XZ );
	state.SetField ( -1, "PLANE_YZ",	( u32 )ZLBox::PLANE_YZ );
}

//----------------------------------------------------------------//
void MOAIPartition::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "clear",						_clear },
		{ "hullForPoint",				_hullForPoint },
		{ "hullForRay",					_hullForRay },
		{ "hullList",					_hullList },
		{ "hullListForPoint",			_hullListForPoint },
		{ "hullListForRay",				_hullListForRay },
		{ "hullListForRect",			_hullListForRect },
		{ "reserveLevels",				_reserveLevels },
		{ "setLevel",					_setLevel },
		{ "setPlane",					_setPlane },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPartition::MOAIPartition_DrawDebugBack () {
}

//----------------------------------------------------------------//
void MOAIPartition::MOAIPartition_DrawDebugFront () {
}

//----------------------------------------------------------------//
void MOAIPartition::MOAIPartition_OnInsertHull ( MOAIPartitionHull& hull ) {
	UNUSED ( hull );
}

//----------------------------------------------------------------//	
void MOAIPartition::MOAIPartition_OnRemoveHull ( MOAIPartitionHull& hull ) {
	UNUSED ( hull );
}

//----------------------------------------------------------------//
void MOAIPartition::MOAIPartition_OnUpdateHull ( MOAIPartitionHull& hull ) {
	UNUSED ( hull );
}
