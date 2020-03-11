// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckProxy.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeckProxy::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckProxy, "U" )
	
	self->mDeck.Set ( *self, state.GetLuaObject < MOAIDeck >( 2, true ));
	return 0;
}

//================================================================//
// MOAIDeckProxy
//================================================================//

//----------------------------------------------------------------//
MOAIDeckProxy::MOAIDeckProxy () {
	
	RTTI_BEGIN ( MOAIDeckProxy )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIDeckProxy >)
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckProxy::~MOAIDeckProxy () {

	this->mDeck.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIDeckProxy::MOAIDeck_ComputeMaxAABB () {

	return this->mDeck ? this->mDeck->MOAIDeck_ComputeMaxAABB () : ZLBounds::ZL_BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAIDeckProxy::MOAIDeck_Draw ( ZLIndex idx ) {

	if ( this->mDeck ) {
		this->mDeck->MOAIDeck_Draw ( this->MOAIDeckProxy_Remap ( idx ));
	}
}

//----------------------------------------------------------------//
ZLBounds MOAIDeckProxy::MOAIDeck_GetBounds ( ZLIndex idx ) {

	return this->mDeck ? this->mDeck->MOAIDeck_GetBounds ( this->MOAIDeckProxy_Remap ( idx )) : ZLBounds::ZL_BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeckProxy::MOAIDeck_GetCollisionShape ( ZLIndex idx ) {

	return this->mDeck ? this->mDeck->MOAIDeck_GetCollisionShape ( this->MOAIDeckProxy_Remap ( idx )) : 0;
}

//----------------------------------------------------------------//
bool MOAIDeckProxy::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {

	return this->mDeck ? this->mDeck->MOAIDeck_Overlap ( this->MOAIDeckProxy_Remap ( idx ), vec, granularity, result ) : false;
}

//----------------------------------------------------------------//
bool MOAIDeckProxy::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {

	return this->mDeck ? this->mDeck->MOAIDeck_Overlap ( this->MOAIDeckProxy_Remap ( idx ), vec, granularity, result ) : false;
}

//----------------------------------------------------------------//
ZLIndex MOAIDeckProxy::MOAIDeckProxy_Remap ( ZLIndex idx ) {

	return idx;
}

//----------------------------------------------------------------//
void MOAIDeckProxy::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDeckProxy::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setDeck",				_setDeck },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
