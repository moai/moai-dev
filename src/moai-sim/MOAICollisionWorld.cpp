// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAICollisionProp.h>
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
int MOAICollisionWorld::_insertProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "U" )
	
	MOAICollisionProp* prop = state.GetLuaObject < MOAICollisionProp >( 2, true );
	if ( prop ) {
		self->InsertProp ( *prop );
	}
	return 0;
}

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

//----------------------------------------------------------------//
int MOAICollisionWorld::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "U" )

	self->mPartition.Set ( *self, state.GetLuaObject < MOAIPartition >( 2, true ));
	return 0;
}

//================================================================//
// MOAICollisionWorld
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionWorld::AffirmOverlap ( MOAICollisionProp& prop0, MOAICollisionProp& prop1, const MOAIOverlapInfo& overlapInfo ) {
	
	MOAIPropOverlapLink* overlapLink = prop0.mOverlapLinks;
	for ( ; overlapLink; overlapLink = overlapLink->mNext ) {
	
		if ( overlapLink->mOther == &prop1 ) {
			MOAIPropOverlap& overlap = *overlapLink->mOverlap;
			overlap.mIsValid = true;
			return;
		}
	}
	
	MOAIPropOverlap* overlap = this->mOverlapPool.Alloc ();
	assert ( overlap ); // TODO: fail gracefully
	
	overlap->mIsValid = true; // latch is set
	
	overlap->mLeft.mOther = &prop1;
	overlap->mLeft.mOverlap = overlap;
	overlap->mLeft.mNext = prop0.mOverlapLinks;
	prop0.mOverlapLinks = &overlap->mLeft;
	
	overlap->mRight.mOther = &prop0;
	overlap->mRight.mOverlap = overlap;
	overlap->mRight.mNext = prop1.mOverlapLinks;
	prop1.mOverlapLinks = &overlap->mRight;
	
	overlap->mOverlapListLink.Data ( overlap );
	this->mOverlapList.PushBack ( overlap->mOverlapListLink );
	
	this->DoCallback ( OVERLAP_BEGIN, prop0, prop1, overlapInfo );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::ClearOverlap ( MOAICollisionProp& prop0, MOAICollisionProp& prop1 ) {

	if ( !( prop0.mOverlapLinks && prop1.mOverlapLinks )) return;

	MOAIPropOverlapLink* cursor = prop0.mOverlapLinks;
	prop0.mOverlapLinks = 0;
	
	while ( cursor ) {
	
		MOAIPropOverlapLink* overlapLink = cursor;
		cursor = cursor->mNext;
		
		const MOAICollisionProp* otherProp = overlapLink->mOther;
		
		if ( otherProp == &prop1 ) {
			
			if (( prop0.mOverlapFlags | prop1.mOverlapFlags ) & MOAICollisionProp::OVERLAP_EVENTS_LIFECYCLE ) {
				this->DoCallback ( OVERLAP_END, prop0, prop1 );
			}
		
			prop1.ClearOverlapLink ( prop0 );
			this->mOverlapList.Remove ( overlapLink->mOverlap->mOverlapListLink );
			this->mOverlapPool.Free ( overlapLink->mOverlap );
		}
		else {
		
			overlapLink->mNext = prop0.mOverlapLinks;
			prop0.mOverlapLinks = overlapLink;
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::ClearOverlaps ( MOAICollisionProp& prop ) {
	
	MOAIPropOverlapLink* cursor = prop.mOverlapLinks;
	while ( cursor ) {
	
		MOAIPropOverlapLink* overlapLink = cursor;
		cursor = cursor->mNext;
		
		MOAICollisionProp& other = *overlapLink->mOther;
		
		// see if we should broadcast finish events
		u32 flags = prop.mOverlapFlags | other.mOverlapFlags;
		if ( flags & MOAICollisionProp::OVERLAP_EVENTS_LIFECYCLE ) {
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
void MOAICollisionWorld::DoCallback ( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1 ) {
	
	if ( this->mCallback ) {
		
		MOAIScopedLuaState state = this->mCallback.GetSelf ();
		
		state.Push ( eventID );
		state.Push ( &prop0 );
		state.Push ( &prop1 );
		state.DebugCall ( 3, 0 );
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::DoCallback ( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1, const MOAIOverlapInfo& overlapInfo ) {
	
	if ( this->mCallback ) {
	
		MOAIScopedLuaState state = this->mCallback.GetSelf ();
		
		state.Push ( eventID );
		state.Push ( &prop0 );
		state.Push ( &prop1 );
		
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
void MOAICollisionWorld::DrawCollisionProp ( MOAICollisionProp& prop ) {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008

	bool visible = false;
	
	if ( prop.IsActive ()) {
		
		if ( prop.mOverlapLinks ) {
			visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS );
		}
		
		if ( prop.mTouched == this->mOverlapPass && !visible ) {
			visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS );
		}
		
		if ( !visible ) {
			visible = debugLines.Bind ( MOAIDebugLines::COLLISION_ACTIVE_PROP_BOUNDS );
		}
	}
	
	if ( prop.mOverlapLinks && !visible ) {
		visible = debugLines.Bind ( MOAIDebugLines::COLLISION_OVERLAP_PROP_BOUNDS );
	}
	
	if ( visible ) {
		draw.DrawBoxOutline ( prop.GetBounds ());
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::InsertProp ( MOAICollisionProp& prop ) {

	if ( prop.mCollisionWorld != this ) {
	
		// clears out the old collision world (if any )
		if ( prop.mCollisionWorld ) {
			prop.mCollisionWorld->RemoveProp ( prop );
		}
		
		if ( this->mPartition ) {
			
			// must be set before calling set partition
			prop.mCollisionWorld = this;
		
			// this will invoke PrepareForInsertion () on the prop, which
			// will verify that the prop's collision world matchs 'this'
			prop.SetPartition ( this->mPartition );
			
			// now activate the prop
			if ( prop.mOverlapFlags ) {
				this->MakeActive ( prop );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::InvalidateOverlaps ( MOAICollisionProp& prop, u32 nextPass ) {

	MOAIPropOverlapLink* cursor = prop.mOverlapLinks;
	for ( ; cursor; cursor = cursor->mNext ) {
		cursor->mOverlap->mIsValid = cursor->mOther->mOverlapPass == nextPass;
	}
}

//----------------------------------------------------------------//
bool MOAICollisionWorld::IsDone () {

	return false;
}

//----------------------------------------------------------------//
void MOAICollisionWorld::MakeActive ( MOAICollisionProp& prop ) {

	if ( !prop.mActiveListLink.List ()) {
		this->mActiveList.PushBack ( prop.mActiveListLink );
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::MakeInactive ( MOAICollisionProp& prop ) {

	this->mActiveList.Remove ( prop.mActiveListLink );
}

//----------------------------------------------------------------//
MOAICollisionWorld::MOAICollisionWorld () :
	mUpdated ( false ),
	mOverlapPass ( OVERLAP_PASS_INIT ) {
	
	RTTI_BEGIN
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
	this->mPartition.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::OnUpdate ( double step ) {
	UNUSED ( step );

	this->ProcessOverlaps ();
}

//----------------------------------------------------------------//
void MOAICollisionWorld::ProcessOverlaps () {

	if ( !this->mPartition ) return;

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
		
		MOAICollisionProp& prop = *activeIt->Data ();
		this->InvalidateOverlaps ( prop, nextPass );
		
		u32 interfaceMask = this->mPartition->GetInterfaceMask < MOAICollisionProp >();
		
		// this gives us the coarse filter based on world space bounds
		// TODO: find a way to utilize overlap flags?
		MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
		u32 totalResults = this->mPartition->GatherProps ( buffer, &prop, prop.GetBounds (), interfaceMask );
		
		for ( u32 i = 0; i < totalResults; ++i ) {
		
			MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
			MOAICollisionProp* otherProp = result->mProp->AsType < MOAICollisionProp >();
			
			if ( !otherProp ) continue;
			if ( !( prop.mGroupMask & otherProp->mGroupMask )) continue;
			if ( otherProp->mOverlapPass == nextPass ) continue; // has been processed
			
			// this calculates the detailed overlap, updates the links and sends overlap events
			MOAIOverlapInfo overlapInfo;
			if ( prop.RefineOverlap ( *otherProp, overlapInfo )) {
				
				u32 flags = prop.mOverlapFlags | otherProp->mOverlapFlags;
				
				// create an overlap if needed
				if ( flags & MOAICollisionProp::OVERLAP_EVENTS_LIFECYCLE ) {
					this->AffirmOverlap ( prop, *otherProp, overlapInfo );
				}
				
				// send update event and keep both active
				if ( flags & ( MOAICollisionProp::OVERLAP_EVENTS_ON_UPDATE | MOAICollisionProp::OVERLAP_EVENTS_CONTINUOUS )) {
				
					prop.mTouched = nextPass;
					otherProp->mTouched = nextPass;
				
					this->DoCallback ( OVERLAP_UPDATE, prop, *otherProp, overlapInfo );
				
					if ( flags & MOAICollisionProp::OVERLAP_EVENTS_CONTINUOUS ) {
						this->MakeActive ( *otherProp );
						prop.mStayActive = true;
						otherProp->mStayActive = true;
					}
				}
			}
		}
		
		this->PruneOverlaps ( prop );
		
		prop.mOverlapPass = nextPass;
		activeIt = activeIt->Next ();

		if ( !prop.mStayActive ) {
			this->MakeInactive ( prop );
		}
		prop.mStayActive = false;
	}
	
	this->mOverlapPass = nextPass;
}

//----------------------------------------------------------------//
void MOAICollisionWorld::PruneOverlaps ( MOAICollisionProp& prop ) {

	MOAIPropOverlapLink* cursor = prop.mOverlapLinks;
	prop.mOverlapLinks = 0;
	
	while ( cursor ) {
	
		MOAIPropOverlapLink* overlapLink = cursor;
		cursor = cursor->mNext;
		
		MOAICollisionProp& other = *overlapLink->mOther;
		MOAIPropOverlap& overlap = *overlapLink->mOverlap;
		
		if ( !overlap.mIsValid ) {
			
			if (( prop.mOverlapFlags | other.mOverlapFlags ) & MOAICollisionProp::OVERLAP_EVENTS_LIFECYCLE ) {
				this->DoCallback ( OVERLAP_END, prop, other );
			}
		
			other.ClearOverlapLink ( prop );
			this->mOverlapList.Remove ( overlapLink->mOverlap->mOverlapListLink );
			this->mOverlapPool.Free ( overlapLink->mOverlap );
		}
		else {
		
			overlapLink->mNext = prop.mOverlapLinks;
			prop.mOverlapLinks = overlapLink;
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
	MOAIRenderable::RegisterLuaClass ( state );
	
	state.SetField ( -1, "OVERLAP_BEGIN",				( u32 )OVERLAP_BEGIN );
	state.SetField ( -1, "OVERLAP_END",					( u32 )OVERLAP_END );
	state.SetField ( -1, "OVERLAP_UPDATE",				( u32 )OVERLAP_UPDATE );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	MOAIRenderable::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "insertProp",			_insertProp },
		{ "processOverlaps",	_processOverlaps },
		{ "setCallback",		_setCallback },
		{ "setPartition",		_setPartition },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::RemoveProp ( MOAICollisionProp& prop ) {

	assert ( prop.mCollisionWorld == this );
	assert ( this->mPartition );
	
	this->ClearOverlaps ( prop );
	this->MakeInactive ( prop );
	
	this->mPartition->RemoveProp ( prop );
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

	MOAICollisionProp* drawList = 0;

	OverlapListIt overlapIt = this->mOverlapList.Head ();
	for ( ; overlapIt; overlapIt = overlapIt->Next ()) {
		MOAIPropOverlap& overlap = *overlapIt->Data ();
	
		MOAICollisionProp& prop0 = *overlap.mLeft.mOther;
		MOAICollisionProp& prop1 = *overlap.mRight.mOther;
	
		if ( !prop0.mInDrawList ) {
			prop0.mNextInDrawList = drawList;
			prop0.mInDrawList = true;
			drawList = &prop0;
		}
		
		if ( !prop1.mInDrawList ) {
			prop1.mNextInDrawList = drawList;
			prop1.mInDrawList = true;
			drawList = &prop1;
		}
	}
	
	ActiveListIt activeIt = this->mActiveList.Head ();
	for ( ; activeIt; activeIt = activeIt->Next ()) {
		MOAICollisionProp& prop = *activeIt->Data ();
		
		if ( !prop.mInDrawList ) {
			prop.mInDrawList = true;
			prop.mNextInDrawList = drawList;
			drawList = &prop;
		}
	}
	
	MOAICollisionProp* cursor = drawList;
	while ( cursor ) {
	
		MOAICollisionProp& prop = *cursor;
		cursor = cursor->mNextInDrawList;
		
		this->DrawCollisionProp ( prop );
		prop.mInDrawList = false;
		prop.mNextInDrawList = 0;
	}
}

//----------------------------------------------------------------//
void MOAICollisionWorld::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIAction::SerializeIn ( state, serializer );
	MOAIRenderable::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIAction::SerializeOut ( state, serializer );
	MOAIRenderable::SerializeOut ( state, serializer );
}
