// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLayer.h>
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

	MOAIProp* prop = state.GetLuaObject < MOAIProp >( 2 );
	if ( !prop ) return 0;

	self->InsertProp ( *prop );
	prop->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	propForPoint
	@text	Returns the prop with the highest priority underneath
			the given world space point.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@opt	number sortMode			One of the MOAILayer sort modes. Default value is SORT_PRIORITY_ASCENDING.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@out	MOAIProp prop		The prop under the point or nil if no prop found.
*/
int MOAIPartition::_propForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	USVec2D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );

	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();

	u32 total = self->GatherProps ( buffer, 0, vec );
	if ( total ) {
	
		buffer.PrepareResults ( MOAIPartitionResultBuffer::SORT_NONE );
		
		u32 sortMode = state.GetValue < u32 >( 4, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
		float xScale = state.GetValue < float >( 5, 0.0f );
		float yScale = state.GetValue < float >( 6, 0.0f );
		float priorityScale = state.GetValue < float >( 7, 1.0f );
		
		buffer.PrepareResults ( MOAIPartitionResultBuffer::SORT_NONE );
		MOAIProp* prop = buffer.FindBest ( sortMode, xScale, yScale, 0.0f, priorityScale );
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
	@opt	number sortMode			One of the MOAILayer sort modes. Default value is SORT_NONE.
	@opt	number xScale			X scale for vector sort. Default value is 0.
	@opt	number yScale			Y scale for vector sort. Default value is 0.
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@out	...						The props under the point, all pushed onto the stack.
*/
int MOAIPartition::_propListForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	USVec2D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );

	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();

	u32 total = self->GatherProps ( buffer, 0, vec );
	if ( total ) {
	
		u32 sortMode = state.GetValue < u32 >( 4, MOAIPartitionResultBuffer::SORT_NONE );
		float xScale = state.GetValue < float >( 5, 0.0f );
		float yScale = state.GetValue < float >( 6, 0.0f );
		float priorityScale = state.GetValue < float >( 7, 1.0f );
	
		buffer.PrepareResults ( sortMode, false, xScale, yScale, 0.0f, priorityScale );
		buffer.PushResultProps ( L );
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
	@opt	number priorityScale	Priority scale for vector sort. Default value is 1.
	@out	...						The props under the rect, all pushed onto the stack.
*/
int MOAIPartition::_propListForRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNNNN" )

	USRect rect = state.GetRect < float >( 2 );
	
	MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();

	u32 total = self->GatherProps ( buffer, 0, rect );
	if ( total ) {
	
		u32 sortMode = state.GetValue < u32 >( 6, MOAIPartitionResultBuffer::SORT_NONE );
		float xScale = state.GetValue < float >( 7, 0.0f );
		float yScale = state.GetValue < float >( 8, 0.0f );
		float priorityScale = state.GetValue < float >( 9, 1.0f );
	
		buffer.PrepareResults ( sortMode, false, xScale, yScale, 0.0f, priorityScale );
		buffer.PushResultProps ( L );
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

	MOAIProp* prop = state.GetLuaObject < MOAIProp >( 2 );
	if ( !prop ) return 0;

	self->RemoveProp ( *prop );
	prop->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveLayers
	@text	Reserves a stack of layers in the partition. Layers must be
			initialize with setLayer ().
	
	@in		MOAIPartition self
	@in		number nLayers
	@out	nil
*/
int MOAIPartition::_reserveLayers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UN" )

	u32 totalLayers = state.GetValue < u32 >( 2, 0 );
	
	self->ReserveLayers ( totalLayers );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLayer
	@text	Initializes a layer previously created by reserveLayers (). Each
			layer is a loose grid. Props of a given size may be placed by
			the system into any layer with cells large enough to accomodate them.
			The dimensions of a layer control how many cells the layer contains.
			If an object goes off of the edge of a layer, it will wrap around
			to the other side. It is possible to model a quad tree by initalizing
			layers correctly, but for some simulations better structures
			may be possible.
	
	@in		MOAIPartition self
	@in		number layerID
	@in		number cellSize		Dimensions of the layer's cells.
	@in		number xCells		Width of layer in cells.
	@in		number yCells		Height of layer in cells.
	@out	nil
*/
int MOAIPartition::_setLayer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNNNN" )

	u32 layerID		= state.GetValue < u32 >( 2, 1 ) - 1;
	float cellSize	= state.GetValue < float >( 3, 1.0f );
	u32 width		= state.GetValue < u32 >( 4, 0 );
	u32 height		= state.GetValue < u32 >( 5, 0 );

	self->SetLayer ( layerID, cellSize, width, height );

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

	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].Clear ();
	}
	this->mGlobals.Clear ();
	this->mEmpties.Clear ();
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 mask ) {
	
	results.Reset ();
	
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherProps ( results, ignore, mask );
	}
	this->mGlobals.GatherProps ( results, ignore, mask );
	this->mEmpties.GatherProps ( results, ignore, mask );
	
	return results.mTotalProps;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec2D& point, u32 mask ) {
	
	results.Reset ();
	
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherProps ( results, ignore, point, mask );
	}
	this->mGlobals.GatherProps ( results, ignore, point, mask );
	
	return results.mTotalProps;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USRect& rect, u32 mask ) {
	
	results.Reset ();
	
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherProps ( results, ignore, rect, mask );
	}
	this->mGlobals.GatherProps ( results, ignore, rect, mask );
	
	return results.mTotalProps;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USFrustum& frustum, u32 mask ) {
	
	results.Reset ();
	
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherProps ( results, ignore, frustum, mask );
	}
	this->mGlobals.GatherProps ( results, ignore, frustum, mask );
	
	return results.mTotalProps;
}

