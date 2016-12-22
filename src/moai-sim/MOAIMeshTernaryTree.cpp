// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIMeshTernaryTree.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIMeshTernaryTreeBuilder
//================================================================//

//----------------------------------------------------------------//
void MOAIMeshTernaryTreeBuilder::Build ( MOAIMeshTernaryTree& meshPartition, const MOAIMeshPrimReader& primReader, u32 targetPrimsPerNode, u32 axisMask ) {

	u32 totalMeshPrims = primReader.GetTotalPrims ();
	if ( !totalMeshPrims ) return;

	this->mPrims.Init ( totalMeshPrims );

	u32 totalPrims = 0;
	for ( u32 i = 0; i < totalMeshPrims; ++i ) {
	
		MOAIMeshPrimCoords prim;
		if ( primReader.GetPrimCoords ( i, prim )) {
			
			MOAIMeshTernaryTreeBuilderPrim& partitionPrim = this->mPrims [ totalPrims++ ];
			
			partitionPrim.mIndex = prim.mIndex;
			partitionPrim.mBounds = prim.GetBounds ();
			partitionPrim.mBounds.Bless ();
		}
	}
	
	if ( !totalPrims ) return;
	
	// generate the initial set of sorted prims
	// since the prims will stay sorted during recursion, we only need to sort here
	PrimPtrArray sortedPrims [ 3 ];
	
	ZLLeanArray < PrimSortKey > sortBuffer;
	sortBuffer.Init ( totalMeshPrims * 2 );
	
	// master set of sorted prims
	for ( u32 i = 0; i < 3; ++i ) {
	
		// only need a buffer if it's an axis we care about
		if ( !( axisMask & ( 1 << i ))) continue;
	
		PrimPtrArray& sorted = sortedPrims [ i ];
		sorted.Init ( totalPrims );
	
		for ( u32 j = 0; j < totalPrims; ++j ) {
		
			const MOAIMeshTernaryTreeBuilderPrim& prim = this->mPrims [ j ];
		
			sortBuffer [ j ].mData = j;
			sortBuffer [ j ].mKey = ZLFloat::FloatToIntKey ( prim.mBounds.mMin.GetComponent ( i ));
		}
		
		PrimSortKey* sortedKeys = RadixSort32 < PrimSortKey >( sortBuffer, &sortBuffer [ totalPrims ], totalPrims );
		
		for ( u32 j = 0; j < totalPrims; ++j ) {
			sorted [ j ] = &this->mPrims [ sortedKeys [ j ].mData ];
		}
	}
	
	this->mTargetPrimsPerNode = targetPrimsPerNode;
	
	this->mOverlapSet.Init ( totalPrims );
	meshPartition.mRoot = this->BuildRecurse ( sortedPrims, totalPrims, axisMask );
	
	size_t indexRunStreamLength = this->mIndexRunStream.GetCursor ();
	meshPartition.mIndexRuns.Init ( indexRunStreamLength / 4 );
	this->mIndexRunStream.Seek ( 0 );
	this->mIndexRunStream.WriteBytes ( meshPartition.mIndexRuns.Data (), indexRunStreamLength );
}

