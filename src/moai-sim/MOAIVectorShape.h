// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSHAPE_H
#define	MOAIVECTORSHAPE_H

#include <moai-sim/MOAIVectorTesselator.h>

class MOAIVertexFormat;
class SafeTesselator;

//================================================================//
// MOAIVectorShape
//================================================================//
class MOAIVectorShape {
protected:

	static const int		NVP			= 3;

	MOAIVectorStyle			mStyle;
	bool					mCanGroup;

	//----------------------------------------------------------------//
	void				CopyBoundaries				( SafeTesselator* dest, SafeTesselator* src );
	void				Stroke						( SafeTesselator* tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior );
	void				StrokeBoundaries			( SafeTesselator* tess, SafeTesselator* outline, float width, bool forward, bool interior );

public:

	friend class MOAIVectorTesselator;

	GET_SET ( MOAIVectorStyle&, Style, mStyle );
	
	//----------------------------------------------------------------//
	virtual int			AddFillContours				( SafeTesselator* tess );
	virtual int			AddStrokeContours			( SafeTesselator* tess, bool inside, bool outside );
	bool				CanGroup					();
	virtual bool		GroupShapes					( MOAIVectorShape** shapes, u32 total );
	virtual bool		IsClosed					() = 0;
						MOAIVectorShape				();
	virtual				~MOAIVectorShape			();
	virtual bool		SetVertices					( const ZLVec2D* vertices, u32 total, bool closed );
	
	int					Tesselate					( MOAIVectorTesselator& drawing, SafeTesselator* tess );
	int					Tesselate					( MOAIVectorTesselator& drawing, ZLStream* vertexStream, ZLStream* indexStream, MOAIVertexFormat* format );
};

#endif
