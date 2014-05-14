// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAICollisionFacet.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAICollisionWorld.h>
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
void MOAICollisionWorld::HandleOverlap ( MOAICollisionFacet& facet0, MOAICollisionFacet& facet1, MOAIOverlapInfo& overlapInfo ) {

	u32 flags = facet0.mOverlapFlags | facet1.mOverlapFlags;
	
	if ( flags & MOAICollisionFacet::OVERLAP_EVENTS_LIFECYCLE ) {
	
		MOAIPropOverlapLink* overlapLink = facet0.mOverlapLinks;
		for ( ; overlapLink; overlapLink = overlapLink->mNext ) {
		
			if ( overlapLink->mOther == &facet1 ) {
				
				MOAIPropOverlap& overlap = *overlapLink->mOverlap;
				
				if ( flags & MOAICollisionFacet::OVERLAP_EVENTS_ON_UPDATE ) {
					this->DoCallback ( OVERLAP_UPDATE, facet0, facet1, overlapInfo );
				}
				return;
			}
		}
		
		MOAIPropOverlap* overlap = this->mOverlapPool.Alloc ();
		assert ( overlap ); // TODO: fail gracefully
		
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
	else if ( flags & MOAICollisionFacet::OVERLAP_EVENTS_ON_UPDATE ) {
		
		this->DoCallback ( OVERLAP_UPDATE, facet0, facet1, overlapInfo );
	}
}

//----------------------------------------------------------------//
bool MOAICollisionWorld::IsDone () {

	return false;
}

//----------------------------------------------------------------//
void MOAICollisionWorld::MakeActive ( MOAICollisionFacet& facet ) {

	if ( facet.mOverlapPass == 0 ) {
		this->mActiveList.PushBack ( facet.mActiveListLink );
		facet.mOverlapPass = this->mOverlapPass;
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::MakeInactive ( MOAICollisionFacet& facet ) {

	if ( facet.mOverlapPass != 0 ) {
		this->mActiveList.Remove ( facet.mActiveListLink );
		facet.mOverlapPass = 0;
	}
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
void MOAICollisionWorld::OnUpdate ( float step ) {
	UNUSED ( step );

	this->ProcessOverlaps ();
}

//----------------------------------------------------------------//
void MOAICollisionWorld::ProcessOverlaps () {

	// any prop becomes active when it is updated but only if
	// flags are set

	// a prop's status is given by the combination of its overlap
	// flags and all others
	// OVERLAP_EVENTS_CONTINUOUS - both stay 'active' after overlap
	// OVERLAP_EVENTS_LIFECYCLE - 'active' when updated; does not stay in overlap list

	u32 thisPass = this->mOverlapPass;
	u32 nextPass = thisPass ^ OVERLAP_PASS_XOR;

	// cycle through the active props
	ActiveListIt activeIt = this->mActiveList.Head ();
	while ( activeIt ) {
		
		MOAICollisionFacet& facet = *activeIt->Data ();
		MOAIProp& prop = facet.GetProp ();
		
		u32 overlapFlags = facet.mOverlapFlags;
		
		// this gives us the coarse filter based on world space bounds
		// TODO: find a way to utilize overlap flags?
		MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
		u32 totalResults = this->GatherProps ( buffer, &prop, prop.GetBounds (), MOAIProp::CAN_OVERLAP );
		
		for ( u32 i = 0; i < totalResults; ++i ) {
			MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
			MOAICollisionFacet* otherFacet = result->mProp->GetCollisionFacet ();
			if ( !otherFacet ) continue;
			
			if ( otherFacet->mOverlapPass != thisPass ) continue; // has been processed or removed
			
			// this calculates the detailed overlap, updates the links and sends overlap events
			MOAIOverlapInfo overlapInfo;
			if ( facet.RefineOverlap ( *otherFacet, overlapInfo )) {
				
				this->HandleOverlap ( facet, *otherFacet, overlapInfo );
				overlapFlags |= otherFacet->mOverlapFlags;
				
				// keep both active
				if ( overlapFlags & MOAICollisionFacet::OVERLAP_EVENTS_ON_UPDATE ) {
					this->MakeActive ( *otherFacet );
				}
			}
		}
		
		facet.mOverlapPass = nextPass;
		activeIt = activeIt->Next ();
		
		if ( totalResults == 0 ) {
			this->ClearOverlaps ( facet );
			this->MakeInactive ( facet );
		}
		else if ( !( overlapFlags & MOAICollisionFacet::OVERLAP_EVENTS_ON_UPDATE )) {
			this->MakeInactive ( facet );
		}
	}
	
	this->mOverlapPass = nextPass;
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

	ActiveListIt activeIt = this->mActiveList.Head ();
	for ( ; activeIt; activeIt = activeIt->Next ()) {
		
		MOAICollisionFacet& facet = *activeIt->Data ();
		
		if ( debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_PROP_BOUNDS )) {
			draw.DrawBoxOutline ( facet.GetProp ().GetBounds ());
		}
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