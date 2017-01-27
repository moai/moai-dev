// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIOVERLAP_H
#define	MOAIOVERLAP_H

class MOAIOverlapHandler;
class MOAITransformBase;

#define COLLISION_COMBO_CODE(shapeType0, shapeType1) (( 0x00000001 << shapeType0 ) | ( 0x00010000 << shapeType1 ))

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
// MOAIOverlap
//================================================================//
class MOAIOverlap {
public:

	// the intent is to determine a fine-gained overlap.
	// geometry is given in *local* space.
	// to overlap, 'self' and 'other' transforms must be provided.
	// it's up to each overlap implementation to do the math in whatever space is appropriate.

	enum {
		BOX,		// 3D (AXIS-ALIGNED)
		CIRCLE,		// 2D (AXIS-ALIGNED)
		FRUSTUM,	// 3D
		POLYGON,	// 2D
		PRISM,		// 3D
		QUAD,		// 2D
		RECT,		// 2D (AXIS-ALIGNED)
		SPHERE,		// 3D (AXIS-ALIGNED)
		NONE,
	};

	//----------------------------------------------------------------//
	static void		Overlap					( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, MOAIOverlapHandler& handler );
	static bool		Overlap					( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );

private:

	static const u32 BOX__BOX				= COLLISION_COMBO_CODE ( BOX, BOX );
	static const u32 BOX__FRUSTUM			= COLLISION_COMBO_CODE ( BOX, FRUSTUM );
	static const u32 BOX__PRISM				= COLLISION_COMBO_CODE ( BOX, PRISM );
	static const u32 BOX__SPHERE			= COLLISION_COMBO_CODE ( BOX, SPHERE );

	static const u32 CIRCLE__CIRCLE			= COLLISION_COMBO_CODE ( CIRCLE, CIRCLE );
	static const u32 CIRCLE__POLYGON		= COLLISION_COMBO_CODE ( CIRCLE, POLYGON );
	static const u32 CIRCLE__QUAD			= COLLISION_COMBO_CODE ( CIRCLE, QUAD );
	static const u32 CIRCLE__RECT			= COLLISION_COMBO_CODE ( CIRCLE, RECT );

	static const u32 FRUSTUM__BOX			= COLLISION_COMBO_CODE ( FRUSTUM, BOX );
	static const u32 FRUSTUM__FRUSTUM		= COLLISION_COMBO_CODE ( FRUSTUM, FRUSTUM );
	static const u32 FRUSTUM__PRISM			= COLLISION_COMBO_CODE ( FRUSTUM, PRISM );
	static const u32 FRUSTUM__SPHERE		= COLLISION_COMBO_CODE ( FRUSTUM, SPHERE );

	static const u32 POLYGON__CIRCLE		= COLLISION_COMBO_CODE ( POLYGON, CIRCLE );
	static const u32 POLYGON__POLYGON		= COLLISION_COMBO_CODE ( POLYGON, POLYGON );
	static const u32 POLYGON__QUAD			= COLLISION_COMBO_CODE ( POLYGON, QUAD );
	static const u32 POLYGON__RECT			= COLLISION_COMBO_CODE ( POLYGON, RECT );
	
	static const u32 PRISM__BOX				= COLLISION_COMBO_CODE ( PRISM, BOX );
	static const u32 PRISM__FRUSTUM			= COLLISION_COMBO_CODE ( PRISM, FRUSTUM );
	static const u32 PRISM__PRISM			= COLLISION_COMBO_CODE ( PRISM, PRISM );
	static const u32 PRISM__SPHERE			= COLLISION_COMBO_CODE ( PRISM, SPHERE );
	
	static const u32 QUAD__CIRCLE			= COLLISION_COMBO_CODE ( QUAD, CIRCLE );
	static const u32 QUAD__POLYGON			= COLLISION_COMBO_CODE ( QUAD, POLYGON );
	static const u32 QUAD__QUAD				= COLLISION_COMBO_CODE ( QUAD, QUAD );
	static const u32 QUAD__RECT				= COLLISION_COMBO_CODE ( QUAD, RECT );