//----------------------------------------------------------------//
void MOAIPartition::InsertProp ( MOAIProp& prop ) {
	
	if ( prop.mPartition == this ) return;
	
	this->LuaRetain ( prop );
	
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
MOAIPartition::MOAIPartition () :
	mPriorityCounter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPartition::~MOAIPartition () {
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "clear",						_clear },
		{ "insertProp",					_insertProp },
		{ "propForPoint",				_propForPoint },
		{ "propListForPoint",			_propListForPoint },
		{ "propListForRect",			_propListForRect },
		{ "removeProp",					_removeProp },
		{ "reserveLayers",				_reserveLayers },
		{ "setLayer",					_setLayer },
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
	this->LuaRelease ( prop );
}

//----------------------------------------------------------------//
void MOAIPartition::ReserveLayers ( int totalLayers ) {

	this->mLayers.Init ( totalLayers );
}

//----------------------------------------------------------------//
void MOAIPartition::SetLayer ( int layerID, float cellSize, int width, int height ) {

	this->mLayers [ layerID ].Init ( cellSize, width, height );
}

//----------------------------------------------------------------//
void MOAIPartition::UpdateProp ( MOAIProp& prop, u32 status ) {

	prop.SetBounds ();

	if ( status == MOAIProp::BOUNDS_GLOBAL ) {
		this->mGlobals.InsertProp ( prop );
	}
	else {
		this->mEmpties.InsertProp ( prop );
	}
}

//----------------------------------------------------------------//
void MOAIPartition::UpdateProp ( MOAIProp& prop, const USRect& bounds, u32 status ) {

	if ( status != MOAIProp::BOUNDS_OK ) {
	
		this->UpdateProp ( prop, status );
		return;
	}

	u32 layerID;
	prop.SetBounds ( bounds );

	float cellSize = prop.mCellSize;
	if ( cellSize > 0.0f ) {
		
		MOAIPartitionLayer* layer = 0;
		
		u32 totalLayers = this->mLayers.Size ();
		for ( u32 i = 0; i < totalLayers; ++i ) {
		
			MOAIPartitionLayer* testLayer = &this->mLayers [ i ];
			
			if ( cellSize <= testLayer->mCellSize ) {
				if (( layer == 0 ) || ( testLayer->mCellSize  < layer->mCellSize )) {
					layer = testLayer;
					layerID = i;
				}
			}
		}
		
		prop.mLayer = layer;
		
		if ( layer ) {
			//printf ( "layer %d ", ( int )layerID );
			layer->PlaceProp ( prop );
		}
		else {
			//printf ( "global cell\n" );
			this->mGlobals.InsertProp ( prop );
		}
	}
	else {
		//printf ( "empty cell\n" );
		prop.mLayer = 0;
		this->mEmpties.InsertProp ( prop );
	}
}









