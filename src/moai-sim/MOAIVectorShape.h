// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSHAPE_H
#define	MOAIVECTORSHAPE_H

#include <moai-sim/MOAIVectorStyle.h>

struct TESStesselator;
class MOAIVectorDrawing;

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
// MOAIVectorShape
//================================================================//
class MOAIVectorShape {
protected:

	static const int		NVP			= 3;
	static const ZLVec3D	sNormal;

	MOAIVectorStyle			mStyle;
	bool					mOpen;

	//----------------------------------------------------------------//
	void				ComputeLineJoins			( MOAIVectorLineJoin* joins, const ZLVec2D* verts, int nVerts, bool open, bool forward, bool interior );
	void				CopyBoundaries				( TESStesselator* dest, TESStesselator* src );
	void				Stroke						( TESStesselator* tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior );
	int					Stroke						( ZLVec2D* verts, const MOAIVectorLineJoin* joins, int nJoins, float width, bool exact );
	void				StrokeBoundaries			( TESStesselator* tess, TESStesselator* outline, float width, bool forward, bool interior );

public:

	friend class MOAIVectorDrawing;

	GET_SET ( MOAIVectorStyle&, Style, mStyle );
	GET_SET ( bool, Open, mOpen );
	
	//----------------------------------------------------------------//
	virtual void		AddFillContours				( TESStesselator* tess );
	virtual void		AddStrokeContours			( TESStesselator* tess );
	virtual bool		GroupShapes					( MOAIVectorShape** shapes, u32 total );
	virtual bool		GroupVertices				( MOAIVectorDrawing& drawing, u32 total );
						MOAIVectorShape				();
	virtual				~MOAIVectorShape			();
	virtual void		Tessalate					( MOAIVectorDrawing& drawing );
};

#endif
