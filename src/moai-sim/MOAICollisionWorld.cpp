// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAICollisionFacet.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAICollisionWorld.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIProp.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAICollisionWorld::_processOverlaps ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "U" )

	self->ProcessOverlaps ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionWorld::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "U" )
	
	self->mCallback.SetRef ( state, 2 );
	return 0;
}

//================================================================//
// MOAICollisionWorld
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionWorld::AffirmOverlap ( MOAICollisionFacet& facet0, MOAICollisionFacet& facet1, const MOAIOverlapInfo& overlapInfo ) {
	
	MOAIPropOverlapLink* overlapLink = facet0.mOverlapLinks;
	for ( ; overlapLink; overlapLink = overlapLink->mNext ) {
	
		if ( overlapLink->mOther == &facet1 ) {
			MOAIPropOverlap& overlap = *overlapLink->mOverlap;
			overlap.mIsValid = true;
			return;
		}
	}
	
	MOAIPropOverlap* overlap = this->mOverlapPool.Alloc ();
	assert ( overlap ); // TODO: fail gracefully
	
	overlap->mIsValid = true; // latch is set
	
	overlap->mLeft.mOther = &facet1;
	overlap->mLeft.mOverlap = overlap;
	overlap->mLeft.mNext = facet0.mOverlapLinks;
	facet0.mOverlapLinks = &overlap->mLeft;
	
	overlap->mRight.mOther = &facet0;
	overlap->mRight.mOverlap = overlap;
	overlap->mRight.mNext = facet1.mOverlapLinks;
	facet1.mOverlapLinks = &overlap->mRight;
	
	overlap->mOverlapListLink.Data ( overlap );
	this->mOverlapList.PushBack ( overlap->mOverlapListLink );
	
	this->DoCallback ( OVERLAP_BEGIN, facet0, facet1, overlapInfo );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::ClearOverlap ( MOAICollisionFacet& facet0, MOAICollisionFacet& facet1 ) {

	if ( !( facet0.mOverlapLinks && facet1.mOverlapLinks )) return;

	MOAIPropOverlapLink* cursor = facet0.mOverlapLinks;
	facet0.mOverlapLinks = 0;
	
	while ( cursor ) {
	
		MOAIPropOverlapLink* overlapLink = cursor;
		cursor = cursor->mNext;
		
		MOAICollisionFacet& other = *overlapLink->mOther;
		
		if ( overlapLink->mOther == &facet1 ) {
			
			if (( facet0.mOverlapFlags | facet1.mOverlapFlags ) & MOAICollisionFacet::OVERLAP_EVENTS_LIFECYCLE ) {
				this->DoCallback ( OVERLAP_END, facet0, facet1 );
			}
		
			facet1.ClearOverlapLink ( facet0 );
			this->mOverlapList.Remove ( overlapLink->mOverlap->mOverlapListLink );
			this->mOverlapPool.Free ( overlapLink->mOverlap );
		}
		else {
		
			overlapLink->mNext = facet0.mOverlapLinks;
			facet0.mOverlapLinks = overlapLink;
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::ClearOverlaps ( MOAICollisionFacet& prop ) {
	
	MOAIPropOverlapLink* cursor = prop.mOverlapLinks;
	while ( cursor ) {
	
		MOAIPropOverlapLink* overlapLink = cursor;
		cursor = cursor->mNext;
		
		MOAICollisionFacet& other = *overlapLink->mOther;
		
		// see if we should broadcast finish events
		u32 flags = prop.mOverlapFlags | other.mOverlapFlags;
		if ( flags & MOAICollisionFacet::OVERLAP_EVENTS_LIFECYCLE ) {
			this->DoCallback ( OVERLAP_END, prop, other );
		}
		
		// clear out the linkbacks
		prop.ClearOverlapLink ( other );
		other.ClearOverlapLink ( prop );
		
		// remove from the list
		this->mOverlapList.Remove ( overlapLink->mOverlap->mOverlapListLink );
		
		// done with the overlap and links
		this->mOverlapPool.Free ( overlapLink->mOverlap );
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::DoCallback ( u32 eventID, MOAICollisionFacet& facet0, MOAICollisionFacet& facet1 ) {
	
	if ( this->mCallback ) {
		
		MOAIScopedLuaState state = this->mCallback.GetSelf ();
		
		state.Push ( eventID );
		state.Push ( &facet0.GetProp ());
		state.Push ( &facet1.GetProp ());
		state.DebugCall ( 3, 0 );
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::DoCallback ( u32 eventID, MOAICollisionFacet& facet0, MOAICollisionFacet& facet1, const MOAIOverlapInfo& overlapInfo ) {
	
	if ( this->mCallback ) {
	
		MOAIScopedLuaState state = this->mCallback.GetSelf ();
		
		state.Push ( eventID );
		state.Push ( &facet0.GetProp ());
		state.Push ( &facet1.GetProp ());
		
		const ZLVec3D& center = overlapInfo.mCenter;
		
		state.Push ( center.mX );
		state.Push ( center.mY );
		state.Push ( center.mZ );
		
		if ( overlapInfo.mHasBounds ) {
		
			const ZLBox& bounds = overlapInfo.mBounds;
		
			state.Push ( bounds.mMin.mX );
			state.Push ( bounds.mMin.mY );
			state.Push ( bounds.mMin.mZ );
			state.Push ( bounds.mMax.mX );
			state.Push ( bounds.mMax.mY );
			state.Push ( bounds.mMax.mZ );
			
			state.DebugCall ( 12, 0 );
		}
		else {
			state.DebugCall ( 6, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::DrawFacet ( MOAICollisionFacet& facet ) {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008

	MOAIGraphicsProp* prop = facet.GetProp ().GetGraphicsProp ();
		
	if ( prop && prop->IsVisible () && !prop->IsClear ()) {
		
		bool visible = false;
		
		if ( facet.IsActive ()) {
			
			if ( facet.mOverlapLinks ) {
				visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS );
			}
			
			if ( facet.mTouched == this->mOverlapPass && !visible ) {
				visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS );
			}
			
			if ( !visible ) {
				visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_PROP_BOUNDS );
			}
		}
		
		if ( facet.mOverlapLinks && !visible ) {
			visible = debugLines.Bind ( MOAIDebugLines::COLLISION_OVERLAP_PROP_BOUNDS );
		}
		
		if ( visible ) {
			draw.DrawBoxOutline ( prop->GetBounds ());
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::InvalidateOverlaps ( MOAICollisionFacet& facet, u32 nextPass ) {

	MOAIPropOverlapLink* cursor = facet.mOverlapLinks;
	for ( ; cursor; cursor = cursor->mNext ) {
		cursor->mOverlap->mIsValid = cursor->mOther->mOverlapPass == nextPass;
	}
}

//----------------------------------------------------------------//
bool MOAICollisionWorld::IsDone () {

	return false;
}

//----------------------------------------------------------------//
void MOAICollisionWorld::MakeActive ( MOAICollisionFacet& facet ) {

	if ( !facet.mActiveListLink.List ()) {
		this->mActiveList.PushBack ( facet.mActiveListLink );
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::MakeInactive ( MOAICollisionFacet& facet ) {

	this->mActiveList.Remove ( facet.mActiveListLink );
}

//----------------------------------------------------------------//
MOAICollisionWorld::MOAICollisionWorld () :
	mUpdated ( false ),
	mOverlapPass ( OVERLAP_PASS_INIT ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPartition )
		RTTI_EXTEND ( MOAIAction )
		RTTI_EXTEND ( MOAIRenderable )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICollisionWorld::~MOAICollisionWorld () {

	while ( this->mOverlapList.Count ()) {
		MOAIPropOverlap* overlap = this->mOverlapList.Front ();
		this->mOverlapList.Remove ( overlap->mOverlapListLink );
		this->mOverlapPool.Free ( overlap );
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::OnPropInserted ( MOAIProp& prop ) {

	this->OnPropUpdated ( prop );
}

//----------------------------------------------------------------//	
void MOAICollisionWorld::OnPropRemoved ( MOAIProp& prop ) {
	
	MOAICollisionFacet* collisionFacet = prop.GetCollisionFacet ();
	
	if ( collisionFacet ) {
		this->ClearOverlaps ( *collisionFacet );
		this->MakeInactive ( *collisionFacet );
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::OnPropUpdated ( MOAIProp& prop ) {
	
	MOAICollisionFacet* collisionFacet = prop.GetCollisionFacet ();
	
	if ( collisionFacet && collisionFacet->mOverlapFlags ) {
		this->MakeActive ( *collisionFacet );
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::OnUpdate ( double step ) {
	UNUSED ( step );

	this->ProcessOverlaps ();
}

//----------------------------------------------------------------//
void MOAICollisionWorld::ProcessOverlaps () {

	u32 thisPass = this->mOverlapPass;
	u32 nextPass = thisPass + 1;

	// any prop becomes active when it is updated but only if
	// flags are set

	// a prop's status is given by the combination of its overlap
	// flags and all others
	// OVERLAP_EVENTS_CONTINUOUS - both stay 'active' after overlap
	// OVERLAP_EVENTS_LIFECYCLE - 'active' when updated; does not stay in overlap list

	// cycle through the active props
	ActiveListIt activeIt = this->mActiveList.Head ();
	while ( activeIt ) {
		
		MOAICollisionFacet& facet = *activeIt->Data ();
		this->InvalidateOverlaps ( facet, nextPass );
		
		MOAIProp& prop = facet.GetProp ();
		
		u32 overlapFlags = facet.mOverlapFlags;
		
		// this gives us the coarse filter based on world space bounds
		// TODO: find a way to utilize overlap flags?
		MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
		//u32 totalResults = this->GatherProps ( buffer, &prop, prop.GetBounds (), MOAIProp::CAN_OVERLAP );
		u32 totalResults = 0;
		
		for ( u32 i = 0; i < totalResults; ++i ) {
		
			MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
			MOAICollisionFacet* otherFacet = result->mProp->GetCollisionFacet ();
			
			if ( !otherFacet ) continue;
			if ( !( facet.mGroupMask & otherFacet->mGroupMask )) continue;
			if ( otherFacet->mOverlapPass == nextPass ) continue; // has been processed
			
			// this calculates the detailed overlap, updates the links and sends overlap events
			MOAIOverlapInfo overlapInfo;
			if ( facet.RefineOverlap ( *otherFacet, overlapInfo )) {
				
				u32 flags = facet.mOverlapFlags | otherFacet->mOverlapFlags;
				
				// create an overlap if needed
				if ( flags & MOAICollisionFacet::OVERLAP_EVENTS_LIFECYCLE ) {
					this->AffirmOverlap ( facet, *otherFacet, overlapInfo );
				}
				
				// send update event and keep both active
				if ( flags & ( MOAICollisionFacet::OVERLAP_EVENTS_ON_UPDATE | MOAICollisionFacet::OVERLAP_EVENTS_CONTINUOUS )) {
				
					facet.mTouched = nextPass;
					otherFacet->mTouched = nextPass;
				
					this->DoCallback ( OVERLAP_UPDATE, facet, *otherFacet, overlapInfo );
				
					if ( flags & MOAICollisionFacet::OVERLAP_EVENTS_CONTINUOUS ) {
						this->MakeActive ( *otherFacet );
						facet.mStayActive = true;
						otherFacet->mStayActive = true;
					}
				}
			}
		}
		
		this->PruneOverlaps ( facet );
		
		facet.mOverlapPass = nextPass;
		activeIt = activeIt->Next ();

		if ( !facet.mStayActive ) {
			this->MakeInactive ( facet );
		}
		facet.mStayActive = false;
	}
	
	this->mOverlapPass = nextPass;
}

//----------------------------------------------------------------//
void MOAICollisionWorld::PruneOverlaps ( MOAICollisionFacet& facet ) {

	MOAIPropOverlapLink* cursor = facet.mOverlapLinks;
	facet.mOverlapLinks = 0;
	
	while ( cursor ) {
	
		MOAIPropOverlapLink* overlapLink = cursor;
		cursor = cursor->mNext;
		
		MOAICollisionFacet& other = *overlapLink->mOther;
		MOAIPropOverlap& overlap = *overlapLink->mOverlap;
		
		if ( !overlap.mIsValid ) {
			
			if (( facet.mOverlapFlags | other.mOverlapFlags ) & MOAICollisionFacet::OVERLAP_EVENTS_LIFECYCLE ) {
				this->DoCallback ( OVERLAP_END, facet, other );
			}
		
			other.ClearOverlapLink ( facet );
			this->mOverlapList.Remove ( overlapLink->mOverlap->mOverlapListLink );
			this->mOverlapPool.Free ( overlapLink->mOverlap );
		}
		else {
		
			overlapLink->mNext = facet.mOverlapLinks;
			facet.mOverlapLinks = overlapLink;
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIPartition::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );
	
	state.SetField ( -1, "OVERLAP_BEGIN",				( u32 )OVERLAP_BEGIN );
	state.SetField ( -1, "OVERLAP_END",					( u32 )OVERLAP_END );
	state.SetField ( -1, "OVERLAP_UPDATE",				( u32 )OVERLAP_UPDATE );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIPartition::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "processOverlaps",	_processOverlaps },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::Render () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008

	draw.Bind ();

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_WORLD, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );

	MOAICollisionFacet* drawList = 0;

	OverlapListIt overlapIt = this->mOverlapList.Head ();
	for ( ; overlapIt; overlapIt = overlapIt->Next ()) {
		MOAIPropOverlap& overlap = *overlapIt->Data ();
	
		MOAICollisionFacet& facet0 = *overlap.mLeft.mOther;
		MOAICollisionFacet& facet1 = *overlap.mRight.mOther;
	
		if ( !facet0.mInDrawList ) {
			facet0.mNextInDrawList = drawList;
			facet0.mInDrawList = true;
			drawList = &facet0;
		}
		
		if ( !facet1.mInDrawList ) {
			facet1.mNextInDrawList = drawList;
			facet1.mInDrawList = true;
			drawList = &facet1;
		}
	}
	
	ActiveListIt activeIt = this->mActiveList.Head ();
	for ( ; activeIt; activeIt = activeIt->Next ()) {
		MOAICollisionFacet& facet = *activeIt->Data ();
		
		if ( !facet.mInDrawList ) {
			facet.mInDrawList = true;
			facet.mNextInDrawList = drawList;
			drawList = &facet;
		}
	}
	
	MOAICollisionFacet* cursor = drawList;
	while ( cursor ) {
	
		MOAICollisionFacet& facet = *cursor;
		cursor = cursor->mNextInDrawList;
		
		this->DrawFacet ( facet );
		facet.mInDrawList = false;
		facet.mNextInDrawList = 0;
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIPartition::SerializeIn ( state, serializer );
	MOAIAction::SerializeIn ( state, serializer );
	MOAIRenderable::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIPartition::SerializeOut ( state, serializer );
	MOAIAction::SerializeOut ( state, serializer );
	MOAIRenderable::SerializeOut ( state, serializer );
}