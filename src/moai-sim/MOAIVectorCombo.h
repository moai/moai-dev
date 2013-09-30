// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORCOMBO_H
#define	MOAIVECTORCOMBO_H

#include <moai-sim/MOAIVectorShape.h>

//================================================================//
// MOAIVectorCombo
//================================================================//
class MOAIVectorCombo :
	public MOAIVectorShape {
private:

	ZLLeanArray < MOAIVectorShape* > mShapes;

public:
	
	//----------------------------------------------------------------//
					MOAIVectorCombo			();
					~MOAIVectorCombo		();
	void			Reserve					( u32 total );
	void			SetShape				( u32 idx, MOAIVectorShape* shape );
	void			ToOutline				( TESStesselator* tess );
};

#endif
