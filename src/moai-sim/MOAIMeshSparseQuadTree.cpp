// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIMeshSparseQuadTree.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIMeshSparseQuadTreeBuilder
//================================================================//

//----------------------------------------------------------------//
void MOAIMeshSparseQuadTreeBuilder::Build ( MOAIMeshSparseQuadTree& meshPartition, const MOAIMeshPrimReader& primReader, u32 targetPrimsPerNode ) {

	meshPartition.Clear ();

	u32 totalMeshPrims = primReader.GetTotalPrims ();
	if ( !totalMeshPrims ) return;

	this->mPrims.Init ( totalMeshPrims );

	ZLRect bounds;

	u32 totalPrims = 0;
	for ( u32 i = 0; i < totalMeshPrims; ++i ) {
	
		MOAIMeshPrimCoords prim;
		if ( primReader.GetPrimCoords ( i, prim )) {
			
			MOAIMeshSparseQuadBuilderPrim& partitionPrim = this->mPrims [ totalPrims++ ];
			
			partitionPrim.mIndex = prim.mIndex;
			partitionPrim.mBounds = prim.GetBounds ();
			partitionPrim.mBounds.Bless ();
			partitionPrim.mNext = 0;
			
			ZLRect frame = partitionPrim.mBounds.GetRect ( ZLBox::PLANE_XY );
			
			if ( i == 0 ) {
				bounds = frame;
			}
			else {
				bounds.Grow ( frame );
			}
		}
	}
	
	if ( !totalPrims ) return;

	this->mTargetPrimsPerNode = targetPrimsPerNode;

	float width = MAX ( bounds.Width (), bounds.Height ());

	this->mWidth = 1.0f;
	while ( this->mWidth < width ) {
		this->mWidth = this->mWidth * 2.0f;
	}

	this->mTotalNodes = 0;
	this->mDepth = 0;
	this->mMaxDepth = 0;

	ZLQuadCoord coord;
	coord.Clear ();
	
	MOAIMeshSparseQuadTreeNode*	root = 0;
	for ( u32 i = 0; i < totalPrims; ++i ) {
		root = this->InsertPrim ( coord, root, this->mPrims [ i ]);
	}
	
	this->WritePrims ( root );
	
	size_t leafStreamLength = this->mLeafStream.GetCursor ();
	meshPartition.mMembers.Init ( leafStreamLength / sizeof ( MOAIMeshSparseQuadTreeLeaf ));
	this->mLeafStream.Seek ( 0 );
	this->mLeafStream.WriteBytes ( meshPartition.mMembers.Data (), leafStreamLength );
	
	meshPartition.mMaxDepth = this->mMaxDepth;
	meshPartition.mTotalNodes = this->mTotalNodes;
	
	meshPartition.mRoot = root;
}

