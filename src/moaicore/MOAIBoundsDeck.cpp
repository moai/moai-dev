// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIBoundsDeck.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserveBounds
	@text	Reserve an array of bounds to be indexed.
	
	@in		MOAIBoundsDeck self
	@in		number nBounds
	@out	nil
*/
int	MOAIBoundsDeck::_reserveBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBoundsDeck, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mBoundsArray.Init ( total );
	
	for ( u32 i = 0; i < total; ++i ) {
		USBox& bounds = self->mBoundsArray [ i ];
		bounds.Init ( -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveIndices
	@text	Reserve indices. Each index maps a deck item onto a
			bounding box.
	
	@in		MOAIBoundsDeck self
	@in		number nIndices
	@out	nil
*/
int	MOAIBoundsDeck::_reserveIndices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBoundsDeck, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mIndexMap.Init ( total );
	
	for ( u32 i = 0; i < total; ++i ) {
		self->mIndexMap [ i ] = i;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setBounds
	@text	Set the dimensions of a bounding box at a given index.
	
	@in		MOAIBoundsDeck self
	@in		number idx
	@in		number xMin
	@in		number yMin
	@in		number zMin
	@in		number xMax
	@in		number yMax
	@in		number zMax
	@out	nil
*/
int	MOAIBoundsDeck::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBoundsDeck, "UN" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	if ( idx < self->mBoundsArray.Size ()) {
		USBox bounds = state.GetBox ( 3 );
		bounds.Bless ();
		self->mBoundsArray [ idx ] = bounds;
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setIndex
	@text	Associate a deck index with a bounding box.
	
	@in		MOAIBoundsDeck self
	@in		number idx
	@in		number boundsID
	@out	nil
*/
int	MOAIBoundsDeck::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBoundsDeck, "UN" )
	
	u32 idx			= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 boundsID	= state.GetValue < u32 >( 3, 1 ) - 1;
	
	if ( idx < self->mIndexMap.Size ()) {
		self->mIndexMap [ idx ] = boundsID;
	}
	return 0;
}

//================================================================//
// MOAIBoundsDeck
//================================================================//

//----------------------------------------------------------------//
USBox MOAIBoundsDeck::ComputeMaxBounds () {

	USBox bounds;

	u32 size = this->mBoundsArray.Size ();
	if ( size == 0 ) {
		bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	}
	else {
		this->mMaxBounds = this->mBoundsArray [ 0 ];
		for ( u32 i = 1; i < size; ++i ) {
			bounds.Grow ( this->mBoundsArray [ i ]);
		}
	}
	return bounds;
}

//----------------------------------------------------------------//
USBox MOAIBoundsDeck::GetItemBounds ( u32 idx ) {

	idx = ( idx - 1 ) % this->mIndexMap.Size ();
	idx = this->mIndexMap [ idx ] % this->mBoundsArray.Size ();
	return this->mBoundsArray [ idx ];
}

//----------------------------------------------------------------//
MOAIBoundsDeck::MOAIBoundsDeck () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	//this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIBoundsDeck::~MOAIBoundsDeck () {
}

//----------------------------------------------------------------//
void MOAIBoundsDeck::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBoundsDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserveBounds",			_reserveBounds },
		{ "reserveIndices",			_reserveIndices },
		{ "setBounds",				_setBounds },
		{ "setIndex",				_setIndex },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
