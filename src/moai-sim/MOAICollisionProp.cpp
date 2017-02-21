// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAICollisionProp.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAICollisionWorld.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIMoveConstraint2D.h>
#include <moai-sim/MOAIOverlap.h>
#include <moai-sim/MOAIOverlapResolver.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIViewport.h>

// uncomment me to debug log
//#define MOAICOLLISIONPROP_DEBUG

#ifdef MOAICOLLISIONPROP_DEBUG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionProp::_collisionMove ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionProp, "U" )
	
	ZLVec3D move = state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );
	
	self->Move ( move );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionProp::_getOverlaps ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionProp, "U" )

	u32 total = 0;
	MOAIPropOverlapLink* link = self->mOverlapLinks;
	for ( ; link; link = link->mNext, ++total ) {
		state.Push ( link->mOtherLink->mProp );
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
//int MOAICollisionProp::_setGroupMask ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAICollisionProp, "U" )
//	
//	self->mGroupMask = state.GetValue < u32 >( 2, 0 );
//	return 0;
//}

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
void MOAICollisionProp::ClearOverlapLink ( MOAIPropOverlap& overlap ) {

	MOAIPropOverlapLink* cursor = this->mOverlapLinks;
	this->mOverlapLinks = 0;
	
	while ( cursor ) {
		MOAIPropOverlapLink* overlapLink = cursor;
		cursor = cursor->mNext;
		
		if ( overlapLink->mOverlap != &overlap ) {
			overlapLink->mNext = this->mOverlapLinks;
			this->mOverlapLinks = overlapLink;
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionProp::DrawContactPoints ( MOAIDrawShape& draw, const MOAIMoveConstraint2D* contacts, u32 nContacts ) {

	draw.SetPenWidth ( 1.0f );

	for ( u32 i = 0; i < nContacts; ++i ) {
	
		const MOAIMoveConstraint2D& contact = contacts [ i ];
	
		ZLVec3D point ( contact.mPoint.mX, contact.mPoint.mY, 0.0f );
		ZLVec3D normal ( contact.mNormal.mX, contact.mNormal.mY, 0.0f );
		const ZLVec2D cornerTangent = contact.mCornerTangent;
		const ZLVec2D edgeNormal = contact.mEdgeNormal;
		
		draw.SetPenColor ( ZLColor::PackRGBA ( 0.0f, 1.0f, 0.0f, 1.0f ));
		draw.DrawRay ( point.mX, point.mY, normal.mX, normal.mY, 32.0f );
		
		switch ( contact.mType ) {
		
			case MOAIMoveConstraint2D::LEAVING:

				draw.SetPenColor ( ZLColor::PackRGBA ( 1.0f, 0.0f, 1.0f, 1.0f ));
				break;
				
			case MOAIMoveConstraint2D::CROSSING:

				draw.SetPenColor ( ZLColor::PackRGBA ( 1.0f, 0.0f, 0.0f, 1.0f ));
				break;
				
			case MOAIMoveConstraint2D::PARALLEL:

				draw.SetPenColor ( ZLColor::PackRGBA ( 0.0f, 1.0f, 1.0f, 1.0f ));
				break;
				
			case MOAIMoveConstraint2D::CORNER:
				
				draw.SetPenColor ( ZLColor::PackRGBA ( 0.85f, 0.0f, 1.0f, 1.0f ));
				draw.DrawRay ( point.mX, point.mY, cornerTangent.mX, cornerTangent.mY, 48.0f );
			
				draw.SetPenColor ( ZLColor::PackRGBA ( 1.0f, 0.0f, 0.0f, 1.0f ));
				draw.DrawRay ( point.mX, point.mY, edgeNormal.mX, edgeNormal.mY, 32.0f );
			
				draw.SetPenColor ( ZLColor::PackRGBA ( 1.0f, 1.0f, 0.0f, 1.0f ));
				break;
		}
		
		draw.DrawRectFill ( point.mX - 4.0f, point.mY + 4.0f, point.mX + 4.0f, point.mY - 4.0f );
	}
}

//----------------------------------------------------------------//
void MOAICollisionProp::GatherAndProcess ( MOAICollisionPrimVisitor& visitor, const ZLBox& worldBounds ) {

	MOAICollisionWorld& world = *this->mCollisionWorld;

	MOAIScopedPartitionResultBufferHandle scopedBufferHandle = MOAIPartitionResultMgr::Get ().GetBufferHandle ();
	MOAIPartitionResultBuffer& buffer = scopedBufferHandle;
	
	u32 interfaceMask = world.GetInterfaceMask < MOAICollisionProp >();
	u32 totalResults = world.GatherHulls ( buffer, this, worldBounds, interfaceMask );
	
	for ( u32 i = 0; i < totalResults; ++i ) {
		MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
		MOAICollisionProp* other = result->AsType < MOAICollisionProp >();
		if ( !other ) continue;
		
		this->Process ( visitor, *other );
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
	mCategory ( CATEGORY_MASK_ALL ),
	mMask ( CATEGORY_MASK_ALL ),
	mOverlapFlags ( DEFAULT_OVERLAP_FLAGS ),
	mOverlapPass ( MOAICollisionWorld::OVERLAP_PASS_INIT ),
	mOverlapLinks ( 0 ),
	mStayActive ( false ),
	mTouched ( MOAICollisionWorld::OVERLAP_PASS_INIT ),
	mCollisionWorld ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPartitionHull )
		RTTI_EXTEND ( MOAIDrawable )
		RTTI_EXTEND ( MOAIIndexedPropBase )
	RTTI_END
	
	this->mActiveListLink.Data ( this );
}

//----------------------------------------------------------------//
MOAICollisionProp::~MOAICollisionProp () {
}

//----------------------------------------------------------------//
void MOAICollisionProp::Move ( ZLVec3D move ) {

	// just cramp everytning in here for now.
	// possible to break into steps for collision world later *if*
	// more traditional physics approach is desired.
	
	// purpose here is just a proof of concept for an
	// edge-following algorithm. will make all this user-configurable
	// later.

	static u32 MAX_PASSES = 8;
	
	MOAIMoveConstraint2D* contacts = ( MOAIMoveConstraint2D* )alloca ( 128 * sizeof ( MOAIMoveConstraint2D ));
	MOAIMoveConstraintAccumulator2D contactAccumulator ( contacts, 128 );
	
	ZLVec2D moveNorm ( move.mX, move.mY );
	float moveLength = moveNorm.NormSafe ();
	
	if ( moveLength > EPSILON ) {
	
		for ( u32 i = 0; i < MAX_PASSES; ++i ) {
		
			// find best contact points
			
			float bestPushDot = 2.0f;
			const MOAIMoveConstraint2D* bestPushContact = 0;
			
			float bestPullDot = 2.0f;
			const MOAIMoveConstraint2D* bestPullContact = 0;
		
			// find contacts
			ZLBox worldBounds = this->GetWorldBounds ();
			worldBounds.Inflate ( 10.0f ); // TODO: epsilon
		
			contactAccumulator.Reset ();
			this->GatherAndProcess ( contactAccumulator, worldBounds );
			u32 nContacts = contactAccumulator.Top ();
			
			for ( u32 i = 0; i < nContacts; ++i ) {
			
				const MOAIMoveConstraint2D& contact = contacts [ i ];
				
				// ignore corner contacts if they are behind the move
				if ( contact.mType == MOAIMoveConstraint2D::CORNER ) {
					if ( !(( moveNorm.Dot ( contact.mCornerTangent ) > -EPSILON ) && ( moveNorm.Dot ( contact.mEdgeNormal ) > -EPSILON ))) continue;
				}
				
				float d = moveNorm.Dot ( contact.mNormal );
				
				if ( d <= EPSILON ) {

					// heading into - push
					if ( d < bestPushDot ) {
						bestPushContact = &contact;
						bestPushDot = d;
					}
				}
				else {

					// heading out of - pull
					if ( d < bestPullDot ) {
						bestPullContact = &contact;
						bestPullDot = d;
					}
				}
			}

			const MOAIMoveConstraint2D* bestContact = 0;

			if ( bestPushContact ) {

				// straight into a wall
				if ( bestPushDot <= ( EPSILON - 1.0f )) {
					break;
				}
				bestContact = bestPushContact;
			}
			else if ( bestPullContact ) {
			
				// straight out of a wall
				if ( bestPullDot >= ( 1.0f - EPSILON )) {
					break;
				}
				bestContact = bestPullContact;
			}
			else if ( nContacts ) {
			
				break;
			}

			ZLVec2D stepMoveNorm;
			float stepMoveLength;

			if ( bestContact ) {
						
				stepMoveNorm.Init ( move.mX, move.mY );
				stepMoveNorm.PerpProject ( bestContact->mNormal );
				stepMoveNorm.Norm ();
				
				float duh = stepMoveNorm.Dot ( bestContact->mTangent );
				
				float maxMove = duh > 0.0f ? bestContact->mPosD : bestContact->mNegD;
				stepMoveLength = moveLength < maxMove ? moveLength : maxMove;
				
				DEBUG_LOG ( "----> STEP: (%g, %g) EDGE (%g, %g) MAX MOVE: %g\n",
					move.mX, move.mY,
					bestContact->mTangent.mX, bestContact->mTangent.mY,
					maxMove
				);
				DEBUG_LOG ( "\n" );
			}
			else {
			
				stepMoveNorm.Init ( moveNorm.mX, moveNorm.mY );
				stepMoveLength = moveLength;
			}
			
			// TODO: final check that we aren't moving into a wall

			// move (and force a dep node update)
			// TODO: this needs to be *way* more efficient!
			this->mLoc.mX += stepMoveNorm.mX * stepMoveLength;
			this->mLoc.mY += stepMoveNorm.mY * stepMoveLength;
			this->MOAITransformBase::MOAINode_Update ();
			this->MOAIPartitionHull::MOAINode_Update ();

			// prepare the next step or break

			moveLength = moveLength - stepMoveLength;
			if ( moveLength < EPSILON ) break;
		}
	}
	
	MOAICollisionProp::DrawContactPoints ( *this->mCollisionWorld, contacts, contactAccumulator.Top ());
	
//	this->mLoc.Add ( move );
//	this->MOAITransformBase::MOAINode_Update ();
//	this->MOAIPartitionHull::MOAINode_Update ();
	
	// resolve overlaps
	MOAIOverlapResolver overlapResolver;
	this->GatherAndProcess ( overlapResolver, this->GetWorldBounds ());
	
	ZLVec3D resolveOverlaps = overlapResolver.GetResult ();
	
	this->mLoc.Add ( resolveOverlaps );
	this->ScheduleUpdate ();

	if ( move.LengthSqrd () && this->mCollisionWorld ) {
		
		MOAIDrawShapeRetained& draw = *this->mCollisionWorld;
		
		draw.SetPenWidth ( 1.0f );
		draw.SetPenColor ( ZLColor::PackRGBA ( 1.0f, 1.0f, 0.0f, 1.0f ));
		
		ZLRect worldRect = this->GetWorldBounds ().GetRect ();
		float width = worldRect.Width ();
		float height = worldRect.Height ();
		
		float radius = ( width < height ? width : height ) * 0.25f;
		
		draw.DrawCircleOutline ( this->mLoc.mX, this->mLoc.mY, radius, 32 );
		
		move.Norm ();
		move.Scale ( radius );
		draw.DrawLine ( this->mLoc.mX, this->mLoc.mY, this->mLoc.mX + move.mX, this->mLoc.mY + move.mY );
	}
}

//----------------------------------------------------------------//
void MOAICollisionProp::Process ( MOAICollisionPrimVisitor& visitor, MOAICollisionProp& other ) {

	MOAICollisionProp::Process ( visitor, *this, other );
}

//----------------------------------------------------------------//
void MOAICollisionProp::Process ( MOAICollisionPrimVisitor& visitor, MOAICollisionProp& prop0, MOAICollisionProp& prop1 ) {

	const MOAICollisionShape* shape0 = prop0.GetCollisionShape ();
	const MOAICollisionShape* shape1 = prop1.GetCollisionShape ();
	
	ZLAffine3D t0 = prop0.GetLocalToWorldMtx ();
	ZLAffine3D t1 = prop1.GetLocalToWorldMtx ();
	
	if ( shape0 || shape1 ) {
	
		if ( shape0 && shape1 ) {
		
			shape0->Process ( visitor, *shape1, t0, t1 );
		}
		else if ( shape0 ){
		
			ZLBounds bounds = prop1.GetModelBounds ();
			shape0->Process ( visitor, bounds, t0, t1 );
		}
		else if ( shape1 ) {
		
			ZLBounds bounds = prop0.GetModelBounds ();
			shape0->Process ( visitor, bounds, t0, t1 );
		}
	}
	else {
	
		MOAIOverlapBox shape0;
		shape0.mShape = prop0.GetModelBounds ();;
		shape0.mBounds = shape0.mShape;
		
		MOAIOverlapBox shape1;
		shape1.mShape = prop1.GetModelBounds ();;
		shape1.mBounds = shape1.mShape;
		
		visitor.Process ( shape0, shape1, t0, t1 );
	}
}

//----------------------------------------------------------------//
void MOAICollisionProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaClass ( state );
	MOAIIndexedPropBase::RegisterLuaClass ( state );
	
	MOAIDebugLinesMgr::Get ().ReserveStyleSet < MOAICollisionProp >( TOTAL_DEBUG_LINE_STYLES );
	
	state.SetField ( -1, "DEBUG_DRAW_COLLISION_PROP_MASTER",					MOAIDebugLinesMgr::Pack < MOAICollisionProp >( -1 ));
	state.SetField ( -1, "DEBUG_DRAW_COLLISION_ACTIVE_PROP_BOUNDS",				MOAIDebugLinesMgr::Pack < MOAICollisionProp >( DEBUG_DRAW_COLLISION_ACTIVE_PROP_BOUNDS ));
	state.SetField ( -1, "DEBUG_DRAW_COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS",		MOAIDebugLinesMgr::Pack < MOAICollisionProp >( DEBUG_DRAW_COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS ));
	state.SetField ( -1, "DEBUG_DRAW_COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS",		MOAIDebugLinesMgr::Pack < MOAICollisionProp >( DEBUG_DRAW_COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS ));
	state.SetField ( -1, "DEBUG_DRAW_COLLISION_OVERLAP_PROP_BOUNDS",			MOAIDebugLinesMgr::Pack < MOAICollisionProp >( DEBUG_DRAW_COLLISION_OVERLAP_PROP_BOUNDS ));
	state.SetField ( -1, "DEBUG_DRAW_COLLISION_OVERLAPS",						MOAIDebugLinesMgr::Pack < MOAICollisionProp >( DEBUG_DRAW_COLLISION_OVERLAPS ));
	state.SetField ( -1, "DEBUG_DRAW_COLLISION_WORLD_BOUNDS",					MOAIDebugLinesMgr::Pack < MOAICollisionProp >( DEBUG_DRAW_COLLISION_WORLD_BOUNDS ));
	
	state.SetField ( -1, "OVERLAP_EVENTS_ON_UPDATE",		( u32 )OVERLAP_EVENTS_ON_UPDATE );
	state.SetField ( -1, "OVERLAP_EVENTS_CONTINUOUS",		( u32 )OVERLAP_EVENTS_CONTINUOUS );
	state.SetField ( -1, "OVERLAP_EVENTS_LIFECYCLE",		( u32 )OVERLAP_EVENTS_LIFECYCLE );
	state.SetField ( -1, "OVERLAP_GRANULARITY_FINE",		( u32 )OVERLAP_GRANULARITY_FINE );
	state.SetField ( -1, "OVERLAP_CALCULATE_BOUNDS",		( u32 )OVERLAP_CALCULATE_BOUNDS );
	
	state.SetField ( -1, "CATEGORY_MASK_ALL",				( u32 )CATEGORY_MASK_ALL );
}

//----------------------------------------------------------------//
void MOAICollisionProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaFuncs ( state );
	MOAIIndexedPropBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "collisionMove",		_collisionMove },
		{ "getOverlaps",		_getOverlaps },
		{ "hasOverlaps",		_hasOverlaps },
		//{ "setGroupMask",		_setGroupMask },
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
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionProp::MOAIDrawable_DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIDebugLinesMgr& debugLines = MOAIDebugLinesMgr::Get ();
	if ( !( debugLines.IsVisible () && debugLines.SelectStyleSet < MOAICollisionProp >())) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();

	if ( debugLines.Bind ( MOAICollisionProp::DEBUG_DRAW_COLLISION_WORLD_BOUNDS )) {
		gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX ));
		draw.DrawBoxOutline ( this->GetWorldBounds ());
	}
	
	MOAICollisionShape* shape = this->GetCollisionShape ();
		
	if ( shape ) {
		const ZLAffine3D& localToWorldMtx = this->GetLocalToWorldMtx ();
		gfxMgr.mGfxState.SetMtx ( MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX, localToWorldMtx );
		shape->Draw ( localToWorldMtx );
	}

	bool visible = false;
	
	if ( this->IsActive ()) {
		
		if ( this->mOverlapLinks ) {
			visible = debugLines.Bind ( DEBUG_DRAW_COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS );
		}
		
		if ( this->mTouched == this->mOverlapPass && !visible ) {
			visible = debugLines.Bind ( DEBUG_DRAW_COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS );
		}
		
		if ( !visible ) {
			visible = debugLines.Bind ( DEBUG_DRAW_COLLISION_ACTIVE_PROP_BOUNDS );
		}
	}
	
	if ( this->mOverlapLinks && !visible ) {
		visible = debugLines.Bind ( DEBUG_DRAW_COLLISION_OVERLAP_PROP_BOUNDS );
	}
	
	if ( visible ) {
		
		MOAICollisionShape* shape = this->GetCollisionShape ();
		
		if ( shape ) {
			const ZLAffine3D& localToWorldMtx = this->GetLocalToWorldMtx ();
			gfxMgr.mGfxState.SetMtx ( MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX, localToWorldMtx );
			shape->Draw ( localToWorldMtx );
		}
		else {
			gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX ));
			draw.DrawBoxOutline ( this->GetWorldBounds ());
		}
	}
	
	if ( debugLines.Bind ( MOAICollisionProp::DEBUG_DRAW_COLLISION_OVERLAPS )) {
	
		gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX ));

		MOAIPropOverlapLink* overlapLinkIt = this->mOverlapLinks;
		for ( ; overlapLinkIt; overlapLinkIt = overlapLinkIt->mNext ) {
			const ZLBounds& bounds = overlapLinkIt->mOverlap->mBounds;
			if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) {
				draw.DrawBoxOutline ( bounds );
			}
		}
	}
}

//----------------------------------------------------------------//
bool MOAICollisionProp::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	if ( MOAIIndexedPropBase::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIPartitionHull::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAICollisionProp::MOAINode_Update () {
	
	MOAIPartitionHull::MOAINode_Update ();
	
	if ( this->mCollisionWorld && this->mOverlapFlags ) {
		this->mCollisionWorld->MakeActive ( *this );
	}
}

//----------------------------------------------------------------//
void MOAICollisionProp::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetWorldBounds ());
}

//----------------------------------------------------------------//
u32 MOAICollisionProp::MOAIPartitionHull_AffirmInterfaceMask ( MOAIPartition& partition ) {

	return (
		partition.AffirmInterfaceMask < MOAICollisionProp >() |
		partition.AffirmInterfaceMask < MOAIDrawable >()
	);
}

//----------------------------------------------------------------//
ZLBounds MOAICollisionProp::MOAIPartitionHull_GetModelBounds () {

	MOAICollisionShape* shape = this->GetCollisionShape ();
	if ( shape ) {
		ZLBounds bounds;
		bounds.Init ( shape->GetBounds ());
		return bounds;
	}
	return ZLBounds::EMPTY;
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
