// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHSPARSEQUADTREE_H
#define	MOAIMESHSPARSEQUADTREE_H

#include <moai-sim/MOAIMeshPartition.h>

class MOAIMeshSparseQuadTree;
class MOAIMeshSparseQuadTreeNode;

//================================================================//
// MOAIMeshSparseQuadTreeLeaf
//================================================================//
class MOAIMeshSparseQuadTreeLeaf {
private:

	friend class MOAIMeshSparseQuadTree;
	friend class MOAIMeshSparseQuadTreeBuilder;
	
	u32			mIndex;
	ZLBox		mBounds;
};

//================================================================//
// MOAIMeshSparseQuadBuilderPrim
//================================================================//
class MOAIMeshSparseQuadBuilderPrim :
	public MOAIMeshSparseQuadTreeLeaf {
private:

	friend class MOAIMeshSparseQuadTree;
	friend class MOAIMeshSparseQuadTreeBuilder;
	
	bool	mCanDescend;	// true if this prim descend to next level of quad tree
	u32		mXOff;			// x tile offset for descent
	u32		mYOff;			// y tile offset for descent
	
	MOAIMeshSparseQuadBuilderPrim*		mNext;
};

//================================================================//
// MOAIMeshSparseQuadTreeBuilder
//================================================================//
class MOAIMeshSparseQuadTreeBuilder {
private:

	friend class MOAIMeshSparseQuadTree;
	
	typedef MOAIMeshSparseQuadBuilderPrim		Prim;
	typedef ZLLeanArray < Prim >				PrimArray;
	typedef MOAIMeshSparseQuadTreeNode			Node;
	
	
	PrimArray		mPrims;
	u32				mTargetPrimsPerNode;
	float			mWidth;

	ZLMemStream		mLeafStream;
	
	u32				mTotalNodes;
	u32				mDepth;
	u32				mMaxDepth;

	//----------------------------------------------------------------//
	void			Build					( MOAIMeshSparseQuadTree& meshPartition, const MOAIMeshPrimReader& primReader, u32 targetPrimsPerNode );
	bool			Descend					( ZLQuadCoord coord, Node* node, Prim& prim, bool force );
	Node*			InsertPrim				( ZLQuadCoord coord, Node* node, Prim& prim );
	void			Push					( Node* node, Prim& prim );
	void			WritePrims				( Node* node );
};

//================================================================//
// MOAIMeshSparseQuadTreeNode
//================================================================//
class MOAIMeshSparseQuadTreeNode {
private:

	friend class MOAIMeshSparseQuadTree;
	friend class MOAIMeshSparseQuadTreeBuilder;
	
	enum {
		LEFT_TOP,
		RIGHT_TOP,
		LEFT_BOTTOM,
		RIGHT_BOTTOM,
	};
	
	u32		mTotalChildren;
	u32		mTotalMembers;
	u32		mTotalCanDescend;
	
	union {
		u32									mFirstMemberIndex;
		MOAIMeshSparseQuadBuilderPrim*		mHead;
		
	} mMembers;
	
	MOAIMeshSparseQuadTreeNode*		mChildren [ 4 ];
	
	//----------------------------------------------------------------//
				MOAIMeshSparseQuadTreeNode		();
				~MOAIMeshSparseQuadTreeNode		();
	void		Print							( u32 indent );
};

//================================================================//
// MOAIMeshSparseQuadTree
//================================================================//
class MOAIMeshSparseQuadTree :
	public MOAIMeshPartition {
private:

	friend class MOAIMeshSparseQuadTreeBuilder;

	MOAIMeshSparseQuadTreeNode*						mRoot;
	
	u32	mTotalNodes;
	u32 mMaxDepth;
	
	ZLLeanArray < MOAIMeshSparseQuadTreeLeaf >		mMembers;		// runs of indices per grid cell. first entry per cell is # of indices in the run.

public:
	
	static const u32	DEFAULT_TARGET_PRIMS_PER_NODE = 4;
	
	//----------------------------------------------------------------//
	void		Clear						();
	void		Init						( const MOAIMeshPrimReader& primReader, u32 targetPrimsPerNode );
				MOAIMeshSparseQuadTree		();
				~MOAIMeshSparseQuadTree		();
	void		Print						();
};

#endif
