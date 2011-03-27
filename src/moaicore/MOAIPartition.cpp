// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLayer.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>insertPrim ( self, prop )</tt>\n
\n
	Adds a primitive to the parition.
	@param self (in)
	@param prop (in)
*/
int MOAIPartition::_insertPrim ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UU" )

	MOAIProp* prop = state.GetLuaData < MOAIProp >( 2 );
	if ( !prop ) return 0;

	self->InsertProp ( *prop );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( prop ) primForPoint ( self, x, y )</tt>\n
\n
	Returns the hightest priority primitive (if any) under a given point.
	@param self (in)
	@param x (in)
	@param y (in)
	@param prop (out) The primitive, or 'nil' if none found.
*/
int MOAIPartition::_primForPoint ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UNN" )

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
		prop->PushLuaInstance ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( prims ) primListForPoint ( self, x, y )</tt>\n
\n
	Returns all primitives under a given point.
	@param self (in)
	@param x (in)
	@param y (in)
	@param prims (out) Found primitives, pushed onto stack. (TODO: change to list)
*/
int MOAIPartition::_primListForPoint ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UNN" )

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
/**	@brief <tt>( prims ) primListForRect ( self, xMin, yMin, xMax, yMax )</tt>\n
\n
	Returns all primitives under a given rectangle.
	@param self (in)
	@param xMin (in)
	@param yMin (in)
	@param xMax (in)
	@param yMax (in)
	@param prims (out) Found primitives, pushed onto stack. (TODO: change to list)
*/
int MOAIPartition::_primListForRect ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UNNNN" )

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
/**	@brief <tt>removePrim ( self, prop )</tt>\n
\n
	Removes a primitive from the partition.
	@param self (in)
	@param prop (in)
*/
int MOAIPartition::_removePrim ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UU" )

	MOAIProp* prop = state.GetLuaData < MOAIProp >( 2 );
	if ( !prop ) return 0;

	self->RemoveProp ( *prop );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>reserveLayers ( self, total )</tt>\n
\n
	Reserves a stack of layers in the partition. Layers must be initialize with setLayer ().
	@param self (in)
	@param total (in)
*/
int MOAIPartition::_reserveLayers ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UN" )

	u32 totalLayers = state.GetValue < u32 >( 2, 0 );
	
	self->ReserveLayers ( totalLayers );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setLayer ( self, layerID, cellSize, xCells, yCells )</tt>\n
\n
	Initializes a layer previously created by reserveLayers ().
	@param self (in)
	@param layerID (in)
	@param cellSize (in) Dimentions of cell.
	@param xCells (in) Width of layer in cells.
	@param yCells (in) Height of layer in cells.
*/
int MOAIPartition::_setLayer ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UNNNN" )

	u32 layerID		= state.GetValue < u32 >( 2, 1 ) - 1;
	float cellSize	= state.GetValue < float >( 3, 1.0f );
	u32 width		= state.GetValue < u32 >( 4, 0 );
	u32 height		= state.GetValue < u32 >( 5, 0 );

	self->SetLayer ( layerID, cellSize, width, height );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( prims ) sortedPrimListForPoint ( self, x, y )</tt>\n
\n
	Returns all primitives under a given point sorted by priority.
	@param self (in)
	@param x (in)
	@param y (in)
	@param prims (out) Found primitives, pushed onto stack. (TODO: change to list)
*/
int MOAIPartition::_sortedPrimListForPoint ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UNN" )

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
/**	@brief <tt>( prims ) sortedPrimListForRect ( self, xMin, yMin, xMax, yMax )</tt>\n
\n
	Returns all primitives under a given rectangle sorted by priority.
	@param self (in)
	@param xMin (in)
	@param yMin (in)
	@param xMax (in)
	@param yMax (in)
	@param prims (out) Found primitives, pushed onto stack. (TODO: change to list)
*/
int MOAIPartition::_sortedPrimListForRect ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UNNNN" )

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
	
	this->mEmpties.InsertProp ( prop );
	
	prop.mPartition = this;
	prop.ScheduleUpdate ();
}

//----------------------------------------------------------------//
MOAIPartition::MOAIPartition () :
	mTotalResults ( 0 ),
	mResults ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaData )
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
		prop->PushLuaInstance ( state );
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
	
	for ( u32 i = 0; i < total; ++i ) {
	
		MOAIProp* prop = sort [ i ].mData;
		lua_pushnumber ( state, i + 1 );
		prop->PushLuaInstance ( state );
		lua_settable ( state, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPartition::RegisterLuaFuncs ( USLuaState& state ) {
	
	LuaReg regTable [] = {
		{ "insertPrim",					_insertPrim },
		{ "primForPoint",				_primForPoint },
		{ "primListForPoint",			_primListForPoint },
		{ "primListForRect",			_primListForRect },
		{ "removePrim",					_removePrim },
		{ "reserveLayers",				_reserveLayers },
		{ "setLayer",					_setLayer },
		{ "sortedPrimListForPoint",		_sortedPrimListForPoint },
		{ "sortedPrimListForRect",		_sortedPrimListForRect },
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

	STLString repr( USLuaData::ToString ());

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