//----------------------------------------------------------------//
MOAIMeshTernaryTreeNode* MOAIMeshTernaryTreeBuilder::BuildRecurse ( PrimPtrArray* sortedPrims, u32 totalPrims, u32 axisMask ) {

	if ( !totalPrims ) return 0;

	MOAIMeshTernaryTreeNode* node = new MOAIMeshTernaryTreeNode ();

	// pick the best axis and dividing planes
	
	u32 bestAxis		= INVALID;
	u32 bestBalance		= UINT32_MAX;
	float plane			= 0.0f;
	
	if ( totalPrims > this->mTargetPrimsPerNode ) {
	
		for ( u32 i = 0; i < 3; ++i ) {
		
			// skip components not in the axis mask
			if ( axisMask & ( 1 << i )) {
				this->Eval ( sortedPrims [ i ], totalPrims, i, bestAxis, bestBalance, plane );
			}
		}
	}
	
	if ( bestAxis == INVALID ) {
	
		// base of the index run
		node->mIndexRun = ( u32 )( this->mIndexRunStream.GetCursor () / 4 );
	
		// write the index run and return the node
		this->mIndexRunStream.Write < u32 >( totalPrims );
		
		// skip missing axes
		for ( u32 i = 0; i < 3; ++i ) {
		
			if ( !( axisMask & ( 1 << i ))) continue;
		
			PrimPtrArray& sortedPrimsAxis = sortedPrims [ i ];
	
			for ( u32 j = 0; j < totalPrims; ++j ) {
				this->mIndexRunStream.Write < u32 >( sortedPrimsAxis [ j ]->mIndex );
			}
			break;
		}
		return node;
	}
	
	node->mType			= bestAxis;
	node->mDistance		= plane;
	
	// classify all prims as in front of, overlapping or behind
	
	u32 totalFront		= 0;
	u32 totalBack		= 0;
	u32 totalOverlap	= 0;
	
	for ( u32 i = 0; i < totalPrims; ++i ) {
	
		MOAIMeshTernaryTreeBuilderPrim* prim = sortedPrims [ bestAxis ][ i ];
		
		float lower = prim->mBounds.mMin.GetComponent ( bestAxis );
		float upper = prim->mBounds.mMax.GetComponent ( bestAxis );
		
		if ((( lower == plane ) && ( upper == plane )) || (( lower < plane ) && ( upper > plane ))) {
			prim->mPosition = PRIM_OVERLAP;
			totalOverlap++;
		}
		else if ( lower < plane ) {
			prim->mPosition = PRIM_BACK;
			totalBack++;
		}
		else {
			prim->mPosition = PRIM_FRONT;
			totalFront++;
		}
	}
	
	assert ( totalFront && totalBack );
	
	// for each axis allocate three new buffers
	// copy all the front prims into the first buffer, preserving order
	// copy all the back prims into the second buffer, preserving order
	// copy all the overlap prims into the third buffer, preserving order
	
	PrimPtrArray frontPrims [ 3 ];
	PrimPtrArray backPrims [ 3 ];
	PrimPtrArray overlapPrims [ 3 ];
	
	for ( u32 i = 0; i < 3; ++i ) {
	
		if ( !( axisMask & ( 1 << i ))) continue;
	
		PrimPtrArray& frontPrimsAxis = frontPrims [ i ];
		PrimPtrArray& backPrimsAxis = backPrims [ i ];
		PrimPtrArray& overlapPrimsAxis = overlapPrims [ i ];
	
		frontPrimsAxis.Init ( totalFront );
		backPrimsAxis.Init ( totalBack );
		overlapPrimsAxis.Init ( totalOverlap );
		
		u32 frontCount = 0;
		u32 backCount = 0;
		u32 overlapCount = 0;
	
		const PrimPtrArray& sortedPrimsAxis = sortedPrims [ i ];
	
		for ( u32 j = 0; j < totalPrims; ++j ) {
		
			MOAIMeshTernaryTreeBuilderPrim* prim = sortedPrimsAxis [ j ];
			assert ( prim );
			
			u32 position = prim->mPosition;
			
			switch ( position ) {
			
				case PRIM_FRONT:
					frontPrimsAxis [ frontCount++ ] = prim;
					break;
				
				case PRIM_BACK:
					backPrimsAxis [ backCount++ ] = prim;
					break;
				
				case PRIM_OVERLAP:
					overlapPrimsAxis [ overlapCount++ ] = prim;
					break;
			}
		}
		
		assert ( frontCount == totalFront );
		assert ( backCount == totalBack );
		assert ( overlapCount == totalOverlap );
	}
	
	// recurse for each triplet of buffers
	// for the overlap buffers, ignore the diving plane
	
	node->mFront	= this->BuildRecurse ( frontPrims, totalFront, axisMask );
	node->mBack		= this->BuildRecurse ( backPrims, totalBack, axisMask );
	node->mOverlap	= this->BuildRecurse ( overlapPrims, totalOverlap, axisMask );
	
	return node;
}

//----------------------------------------------------------------//
void MOAIMeshTernaryTreeBuilder::CountPlaneOverlaps ( float lower, float upper, float plane, u32& frontCount, u32& backCount, u32& overlapCount ) {

	if ((( lower == plane ) && ( upper == plane )) || (( lower < plane ) && ( upper > plane ))) {
		overlapCount++;
		frontCount--;
	}
	else if ( lower < plane ) {
		assert ( upper <= plane );
		backCount++;
		frontCount--;
	}
}

