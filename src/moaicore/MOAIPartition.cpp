// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLayer.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// local
//================================================================//

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

	return 0;
}

//----------------------------------------------------------------//
/**	@name	propForPoint
	@text	Returns the prop with the highest priority underneath
			the given world space point.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@out	MOAIProp prop	The prop under the point or nil if no prop found.
*/
int MOAIPartition::_propForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	USVec2D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );

	u32 total = self->GatherProps ( vec, 0 );
	if ( total ) {
	
		MOAIProp* prop = self->PopResult ();
		while ( MOAIProp* test = self->PopResult ()) {
			if ( test->GetPriority () > prop->GetPriority ()) {
				prop = test;
			}
		}
		prop->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	propListForPoint
	@text	Returns all props under a given world space point.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@out	...				The props under the point, all pushed onto the stack.
*/
int MOAIPartition::_propListForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	USVec2D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );

	u32 total = self->GatherProps ( vec, 0 );
	if ( total ) {
		self->PushResultsList ( L );
		return 1;
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
	@out	...				The props under the rect, all pushed onto the stack.
*/
int MOAIPartition::_propListForRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNNNN" )

	USRect rect;
	rect.mXMin = state.GetValue < float >( 2, 0.0f );
	rect.mYMin = state.GetValue < float >( 3, 0.0f );
	rect.mXMax = state.GetValue < float >( 4, 0.0f );
	rect.mYMax = state.GetValue < float >( 5, 0.0f );

	u32 total = self->GatherProps ( rect, 0 );
	if ( total ) {
		self->PushResultsList ( L );
		return 1;
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

//----------------------------------------------------------------//
/**	@name	propListForPoint
	@text	Returns all props under a given world space point sorted
			by priority.
	
	@in		MOAIPartition self
	@in		number x
	@in		number y
	@out	...				The props under the point, sorted and pushed onto the stack.
*/
int MOAIPartition::_sortedPropListForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNN" )

	USVec2D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );
	
	u32 total = self->GatherProps ( vec, 0 );
	if ( total ) {
		self->PushSortedResultsList ( L );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	propListForRect
	@text	Returns all props under a given world space rect sorted
			by priority.
	
	@in		MOAIPartition self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	...				The props under the rect, sorted and pushed onto the stack.
*/
int MOAIPartition::_sortedPropListForRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartition, "UNNNN" )

	USRect rect;
	rect.mXMin = state.GetValue < float >( 2, 0.0f );
	rect.mYMin = state.GetValue < float >( 3, 0.0f );
	rect.mXMax = state.GetValue < float >( 4, 0.0f );
	rect.mYMax = state.GetValue < float >( 5, 0.0f );
	
	u32 total = self->GatherProps ( rect, 0 );
	if ( total ) {
		self->PushSortedResultsList ( L );
		return 1;
	}
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

	this->GatherProps ( 0, 0 );
	while ( MOAIProp* prop = this->PopResult ()) {
		this->RemoveProp ( *prop );
	}
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( MOAIProp* ignore, u32 mask ) {
	
	this->ResetResults ();
	
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherProps ( *this, ignore, mask );
	}
	this->mGlobals.GatherProps ( *this, ignore, mask );
	this->mEmpties.GatherProps ( *this, ignore, mask );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( USVec2D& point, MOAIProp* ignore, u32 mask ) {
	
	this->ResetResults ();
	
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherProps ( *this, ignore, point, mask );
	}
	this->mGlobals.GatherProps ( *this, ignore, point, mask );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
u32 MOAIPartition::GatherProps ( USRect& rect, MOAIProp* ignore, u32 mask ) {
	
	this->ResetResults ();
	
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherProps ( *this, ignore, rect, mask );
	}
	this->mGlobals.GatherProps ( *this, ignore, rect, mask );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
void MOAIPartition::InsertProp ( MOAIProp& prop ) {
	
	if ( prop.mPartition == this ) return;
	
	prop.Retain ();
	
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
	mTotalResults ( 0 ),
	mResults ( 0 ),
	mPriorityCounter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPartition::~MOAIPartition () {

	this->Clear ();
}

//----------------------------------------------------------------//
MOAIProp* MOAIPartition::PopResult () {

	MOAIProp* result = 0;

	if ( this->mResults ) {
		
		result = this->mResults;
		this->mResults = this->mResults->mNextResult;
	}
	return result;
}

//----------------------------------------------------------------//
void MOAIPartition::PushResult ( MOAIProp& result ) {

	result.mNextResult = this->mResults;
	this->mResults = &result;
	this->mTotalResults++;
}

//----------------------------------------------------------------//
void MOAIPartition::PushResultsList ( lua_State* L ) {
	USLuaState state ( L );

	u32 total = this->GetTotalResults ();
	total =  ( total > MAX_RESULTS ) ? MAX_RESULTS : total;
	
	lua_createtable ( state, total, 0 );
	
	u32 i = 1;
	while ( MOAIProp* prop = this->PopResult ()) {
		lua_pushnumber ( state, i++ );
		prop->PushLuaUserdata ( state );
		lua_settable ( state, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIPartition::PushSortedResultsList ( lua_State* L ) {
	USLuaState state ( L );

	u32 total = this->GetTotalResults ();
	total =  ( total > MAX_RESULTS ) ? MAX_RESULTS : total;
	
	lua_createtable ( state, total, 0 );
	
	// initialize the sort buffer
	USRadixKey16 < MOAIProp* > key [ MAX_RESULTS ];
	USRadixKey16 < MOAIProp* > swap [ MAX_RESULTS ];
	
	u32 i = 0;
	while ( MOAIProp* prop = this->PopResult ()) {
		s16 priority = ( s16 )prop->GetPriority ();
		key [ i ].mKey = ( s16 )(( priority ^ 0x80000000 ) | ( priority & 0x7fffffff ));
		key [ i ].mData = prop;
		++i;
	}

	// sort
	USRadixKey16 < MOAIProp* >* sort = RadixSort16 < MOAIProp* >( key, swap, total );
	
	for ( i = 0; i < total; ++i ) {
	
		MOAIProp* prop = sort [ i ].mData;
		lua_pushnumber ( state, i + 1 );
		prop->PushLuaUserdata ( state );
		lua_settable ( state, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaFuncs ( USLuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "insertProp",					_insertProp },
		{ "propForPoint",				_propForPoint },
		{ "propListForPoint",			_propListForPoint },
		{ "propListForRect",			_propListForRect },
		{ "removeProp",					_removeProp },
		{ "reserveLayers",				_reserveLayers },
		{ "setLayer",					_setLayer },
		{ "sortedPropListForPoint",		_sortedPropListForPoint },
		{ "sortedPropListForRect",		_sortedPropListForRect },
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
	prop.Release ();
}

//----------------------------------------------------------------//
void MOAIPartition::ReserveLayers ( int totalLayers ) {

	this->mLayers.Init ( totalLayers );
}

//----------------------------------------------------------------//
void MOAIPartition::ResetResults () {

	this->mTotalResults = 0;
	this->mResults = 0;
}

//----------------------------------------------------------------//
void MOAIPartition::SetLayer ( int layerID, float cellSize, int width, int height ) {

	this->mLayers [ layerID ].Init ( cellSize, width, height );
}

//----------------------------------------------------------------//
STLString MOAIPartition::ToString () {

	STLString repr( USLuaObject::ToString ());

	return repr;
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









