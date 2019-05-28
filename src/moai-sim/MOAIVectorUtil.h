// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORUTIL_H
#define	MOAIVECTORUTIL_H

#include <moai-sim/MOAIVectorStyle.h>

class MOAIIndexBufferGL;
class MOAIVectorShape;
class MOAIVertexBufferGL;
class MOAIVertexFormatGL;

struct TESStesselator;
typedef float TESSreal;

//================================================================//
// SafeTesselator
//================================================================//
class SafeTesselator {
public:

	static const ZLVec3D	sNormal;

	struct TESStesselator*	mTess;

	//----------------------------------------------------------------//
	void			AddContour2D			( const ZLVec2D* vertices, size_t numVertices );
	void			AddContour2D			( const ZLVec2D* vertices, size_t numVertices, float precision );
	void			CopyBoundaries 			( const SafeTesselator& src );
	u32				GetTriangles			( MOAIVertexFormatGL& format, ZLStream& vtxStream, ZLStream& idxStream );
	u32				GetTriangles			( MOAIVertexFormatGL& format, MOAIVertexBufferGL& vtxBuffer, MOAIIndexBufferGL& idxBuffer, u32 idxSizeInBytes );
	void			Print					();
	void			Reset					();
					SafeTesselator			();
					~SafeTesselator			();
	int				Tesselate				( int windingRule, int elementType, int polySize, int vertexSize, const TESSreal* normal = 0 );
};

//================================================================//
// MOAIVectorLineJoin
//================================================================//
class MOAIVectorLineJoin {
public:

		ZLVec2D		mVertex;		// the join vertex
		ZLVec2D		mEdgeVec;		// vector of the next edge
		ZLVec2D		mEdgeNorm;		// normal of preceding edge
		ZLVec2D		mJointNorm;		// avg normal (same as N0 or N1 if path not closed)
		bool		mIsCap;
};

//================================================================//
// MOAIVectorUtil
//================================================================//
class MOAIVectorUtil {
private:

	//----------------------------------------------------------------//
	static int		StrokeWedge				( const MOAIVectorStyle& style, ZLVec2D*& verts, const ZLVec2D& origin, const ZLVec2D& n0, const ZLVec2D& n1, float width );
	
public:
	
	//----------------------------------------------------------------//
	static void		ComputeLineJoins		( MOAIVectorLineJoin* joins, const ZLVec2D* verts, int nVerts, bool open, bool forward, bool interior );
	static u32		GetTriangles			( ZLStream& vtxStream, MOAIVertexBufferGL& vtxBuffer, ZLStream& idxStream, MOAIIndexBufferGL& idxBuffer, u32 idxSizeInBytes );
	static int		StrokeLine				( const MOAIVectorStyle& style, ZLVec2D* verts, const MOAIVectorLineJoin* joins, int nJoins, float width, bool exact );
};

#endif
