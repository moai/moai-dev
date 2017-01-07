// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAICollisionWorld.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxMgr.h>
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
int MOAICollisionProp::_getOverlaps ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionProp, "U" )

	u32 total = 0;
	MOAIPropOverlapLink* link = self->mOverlapLinks;
	for ( ; link; link = link->mNext, ++total ) {
		state.Push ( link->mOther );
	}
	return total;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionProp::_hasOverlaps ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionProp, "U" )

	return self->mOverlapLinks != 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionProp::_setGroupMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionProp, "U" )
	
	self->mGroupMask = state.GetValue < u32 >( 2, 0 );
	return 0;
}

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
void MOAICollisionProp::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();

	if ( debugLines.Bind ( MOAIDebugLines::PROP_WORLD_BOUNDS )) {
		gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::VIEW_PROJ_MTX ));
		draw.DrawBoxOutline ( this->GetBounds ());
	}

	bool visible = false;
	
	if ( this->IsActive ()) {
		
		if ( this->mOverlapLinks ) {
			visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS );
		}
		
		if ( this->mTouched == this->mOverlapPass && !visible ) {
			visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS );
		}
		
		if ( !visible ) {
			visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_PROP_BOUNDS );
		}
	}
	
	if ( this->mOverlapLinks && !visible ) {
		visible = debugLines.Bind ( MOAIDebugLines::COLLISION_OVERLAP_PROP_BOUNDS );
	}
	
	if ( visible ) {
		
		MOAICollisionShape* shape = this->GetCollisionShape ();
		
		if ( shape ) {
			const ZLAffine3D& localToWorldMtx = this->GetLocalToWorldMtx ();
			gfxMgr.mGfxState.SetMtx ( MOAIGfxGlobalsCache::WORLD_MTX, localToWorldMtx );
			shape->Draw ( localToWorldMtx );
		}
		else {
			gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::VIEW_PROJ_MTX ));
			draw.DrawBoxOutline ( this->GetBounds ());
		}
	}
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAICollisionProp::GetCollisionShape () {

	return this->mDeck ? this->mDeck->GetCollisionShape ( this->mIndex - 1 ) : 0;
}

//----------------------------------------------------------------//
bool MOAICollisionProp::IsActive () {

	return this->mActiveListLink.List () != 0;
}

//----------------------------------------------------------------//
MOAICollisionProp::MOAICollisionProp () :
	mGroupMask ( GROUP_MASK_ALL ),
	mOverlapFlags ( DEFAULT_OVERLAP_FLAGS ),
	mOverlapPass ( MOAICollisionWorld::OVERLAP_PASS_INIT ),
	mOverlapLinks ( 0 ),
	mInDrawList ( false ),
	mNextInDrawList ( 0 ),
	mStayActive ( false ),
	mTouched ( MOAICollisionWorld::OVERLAP_PASS_INIT ),
	mCollisionWorld ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPartitionHull )
		RTTI_EXTEND ( MOAIIndexedPropBase )
	RTTI_END
	
	this->mActiveListLink.Data ( this );
}

//----------------------------------------------------------------//
MOAICollisionProp::~MOAICollisionProp () {
}

//----------------------------------------------------------------//
bool MOAICollisionProp::RefineOverlap ( MOAICollisionProp& other, MOAIOverlapInfo& info ) {
	UNUSED ( other );

	// TODO: actually pay attention to OVERLAP_GRANULARITY_FINE and OVERLAP_CALCULATE_BOUNDS
	info.mHasBounds = false;
	
	MOAICollisionShape* shape0 = this->GetCollisionShape ();
	MOAICollisionShape* shape1 = other.GetCollisionShape ();
	
	if ( shape0 || shape1 ) {
	
		if ( shape0 && shape1 ) {
		
			return shape0->Overlap ( *shape1, *this, other, info.mBounds );
		}
		else if ( shape0 ){
		
			ZLBox bounds;
			other.GetModelBounds ( bounds );
			return shape0->Overlap ( bounds, *this, other, info.mBounds );
		}
		else if ( shape1 ) {
		
			ZLBox bounds;
			this->GetModelBounds ( bounds );
			return shape1->Overlap ( bounds, other, *this, info.mBounds );
		}
	}
	return true;
}

//----------------------------------------------------------------//
void MOAICollisionProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaClass ( state );
	MOAIIndexedPropBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "OVERLAP_EVENTS_ON_UPDATE",		( u32 )OVERLAP_EVENTS_ON_UPDATE );
	state.SetField ( -1, "OVERLAP_EVENTS_CONTINUOUS",		( u32 )OVERLAP_EVENTS_CONTINUOUS );
	state.SetField ( -1, "OVERLAP_EVENTS_LIFECYCLE",		( u32 )OVERLAP_EVENTS_LIFECYCLE );
	state.SetField ( -1, "OVERLAP_GRANULARITY_FINE",		( u32 )OVERLAP_GRANULARITY_FINE );
	state.SetField ( -1, "OVERLAP_CALCULATE_BOUNDS",		( u32 )OVERLAP_CALCULATE_BOUNDS );
	
	state.SetField ( -1, "GROUP_MASK_ALL",					( u32 )GROUP_MASK_ALL );
}

//----------------------------------------------------------------//
void MOAICollisionProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaFuncs ( state );
	MOAIIndexedPropBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getOverlaps",		_getOverlaps },
		{ "hasOverlaps",		_hasOverlaps },
		{ "setGroupMask",		_setGroupMask },
		{ "setOverlapFlags",	_setOverlapFlags },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICollisionProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIPartitionHull::SerializeIn ( state, serializer );
	MOAIIndexedPropBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAICollisionProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIPartitionHull::SerializeOut ( state, serializer );
	MOAIIndexedPropBase::SerializeOut ( state, serializer );
}

//================================================================//
// MOAICollisionProp virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAICollisionProp::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	if ( MOAIIndexedPropBase::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIPartitionHull::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAICollisionProp::MOAINode_Update () {
	
	MOAIPartitionHull::MOAINode_Update ();
}

//----------------------------------------------------------------//
void MOAICollisionProp::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetBounds ());
}

//----------------------------------------------------------------//
u32 MOAICollisionProp::MOAIPartitionHull_AffirmInterfaceMask ( MOAIPartition& partition ) {

	return partition.AffirmInterfaceMask < MOAICollisionProp >();
}

//----------------------------------------------------------------//
void MOAICollisionProp::MOAIPartitionHull_BoundsDidChange () {
	
	if ( this->mCollisionWorld && this->mOverlapFlags ) {
		this->mCollisionWorld->MakeActive ( *this );
	}
}

//----------------------------------------------------------------//
u32 MOAICollisionProp::MOAIPartitionHull_GetModelBounds ( ZLBox& bounds ) {
	UNUSED ( bounds );

	MOAICollisionShape* shape = this->GetCollisionShape ();
	if ( shape ) {
		bounds = shape->GetBounds ();
		return MOAIPartitionHull::BOUNDS_OK;
	}
	return MOAIPartitionHull::BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
bool MOAICollisionProp::MOAIPartitionHull_PrepareForInsertion ( const MOAIPartition& partition ) {
	UNUSED ( partition );
	return true;
}

//----------------------------------------------------------------//
void MOAICollisionProp::MOAIPartitionHull_WasRemovedFromPartition () {

	this->mCollisionWorld = 0;
}
