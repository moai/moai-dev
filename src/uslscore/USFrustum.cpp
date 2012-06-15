// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USFrustum.h>
#include <uslscore/USDistance.h>
#include <uslscore/USIntersect.h>
#include <uslscore/USMathConsts.h>
#include <uslscore/USPrism.h>
#include <uslscore/USRhombus.h>

#define MIN_FILL_RATIO 0.95f

//================================================================//
// local
//================================================================//

static double	_frustArea		( const USFrustum& frust );
static double	_quadArea		( const USVec3D& v0, const USVec3D& v1, const USVec3D& v2, const USVec3D& v3 );
static bool		_vecToXYPlane	( const USVec3D& v0, const USVec3D& v1, USVec2D& result );

//----------------------------------------------------------------//
double _frustArea ( const USFrustum& frust ) {

	double area [ USFrustum::TOTAL_PLANES ];
	
	area [ USFrustum::LEFT_PLANE ] = _quadArea (
		frust.mPoints [ USFrustum::NEAR_LT_POINT ],
		frust.mPoints [ USFrustum::FAR_LT_POINT ],
		frust.mPoints [ USFrustum::FAR_LB_POINT ],
		frust.mPoints [ USFrustum::NEAR_LB_POINT ]
	);
	
	area [ USFrustum::RIGHT_PLANE ] = _quadArea (
		frust.mPoints [ USFrustum::NEAR_RT_POINT ],
		frust.mPoints [ USFrustum::FAR_RT_POINT ],
		frust.mPoints [ USFrustum::FAR_RB_POINT ],
		frust.mPoints [ USFrustum::NEAR_RB_POINT ]
	);
	
	area [ USFrustum::TOP_PLANE ] = _quadArea (
		frust.mPoints [ USFrustum::NEAR_LT_POINT ],
		frust.mPoints [ USFrustum::FAR_LT_POINT ],
		frust.mPoints [ USFrustum::FAR_RT_POINT ],
		frust.mPoints [ USFrustum::NEAR_RT_POINT ]
	);
	
	area [ USFrustum::BOTTOM_PLANE ] = _quadArea (
		frust.mPoints [ USFrustum::NEAR_LB_POINT ],
		frust.mPoints [ USFrustum::FAR_LB_POINT ],
		frust.mPoints [ USFrustum::FAR_RB_POINT ],
		frust.mPoints [ USFrustum::NEAR_RB_POINT ]
	);
	
	area [ USFrustum::NEAR_PLANE ] = _quadArea (
		frust.mPoints [ USFrustum::NEAR_LT_POINT ],
		frust.mPoints [ USFrustum::NEAR_RT_POINT ],
		frust.mPoints [ USFrustum::NEAR_RB_POINT ],
		frust.mPoints [ USFrustum::NEAR_LB_POINT ]
	);
	
	area [ USFrustum::FAR_PLANE ] = _quadArea (
		frust.mPoints [ USFrustum::FAR_LT_POINT ],
		frust.mPoints [ USFrustum::FAR_RT_POINT ],
		frust.mPoints [ USFrustum::FAR_RB_POINT ],
		frust.mPoints [ USFrustum::FAR_LB_POINT ]
	);
	
	double j =
		( frust.mPlanes [ USFrustum::LEFT_PLANE ].mNorm.Dot ( frust.mPoints [ USFrustum::NEAR_LT_POINT ] ) * area [ USFrustum::LEFT_PLANE ]) +
		( frust.mPlanes [ USFrustum::RIGHT_PLANE ].mNorm.Dot ( frust.mPoints [ USFrustum::NEAR_RB_POINT ] ) * area [ USFrustum::RIGHT_PLANE ]) +
		( frust.mPlanes [ USFrustum::TOP_PLANE ].mNorm.Dot ( frust.mPoints [ USFrustum::NEAR_LT_POINT ] ) * area [ USFrustum::TOP_PLANE ]) +
		( frust.mPlanes [ USFrustum::BOTTOM_PLANE ].mNorm.Dot ( frust.mPoints [ USFrustum::NEAR_RB_POINT ] ) * area [ USFrustum::BOTTOM_PLANE ]) +
		( frust.mPlanes [ USFrustum::NEAR_PLANE ].mNorm.Dot ( frust.mPoints [ USFrustum::NEAR_LT_POINT ] ) * area [ USFrustum::NEAR_PLANE ]) +
		( frust.mPlanes [ USFrustum::FAR_PLANE ].mNorm.Dot ( frust.mPoints [ USFrustum::FAR_LT_POINT ] ) * area [ USFrustum::FAR_PLANE ]);
	
	return j / 3.0f;
}

