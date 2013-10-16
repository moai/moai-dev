// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSHAPE_H
#define	MOAIVECTORSHAPE_H

#include <moai-sim/MOAIVectorDrawing.h>

struct TESStesselator;

//================================================================//
// MOAIVectorShape
//================================================================//
class MOAIVectorShape {
protected:

	static const int		NVP			= 3;
	static const ZLVec3D	sNormal;

	MOAIVectorStyle			mStyle;
	bool					mOpen;

	//----------------------------------------------------------------//
	void				CopyBoundaries				( TESStesselator* dest, TESStesselator* src );
	void				Stroke						( MOAIVectorDrawing& drawing, TESStesselator* tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior );
	void				StrokeBoundaries			( MOAIVectorDrawing& drawing, TESStesselator* tess, TESStesselator* outline, float width, bool forward, bool interior );

public:

	friend class MOAIVectorDrawing;

	GET_SET ( MOAIVectorStyle&, Style, mStyle );
	GET_SET ( bool, Open, mOpen );
	
	//----------------------------------------------------------------//
	virtual void		AddFillContours				( MOAIVectorDrawing& drawing, TESStesselator* tess );
	virtual void		AddStrokeContours			( MOAIVectorDrawing& drawing, TESStesselator* tess );
	virtual bool		GroupShapes					( MOAIVectorShape** shapes, u32 total );
	virtual bool		GroupVertices				( MOAIVectorDrawing& drawing, u32 total );
						MOAIVectorShape				();
	virtual				~MOAIVectorShape			();
	virtual void		Tessalate					( MOAIVectorDrawing& drawing );
};

#endif
