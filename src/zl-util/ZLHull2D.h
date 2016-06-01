// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLHULL_H
#define	ZLHULL_H

#include <zl-util/ZLResult.h>
#include <zl-util/ZLVec2D.h>

class ZLStream;

//================================================================//
// ZLHull2D
//================================================================//
class ZLHull2D {
private:

	//----------------------------------------------------------------//
	static int			MonotoneChainComp		( const void* p1, const void* p2 );

public:

	typedef float VertexComponentType;
	static const size_t VERTEX_SIZE = 2 * sizeof ( VertexComponentType );

	enum {
		SORT_NONE,
		SORT_RADIX,
		SORT_CSTDLIB,
	};

	//----------------------------------------------------------------//
	static ZLSizeResult		MonotoneChain			( ZLVec2D* hull, ZLVec2D* points, size_t nPoints, u32 sort = SORT_CSTDLIB ); // hull buffer *must* be at least 2x the size of points
	static ZLSizeResult		MonotoneChain			( ZLStream& output, ZLStream& input, size_t nPoints, u32 sort = SORT_CSTDLIB );
};

#endif
