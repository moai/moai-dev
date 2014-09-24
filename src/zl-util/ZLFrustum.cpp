// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLFrustum.h>
#include <zl-util/ZLDistance.h>
#include <zl-util/ZLIntersect.h>
#include <zl-util/ZLMathConsts.h>
#include <zl-util/ZLPrism.h>
#include <zl-util/ZLRhombus.h>

#define MIN_FILL_RATIO 0.95f

//================================================================//
// local
//================================================================//

static double	_frustArea		( const ZLFrustum& frust );
static double	_quadArea		( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3 );
static bool		_vecToXYPlane	( const ZLVec3D& v0, const ZLVec3D& v1, ZLVec2D& result );

//----------------------------------------------------------------//
double _frustArea ( const ZLFrustum& frust ) {

	double area [ ZLFrustum::TOTAL_PLANES ];
	
	area [ ZLFrustum::LEFT_PLANE ] = _quadArea (
		frust.mPoints [ ZLFrustum::NEAR_LT_POINT ],
		frust.mPoints [ ZLFrustum::FAR_LT_POINT ],
		frust.mPoints [ ZLFrustum::FAR_LB_POINT ],
		frust.mPoints [ ZLFrustum::NEAR_LB_POINT ]
	);
	
	area [ ZLFrustum::RIGHT_PLANE ] = _quadArea (
		frust.mPoints [ ZLFrustum::NEAR_RT_POINT ],
		frust.mPoints [ ZLFrustum::FAR_RT_POINT ],
		frust.mPoints [ ZLFrustum::FAR_RB_POINT ],
		frust.mPoints [ ZLFrustum::NEAR_RB_POINT ]
	);
	
	area [ ZLFrustum::TOP_PLANE ] = _quadArea (
		frust.mPoints [ ZLFrustum::NEAR_LT_POINT ],
		frust.mPoints [ ZLFrustum::FAR_LT_POINT ],
		frust.mPoints [ ZLFrustum::FAR_RT_POINT ],
		frust.mPoints [ ZLFrustum::NEAR_RT_POINT ]
	);
	
	area [ ZLFrustum::BOTTOM_PLANE ] = _quadArea (
		frust.mPoints [ ZLFrustum::NEAR_LB_POINT ],
		frust.mPoints [ ZLFrustum::FAR_LB_POINT ],
		frust.mPoints [ ZLFrustum::FAR_RB_POINT ],
		frust.mPoints [ ZLFrustum::NEAR_RB_POINT ]
	);
	
	area [ ZLFrustum::NEAR_PLANE ] = _quadArea (
		frust.mPoints [ ZLFrustum::NEAR_LT_POINT ],
		frust.mPoints [ ZLFrustum::NEAR_RT_POINT ],
		frust.mPoints [ ZLFrustum::NEAR_RB_POINT ],
		frust.mPoints [ ZLFrustum::NEAR_LB_POINT ]
	);
	
	area [ ZLFrustum::FAR_PLANE ] = _quadArea (
		frust.mPoints [ ZLFrustum::FAR_LT_POINT ],
		frust.mPoints [ ZLFrustum::FAR_RT_POINT ],
		frust.mPoints [ ZLFrustum::FAR_RB_POINT ],
		frust.mPoints [ ZLFrustum::FAR_LB_POINT ]
	);
	
	double j =
		( frust.mPlanes [ ZLFrustum::LEFT_PLANE ].mNorm.Dot ( frust.mPoints [ ZLFrustum::NEAR_LT_POINT ] ) * area [ ZLFrustum::LEFT_PLANE ]) +
		( frust.mPlanes [ ZLFrustum::RIGHT_PLANE ].mNorm.Dot ( frust.mPoints [ ZLFrustum::NEAR_RB_POINT ] ) * area [ ZLFrustum::RIGHT_PLANE ]) +
		( frust.mPlanes [ ZLFrustum::TOP_PLANE ].mNorm.Dot ( frust.mPoints [ ZLFrustum::NEAR_LT_POINT ] ) * area [ ZLFrustum::TOP_PLANE ]) +
		( frust.mPlanes [ ZLFrustum::BOTTOM_PLANE ].mNorm.Dot ( frust.mPoints [ ZLFrustum::NEAR_RB_POINT ] ) * area [ ZLFrustum::BOTTOM_PLANE ]) +
		( frust.mPlanes [ ZLFrustum::NEAR_PLANE ].mNorm.Dot ( frust.mPoints [ ZLFrustum::NEAR_LT_POINT ] ) * area [ ZLFrustum::NEAR_PLANE ]) +
		( frust.mPlanes [ ZLFrustum::FAR_PLANE ].mNorm.Dot ( frust.mPoints [ ZLFrustum::FAR_LT_POINT ] ) * area [ ZLFrustum::FAR_PLANE ]);
	
	return j / 3.0f;
}

