// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIProp2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>insertPrim ( self, prim )</tt>\n
\n
	Adds a primitive to the parition.
	@param self (in)
	@param prim (in)
*/
int MOAIPartition::_insertPrim ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UU" )

	MOAIProp2D* prim = state.GetLuaData < MOAIProp2D >( 2 );
	if ( !prim ) return 0;

	self->InsertPrim ( *prim );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( prim ) primForPoint ( self, x, y )</tt>\n
\n
	Returns the hightest priority primitive (if any) under a given point.
	@param self (in)
	@param x (in)
	@param y (in)
	@param prim (out) The primitive, or 'nil' if none found.
*/
int MOAIPartition::_primForPoint ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UNN" )

	USVec2D vec;
	vec.mX = state.GetValue < float >( 2, 0.0f );
	vec.mY = state.GetValue < float >( 3, 0.0f );

	u32 total = self->GatherPrims ( vec, 0 );
	if ( total ) {
	
		MOAIProp2D* prim = self->GetResult ( 0 );
		for ( u32 i = 1; i < total; ++i ) {
			MOAIProp2D* test = self->GetResult ( i );
			if ( test->GetPriority () > prim->GetPriority ()) {
				prim = test;
			}
		}
	
		prim->PushLuaInstance ( state );
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

	u32 total = self->GatherPrims ( vec, 0 );
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

	u32 total = self->GatherPrims ( rect, 0 );
	if ( total ) {
		self->PushResultsList ( L );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>removePrim ( self, prim )</tt>\n
\n
	Removes a primitive from the partition.
	@param self (in)
	@param prim (in)
*/
int MOAIPartition::_removePrim ( lua_State* L ) {
	LUA_SETUP ( MOAIPartition, "UU" )

	MOAIProp2D* prim = state.GetLuaData < MOAIProp2D >( 2 );
	if ( !prim ) return 0;

	self->RemovePrim ( *prim );

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
	
	u32 total = self->GatherPrims ( vec, 0 );
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
	
	u32 total = self->GatherPrims ( rect, 0 );
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

	this->GatherPrims ( 0, 0 );
	u32 total = this->GetTotalResults ();

	for ( u32 i = 0; i < total; ++i ) {
		MOAIProp2D* prim = this->GetResult ( i );
		this->RemovePrim ( *prim );
	}
}

//----------------------------------------------------------------//
void MOAIPartition::InsertPrim ( MOAIProp2D& prim ) {

	USPartition < MOAIProp2D >* partition = prim.GetPartition ();
	if ( partition != this ) {
	
		if ( partition ) {
			partition->RemovePrim ( prim );
		}
		else {
			prim.Retain ();
		}
		
		this->USPartition < MOAIProp2D >::InsertPrim ( prim );
		prim.ScheduleUpdate ();
	}
}

//----------------------------------------------------------------//
MOAIPartition::MOAIPartition () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaData )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPartition::~MOAIPartition () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIPartition::PushResultsList ( lua_State* L ) {
	USLuaState state ( L );

	u32 total = this->GetTotalResults ();
	total =  ( total > MAX_RESULTS ) ? MAX_RESULTS : total;
	
	lua_createtable ( state, total, 0 );
	
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIProp2D* prim = this->GetResult ( i );
		lua_pushnumber ( state, i + 1 );
		prim->PushLuaInstance ( state );
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
	USRadixKey16 < MOAIProp2D* > key [ MAX_RESULTS ];
	USRadixKey16 < MOAIProp2D* > swap [ MAX_RESULTS ];
	
	for ( u32 i = 0; i < total; ++i ) {
	
		MOAIProp2D* prim = this->GetResult ( i );
		s16 priority = ( s16 )prim->GetPriority ();
		key [ i ].mKey = ( s16 )(( priority ^ 0x80000000 ) | ( priority & 0x7fffffff ));
		key [ i ].mData = prim;
	}

	// sort
	USRadixKey16 < MOAIProp2D* >* sort = RadixSort16 < MOAIProp2D* >( key, swap, total );
	
	for ( u32 i = 0; i < total; ++i ) {
	
		MOAIProp2D* prim = sort [ i ].mData;
		lua_pushnumber ( state, i + 1 );
		prim->PushLuaInstance ( state );
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
void MOAIPartition::RemovePrim ( MOAIProp2D& prim ) {

	USPartition < MOAIProp2D >* partition = prim.GetPartition ();
	if ( partition == this ) {

		this->USPartition < MOAIProp2D >::RemovePrim ( prim );
		prim.Release ();
	}
}

//----------------------------------------------------------------//
STLString MOAIPartition::ToString () {

	STLString repr( USLuaData::ToString ());

	return repr;
}
