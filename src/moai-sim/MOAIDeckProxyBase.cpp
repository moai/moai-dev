// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckProxyBase.h>
#include <moai-sim/MOAIGfxMgr.h>
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
int MOAIDeckProxyBase::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckProxyBase, "U" )
	
	self->mDeck.Set ( *self, state.GetLuaObject < MOAIDeck >( 2, true ));
	return 0;
}

//================================================================//
// MOAIDeckProxyBase
//================================================================//

//----------------------------------------------------------------//
MOAIDeckProxyBase::MOAIDeckProxyBase () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckProxyBase::~MOAIDeckProxyBase () {

	this->mDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeckProxyBase::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeckProxyBase::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setDeck",				_setDeck },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeckProxyBase::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIDeck::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIDeckProxyBase::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIDeck::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeckProxyBase::MOAIDeck_GetCollisionShape ( u32 idx ) {

	if ( this->mDeck ) {
		return this->mDeck->GetCollisionShape ( idx );
	}
	return 0;
}