//----------------------------------------------------------------//
double _quadArea ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3 ) {

	// sides
	double a = ZLDist::VecToVec ( v0, v1 );
	double b = ZLDist::VecToVec ( v1, v2 );
	double c = ZLDist::VecToVec ( v2, v3 );
	double d = ZLDist::VecToVec ( v3, v0 );
	
	// diagonals
	double p = ZLDist::VecToVec ( v0, v2 );
	double q = ZLDist::VecToVec ( v1, v3 );
	
	// semiperimeter
	double s = ( a + b + c + d ) * 0.5f;
	
	double ac = a * c;
	double bd = b * d;
	double pq = p * q;
	
	return sqrt ((( s - a ) * ( s - b ) * ( s - c ) * ( s - d )) - (( ac + bd + pq ) * ( ac + bd - pq ) * 0.25 ));
}

//----------------------------------------------------------------//
bool _vecToXYPlane ( const ZLVec3D& v0, const ZLVec3D& v1, ZLVec2D& result ) {

	ZLVec3D vec;
	
	vec = v1;
	vec.Sub ( v0 );

	if ( vec.mZ != 0.0f ) {

		result.mX = vec.mX;
		result.mY = vec.mY;
		
		float t = v0.mZ / -vec.mZ;
		
		if (( t > 0.0f ) && ( t <= 1.0f )) {
		
			result.Scale ( t );
		
			result.mX += v0.mX;
			result.mY += v0.mY;
			
			return true;
		}
	}
	return false;
}

//================================================================//
// ZLFrustum
//================================================================//

