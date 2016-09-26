// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHTERNARYTREE_H
#define	MOAIMESHTERNARYTREE_H

#include <moai-sim/MOAIMeshPartition.h>

class MOAIMesh;
class MOAIMeshTernaryTreeBuilder;
class MOAIMeshTernaryTree;
class MOAIMeshTernaryTreeNode;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIMeshTernaryTreeBuilderPrim
//================================================================//
// TODO: doxygen
class MOAIMeshTernaryTreeBuilderPrim {
private:

	friend MOAIMeshTernaryTree;
	friend MOAIMeshTernaryTreeBuilder;

	u32				mIndex;			// the mesh component index (into either the vertex buffer, or the index buffer if the mesh has one)
	ZLBox			mBounds;		// bounds of the prim
	u32				mPosition;		// front, back or overlap
	
	// cache the component bounds during each eval pass
	float			mLower;
	float			mUpper;
};

//================================================================//
// MOAIMeshTernaryTreeBuilder
//================================================================//
// TODO: doxygen
class MOAIMeshTernaryTreeBuilder {
private:

	friend class MOAIMeshTernaryTree;

	static const u32 INVALID			= ( u32 )-1;

	enum {
		PRIM_FRONT,
		PRIM_BACK,
		PRIM_OVERLAP,
	};
	
	typedef ZLLeanArray < MOAIMeshTernaryTreeBuilderPrim >	PrimArray;
	typedef ZLLeanArray < MOAIMeshTernaryTreeBuilderPrim* >	PrimPtrArray;
	typedef ZLRadixKey32 < u32 >							PrimSortKey;
	typedef MOAIMeshTernaryTreeNode							Node;
	
	PrimArray		mPrims;
	u32				mTargetPrimsPerNode;

	PrimPtrArray	mOverlapSet;
	u32				mOverlapTop;
	float			mOverlapMin;

	ZLMemStream		mIndexRunStream;

	//----------------------------------------------------------------//
	void			Build					( MOAIMeshTernaryTree& meshPartition, const MOAIMeshPrimReader& primReader, u32 targetPrimsPerNode, u32 axisMask );
	Node*			BuildRecurse			( PrimPtrArray* sortedPrims, u32 totalPrims, u32 axisMask );
	void			CountPlaneOverlaps		( float lower, float upper, float plane, u32& frontCount, u32& backCount, u32& overlapCount );
	void			Eval					( PrimPtrArray& sortedPrims, u32 totalPrims, u32 axis, u32& bestAxis, u32& bestBalance, float& plane );
	u32				GetBalance				( u32 frontCount, u32 backCount );
};

//================================================================//
// MOAIMeshTernaryTreeNode
//================================================================//
// TODO: doxygen
class MOAIMeshTernaryTreeNode {
private:

	friend class MOAIMeshTernaryTree;
	friend class MOAIMeshTernaryTreeBuilder;
	
	enum {
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		LEAF,
	};
	
	u32			mType;
	float		mDistance;
	u32			mIndexRun;
	
	MOAIMeshTernaryTreeNode*	mFront;
	MOAIMeshTernaryTreeNode*	mBack;
	MOAIMeshTernaryTreeNode*	mOverlap;
	
	//----------------------------------------------------------------//
			MOAIMeshTernaryTreeNode			();
			~MOAIMeshTernaryTreeNode		();
};

//================================================================//
// MOAIMeshTernaryTree
//================================================================//
// TODO: doxygen
class MOAIMeshTernaryTree :
	public MOAIMeshPartition {
private:

	friend class MOAIMeshTernaryTreeBuilder;

	// decided to go with an axis-aligned BSP tree. fast to compute, sparse
	// and avoids polygon splitting.

	MOAIMeshTernaryTreeNode*	mRoot;
	
	ZLLeanArray < u32 >		mIndexRuns;		// runs of indices per grid cell. first entry per cell is # of indices in the run.

public:
	
	static const u32	DEFAULT_TARGET_PRIMS_PER_NODE = 32;
	
	static const u32	X_AXIS_MASK		= 0x01;
	static const u32	Y_AXIS_MASK		= 0x02;
	static const u32	Z_AXIS_MASK		= 0x04;
	
	static const u32	AXIS_MASK_ALL	= 0x07;
	
	//----------------------------------------------------------------//
	void		Clear						();
	void		Init						( const MOAIMeshPrimReader& primReader, u32 targetPrimsPerNode, u32 axisMask = AXIS_MASK_ALL );
				MOAIMeshTernaryTree			();
				~MOAIMeshTernaryTree		();
};

#endif
