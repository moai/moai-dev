// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORELLIPSE_H
#define	MOAIVECTORELLIPSE_H

#include <moai-sim/MOAIVectorShape.h>

class SafeTesselator;

//================================================================//
// MOAIVectorEllipse
//================================================================//
class MOAIVectorEllipse :
	public MOAIVectorShape {
private:

	ZLVec2D			mLoc;
	float			mXRad;
	float			mYRad;

public:
	
	//----------------------------------------------------------------//
	int				AddFillContours			( SafeTesselator* tess );
	int				AddStrokeContours		( SafeTesselator* tess );
	void			Init					( float x, float y, float xRad, float yRad );
	bool			IsClosed				();
					MOAIVectorEllipse		();
					~MOAIVectorEllipse		();
	
};

#endif
