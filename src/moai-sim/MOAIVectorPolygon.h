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

	ZLLeanArray < ZLVec2D > mVertices;

	//----------------------------------------------------------------//
	void			ToStroke				( TESStesselator* outline, float offset );

public:
	
	//----------------------------------------------------------------//
	bool			GroupVertices			( MOAIVectorDrawing& drawing, u32 total );
					MOAIVectorPolygon		();
					~MOAIVectorPolygon		();
	void			SetVertices				( ZLVec2D* vertices, u32 total );
	void			ToOutline				( TESStesselator* outline );
};

#endif
