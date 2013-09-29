// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOLYGONSHAPE_H
#define	MOAIPOLYGONSHAPE_H

#include <moai-sim/MOAIShape.h>

struct TESStesselator;

//================================================================//
// MOAIPolygonShape
//================================================================//
class MOAIPolygonShape :
	public MOAIShape {
private:

	TESStesselator*	mTess;

	//----------------------------------------------------------------//
	void			RenderPrims				( u32 primType );

public:
	
	//----------------------------------------------------------------//
	void			AddContour				( USVec2D* verts, u32 total );
	void			Clear					();
					MOAIPolygonShape		();
					~MOAIPolygonShape		();
	void			Render					();
	void			Tesselate				( u32 windingRule );
};

#endif
