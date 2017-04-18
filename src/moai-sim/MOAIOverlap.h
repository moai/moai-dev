// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIOVERLAP_H
#define	MOAIOVERLAP_H

#include <moai-sim/MOAICollisionPrim.h>

//================================================================//
// MOAIOverlap
//================================================================//
class MOAIOverlap :
	public MOAICollisionPrimVisitor {
protected:

	bool			mCalculateBounds;

	//----------------------------------------------------------------//
	void			MOAICollisionPrimVisitor_Process		( const MOAICollisionPrim& shape0, const MOAICollisionPrim& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 );
	virtual void	MOAIOverlap_OnOverlap					( const ZLBounds& bounds );

public:
	
	GET_SET ( bool, CalculateBounds, mCalculateBounds )
	
	//----------------------------------------------------------------//
					MOAIOverlap				();
	
	bool			Overlap					( const MOAICollisionPrim& shape0, const MOAICollisionPrim& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 );
	
	bool			Overlap					( const ZLBox& p0, const ZLBox& p1,					const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLBox& p0, const ZLFrustum& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLBox& p0, const ZLPrism& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLBox& p0, const ZLSphere& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );
	
	bool			Overlap					( const ZLCircle& p0, const ZLCircle& p1,			const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLCircle& p0, const ZLPolygon2D& p1,		const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLCircle& p0, const ZLQuad& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLCircle& p0, const ZLRect& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );

	bool			Overlap					( const ZLFrustum& p0, const ZLFrustum& p1,			const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLFrustum& p0, const ZLPrism& p1,			const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLFrustum& p0, const ZLSphere& p1,			const ZLAffine3D& t0, const ZLAffine3D& t1 );

	bool			Overlap					( const ZLPolygon2D& p0, const ZLPolygon2D& p1,		const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLPolygon2D& p0, const ZLQuad& p1,			const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLPolygon2D& p0, const ZLRect& p1,			const ZLAffine3D& t0, const ZLAffine3D& t1 );
	
	bool			Overlap					( const ZLPrism& p0, const ZLPrism& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLPrism& p0, const ZLSphere& p1,			const ZLAffine3D& t0, const ZLAffine3D& t1 );
	
	bool			Overlap					( const ZLQuad& p0, const ZLQuad& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );
	bool			Overlap					( const ZLQuad& p0, const ZLRect& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );

	bool			Overlap					( const ZLRect& p0, const ZLRect& p1,				const ZLAffine3D& t0, const ZLAffine3D& t1 );

	bool			Overlap					( const ZLSphere& p0, const ZLSphere& p1,			const ZLAffine3D& t0, const ZLAffine3D& t1 );
};

#endif
