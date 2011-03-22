// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USPOLAR_H
#define USPOLAR_H

#define ERADM		6378137.0			/* Earth's radius in meters (WGS84) */
#define FLATTENING	1.0/298.257223563	/* Earth flattening (WGS84) */
#define EPS			0.000000000005		/* Epsilon - smallest positive double value */

#define METERS_TO_FEET	3.2808399
#define FEET_TO_METERS	( 1.0 / 3.2808399 )

//================================================================//
// USPolarCoord
//================================================================//
class USPolarCoord {
public:
	double mLat;
	double mLon;
};

//================================================================//
// USPolarVec
//================================================================//
class USPolarVec {
public:
	double DistanceInMeters;
	double AngleInDegrees;
};

//================================================================//
// USPolar
//================================================================//
namespace USPolar {

	//----------------------------------------------------------------//
	double			ComposePolar				( double degrees, double minutes, double seconds );
	USPolarVec		FlatEarthCourseDistance		( const USPolarCoord pt1, const USPolarCoord pt2 );
	double			GetAngle					( const USPolarVec v0, const USPolarVec v1 );
	double			GetBearing					( const USPolarCoord p0, const USPolarCoord p1 ); 
	double			GetDegrees					( double polar );
	double			GetDistance					( const USPolarCoord p0, const USPolarCoord p1 );
	double			GetMinutes					( double polar );
	double			GetSeconds					( double polar );
	USPolarVec		GetVector					( const USPolarCoord p0, const USPolarCoord p1 );
	double			GreatCircleDirection		( const USPolarCoord pt1, const USPolarCoord pt2 );
	double			GreatCircleDistance			( const USPolarCoord pt1, const USPolarCoord pt2 );
	USPolarCoord	GreatCircleEndPoint			( const USPolarCoord start, const USPolarVec crs );
	double			LerpAngle					( double d0, double d1, double t );
	USPolarCoord	Translate					( const USPolarCoord p, const USPolarVec v );
	USPolarCoord	NearestPointOnLine			( const USPolarCoord fromPt, const USPolarCoord endPt1, const USPolarCoord endPt2 );
	USPolarCoord	NearestPointOnSegment		( const USPolarCoord fromPt, const USPolarCoord endPt1, const USPolarCoord endPt2 );
	double			NormalizeAngle180			( const double degrees );
	double			NormalizeAngle360			( const double degrees );
	USPolarVec		VectorToLine				( const USPolarCoord fromPt, const USPolarCoord endPt1, const USPolarCoord endPt2 );
};

#endif
