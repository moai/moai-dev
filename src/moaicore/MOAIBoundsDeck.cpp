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
// TODO: doxygen
int	MOAIBoundsDeck::_computeMaxBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBoundsDeck, "U" )
	
	self->ComputeMaxBounds ();
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
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
// TODO: doxygen
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
// TODO: doxygen
int	MOAIBoundsDeck::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBoundsDeck, "UN" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	if ( idx < self->mBoundsArray.Size ()) {
		USBox bounds = state.GetBox ( 3 );
		bounds.Bless ();
		self->mBoundsArray [ idx ] = bounds;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
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
void MOAIBoundsDeck::ComputeMaxBounds () {

	u32 size = this->mBoundsArray.Size ();
	if ( size == 0 ) {
		this->mMaxBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	}
	else {
	
		this->mMaxBounds = this->mBoundsArray [ 0 ];
		for ( u32 i = 1; i < size; ++i ) {
			this->mMaxBounds.Grow ( this->mBoundsArray [ i ]);
		}
	}
}

//----------------------------------------------------------------//
USBox MOAIBoundsDeck::GetBounds () {

	return this->mMaxBounds;
}

//----------------------------------------------------------------//
USBox MOAIBoundsDeck::GetBounds ( u32 idx ) {

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
		{ "computeMaxBounds",		_computeMaxBounds },
		{ "reserveBounds",			_reserveBounds },
		{ "reserveIndices",			_reserveIndices },
		{ "setBounds",				_setBounds },
		{ "setIndex",				_setIndex },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
