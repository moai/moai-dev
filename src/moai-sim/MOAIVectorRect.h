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
	void			AddFillContours			( MOAIVectorDrawing& drawing, TESStesselator* tess );
	void			AddStrokeContours		( MOAIVectorDrawing& drawing, TESStesselator* tess );
	void			Init					( const ZLRect& rect );
					MOAIVectorRect			();
					~MOAIVectorRect			();
	
};

#endif
