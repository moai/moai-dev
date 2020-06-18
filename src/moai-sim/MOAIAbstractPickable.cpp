// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIAbstractPickable.h>

//================================================================//
// MOAIPickResult
//================================================================//

//----------------------------------------------------------------//
MOAIPickResult::MOAIPickResult () :
	mPicked ( false ) {
}

//----------------------------------------------------------------//
MOAIPickResult::MOAIPickResult ( bool picked ) :
	mPicked ( picked ),
	mTime ( 0.0 ),
	mLoc ( ZLVec3D::ORIGIN ),
	mNormal ( ZLVec3D::ORIGIN ) {
}

//----------------------------------------------------------------//
MOAIPickResult::MOAIPickResult ( bool picked, ZLReal time, ZLVec3D loc, ZLVec3D normal ) :
	mPicked ( picked ),
	mTime ( time ),
	mLoc ( loc ),
	mNormal ( normal ) {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractPickable::_pickByPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractPickable, "U" )
	
	ZLVec3D loc = state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );
	
	MOAIPickResult result = self->PickByPoint ( loc );
	state.Push ( result.mPicked );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAbstractPickable::_pickByRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractPickable, "U" )

	ZLVec3D loc = state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );
	ZLVec3D vec = state.GetValue < ZLVec3D >( 5, ZLVec3D::Z_AXIS );
	
	MOAIPickResult result = self->PickByRay ( loc, vec );
	state.Push ( result.mPicked );
	return 1;
}

//================================================================//
// MOAIGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractPickable::MOAIAbstractPickable () {

	RTTI_BEGIN ( MOAILayoutFrame )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractPickable >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractPickable::~MOAIAbstractPickable () {
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPickable::PickByPoint ( ZLVec3D loc ) {

	return this->MOAIAbstractPickable_PickByPoint ( loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPickable::PickByPointHelper ( ZLBounds bounds, ZLVec3D loc) {

	if ( bounds.IsEmpty ()) return MOAIPickResult ( false );
	if ( bounds.IsGlobal ()) return MOAIPickResult ( true );
	
	bounds.mAABB.Bless ();
	return MOAIPickResult ( bounds.mAABB.Contains ( loc ));
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPickable::PickByRay ( ZLVec3D loc, ZLVec3D normal ) {

	return this->MOAIAbstractPickable_PickByRay ( loc, normal );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPickable::PickByRayHelper ( ZLBounds bounds, ZLVec3D loc, ZLVec3D normal ) {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractPickable::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractPickable::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "pickByPoint",		_pickByPoint },
		{ "pickByRay",		_pickByRay },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
