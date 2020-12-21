// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckProxy.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeckProxy::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckProxy, "U" )
	
	self->mDeck.Set ( *self, state.GetLuaObject < MOAIDeck >( 2, true ));
	MOAI_LUA_RETURN_SELF
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
void MOAIDeckProxy::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDeckProxy::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setDeck",				_setDeck },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLBounds MOAIDeckProxy::MOAIDeck_GetBounds () {

	return this->mDeck ? this->mDeck->MOAIDeck_GetBounds () : ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
ZLBounds MOAIDeckProxy::MOAIDeck_GetBounds ( ZLIndex idx ) {

	return this->mDeck ? this->mDeck->MOAIDeck_GetBounds ( this->MOAIDeckProxy_Remap ( idx )) : ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeckProxy::MOAIDeck_GetCollisionShape ( ZLIndex idx ) {

	return this->mDeck ? this->mDeck->MOAIDeck_GetCollisionShape ( this->MOAIDeckProxy_Remap ( idx )) : 0;
}

//----------------------------------------------------------------//
MOAIPickResult MOAIDeckProxy::MOAIDeck_PickByPoint ( ZLIndex idx, ZLVec3D loc ) {

	return this->mDeck ? this->mDeck->MOAIDeck_PickByPoint ( this->MOAIDeckProxy_Remap ( idx ), loc ) : false;
}

//----------------------------------------------------------------//
MOAIPickResult MOAIDeckProxy::MOAIDeck_PickByRay ( ZLIndex idx, ZLVec3D loc, ZLVec3D normal ) {

	return this->mDeck ? this->mDeck->MOAIDeck_PickByRay ( this->MOAIDeckProxy_Remap ( idx ), loc, normal ) : false;
}

//----------------------------------------------------------------//
ZLIndex MOAIDeckProxy::MOAIDeckProxy_Remap ( ZLIndex idx ) {

	return idx;
}

//----------------------------------------------------------------//
void MOAIDeckProxy::MOAIDeck_Render ( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase ) {

	if ( this->mDeck ) {
		this->mDeck->MOAIDeck_Render ( this->MOAIDeckProxy_Remap ( idx ), renderPhase );
	}
}
