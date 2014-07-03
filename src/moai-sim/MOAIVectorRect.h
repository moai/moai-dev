// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORRECT_H
#define	MOAIVECTORRECT_H

#include <moai-sim/MOAIVectorShape.h>

//================================================================//
// MOAIVectorRect
//================================================================//
class MOAIVectorRect :
	public MOAIVectorShape {
private:

	ZLRect			mRect;

public:
	
	//----------------------------------------------------------------//
	int				AddFillContours			( SafeTesselator* tess );
	int				AddStrokeContours		( SafeTesselator* tess );
	void			Init					( float xMin, float yMin, float xMax, float yMax );
	bool			IsClosed				();
					MOAIVectorRect			();
					~MOAIVectorRect			();
	
};

#endif
