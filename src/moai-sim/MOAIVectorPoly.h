// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORPOLY_H
#define	MOAIVECTORPOLY_H

#include <moai-sim/MOAIVectorShape.h>

//================================================================//
// MOAIVectorPoly
//================================================================//
class MOAIVectorPoly :
	public MOAIVectorShape {
protected:

	ZLLeanArray < ZLVec2D > mVertices;
	bool mIsClosed;

public:
	
	//----------------------------------------------------------------//
	int				AddFillContours			( SafeTesselator* tess );
	int				AddStrokeContours		( SafeTesselator* tess );
	bool			IsClosed				();
					MOAIVectorPoly			();
					~MOAIVectorPoly			();
	bool			SetVertices				( const ZLVec2D* vertices, u32 total, bool closed );
};

#endif
