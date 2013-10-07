// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSHAPE_H
#define	MOAIVECTORSHAPE_H

#include <moai-sim/MOAIVectorStyle.h>

struct TESStesselator;
class MOAIVectorDrawing;

//================================================================//
// MOAIVectorShape
//================================================================//
class MOAIVectorShape {
protected:

	MOAIVectorStyle		mStyle;
	bool				mOpen;

	//----------------------------------------------------------------//
	void				StrokeContours				( TESStesselator* outline, TESStesselator* stroke, float offset );

public:

	friend class MOAIVectorDrawing;

	GET_SET ( MOAIVectorStyle&, Style, mStyle );
	GET_SET ( bool, Open, mOpen );
	
	//----------------------------------------------------------------//
	virtual bool		GroupShapes					( MOAIVectorShape** shapes, u32 total );
	virtual bool		GroupVertices				( MOAIVectorDrawing& drawing, u32 total );
						MOAIVectorShape				();
	virtual				~MOAIVectorShape			();
	virtual void		Tessalate					( MOAIVectorDrawing& drawing );
	virtual void		ToOutline					( TESStesselator* outline ) = 0;
	virtual void		ToStroke					( TESStesselator* outline, TESStesselator* triangles );
	virtual void		ToTriangles					( TESStesselator* outline, TESStesselator* triangles );
};

#endif