	static const u32 RECT__CIRCLE			= COLLISION_COMBO_CODE ( RECT, CIRCLE );
	static const u32 RECT__POLYGON			= COLLISION_COMBO_CODE ( RECT, POLYGON );
	static const u32 RECT__QUAD				= COLLISION_COMBO_CODE ( RECT, QUAD );
	static const u32 RECT__RECT				= COLLISION_COMBO_CODE ( RECT, RECT );

	static const u32 SPHERE__BOX			= COLLISION_COMBO_CODE ( SPHERE, BOX );
	static const u32 SPHERE__FRUSTUM		= COLLISION_COMBO_CODE ( SPHERE, FRUSTUM );
	static const u32 SPHERE__PRISM			= COLLISION_COMBO_CODE ( SPHERE, PRISM );
	static const u32 SPHERE__SPHERE			= COLLISION_COMBO_CODE ( SPHERE, SPHERE );

	//----------------------------------------------------------------//
	static bool		Overlap			( const ZLBox& p0, const ZLBox& p1,					const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLBox& p0, const ZLFrustum& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLBox& p0, const ZLPrism& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLBox& p0, const ZLSphere& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	
	static bool		Overlap			( const ZLCircle& p0, const ZLCircle& p1,			const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLCircle& p0, const ZLPolygon2D& p1,		const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLCircle& p0, const ZLQuad& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLCircle& p0, const ZLRect& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );

	static bool		Overlap			( const ZLFrustum& p0, const ZLFrustum& p1,			const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLFrustum& p0, const ZLPrism& p1,			const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLFrustum& p0, const ZLSphere& p1,			const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );

	static bool		Overlap			( const ZLPolygon2D& p0, const ZLPolygon2D& p1,		const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLPolygon2D& p0, const ZLQuad& p1,			const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLPolygon2D& p0, const ZLRect& p1,			const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	
	static bool		Overlap			( const ZLPrism& p0, const ZLPrism& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLPrism& p0, const ZLSphere& p1,			const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	
	static bool		Overlap			( const ZLQuad& p0, const ZLQuad& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
	static bool		Overlap			( const ZLQuad& p0, const ZLRect& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );

	static bool		Overlap			( const ZLRect& p0, const ZLRect& p1,				const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );

	static bool		Overlap			( const ZLSphere& p0, const ZLSphere& p1,			const MOAITransformBase& t0, const MOAITransformBase& t1, bool calculateBounds, ZLBounds& bounds );
};

#define DECLARE_OVERLAP_SHAPE(name, shapeClass, typeID)			\
class MOAIOverlap##name :										\
	public MOAIOverlapShape {									\
public:															\
	shapeClass	mShape;											\
	MOAIOverlap##name () { mType = typeID; mPtr = &mShape; }	\
};

DECLARE_OVERLAP_SHAPE ( Box,		ZLBox,			MOAIOverlap::BOX )
DECLARE_OVERLAP_SHAPE ( Circle,		ZLCircle,		MOAIOverlap::CIRCLE )
DECLARE_OVERLAP_SHAPE ( Frustum,	ZLFrustum,		MOAIOverlap::FRUSTUM )
DECLARE_OVERLAP_SHAPE ( Polygon,	ZLPolygon2D,	MOAIOverlap::POLYGON )
DECLARE_OVERLAP_SHAPE ( Prism,		ZLPrism,		MOAIOverlap::PRISM )
DECLARE_OVERLAP_SHAPE ( Quad,		ZLQuad,			MOAIOverlap::QUAD )
DECLARE_OVERLAP_SHAPE ( Rect,		ZLRect,			MOAIOverlap::RECT )
DECLARE_OVERLAP_SHAPE ( Sphere,		ZLSphere,		MOAIOverlap::SPHERE )

#endif
