//[MOAI LICENSE]

#include "pch.h"
#include <moaicore/MOAICollisionMgr.h>
#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIMultiShape2D.h>
#include <moaicore/MOAIOverlapPrim2D.h>

//================================================================//
// MOAICollisionMgr
//================================================================//

//----------------------------------------------------------------//
MOAIOverlapLink2D* MOAICollisionMgr::AllocLink () {

	if ( !this->mFreeLinks ) {
		this->MoreLinks ();
	}

	if ( this->mFreeLinks ) {
		MOAIOverlapLink2D* link = this->mFreeLinks;
		this->mFreeLinks = link->mNext;
		return link;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAICollisionMgr::FreeLink ( MOAIOverlapLink2D* link ) {

	link->mNext = this->mFreeLinks;
	this->mFreeLinks = link;
}

//----------------------------------------------------------------//
MOAICollisionMgr::MOAICollisionMgr () :
	mFreeLinks ( 0 ),
	mOverlapPrims ( 0 ) {
}

//----------------------------------------------------------------//
MOAICollisionMgr::~MOAICollisionMgr () {

	u32 size = this->mBlocks.Size ();
	for ( u32 i = 0; i < size; ++i ) {
		free ( this->mBlocks [ i ]);
	}
	this->mFreeLinks = 0;
}

//----------------------------------------------------------------//
void MOAICollisionMgr::MoreLinks () {

	u32 size = this->mBlocks.Size ();
	if ( size >= MAX_BLOCKS ) return;
	
	MOAIOverlapLink2D* block = ( MOAIOverlapLink2D* )malloc ( BLOCK_SIZE * sizeof ( MOAIOverlapLink2D ));
	
	for ( u32 i = 0; i < BLOCK_SIZE; ++i ) {
		MOAIOverlapLink2D& link = block [ i ];
		link.mNext = this->mFreeLinks;
		this->mFreeLinks = &link;
	}
	
	this->mBlocks.Resize ( size + 1 );
	this->mBlocks [ size ] = block;
}

//----------------------------------------------------------------//
void MOAICollisionMgr::ProcessOverlaps ( MOAIOverlapPrim2D& overlapPrim ) {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	bool drawDebug = debugLines.Bind ( MOAIDebugLines::OVERLAPS );
	debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );

	overlapPrim.mOverlapState = MOAIOverlapPrim2D::OVERLAP_DONE;
	overlapPrim.WeakenLinks ();

	u32 results = 0;
	
	USPartition < MOAIPrim >* partition = overlapPrim.GetPartition ();
	if ( partition ) {
		USRect worldBounds = overlapPrim.GetWorldBounds ();
		results = partition->GatherPrims ( worldBounds, &overlapPrim, MOAIContentLibrary2D::CAN_GET_OVERLAP_PRIM );
	}
	
	if ( results ) {
	
		MOAIMultiShape2D s0;
		overlapPrim.GetShape ( s0 );
	
		for ( u32 i = 0; i < results; ++i ) {
		
			MOAIOverlapPrim2D* prim = partition->GetResult ( i )->IsOverlapPrim2D ();
			if ( prim ) {
				
				if ( prim->mOverlapState == MOAIOverlapPrim2D::OVERLAP_DONE ) continue;
				
				u32 send = ( overlapPrim.mSendFlags & prim->mReceiveFlags );
				u32 receive = ( overlapPrim.mReceiveFlags & prim->mSendFlags );
				
				if ( !( send || receive )) continue;
				
				MOAIMultiShape2D s1;
				prim->GetShape ( s1 );
				
				USRect rect;
				if ( s0.Intersect ( s1, rect )) {
					
					USVec2D center;
					rect.GetCenter ( center );
					
					overlapPrim.MakeLink ( *prim, center );
					
					if ( drawDebug ) {
						debugLines.DrawRect ( rect );
					}
				}
			}
		}
	}
	
	overlapPrim.PurgeWeakLinks ();
}

//----------------------------------------------------------------//
void MOAICollisionMgr::ScheduleOverlap ( MOAIOverlapPrim2D& overlapPrim ) {

	if ( overlapPrim.mOverlapState != MOAIOverlapPrim2D::OVERLAP_IDLE ) return;
	if ( !overlapPrim.mIsActive ) return;

	overlapPrim.Retain ();

	overlapPrim.mNext = this->mOverlapPrims;
	this->mOverlapPrims = &overlapPrim;
	
	overlapPrim.mOverlapState = MOAIOverlapPrim2D::OVERLAP_SCHEDULED;
}

//----------------------------------------------------------------//
void MOAICollisionMgr::Update () {

	for ( MOAIOverlapPrim2D* overlapPrim = this->mOverlapPrims; overlapPrim; overlapPrim = overlapPrim->mNext ) {
		this->ProcessOverlaps ( *overlapPrim );
	}

	MOAIOverlapPrim2D* cursor = this->mOverlapPrims;
	this->mOverlapPrims = 0;
	
	while ( cursor ) {
	
		MOAIOverlapPrim2D* overlapPrim = cursor;
		cursor = cursor->mNext;
		
		overlapPrim->mOverlapState = MOAIOverlapPrim2D::OVERLAP_IDLE;
		overlapPrim->Release ();
	}
}

