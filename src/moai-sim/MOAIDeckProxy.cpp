// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIBoundsDeck.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckProxy.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// local
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
ZLBox MOAIDeckProxy::ComputeMaxBounds () {

	assert ( this->mDeck );
	return this->mDeck->ComputeMaxBounds ();
}

//----------------------------------------------------------------//
bool MOAIDeckProxy::Contains ( u32 idx, const ZLVec2D& vec ) {
	
	return this->mDeck ? this->mDeck->Contains ( idx, vec ) : false;
}

//----------------------------------------------------------------//
void MOAIDeckProxy::DrawIndex ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {

	if ( this->mDeck ) {
		this->DrawIndex ( idx, materials, offset, scale );
	}
}

//----------------------------------------------------------------//
ZLBox MOAIDeckProxy::GetBounds () {

	assert ( this->mDeck );
	return this->mDeck->GetBounds ();
}

//----------------------------------------------------------------//
ZLBox MOAIDeckProxy::GetBounds ( u32 idx ) {

	assert ( this->mDeck );
	return this->mDeck->GetBounds ( idx );
}

//----------------------------------------------------------------//
void MOAIDeckProxy::GetCollisionShape ( MOAICollisionShape& shape ) {

	if ( this->mDeck ) {
		this->GetCollisionShape ( shape );
	}
}

//----------------------------------------------------------------//
ZLBox MOAIDeckProxy::GetItemBounds ( u32 idx ) {

	assert ( this->mDeck );
	return this->mDeck->GetItemBounds ( idx );
}

//----------------------------------------------------------------//
bool MOAIDeckProxy::Inside ( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad ) {

	return this->mDeck ? this->mDeck->Inside ( idx, materials, granularity, vec, pad ) : false;
}

//----------------------------------------------------------------//
MOAIDeckProxy::MOAIDeckProxy () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckProxy::~MOAIDeckProxy () {

	this->mDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeckProxy::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeckProxy::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setDeck",				_setDeck },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIMaterialBatch& MOAIDeckProxy::ResolveMaterialBatch ( MOAIMaterialBatch* override ) {

	assert ( this->mDeck );
	return this->mDeck->ResolveMaterialBatch ( override );
}