//----------------------------------------------------------------//
void MOAIMeshTernaryTreeBuilder::Eval ( PrimPtrArray& sortedPrims, u32 totalPrims, u32 axis, u32& bestAxis, u32& bestBalance, float& plane ) {

	this->mOverlapTop = 0;
	this->mOverlapMin = 0.0f;

	float lowerBound = sortedPrims [ 0 ]->mBounds.mMin.GetComponent ( axis );
	float upperBound = lowerBound;

	for ( u32 i = 0; i < totalPrims; ++i ) {
	
		MOAIMeshTernaryTreeBuilderPrim* prim = sortedPrims [ i ];
		
		prim->mLower = prim->mBounds.mMin.GetComponent ( axis );
		prim->mUpper = prim->mBounds.mMax.GetComponent ( axis );
		
		if ( upperBound < prim->mUpper ) {
			upperBound = prim->mUpper;
		}
	}

	u32 backCount = 0;

	for ( u32 i = 0; i < totalPrims; ++i ) {
	
		MOAIMeshTernaryTreeBuilderPrim* prim = sortedPrims [ i ];
		this->mOverlapSet [ this->mOverlapTop++ ] = prim;
		
		float lower = prim->mLower;
		float upper = prim->mUpper;
		
		// overlap counts
		u32 lowerFrontCount		= totalPrims - backCount;
		u32 lowerBackCount		= backCount;
		u32 lowerOverlapCount	= 0;
		
		u32 upperFrontCount		= totalPrims - backCount;
		u32 upperBackCount		= backCount;
		u32 upperOverlapCount	= 0;
		
		u32 overlapTop = this->mOverlapTop;
		this->mOverlapTop = 0;
		
		for ( u32 j = 0; j < overlapTop; ++j ) {
		
			MOAIMeshTernaryTreeBuilderPrim* overlapPrim = this->mOverlapSet [ j ];
			
			if ( overlapPrim->mUpper >= lower ) {
				this->mOverlapSet [ this->mOverlapTop++ ] = overlapPrim;
			}
			else {
				backCount++;
			}
			
			this->CountPlaneOverlaps ( overlapPrim->mLower, overlapPrim->mUpper, lower, lowerFrontCount, lowerBackCount, lowerOverlapCount );
			this->CountPlaneOverlaps ( overlapPrim->mLower, overlapPrim->mUpper, upper, upperFrontCount, upperBackCount, upperOverlapCount );
		}
		
		for ( u32 j = i + 1; j < totalPrims; ++j ) {
			
			MOAIMeshTernaryTreeBuilderPrim* overlapPrim = sortedPrims [ j ];
		
			this->CountPlaneOverlaps ( overlapPrim->mLower, overlapPrim->mUpper, lower, lowerFrontCount, lowerBackCount, lowerOverlapCount );
			this->CountPlaneOverlaps ( overlapPrim->mLower, overlapPrim->mUpper, upper, upperFrontCount, upperBackCount, upperOverlapCount );
		
			if ( overlapPrim->mLower > upper ) {
			
				break;
			}
		}
		
		assert (( lowerBackCount + lowerOverlapCount + lowerFrontCount ) == totalPrims );
		assert (( upperBackCount + upperOverlapCount + upperFrontCount ) == totalPrims );
		
		if ( lowerBackCount && lowerFrontCount ) {
			
			u32 balance = this->GetBalance ( lowerFrontCount, lowerBackCount );
		
			if ( balance < bestBalance ) {
				
				bestAxis		= axis;
				bestBalance		= balance;
				plane			= lower;
			}
		}
		
		if ( lower != upper ) {
			
			if ( upperBackCount && upperFrontCount ) {
				
				u32 balance = this->GetBalance ( upperFrontCount, upperBackCount );
				
				if ( balance < bestBalance ) {
					
					bestAxis		= axis;
					bestBalance		= balance;
					plane			= upper;
				}
			}
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIMeshTernaryTreeBuilder::GetBalance ( u32 frontCount, u32 backCount ) {

	return backCount < frontCount ? frontCount - backCount : backCount - frontCount;
}

//================================================================//
// MOAIMeshTernaryTree
//================================================================//

//----------------------------------------------------------------//
MOAIMeshTernaryTreeNode::MOAIMeshTernaryTreeNode () :
	mType ( LEAF ),
	mDistance ( 0.0f ),
	mIndexRun ( 0 ),
	mFront ( 0 ),
	mBack ( 0 ),
	mOverlap ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMeshTernaryTreeNode::~MOAIMeshTernaryTreeNode () {

	if ( this->mFront ) {
		delete ( this->mFront );
	}
	
	if ( this->mBack ) {
		delete ( this->mBack );
	}
	
	if ( this->mOverlap ) {
		delete ( this->mOverlap );
	}
}

//================================================================//
// MOAIMeshTernaryTree
//================================================================//

//----------------------------------------------------------------//
void MOAIMeshTernaryTree::Clear () {

	if ( this->mRoot ) {
		delete ( this->mRoot );
		this->mRoot = 0;
	}
	this->mIndexRuns.Clear ();
}

//----------------------------------------------------------------//
void MOAIMeshTernaryTree::Init ( const MOAIMeshPrimReader& primReader, u32 targetPrimsPerNode, u32 axisMask ) {

	this->Clear ();

	MOAIMeshTernaryTreeBuilder builder;
	builder.Build ( *this, primReader, targetPrimsPerNode, axisMask );
}

//----------------------------------------------------------------//
MOAIMeshTernaryTree::MOAIMeshTernaryTree () :
	mRoot ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMeshTernaryTree::~MOAIMeshTernaryTree () {
}
