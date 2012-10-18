// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLevel.h>
#include <moaicore/MOAIPartitionResultBuffer.h>
#include <moaicore/MOAIPartitionResultMgr.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clear
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
/**	@name	insertProp
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
/**	@name	propForPoint
	@text	Returns the prop with the highest priority that contains
			the given world space point.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@opt	number sortMode			One of the MOAILayer sort modes. Default value is SORT_PRIORITY_ASCENDING.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number zScale			Z scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@out	MOAIProp prop		The prop under the point or nil if no prop found.
*/
int MOAIPartition::_propForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	USVec3D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	vec.mZ = state.GetValue < float >( 4, 0.0f );

	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();

	
	u32 total = self->GatherProps ( buffer, 0, vec );
	if ( total ) {
		
		buffer.Sort ( MOAIPartitionResultBuffer::SORT_NONE );
		
		u32 sortMode = state.GetValue < u32 >( 5, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
		float xScale = state.GetValue < float >( 6, 0.0f );
		float yScale = state.GetValue < float >( 7, 0.0f );
		float zScale = state.GetValue < float >( 8, 0.0f );
		float priorityScale = state.GetValue < float >( 9, 1.0f );
		
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
/**	@name	propForRay
	@text	Returns the prop closest to the camera that intersects the given ray
	 
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@in		number z
	@in		number xdirection
	@in		number ydirection
	@in		number zdirection
	@out	MOAIProp prop		The prop under the point in order of depth or nil if no prop found.
*/
int MOAIPartition::_propForRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )
	
	USVec3D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	vec.mZ = state.GetValue < float >( 4, 0.0f );
	
	USVec3D direction;
	direction.mX = state.GetValue < float >( 5, 0.0f );
	direction.mY = state.GetValue < float >( 6, 0.0f );
	direction.mZ = state.GetValue < float >( 7, 0.0f );
	
	direction.Norm();
	
	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
	
	u32 total = self->GatherProps ( buffer, 0, vec, direction );

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
/**	@name	propListForPoint
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
	@out	...						The props under the point, all pushed onto the stack.
*/
int MOAIPartition::_propListForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	USVec3D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	vec.mZ = state.GetValue < float >( 4, 0.0f );

	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();

	u32 total = self->GatherProps ( buffer, 0, vec );
	if ( total ) {
	
		u32 sortMode = state.GetValue < u32 >( 5, MOAIPartitionResultBuffer::SORT_NONE );
		float xScale = state.GetValue < float >( 6, 0.0f );
		float yScale = state.GetValue < float >( 7, 0.0f );
		float zScale = state.GetValue < float >( 8, 0.0f );
		float priorityScale = state.GetValue < float >( 9, 1.0f );
		
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushProps ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	propListForRay
	@text	Returns all props under a given world space point.
	
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
	@out	...						The props under the point in order of depth, all pushed onto the stack.
*/
int MOAIPartition::_propListForRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	USVec3D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	vec.mZ = state.GetValue < float >( 4, 0.0f );
	
	USVec3D direction;
	direction.mX = state.GetValue < float >( 5, 0.0f );
	direction.mY = state.GetValue < float >( 6, 0.0f );
	direction.mZ = state.GetValue < float >( 7, 0.0f );
	
	direction.Norm();
	
	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
	
	u32 total = self->GatherProps ( buffer, 0, vec, direction );

	if ( total ) {
	
		u32 sortMode = state.GetValue < u32 >( 8, MOAIPartitionResultBuffer::SORT_KEY_ASCENDING );
		float xScale = state.GetValue < float >( 9, 0.0f );
		float yScale = state.GetValue < float >( 10, 0.0f );
		float zScale = state.GetValue < float >( 11, 0.0f );
		float priorityScale = state.GetValue < float >( 12, 1.0f );
		
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushProps ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	propListForRect
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
	@out	...						The props under the rect, all pushed onto the stack.
*/
int MOAIPartition::_propListForRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNNNN" )
	
	USBox box;
	
	box.mMin.mX = state.GetValue < float >( 2, 0.0f );
	box.mMin.mY = state.GetValue < float >( 3, 0.0f );
	box.mMin.mZ = 0.0f;
	
	box.mMax.mX = state.GetValue < float >( 4, 0.0f );
	box.mMax.mY = state.GetValue < float >( 5, 0.0f );
	box.mMax.mZ = 0.0f;
	
	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
	
	u32 total = self->GatherProps ( buffer, 0, box );
	if ( total ) {
	
		u32 sortMode = state.GetValue < u32 >( 6, MOAIPartitionResultBuffer::SORT_NONE );
		float xScale = state.GetValue < float >( 7, 0.0f );
		float yScale = state.GetValue < float >( 8, 0.0f );
		float zScale = state.GetValue < float >( 9, 0.0f );
		float priorityScale = state.GetValue < float >( 10, 1.0f );
	
		buffer.GenerateKeys ( sortMode, xScale, yScale, zScale, priorityScale );
		buffer.Sort ( sortMode );
		buffer.PushProps ( L );
		return total;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	removeProp
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
/**	@name	reserveLayers
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
/**	@name	setLevel
	@text	Initializes a level previously created by reserveLevels ().
			This will trigger a full rebuild of the partition if it contains any props.
			Each level is a loose grid. Props of a given size may be placed by
			the system into any level with cells large enough to accomodate them.
			The dimensions of a level control how many cells the level contains.
			If an object goes off of the edge of a level, it will wrap around
			to the other side. It is possible to model a quad tree by initalizing
			levels correctly, but for some simulations better structures
			may be possible.
	
	@in		MOAIPartition self
	@in		number levelID
	@in		number cellSize		Dimensions of the layer's cells.
	@in		number xCells		Width of layer in cells.
	@in		number yCells		Height of layer in cells.
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
/**	@name	setPlane
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

	u32 planeID	= state.GetValue < u32 >( 2, USBox::PLANE_XY );
	self->SetPlane ( planeID );

	return 0;
}

//================================================================//
// MOAIPartition
//================================================================//

//----------------------------------------------------------------//
void MOAIPartition::AffirmPriority ( MOAIProp& prop ) {

	if ( prop.mPriority == MOAIProp::UNKNOWN_PRIORITY ) {
		prop.mPriority = this->mPriorityCounter++;
		this->mPriorityCounter = this->mPriorityCounter & PRIORITY_MASK;
	}
}

//----------------------------------------------------------------//
void MOAIPartition::Clear () {

	u32 totalLayers = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLevels [ i ].Clear ();
	}
	this->mBiggies.Clear ();
	this->mGlobals.Clear ();
	this->mEmpties.Clear ();
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 mask ) {
	
	results.Reset ();
	
	u32 totalLayers = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, mask );
	}
	this->mBiggies.GatherProps ( results, ignore, mask );
	this->mGlobals.GatherProps ( results, ignore, mask );
	this->mEmpties.GatherProps ( results, ignore, mask );
	
	return results.mTotalResults;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec3D& point, const USVec3D& orientation, u32 mask ) {
	
	results.Reset ();
	
	u32 totalLayers = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, point, orientation, mask );
	}
	this->mBiggies.GatherProps ( results, ignore, point, orientation, mask );
	this->mGlobals.GatherProps ( results, ignore, point, orientation, mask );
	
	return results.mTotalResults;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec3D& point, u32 mask ) {
	
	results.Reset ();
	
	u32 totalLayers = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, point, this->mPlaneID, mask );
	}
	this->mBiggies.GatherProps ( results, ignore, point, mask );
	this->mGlobals.GatherProps ( results, ignore, mask );
	
	return results.mTotalResults;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, USBox box, u32 mask ) {
	
	results.Reset ();
	box.Bless ();
	
	u32 totalLayers = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, box, this->mPlaneID, mask );
	}
	this->mBiggies.GatherProps ( results, ignore, box, mask );
	this->mGlobals.GatherProps ( results, ignore, mask );
	
	return results.mTotalResults;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USFrustum& frustum, u32 mask ) {
	
	results.Reset ();
	
	u32 totalLayers = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLevels [ i ].GatherProps ( results, ignore, frustum, this->mPlaneID, mask );
	}
	this->mBiggies.GatherProps ( results, ignore, frustum, mask );
	this->mGlobals.GatherProps ( results, ignore, mask );
	
	return results.mTotalResults;
}

//----------------------------------------------------------------//
void MOAIPartition::InsertProp ( MOAIProp& prop ) {
	
	if ( prop.mPartition == this ) return;
	
	this->LuaRetain ( &prop );
	
	if ( prop.mPartition ) {
		prop.mPartition->RemoveProp ( prop );
	}
	
	if ( prop.mPriority == MOAIProp::UNKNOWN_PRIORITY ) {
		prop.mPriority = this->mPriorityCounter++;
		this->mPriorityCounter = this->mPriorityCounter & PRIORITY_MASK;
	}
	
	this->mEmpties.InsertProp ( prop );
	this->AffirmPriority ( prop );
	
	prop.mPartition = this;
	prop.ScheduleUpdate ();
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
	mPlaneID ( USBox::PLANE_XY ) {
	
	RTTI_BEGIN
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

	u32 totalLayers = this->mLevels.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
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
	
	state.SetField ( -1, "PLANE_XY",	( u32 )USBox::PLANE_XY );
	state.SetField ( -1, "PLANE_XZ",	( u32 )USBox::PLANE_XZ );
	state.SetField ( -1, "PLANE_YZ",	( u32 )USBox::PLANE_YZ );
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "clear",						_clear },
		{ "insertProp",					_insertProp },
		{ "propForPoint",				_propForPoint },
		{ "propForRay",					_propForRay },
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

	// clear out the layer; layer will be re-calculated below
	// also: prop.mLayer is *only* for debug drawing 
	prop.mLayer = 0;

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

	USRect rect = prop.mBounds.GetRect ( this->mPlaneID );

	float width = rect.Width ();
	float height = rect.Height ();
	float cellSize = ( width > height ) ? width : height;
	
	u32 layerID;
	if ( cellSize > 0.0f ) {
		
		MOAIPartitionLevel* layer = 0;
		
		u32 totalLayers = this->mLevels.Size ();
		for ( u32 i = 0; i < totalLayers; ++i ) {
			
			MOAIPartitionLevel* testLayer = &this->mLevels [ i ];
			
			if ( cellSize <= testLayer->mCellSize ) {
				if (( layer == 0 ) || ( testLayer->mCellSize  < layer->mCellSize )) {
					layer = testLayer;
					layerID = i;
				}
			}
		}
		
		if ( layer ) {
			// layer prop
			layer->PlaceProp ( prop );
			prop.mLayer = layer;
		}
		else {
			// biggie prop - has dimension but too big to fit in any layer
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
