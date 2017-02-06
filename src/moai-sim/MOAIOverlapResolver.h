// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIOVERLAPRESOLVER_H
#define	MOAIOVERLAPRESOLVER_H

#include <moai-sim/MOAICollisionPrims.h>

//================================================================//
// MOAIOverlapResolver
//================================================================//
class MOAIOverlapResolver :
	public MOAIOverlapShapeVisitor {
private:

	ZLVec3D			mAccumulator;
	float			mCount;

	//----------------------------------------------------------------//
	void			Accumulate					( float x, float y, float z );
	bool			Separate					( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1 );

	//----------------------------------------------------------------//
	void			MOAIOverlapShapeVisitor_Process			( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 );

public:

	//----------------------------------------------------------------//
	ZLVec3D			GetResult					();
					MOAIOverlapResolver			();
	void			Reset						();
	bool			Separate					( const ZLQuad& p0, const ZLQuad& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 );
};

#endif