//----------------------------------------------------------------//
double _quadArea ( const USVec3D& v0, const USVec3D& v1, const USVec3D& v2, const USVec3D& v3 ) {

	// sides
	double a = USDist::VecToVec ( v0, v1 );
	double b = USDist::VecToVec ( v1, v2 );
	double c = USDist::VecToVec ( v2, v3 );
	double d = USDist::VecToVec ( v3, v0 );
	
	// diagonals
	double p = USDist::VecToVec ( v0, v2 );
	double q = USDist::VecToVec ( v1, v3 );
	
	// semiperimeter
	double s = ( a + b + c + d ) * 0.5f;
	
	double ac = a * c;
	double bd = b * d;
	double pq = p * q;
	
	return sqrt ((( s - a ) * ( s - b ) * ( s - c ) * ( s - d )) - (( ac + bd + pq ) * ( ac + bd - pq ) * 0.25 ));
}

//----------------------------------------------------------------//
bool _vecToXYPlane ( const USVec3D& v0, const USVec3D& v1, USVec2D& result ) {

	USVec3D vec;
	
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
// USFrustum
//================================================================//

//----------------------------------------------------------------//
bool USFrustum::Cull ( const USVec3D& vec ) const {

	if ( !this->mAABB.Contains ( vec )) return true;
	
	if ( this->mUsePlanes ) {
		if ( USDist::VecToPlane ( vec, mPlanes [ NEAR_PLANE ]) > 0.0f ) return true;
		if ( USDist::VecToPlane ( vec, mPlanes [ FAR_PLANE ]) > 0.0f ) return true;
		if ( USDist::VecToPlane ( vec, mPlanes [ LEFT_PLANE ]) > 0.0f ) return true;
		if ( USDist::VecToPlane ( vec, mPlanes [ RIGHT_PLANE ]) > 0.0f ) return true;
		if ( USDist::VecToPlane ( vec, mPlanes [ TOP_PLANE ]) > 0.0f ) return true;
		if ( USDist::VecToPlane ( vec, mPlanes [ BOTTOM_PLANE ]) > 0.0f ) return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool USFrustum::Cull ( const USBox& box ) const {

	if ( !box.Overlap ( mAABB )) return true;

	if ( this->mUsePlanes ) {
		if ( USSect::BoxToPlane ( box, mPlanes [ NEAR_PLANE ]) > 0 ) return true;
		if ( USSect::BoxToPlane ( box, mPlanes [ FAR_PLANE ]) > 0 ) return true;
		if ( USSect::BoxToPlane ( box, mPlanes [ LEFT_PLANE ]) > 0 ) return true;
		if ( USSect::BoxToPlane ( box, mPlanes [ RIGHT_PLANE ]) > 0 ) return true;
		if ( USSect::BoxToPlane ( box, mPlanes [ TOP_PLANE ]) > 0 ) return true;
		if ( USSect::BoxToPlane ( box, mPlanes [ BOTTOM_PLANE ]) > 0 ) return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool USFrustum::Cull ( const USPrism& prism ) const {

	if ( USSect::PrismToPlane ( prism, mPlanes [ LEFT_PLANE ]) > 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ RIGHT_PLANE ]) > 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ TOP_PLANE ]) > 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ BOTTOM_PLANE ]) > 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ NEAR_PLANE ]) > 0 ) return true;
	if ( USSect::PrismToPlane ( prism, mPlanes [ FAR_PLANE ]) > 0 ) return true;

	return false;
}

//----------------------------------------------------------------//
bool USFrustum::Cull ( const USRhombus& rhombus ) const {

	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ LEFT_PLANE ]) > 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ RIGHT_PLANE ]) > 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ TOP_PLANE ]) > 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ BOTTOM_PLANE ]) > 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ NEAR_PLANE ]) > 0 ) return true;
	if ( USSect::RhombusToPlane ( rhombus, mPlanes [ FAR_PLANE ]) > 0 ) return true;

	return false;
}