//----------------------------------------------------------------//
bool ZLFrustum::Cull ( const ZLVec3D& vec ) const {

	if ( !this->mAABB.Contains ( vec )) return true;
	
	if ( this->mUsePlanes ) {
		if ( ZLDist::VecToPlane ( vec, mPlanes [ NEAR_PLANE ]) > 0.0f ) return true;
		if ( ZLDist::VecToPlane ( vec, mPlanes [ FAR_PLANE ]) > 0.0f ) return true;
		if ( ZLDist::VecToPlane ( vec, mPlanes [ LEFT_PLANE ]) > 0.0f ) return true;
		if ( ZLDist::VecToPlane ( vec, mPlanes [ RIGHT_PLANE ]) > 0.0f ) return true;
		if ( ZLDist::VecToPlane ( vec, mPlanes [ TOP_PLANE ]) > 0.0f ) return true;
		if ( ZLDist::VecToPlane ( vec, mPlanes [ BOTTOM_PLANE ]) > 0.0f ) return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLFrustum::Cull ( const ZLBox& box ) const {

	if ( !box.Overlap ( mAABB )) return true;

	if ( this->mUsePlanes ) {
		if ( ZLSect::BoxToPlane ( box, mPlanes [ NEAR_PLANE ]) > 0 ) return true;
		if ( ZLSect::BoxToPlane ( box, mPlanes [ FAR_PLANE ]) > 0 ) return true;
		if ( ZLSect::BoxToPlane ( box, mPlanes [ LEFT_PLANE ]) > 0 ) return true;
		if ( ZLSect::BoxToPlane ( box, mPlanes [ RIGHT_PLANE ]) > 0 ) return true;
		if ( ZLSect::BoxToPlane ( box, mPlanes [ TOP_PLANE ]) > 0 ) return true;
		if ( ZLSect::BoxToPlane ( box, mPlanes [ BOTTOM_PLANE ]) > 0 ) return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLFrustum::Cull ( const ZLPrism& prism ) const {

	if ( ZLSect::PrismToPlane ( prism, mPlanes [ LEFT_PLANE ]) > 0 ) return true;
	if ( ZLSect::PrismToPlane ( prism, mPlanes [ RIGHT_PLANE ]) > 0 ) return true;
	if ( ZLSect::PrismToPlane ( prism, mPlanes [ TOP_PLANE ]) > 0 ) return true;
	if ( ZLSect::PrismToPlane ( prism, mPlanes [ BOTTOM_PLANE ]) > 0 ) return true;
	if ( ZLSect::PrismToPlane ( prism, mPlanes [ NEAR_PLANE ]) > 0 ) return true;
	if ( ZLSect::PrismToPlane ( prism, mPlanes [ FAR_PLANE ]) > 0 ) return true;

	return false;
}

//----------------------------------------------------------------//
bool ZLFrustum::Cull ( const ZLRhombus& rhombus ) const {

	if ( ZLSect::RhombusToPlane ( rhombus, mPlanes [ LEFT_PLANE ]) > 0 ) return true;
	if ( ZLSect::RhombusToPlane ( rhombus, mPlanes [ RIGHT_PLANE ]) > 0 ) return true;
	if ( ZLSect::RhombusToPlane ( rhombus, mPlanes [ TOP_PLANE ]) > 0 ) return true;
	if ( ZLSect::RhombusToPlane ( rhombus, mPlanes [ BOTTOM_PLANE ]) > 0 ) return true;
	if ( ZLSect::RhombusToPlane ( rhombus, mPlanes [ NEAR_PLANE ]) > 0 ) return true;
	if ( ZLSect::RhombusToPlane ( rhombus, mPlanes [ FAR_PLANE ]) > 0 ) return true;

	return false;
}

//----------------------------------------------------------------//
bool ZLFrustum::GetXYSectRect ( const ZLAffine3D& mtx, ZLRect& rect ) const {

	u32 nHits = 0;
	ZLVec2D hits [ 12 ];

	ZLVec3D nlt = this->mPoints [ NEAR_LT_POINT ];
	ZLVec3D nrt = this->mPoints [ NEAR_RT_POINT ];
	ZLVec3D nrb = this->mPoints [ NEAR_RB_POINT ];
	ZLVec3D nlb = this->mPoints [ NEAR_LB_POINT ];
	
	ZLVec3D flt = this->mPoints [ FAR_LT_POINT ];
	ZLVec3D frt = this->mPoints [ FAR_RT_POINT ];
	ZLVec3D frb = this->mPoints [ FAR_RB_POINT ];
	ZLVec3D flb = this->mPoints [ FAR_LB_POINT ];
	
	mtx.Transform ( nlt );
	mtx.Transform ( nrt );
	mtx.Transform ( nrb );
	mtx.Transform ( nlb );
	
	mtx.Transform ( flt );
	mtx.Transform ( frt );
	mtx.Transform ( frb );
	mtx.Transform ( flb );
	
	if ( _vecToXYPlane ( nlt, flt, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( nrt, frt, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( nrb, frb, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( nlb, flb, hits [ nHits ])) ++nHits;
	
	if ( _vecToXYPlane ( nlt, nrt, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( nrt, nrb, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( nrb, nlb, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( nlb, nlt, hits [ nHits ])) ++nHits;
	
	if ( _vecToXYPlane ( flt, frt, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( frt, frb, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( frb, flb, hits [ nHits ])) ++nHits;
	if ( _vecToXYPlane ( flb, flt, hits [ nHits ])) ++nHits;
	
	if ( nHits ) {
		rect.Init ( hits [ 0 ]);
		for ( u32 i = 1; i < nHits; ++i ) {
			rect.Grow ( hits [ i ]);
		}
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void ZLFrustum::Init ( const ZLMatrix4x4& invViewProjMtx ) {

	// set up the homogenous coordinates of the canonical view volume
	ZLVec3D nlt ( -1.0f, 1.0f, -1.0f );
	ZLVec3D nrt ( 1.0f, 1.0f, -1.0f );
	ZLVec3D nrb ( 1.0f, -1.0f, -1.0f );
	ZLVec3D nlb ( -1.0f, -1.0f, -1.0f );

	ZLVec3D flt ( -1.0f, 1.0f, 1.0f );
	ZLVec3D frt ( 1.0f, 1.0f, 1.0f );
	ZLVec3D frb ( 1.0f, -1.0f, 1.0f );
	ZLVec3D flb ( -1.0f, -1.0f, 1.0f );

	// compute the corners of the frustum
	invViewProjMtx.Project ( nlt );
	invViewProjMtx.Project ( nrt );
	invViewProjMtx.Project ( nrb );
	invViewProjMtx.Project ( nlb );
	
	invViewProjMtx.Project ( flt );
	invViewProjMtx.Project ( frt );
	invViewProjMtx.Project ( frb );
	invViewProjMtx.Project ( flb );

	this->mPoints [ NEAR_LT_POINT ].Init ( nlt.mX, nlt.mY, nlt.mZ );
	this->mPoints [ NEAR_RT_POINT ].Init ( nrt.mX, nrt.mY, nrt.mZ );
	this->mPoints [ NEAR_RB_POINT ].Init ( nrb.mX, nrb.mY, nrb.mZ );
	this->mPoints [ NEAR_LB_POINT ].Init ( nlb.mX, nlb.mY, nlb.mZ );
	
	this->mPoints [ FAR_LT_POINT ].Init ( flt.mX, flt.mY, flt.mZ );
	this->mPoints [ FAR_RT_POINT ].Init ( frt.mX, frt.mY, frt.mZ );
	this->mPoints [ FAR_RB_POINT ].Init ( frb.mX, frb.mY, frb.mZ );
	this->mPoints [ FAR_LB_POINT ].Init ( flb.mX, flb.mY, flb.mZ );

	// Compute the frustum's axis-aligned bounding box
	this->mAABB.Init ( nlt );
	this->mAABB.Grow ( nrt );
	this->mAABB.Grow ( nrb );
	this->mAABB.Grow ( nlb );
	
	this->mAABB.Grow ( flt );
	this->mAABB.Grow ( frt );
	this->mAABB.Grow ( frb );
	this->mAABB.Grow ( flb );
	
	// intialize the planes
	this->mPlanes [ LEFT_PLANE ].Init ( nlt, flt, flb );
	this->mPlanes [ RIGHT_PLANE ].Init ( nrb, frb, frt );
	
	this->mPlanes [ TOP_PLANE ].Init ( nrt, frt, flt );
	this->mPlanes [ BOTTOM_PLANE ].Init ( nlb, flb, frb );
	
	this->mPlanes [ NEAR_PLANE ].Init ( nrt, nlt, nlb );
	this->mPlanes [ FAR_PLANE ].Init ( flt, frt, frb );
	
	double frustArea = _frustArea ( *this );
	double boxArea = this->mAABB.Area ();
	
	this->mUsePlanes = (( float )( frustArea / boxArea )) < MIN_FILL_RATIO;
}

//----------------------------------------------------------------//
//void ZLFrustum::Init ( ZLVec3D& loc, ZLRhombus rhombus, ZLPlane3D& near, ZLPlane3D& far ) {
//
//	this->mLoc = loc;
//
//	// Init near & far planes
//	this->mPlanes [ NEAR_PLANE ] = near;
//	this->mPlanes [ FAR_PLANE ] = far;
//
//	// Get the plane containing the rhombus
//	ZLPlane3D rhomPlane;
//	rhombus.GetPlane ( rhomPlane );
//	
//	// Bless the rhombus
//	if ( ZLDist::VecToPlane ( loc, rhomPlane ) < 0.0f ) {
//	
//		// We're behind the rhombus plane; the plane's normal is facing away from us
//		// So flip the plane normal as well as the rhombus' Y axis
//		rhomPlane.mNorm.Invert ();
//		rhombus.mLoc.Add ( rhombus.mYAxis );
//		rhombus.mYAxis.Invert ();
//	}
//
//	// Get vertices of rhombus
//	ZLVec3D leftBottom = rhombus.mLoc;
//	
//	ZLVec3D rightBottom = leftBottom;
//	rightBottom.Add ( rhombus.mXAxis );
//	
//	ZLVec3D rightTop = rightBottom;
//	rightTop.Add ( rhombus.mYAxis );
//	
//	ZLVec3D leftTop = rightTop;
//	leftTop.Sub ( rhombus.mXAxis );
//	
//	// Init the side planes
//	this->mPlanes [ BOTTOM_PLANE ].Init ( loc, rightBottom, leftBottom );
//	this->mPlanes [ TOP_PLANE ].Init ( loc, leftTop, rightTop );
//	this->mPlanes [ LEFT_PLANE ].Init ( loc, leftBottom, leftTop );
//	this->mPlanes [ RIGHT_PLANE ].Init ( loc, rightTop, rightBottom );
//	
//	this->UpdateAABB ();
//}

//----------------------------------------------------------------//
// viewAxis and worldUP should be normalized.
// All frustum plane normals will face *into* of the frustum.
// Please pass in the stuff and things using RADIANS.
//void ZLFrustum::Init ( ZLVec3D& loc, ZLVec3D& viewAxis, ZLVec3D& worldUp, float nearD, float farD, float hFOV, float vFOV ) {
//
//	this->mLoc = loc;
//
//	// USCamera3D relative x axis in world space
//	ZLVec3D xAxis;
//	xAxis.Cross ( viewAxis, worldUp );
//	xAxis.Norm ();
//
//	// USCamera3D relative y axis in world space
//	ZLVec3D yAxis;
//	yAxis.Cross ( xAxis, viewAxis );
//	yAxis.Norm ();
//	
//	ZLVec3D planeNorm;
//	ZLVec3D planeLoc;
//	
//	// Near plane
//	planeLoc = loc;
//	planeLoc.Offset ( viewAxis, nearD );
//	
//	planeNorm = viewAxis;
//	
//	mPlanes [ NEAR_PLANE ].Init ( planeLoc, planeNorm );
//	
//	// Far plane
//	planeLoc = loc;
//	planeLoc.Offset ( viewAxis, farD );
//
//	planeNorm = viewAxis;
//	planeNorm.Invert ();
//
//	mPlanes [ FAR_PLANE ].Init ( planeLoc, planeNorm );
//	
//	// Left plane
//	planeLoc = loc;
//
//	planeNorm = xAxis;
//	planeNorm.RotateInBasis ( xAxis, viewAxis, ( hFOV * 0.5f ));
//	mPlanes [ LEFT_PLANE ].Init ( planeLoc, planeNorm );
//
//	// Right plane
//	planeNorm.Reflect ( xAxis );
//	mPlanes [ RIGHT_PLANE ].Init ( planeLoc, planeNorm );
//
//	// Top plane
//	planeNorm = viewAxis;
//	planeNorm.RotateInBasis ( viewAxis, yAxis, ( vFOV * 0.5f ) - ( 90.0f * ( float )D2R ));
//	mPlanes [ TOP_PLANE ].Init ( planeLoc, planeNorm );
//
//	// Bottom plane
//	planeNorm.Reflect ( yAxis );
//	mPlanes [ BOTTOM_PLANE ].Init ( planeLoc, planeNorm );
//	
//	this->UpdateAABB ();
//}

//----------------------------------------------------------------//
//void ZLFrustum::UpdateAABB () {
//
//	ZLPlane3D& left = this->mPlanes [ LEFT_PLANE ];
//	ZLPlane3D& right = this->mPlanes [ RIGHT_PLANE ];
//	ZLPlane3D& top = this->mPlanes [ TOP_PLANE ];
//	ZLPlane3D& bottom = this->mPlanes [ BOTTOM_PLANE ];
//	ZLPlane3D& near = this->mPlanes [ NEAR_PLANE ];
//	ZLPlane3D& far = this->mPlanes [ FAR_PLANE ];
//
//	ZLVec3D leftBottom;
//	ZLVec3D rightBottom;
//	ZLVec3D rightTop;
//	ZLVec3D leftTop;
//
//	// Get the vecs to verts
//	leftBottom.Cross ( bottom.mNorm, left.mNorm );
//	rightBottom.Cross ( right.mNorm, bottom.mNorm );
//	rightTop.Cross ( top.mNorm, right.mNorm );
//	leftTop.Cross ( left.mNorm, top.mNorm );
//	
//	float t;
//	
//	// Intersect the vecs to verts to get the frustum corners
//	ZLSect::VecToPlane ( this->mLoc, leftBottom, near, t, this->mPoints [ NEAR_LB_POINT ]);
//	ZLSect::VecToPlane ( this->mLoc, leftBottom, far, t, this->mPoints [ FAR_LB_POINT ]);
//	
//	ZLSect::VecToPlane ( this->mLoc, rightBottom, near, t, this->mPoints [ NEAR_RB_POINT ]);
//	ZLSect::VecToPlane ( this->mLoc, rightBottom, far, t, this->mPoints [ FAR_RB_POINT ]);
//	
//	ZLSect::VecToPlane ( this->mLoc, leftTop, near, t, this->mPoints [ NEAR_LT_POINT ]);
//	ZLSect::VecToPlane ( this->mLoc, leftTop, far, t, this->mPoints [ FAR_LT_POINT ]);
//	
//	ZLSect::VecToPlane ( this->mLoc, rightTop, near, t, this->mPoints [ NEAR_RT_POINT ]);
//	ZLSect::VecToPlane ( this->mLoc, rightTop, far, t, this->mPoints [ FAR_RT_POINT ]);
//	
//	// Compute the frustum's axis-aligned bounding box
//	mAABB.Init ( mPoints [ 0 ]);
//	mAABB.Grow ( mPoints [ 1 ]);
//	mAABB.Grow ( mPoints [ 2 ]);
//	mAABB.Grow ( mPoints [ 3 ]);
//	mAABB.Grow ( mPoints [ 4 ]);
//	mAABB.Grow ( mPoints [ 5 ]);
//	mAABB.Grow ( mPoints [ 6 ]);
//	mAABB.Grow ( mPoints [ 7 ]);
//}

//----------------------------------------------------------------//
ZLFrustum::ZLFrustum () {
}
