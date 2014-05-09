// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAICollisionProp.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionProp::_setOverlapFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionProp, "U" )
	
	self->mOverlapFlags = state.GetValue < u32 >( 2, 0 );
	
	return 0;
}

//================================================================//
// MOAICollisionProp
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionProp::ClearOverlapLink ( MOAICollisionProp& other ) {

	MOAIPropOverlapLink* cursor = this->mOverlapLinks;
	this->mOverlapLinks = 0;
	
	while ( cursor ) {
		MOAIPropOverlapLink* overlapLink = cursor;
		cursor = cursor->mNext;
		
		if ( overlapLink->mOther != &other ) {
			overlapLink->mNext = this->mOverlapLinks;
			this->mOverlapLinks = overlapLink;
		}
	}
}

//----------------------------------------------------------------//
MOAICollisionProp* MOAICollisionProp::GetCollisionProp () {

	return this;
}

//----------------------------------------------------------------//
MOAICollisionProp::MOAICollisionProp () :
	mOverlapFlags ( DEFAULT_OVERLAP_FLAGS ),
	mOverlapPass ( 0 ),
	mOverlapLinks ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
	RTTI_END
	
	this->mActiveListLink.Data ( this );
	this->SetMask ( MOAIProp::CAN_OVERLAP );
}

//----------------------------------------------------------------//
MOAICollisionProp::~MOAICollisionProp () {
}

//----------------------------------------------------------------//
bool MOAICollisionProp::RefineOverlap ( const MOAICollisionProp& other, MOAIOverlapInfo& info ) const {

	// TODO: actually pay attention to OVERLAP_GRANULARITY_FINE and OVERLAP_CALCULATE_BOUNDS
	info.mHasBounds = false;
	
	// yeah, refining not so much...
	return true;
}

//----------------------------------------------------------------//
void MOAICollisionProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaClass ( state );
	
	state.SetField ( -1, "OVERLAP_EVENTS_ON_UPDATE",		( u32 )OVERLAP_EVENTS_ON_UPDATE );
	state.SetField ( -1, "OVERLAP_EVENTS_LIFECYCLE",		( u32 )OVERLAP_EVENTS_LIFECYCLE );
	state.SetField ( -1, "OVERLAP_GRANULARITY_FINE",		( u32 )OVERLAP_GRANULARITY_FINE );
	state.SetField ( -1, "OVERLAP_CALCULATE_BOUNDS",		( u32 )OVERLAP_CALCULATE_BOUNDS );
}

//----------------------------------------------------------------//
void MOAICollisionProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setOverlapFlags",	_setOverlapFlags },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICollisionProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIProp::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAICollisionProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIProp::SerializeOut ( state, serializer );
}
