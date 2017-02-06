// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONPRIMS_H
#define	MOAICOLLISIONPRIMS_H

#include <moai-sim/MOAICollisionConsts.h>

#define DECLARE_OVERLAP_SHAPE(name, shapeClass, typeID)			\
class MOAIOverlap##name :										\
	public MOAIOverlapShape {									\
public:															\
	shapeClass	mShape;											\
	MOAIOverlap##name () { mType = typeID; mPtr = &mShape; }	\
};

//================================================================//
// MOAIOverlapShape
//================================================================//
class MOAIOverlapShape {
public:

	u32			mCategory;
	u32			mMask;
	u32			mIdentifier;

	u32			mType;
	void*		mPtr;
	ZLBox		mBounds;
};

//================================================================//
// MOAIOverlapShapeVisitor
//================================================================//
class MOAIOverlapShapeVisitor {
protected:

	//----------------------------------------------------------------//
	virtual void		MOAIOverlapShapeVisitor_Process			( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) = 0;

public:

	//----------------------------------------------------------------//
	void Process ( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {
	
		this->MOAIOverlapShapeVisitor_Process ( shape0, shape1, t0, t1 );
	}
};

DECLARE_OVERLAP_SHAPE ( Box,		ZLBox,			MOAICollisionConsts::BOX )
DECLARE_OVERLAP_SHAPE ( Circle,		ZLCircle,		MOAICollisionConsts::CIRCLE )
DECLARE_OVERLAP_SHAPE ( Frustum,	ZLFrustum,		MOAICollisionConsts::FRUSTUM )
DECLARE_OVERLAP_SHAPE ( Polygon,	ZLPolygon2D,	MOAICollisionConsts::POLYGON )
DECLARE_OVERLAP_SHAPE ( Prism,		ZLPrism,		MOAICollisionConsts::PRISM )
DECLARE_OVERLAP_SHAPE ( Quad,		ZLQuad,			MOAICollisionConsts::QUAD )
DECLARE_OVERLAP_SHAPE ( Rect,		ZLRect,			MOAICollisionConsts::RECT )
DECLARE_OVERLAP_SHAPE ( Sphere,		ZLSphere,		MOAICollisionConsts::SPHERE )

#endif
