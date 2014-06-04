// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORCOMBO_H
#define	MOAIVECTORCOMBO_H

#include <moai-sim/MOAIVectorShape.h>

//================================================================//
// MOAIVectorGroup
//================================================================//
class MOAIVectorCombo :
	public MOAIVectorShape {
private:

	ZLLeanArray < MOAIVectorShape* > mShapes;

public:
	
	//----------------------------------------------------------------//
	void			AddFillContours			( TESStesselator* tess );
	bool			IsClosed				();
	bool			GroupShapes				( MOAIVectorShape** shapes, u32 total );
					MOAIVectorCombo			();
					~MOAIVectorCombo		();
};

#endif
