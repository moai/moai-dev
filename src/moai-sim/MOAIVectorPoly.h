// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORPOLY_H
#define	MOAIVECTORPOLY_H

#include <moai-sim/MOAIVectorPath.h>

//================================================================//
// MOAIVectorPoly
//================================================================//
class MOAIVectorPoly :
	public MOAIVectorPath {
private:

public:
	
	//----------------------------------------------------------------//
	void			AddFillContours			( MOAIVectorDrawing& drawing, TESStesselator* tess );
	void			AddStrokeContours		( MOAIVectorDrawing& drawing, TESStesselator* tess );
					MOAIVectorPoly			();
					~MOAIVectorPoly			();
	
};

#endif
