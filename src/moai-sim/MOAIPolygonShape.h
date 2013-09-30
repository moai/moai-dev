// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOLYGONSHAPE_H
#define	MOAIPOLYGONSHAPE_H

#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIShape.h>
#include <moai-sim/MOAIVertexBuffer.h>

struct TESStesselator;

//================================================================//
// MOAIPolygonShape
//================================================================//
class MOAIPolygonShape :
	public MOAIShape {
private:

	TESStesselator*	mTess;
	TESStesselator*	mOutline;

	MOAIIndexBuffer		mIdxBuffer;
	MOAIVertexBuffer	mVtxBuffer;

	MOAIIndexBuffer		mContourIdxBuffer;
	MOAIVertexBuffer	mContourVtxBuffer;

	//----------------------------------------------------------------//
	static void		DumpContourIndices		( TESStesselator* tess, MOAIIndexBuffer& idxBuffer );
	static void		DumpTriangleIndices		( TESStesselator* tess, MOAIIndexBuffer& idxBuffer );
	static void		DumpVertices			( TESStesselator* tess, MOAIVertexBuffer& vtxBuffer );
	static void		DrawVertex				( u32 index, const float* verts );
	void			RenderContours			( u32 primType );
	void			RenderPrims				( u32 primType );

public:
	
	//----------------------------------------------------------------//
	void			AddContour				( USVec2D* verts, u32 total );
	void			Clear					();
					MOAIPolygonShape		();
					~MOAIPolygonShape		();
	void			Render					();
	void			Tesselate				( u32 windingRule, bool preTessalate );
};

#endif