//----------------------------------------------------------------//
bool MOAIMeshSparseQuadTreeBuilder::Descend ( ZLQuadCoord coord, Node* node, Prim& prim, bool force ) {

	if ( prim.mCanDescend ) {
		u32 child = ( prim.mYOff << 1 ) + prim.mXOff;
		if ( force || node->mChildren [ child ]) {
			coord.StepDown ();
			coord.Offset ( prim.mXOff, prim.mYOff );
			node->mChildren [ child ] = this->InsertPrim ( coord, node->mChildren [ child ], prim );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
MOAIMeshSparseQuadTreeNode* MOAIMeshSparseQuadTreeBuilder::InsertPrim ( ZLQuadCoord coord, MOAIMeshSparseQuadTreeNode* node, MOAIMeshSparseQuadBuilderPrim& prim ) {

	this->mDepth++;
	if ( this->mDepth > this->mMaxDepth ) {
		this->mMaxDepth = this->mDepth;
	}

	if ( !node ) {
		node = new MOAIMeshSparseQuadTreeNode ();
		this->mTotalNodes++;
	}

	prim.mCanDescend = false;

	ZLQuadCoord origin = coord;
	origin.Set ( 0, 0 );

	ZLRect cellBounds = origin.GetRect ( this->mWidth, this->mWidth );
	ZLRect primBounds = prim.mBounds.GetRect ( ZLBox::PLANE_XY );

	float tileSize = cellBounds.Width ();

	float xTile = floorf ( primBounds.mXMin / tileSize );
	float yTile = floorf ( primBounds.mYMin / tileSize );

	primBounds.Offset ( -( xTile * tileSize ), -( yTile * tileSize ));

	if ( cellBounds.Contains ( primBounds )) {
		
		float xMiddle = (( cellBounds.mXMin + cellBounds.mXMax ) / 2.0f );
		float yMiddle = (( cellBounds.mYMin + cellBounds.mYMax ) / 2.0f );
	
		if ( primBounds.mXMax < xMiddle ) {
		
			if ( primBounds.mYMax < yMiddle ) {
				
				prim.mCanDescend = true;
				prim.mXOff = 0;
				prim.mYOff = 0;
			}
			else if ( primBounds.mYMin > yMiddle ) {
			
				prim.mCanDescend = true;
				prim.mXOff = 0;
				prim.mYOff = 1;
			}
		}
		else if ( primBounds.mXMin > xMiddle ) {
		
			if ( primBounds.mYMax < yMiddle ) {
			
				prim.mCanDescend = true;
				prim.mXOff = 1;
				prim.mYOff = 0;
			}
			else if ( primBounds.mYMin > yMiddle ) {
			
				prim.mCanDescend = true;
				prim.mXOff = 1;
				prim.mYOff = 1;
			}
		}
	}
	
	if ( !this->Descend ( coord, node, prim, false )) {

		this->Push ( node, prim );

		if ( node->mTotalCanDescend && ( node->mTotalMembers > this->mTargetPrimsPerNode )) {
		
			MOAIMeshSparseQuadBuilderPrim* cursor = node->mMembers.mHead;
			node->mMembers.mHead	= 0;
			node->mTotalMembers		= 0;
			node->mTotalCanDescend	= 0;
		
			while ( cursor ) {
			
				MOAIMeshSparseQuadBuilderPrim* memberPrim = cursor;
				cursor = cursor->mNext;
			
				if ( !this->Descend ( coord, node, *memberPrim, true )) {
				
					this->Push ( node, *memberPrim );
				}
			}
		}
	}
	
	this->mDepth--;
	
	return node;
}

//----------------------------------------------------------------//
void MOAIMeshSparseQuadTreeBuilder::Push ( Node* node, Prim& prim ) {

	assert ( node );
	assert ( node->mMembers.mHead != &prim );

	prim.mNext = node->mMembers.mHead;
	node->mMembers.mHead = &prim;
	node->mTotalMembers++;
	
	if ( prim.mCanDescend ) {
		node->mTotalCanDescend++;
	}
}

//----------------------------------------------------------------//
void MOAIMeshSparseQuadTreeBuilder::WritePrims ( MOAIMeshSparseQuadTreeNode* node ) {

	if ( !node ) return;

	MOAIMeshSparseQuadBuilderPrim* prim = node->mMembers.mHead;
	
	node->mMembers.mFirstMemberIndex = ( u32 )( this->mLeafStream.GetCursor () / sizeof ( MOAIMeshSparseQuadTreeLeaf ));
	
	while ( prim ) {
		this->mLeafStream.Write < MOAIMeshSparseQuadTreeLeaf >( *prim );
		prim = prim->mNext;
	}
	
	for ( u32 i = 0; i < 4; ++i ) {
		this->WritePrims ( node->mChildren [ i ]);
	}
}

//================================================================//
// MOAIMeshSparseQuadTreeNode
//================================================================//

//----------------------------------------------------------------//
MOAIMeshSparseQuadTreeNode::MOAIMeshSparseQuadTreeNode () :
	mTotalChildren ( 0 ),
	mTotalMembers ( 0 ),
	mTotalCanDescend ( 0 ) {
	
	for ( u32 i = 0; i < 4; ++i ) {
		this->mChildren [ i ] = 0;
	}
	
	this->mMembers.mFirstMemberIndex = 0;
	this->mMembers.mHead = 0;
}

//----------------------------------------------------------------//
MOAIMeshSparseQuadTreeNode::~MOAIMeshSparseQuadTreeNode () {

	for ( u32 i = 0; i < 4; ++i ) {
	
		if ( this->mChildren [ i ]) {
			delete ( this->mChildren [ i ]);
			this->mChildren [ i ] = 0;
		}
	}
}

//----------------------------------------------------------------//
void MOAIMeshSparseQuadTreeNode::Print ( u32 indent ) {

	for ( u32 i = 0; i < indent; ++i ) {
		printf ( " " );
	}
	
	printf ( "node: %d childen, %d members\n", this->mTotalChildren, this->mTotalMembers );
	
	for ( u32 i = 0; i < 4; ++i ) {
		if ( this->mChildren [ i ]) {
			this->mChildren [ i ]->Print ( indent + 2 );
		}
	}
}

//================================================================//
// MOAIMeshSparseQuadTree
//================================================================//

//----------------------------------------------------------------//
void MOAIMeshSparseQuadTree::Clear () {

	if ( this->mRoot ) {
		delete ( this->mRoot );
		this->mRoot = 0;
	}
	this->mMembers.Clear ();
}

//----------------------------------------------------------------//
void MOAIMeshSparseQuadTree::Init ( const MOAIMeshPrimReader& primReader, u32 targetPrimsPerNode ) {

	this->Clear ();

	MOAIMeshSparseQuadTreeBuilder builder;
	builder.Build ( *this, primReader, targetPrimsPerNode );
}

//----------------------------------------------------------------//
MOAIMeshSparseQuadTree::MOAIMeshSparseQuadTree () :
	mRoot ( 0 ),
	mTotalNodes ( 0 ),
	mMaxDepth ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMeshSparseQuadTree::~MOAIMeshSparseQuadTree () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIMeshSparseQuadTree::Print () {

	printf ( "MOAIMeshSparseQuadTree: %d nodes, depth %d\n", this->mTotalNodes, this->mMaxDepth );

	if ( this->mRoot ) {
		this->mRoot->Print ( 0 );
	}
}
