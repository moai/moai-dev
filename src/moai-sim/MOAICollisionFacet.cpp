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
#include <moai-sim/MOAICollisionFacet.h>
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
int MOAICollisionFacet::_setOverlapFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionFacet, "U" )
	
	self->mOverlapFlags = state.GetValue < u32 >( 2, 0 );
	
	return 0;
}

//================================================================//
// MOAICollisionFacet
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionFacet::ClearOverlapLink ( MOAICollisionFacet& other ) {

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
MOAICollisionFacet* MOAICollisionFacet::GetCollisionProp () {

	return this;
}

//----------------------------------------------------------------//
MOAICollisionFacet::MOAICollisionFacet () :
	mOverlapFlags ( DEFAULT_OVERLAP_FLAGS ),
	mOverlapPass ( 0 ),
	mOverlapLinks ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFacet )
	RTTI_END
	
	this->mActiveListLink.Data ( this );
}

//----------------------------------------------------------------//
MOAICollisionFacet::~MOAICollisionFacet () {
}

//----------------------------------------------------------------//
void MOAICollisionFacet::OnAttach ( MOAIProp& prop ) {

	prop.SetMask ( prop.GetMask () | MOAIProp::CAN_OVERLAP );
}

//----------------------------------------------------------------//
void MOAICollisionFacet::OnDetach ( MOAIProp& prop ) {

	prop.SetMask ( prop.GetMask () & ~MOAIProp::CAN_OVERLAP );
}

//----------------------------------------------------------------//
bool MOAICollisionFacet::RefineOverlap ( const MOAICollisionFacet& other, MOAIOverlapInfo& info ) const {

	// TODO: actually pay attention to OVERLAP_GRANULARITY_FINE and OVERLAP_CALCULATE_BOUNDS
	info.mHasBounds = false;
	
	// yeah, refining not so much...
	return true;
}

//----------------------------------------------------------------//
void MOAICollisionFacet::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIFacet::RegisterLuaClass ( state );
	
	state.SetField ( -1, "OVERLAP_EVENTS_ON_UPDATE",		( u32 )OVERLAP_EVENTS_ON_UPDATE );
	state.SetField ( -1, "OVERLAP_EVENTS_LIFECYCLE",		( u32 )OVERLAP_EVENTS_LIFECYCLE );
	state.SetField ( -1, "OVERLAP_GRANULARITY_FINE",		( u32 )OVERLAP_GRANULARITY_FINE );
	state.SetField ( -1, "OVERLAP_CALCULATE_BOUNDS",		( u32 )OVERLAP_CALCULATE_BOUNDS );
}

//----------------------------------------------------------------//
void MOAICollisionFacet::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIFacet::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setOverlapFlags",	_setOverlapFlags },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICollisionFacet::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIFacet::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAICollisionFacet::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIFacet::SerializeOut ( state, serializer );
}
