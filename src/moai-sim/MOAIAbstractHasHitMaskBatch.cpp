// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIAbstractHasHitMask.h>
#include <moai-sim/MOAIAbstractHasHitMaskBatch.h>
#include <moai-sim/MOAIAbstractHitMaskBatch.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractHasHitMaskBatch::_getHitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasHitMaskBatch, "U" )

	ZLIndex index = state.GetValue < MOAILuaIndex >( 2, 0 );
	state.Push ( self->GetHitMask ( index ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasHitMaskBatch::_hitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasHitMaskBatch, "U" );
	state.Push ( &self->AffirmHitMask ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
int MOAIAbstractHasHitMaskBatch::_reserveHitMasks ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasHitMaskBatch, "U" )

	ZLIndex size = state.GetValue < MOAILuaSize >( 2, 0 );
	self->ReserveHitMasks ( size );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractHasHitMaskBatch::_setHitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasHitMaskBatch, "U" )

	ZLIndex index			= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIHitMask* hitMask	= state.GetLuaObject < MOAIHitMask >( 3, false );

	self->SetHitMask ( index, hitMask );
	return 0;
}

//================================================================//
// MOAIAbstractHasHitMaskBatch
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractHitMask& MOAIAbstractHasHitMaskBatch::AffirmHitMask ( ZLIndex index ) {

	return this->AffirmHitMaskBatch ().MOAIAbstractHitMaskBatch_AffirmHitMask ( index );
}

//----------------------------------------------------------------//
MOAIAbstractHitMaskBatch& MOAIAbstractHasHitMaskBatch::AffirmHitMaskBatch () {

	return this->MOAIAbstractHasHitMaskBatch_AffirmHitMaskBatch ();
}

//----------------------------------------------------------------//
MOAIAbstractHitMask* MOAIAbstractHasHitMaskBatch::GetHitMask ( ZLIndex index ) {

	return this->AffirmHitMaskBatch ().MOAIAbstractHitMaskBatch_GetHitMask ( index );
}

//----------------------------------------------------------------//
MOAIAbstractHasHitMaskBatch::MOAIAbstractHasHitMaskBatch () {

	RTTI_BEGIN ( MOAIAbstractHasHitMaskBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractHasHitMaskBatch >)
		RTTI_EXTEND ( MOAIAbstractHasHitMask )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractHasHitMaskBatch::~MOAIAbstractHasHitMaskBatch () {
}

//----------------------------------------------------------------//
void MOAIAbstractHasHitMaskBatch::ReserveHitMasks ( ZLSize size ) {

	this->AffirmHitMaskBatch ().MOAIAbstractHitMaskBatch_ReserveHitMasks ( size );
}

//----------------------------------------------------------------//
void MOAIAbstractHasHitMaskBatch::SetHitMask ( ZLIndex index, MOAIAbstractHitMask* hitMask ) {

	this->AffirmHitMaskBatch ().MOAIAbstractHitMaskBatch_SetHitMask ( index, hitMask );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractHasHitMaskBatch::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractHasHitMaskBatch::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getHitMask",				_getHitMask },
		{ "hitMask",				_hitMask },
		{ "reserveHitMasks",		_reserveHitMasks },
		{ "setHitMask",				_setHitMask },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIAbstractHitMask& MOAIAbstractHasHitMaskBatch::MOAIAbstractHasHitMask_AffirmHitMask () {

	return this->AffirmHitMask ( 0 );
}
