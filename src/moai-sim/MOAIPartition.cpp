// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionCell.h>
#include <moai-sim/MOAIPartitionLevel.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
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
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartition::_hulls ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UU" )

	MOAIPartitionResultBuffer* buffer = state.GetLuaObject < MOAIPartitionResultBuffer >( 2, true );
	if ( buffer ) {
	
		ZLTypeID typeID			= state.GetValue < ZLTypeID >( 3, ZLType::GetID < MOAIPartitionHull >());
		u32 queryMask			= state.GetValue < u32 >( 4, MASK_ANY );

		self->GatherHulls ( *buffer, 0, typeID, queryMask );
		buffer->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartition::_hullsForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UU" )

	MOAIPartitionResultBuffer* buffer = state.GetLuaObject < MOAIPartitionResultBuffer >( 2, true );
	if ( buffer ) {

		ZLVec3D vec				= state.GetValue < ZLVec3D >( 3, ZLVec3D::ORIGIN );
		ZLTypeID typeID			= state.GetValue < ZLTypeID >( 6, ZLType::GetID < MOAIPartitionHull >());
		u32 queryMask			= state.GetValue < u32 >( 7, MASK_ANY );

		self->GatherHulls ( *buffer, 0, vec, typeID, queryMask );
		buffer->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartition::_hullsForRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UU" )

	MOAIPartitionResultBuffer* buffer = state.GetLuaObject < MOAIPartitionResultBuffer >( 2, true );
	if ( buffer ) {

		ZLVec3D vec				= state.GetValue < ZLVec3D >( 3, ZLVec3D::ORIGIN );
		ZLVec3D direction		= state.GetValue < ZLVec3D >( 6, ZLVec3D::ORIGIN );
		ZLTypeID typeID			= state.GetValue < ZLTypeID >( 9, ZLType::GetID < MOAIPartitionHull >());
		u32 queryMask			= state.GetValue < u32 >( 10, MASK_ANY );
		
		direction.Norm ();
		self->GatherHulls ( *buffer, 0, vec, direction, typeID, queryMask );
		buffer->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartition::_hullsForRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UU" )
	
	MOAIPartitionResultBuffer* buffer = state.GetLuaObject < MOAIPartitionResultBuffer >( 2, true );
	if ( buffer ) {
	
		ZLRect rect				= state.GetValue < ZLRect >( 3, ZLRect::EMPTY );
		ZLTypeID typeID			= state.GetValue < ZLTypeID >( 7, ZLType::GetID < MOAIPartitionHull >());
		u32 queryMask			= state.GetValue < u32 >( 8, MASK_ANY );
	
		self->GatherHulls ( *buffer, 0, rect, typeID, queryMask );
		buffer->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartition::_hullsForViewProj ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "U" )
	
	MOAIPartitionResultBuffer* buffer = state.GetLuaObject < MOAIPartitionResultBuffer >( 2, true );
	if ( buffer ) {
	
		MOAIViewport* viewport	= state.GetLuaObject < MOAIViewport >( 3, false );
		MOAICamera* camera		= state.GetLuaObject < MOAICamera >( 4, false );
		ZLTypeID typeID			= state.GetValue < ZLTypeID >( 5, ZLType::GetID < MOAIPartitionHull >());
		u32 queryMask			= state.GetValue < u32 >( 6, MASK_ANY );
		
		// TODO: get rid of the NULL viewport idiom
		ZLViewport worldViewport;
		if ( viewport ) {
			worldViewport = viewport->GetWorldViewport ();
		}
		
		self->GatherHulls ( *buffer, 0, viewport ? &worldViewport : NULL, camera, typeID, queryMask );
		buffer->PushLuaUserdata ( state );
		return 1;
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
	
	MOAI_LUA_RETURN_SELF
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

	MOAI_LUA_RETURN_SELF
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

	MOAI_LUA_RETURN_SELF
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
u32 MOAIPartition::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignoreProp, const ZLViewport* viewport, const ZLCamera* camera, ZLTypeID typeID, u32 mask ) {

	if ( !viewport ) return this->GatherHulls ( results, ignoreProp, typeID, mask );
		
	ZLMatrix4x4 viewMtx = ZLViewProj::GetViewMtx ( camera );
	ZLMatrix4x4 invViewProjMtx = viewMtx;
	invViewProjMtx.Append ( ZLViewProj::GetProjectionMtx ( viewport, camera ));
	invViewProjMtx.Inverse ();

	ZLFrustum viewVolume;
	viewVolume.Init ( invViewProjMtx );
	
	return this->GatherHulls ( results, 0, viewVolume, typeID );
	
//	if ( self->mPartitionCull2D ) {
//		totalResults = partition->GatherHulls ( *buffer, 0, viewVolume.mAABB, typeID );
//	}
//	else {
//		totalResults = partition->GatherHulls ( *buffer, 0, viewVolume, typeID );
//	}
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
MOAIPartition::MOAIPartition ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
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
	
	// status is not 'OK' so hull is either global or empty
	if ( hull.mWorldBounds.IsGlobal ()) {
		this->mGlobals.InsertHull ( hull );
		return;
	}
	
	if ( hull.mWorldBounds.IsEmpty ()) {
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
	if ( history.Visit ( *this )) return;
	
	state.SetField ( -1, "PLANE_XY",	( u32 )ZLBox::PLANE_XY );
	state.SetField ( -1, "PLANE_XZ",	( u32 )ZLBox::PLANE_XZ );
	state.SetField ( -1, "PLANE_YZ",	( u32 )ZLBox::PLANE_YZ );
}

//----------------------------------------------------------------//
void MOAIPartition::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "clear",						_clear },
		{ "hulls",						_hulls },
		{ "hullsForPoint",				_hullsForPoint },
		{ "hullsForRay",				_hullsForRay },
		{ "hullsForRect",				_hullsForRect },
		{ "hullsForViewProj",			_hullsForViewProj },
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
