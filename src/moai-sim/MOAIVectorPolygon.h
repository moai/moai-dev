// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORPOLYGON_H
#define	MOAIVECTORPOLYGON_H

#include <moai-sim/MOAIVectorShape.h>

//================================================================//
// MOAIVectorPolygon
//================================================================//
class MOAIVectorPolygon :
	public MOAIVectorShape {
private:

	ZLLeanArray < USVec2D > mVertices;

public:
	
	//----------------------------------------------------------------//
					MOAIVectorPolygon		();
					~MOAIVectorPolygon		();
	void			SetVertices				( USVec2D* vertices, u32 total );
	void			ToOutline				( TESStesselator* outline );
};

#endif