//----------------------------------------------------------------//
bool USFrustum::GetXYSectRect ( const USAffine3D& mtx, USRect& rect ) const {

	u32 nHits = 0;
	USVec2D hits [ 12 ];

	USVec3D nlt = this->mPoints [ NEAR_LT_POINT ];
	USVec3D nrt = this->mPoints [ NEAR_RT_POINT ];
	USVec3D nrb = this->mPoints [ NEAR_RB_POINT ];
	USVec3D nlb = this->mPoints [ NEAR_LB_POINT ];
	
	USVec3D flt = this->mPoints [ FAR_LT_POINT ];
	USVec3D frt = this->mPoints [ FAR_RT_POINT ];
	USVec3D frb = this->mPoints [ FAR_RB_POINT ];
	USVec3D flb = this->mPoints [ FAR_LB_POINT ];
	
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
void USFrustum::Init ( const USMatrix4x4& mtx ) {

	// set up the homogenous coordinates of the canonical view volume
	USVec3D nlt ( -1.0f, 1.0f, -1.0f );
	USVec3D nrt ( 1.0f, 1.0f, -1.0f );
	USVec3D nrb ( 1.0f, -1.0f, -1.0f );
	USVec3D nlb ( -1.0f, -1.0f, -1.0f );

	USVec3D flt ( -1.0f, 1.0f, 1.0f );
	USVec3D frt ( 1.0f, 1.0f, 1.0f );
	USVec3D frb ( 1.0f, -1.0f, 1.0f );
	USVec3D flb ( -1.0f, -1.0f, 1.0f );

	// compute the corners of the frustum
	mtx.Project ( nlt );
	mtx.Project ( nrt );
	mtx.Project ( nrb );
	mtx.Project ( nlb );
	
	mtx.Project ( flt );
	mtx.Project ( frt );
	mtx.Project ( frb );
	mtx.Project ( flb );

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
	
	USVec3D center;
	mtx.GetTranslation ( center );
	
	for ( u32 i = 0; i < TOTAL_PLANES; ++i ) {
		if ( USDist::VecToPlane ( center, this->mPlanes [ i ]) > 0.0f ) {
			this->mPlanes [ i ].Flip ();
		}
	}
	
	double frustArea = _frustArea ( *this );
	double boxArea = this->mAABB.Area ();
	
	this->mUsePlanes = (( float )( frustArea / boxArea )) < MIN_FILL_RATIO;
}

//----------------------------------------------------------------//
//void USFrustum::Init ( USVec3D& loc, USRhombus rhombus, USPlane3D& near, USPlane3D& far ) {
//
//	this->mLoc = loc;
//
//	// Init near & far planes
//	this->mPlanes [ NEAR_PLANE ] = near;
//	this->mPlanes [ FAR_PLANE ] = far;
//
//	// Get the plane containing the rhombus
//	USPlane3D rhomPlane;
//	rhombus.GetPlane ( rhomPlane );
//	
//	// Bless the rhombus
//	if ( USDist::VecToPlane ( loc, rhomPlane ) < 0.0f ) {
//	
//		// We're behind the rhombus plane; the plane's normal is facing away from us
//		// So flip the plane normal as well as the rhombus' Y axis
//		rhomPlane.mNorm.Invert ();
//		rhombus.mLoc.Add ( rhombus.mYAxis );
//		rhombus.mYAxis.Invert ();
//	}
//
//	// Get vertices of rhombus
//	USVec3D leftBottom = rhombus.mLoc;
//	
//	USVec3D rightBottom = leftBottom;
//	rightBottom.Add ( rhombus.mXAxis );
//	
//	USVec3D rightTop = rightBottom;
//	rightTop.Add ( rhombus.mYAxis );
//	
//	USVec3D leftTop = rightTop;
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
//void USFrustum::Init ( USVec3D& loc, USVec3D& viewAxis, USVec3D& worldUp, float nearD, float farD, float hFOV, float vFOV ) {
//
//	this->mLoc = loc;
//
//	// USCamera3D relative x axis in world space
//	USVec3D xAxis;
//	xAxis.Cross ( viewAxis, worldUp );
//	xAxis.Norm ();
//
//	// USCamera3D relative y axis in world space
//	USVec3D yAxis;
//	yAxis.Cross ( xAxis, viewAxis );
//	yAxis.Norm ();
//	
//	USVec3D planeNorm;
//	USVec3D planeLoc;
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
//void USFrustum::UpdateAABB () {
//
//	USPlane3D& left = this->mPlanes [ LEFT_PLANE ];
//	USPlane3D& right = this->mPlanes [ RIGHT_PLANE ];
//	USPlane3D& top = this->mPlanes [ TOP_PLANE ];
//	USPlane3D& bottom = this->mPlanes [ BOTTOM_PLANE ];
//	USPlane3D& near = this->mPlanes [ NEAR_PLANE ];
//	USPlane3D& far = this->mPlanes [ FAR_PLANE ];
//
//	USVec3D leftBottom;
//	USVec3D rightBottom;
//	USVec3D rightTop;
//	USVec3D leftTop;
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
//	USSect::VecToPlane ( this->mLoc, leftBottom, near, t, this->mPoints [ NEAR_LB_POINT ]);
//	USSect::VecToPlane ( this->mLoc, leftBottom, far, t, this->mPoints [ FAR_LB_POINT ]);
//	
//	USSect::VecToPlane ( this->mLoc, rightBottom, near, t, this->mPoints [ NEAR_RB_POINT ]);
//	USSect::VecToPlane ( this->mLoc, rightBottom, far, t, this->mPoints [ FAR_RB_POINT ]);
//	
//	USSect::VecToPlane ( this->mLoc, leftTop, near, t, this->mPoints [ NEAR_LT_POINT ]);
//	USSect::VecToPlane ( this->mLoc, leftTop, far, t, this->mPoints [ FAR_LT_POINT ]);
//	
//	USSect::VecToPlane ( this->mLoc, rightTop, near, t, this->mPoints [ NEAR_RT_POINT ]);
//	USSect::VecToPlane ( this->mLoc, rightTop, far, t, this->mPoints [ FAR_RT_POINT ]);
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
USFrustum::USFrustum () {
}
