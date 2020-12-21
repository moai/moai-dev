// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractPickable.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeck::_draw ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	ZLIndex index = state.GetValue < u32 >( 2, 0 );
	MOAIRenderPhaseEnum::_ renderPhase = state.GetEnum ( 2, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );

	self->Render ( index, renderPhase );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getBounds
	@text	Return bounds for an item or the maximum bounds for the
			deck.

	@overload

		@in		number idx
		@out	xMin
		@out	yMin
		@out	zMin
		@out	xMax
		@out	yMax
		@out	zMax
	
	@overload
		@out	xMin
		@out	yMin
		@out	zMin
		@out	xMax
		@out	yMax
		@out	zMax
*/
int MOAIDeck::_getBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )
	
	ZLBox box;
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
	
		ZLIndex idx = state.GetValue < MOAILuaIndex >( 2, 0 );
		box = self->GetBounds ( idx ).mAABB;
	}
	else {
		box = self->GetBounds ().mAABB;
	}
	
	state.Push ( box );
	return 6;
}

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIDeck::GetBounds () {

	return this->GetBounds ( 0 );
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::GetBounds ( ZLIndex idx ) {

	return this->MOAIDeck_GetBounds ( idx );
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeck::GetCollisionShape ( ZLIndex idx ) {

	return MOAIDeck_GetCollisionShape ( idx );
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () {
	
	RTTI_BEGIN ( MOAIDeck )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIDeck >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeck::~MOAIDeck () {
}

//----------------------------------------------------------------//
MOAIPickResult MOAIDeck::PickByPoint ( ZLIndex idx, ZLVec3D loc ) {

	return this->MOAIDeck_PickByPoint ( idx, loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIDeck::PickByRay ( ZLIndex idx, ZLVec3D loc, ZLVec3D normal ) {

	return this->MOAIDeck_PickByRay ( idx, loc, normal );
}

//----------------------------------------------------------------//
void MOAIDeck::Render ( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase ) {

	this->MOAIDeck_Render ( idx, renderPhase );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDeck::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDeck::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "draw",					_draw },
		{ "getBounds",				_getBounds },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::MOAIDeck_GetBounds () {
	return ZLBounds::GLOBAL;
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::MOAIDeck_GetBounds ( ZLIndex idx ) {
	UNUSED ( idx );

	return ZLBounds::GLOBAL;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeck::MOAIDeck_GetCollisionShape ( ZLIndex idx ) {
	UNUSED ( idx );

	return 0;
}

//----------------------------------------------------------------//
MOAIPickResult MOAIDeck::MOAIDeck_PickByPoint ( ZLIndex idx, ZLVec3D loc ) {

	return MOAIAbstractPickable::PickByPointHelper ( this->GetBounds ( idx ), loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIDeck::MOAIDeck_PickByRay ( ZLIndex idx, ZLVec3D loc, ZLVec3D normal ) {
	
	return MOAIAbstractPickable::PickByRayHelper ( this->GetBounds ( idx ), loc, normal );
}

////----------------------------------------------------------------//
//bool MOAIDeck::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {
//	UNUSED ( result );
//	UNUSED ( granularity );
//
//	ZLBounds bounds = this->GetBounds ( idx );
//	return (( bounds.HasGeometry ()) && bounds.mAABB.Contains ( ZLVec3D ( vec.mY, vec.mY, 0.0f ), ZLBox::PLANE_XY ));
//}
//
////----------------------------------------------------------------//
//bool MOAIDeck::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {
//	UNUSED ( result );
//	UNUSED ( granularity );
//
//	ZLBounds bounds = this->GetBounds ( idx );
//	return (( bounds.HasGeometry ()) && bounds.mAABB.Contains ( vec ));
//}

//----------------------------------------------------------------//
void MOAIDeck::MOAIDeck_Render ( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase ) {
	UNUSED ( idx );
}
