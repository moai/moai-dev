// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <uslscore/USFloat.h>
#include <uslscore/USMathConsts.h>
#include <uslscore/USPolar.h>
#include <uslscore/USTrig.h>

#ifndef PI
	#define PI                3.14159265359		/* Pi */
#endif

#ifndef TWOPI
	#define TWOPI             6.28318530718		/* 2 * Pi */
#endif

//#define MIN(a,b) ( a < b ? a : b )
//#define MAX(a,b) ( a > b ? a : b )

//================================================================//
// USPolar
//================================================================//

//----------------------------------------------------------------//
double USPolar::ComposePolar ( double degrees, double minutes, double seconds ) {

	double i = 1.0 / 60.0;
	return degrees + ( minutes * i ) + ( seconds * i * i );
}

//----------------------------------------------------------------//
USPolarVec USPolar::FlatEarthCourseDistance ( const USPolarCoord pt1, const USPolarCoord pt2 ) {

	// Calculates the direction (in degrees) and distance (in meters) between two points
	// using the assumption that they are on a flat plane.
	// Works well for relatively short distances.
	// From Ed Williams' Aviation Formulary V1.42

	// If you stay in the vicinity of a given fixed point (lat0,lon0),
	// it may be a good enough approximation to consider the earth as "flat",
	// and use a North, East, Down rectangular coordinate system with origin at the fixed point.
	// If we call the changes in latitude and longitude dlat=lat-lat0, dlon=lon-lon0
	// (Here treating North and East as positive!), then

	//		distance_North=R1*dlat
	//		distance_East=R2*cos(lat0)*dlon

	// R1 and R2 are called the meridional radius of curvature and
	// the radius of curvature in the prime vertical, respectively.

	//		R1=a(1-e^2)/(1-e^2*(sin(lat0))^2)^(3/2)
	//		R2=a/sqrt(1-e^2*(sin(lat0))^2)

	// a is the equatorial radius of the earth (=6378.137000km for WGS84),
	// and e^2=f*(2-f) with the flattening f=1/298.257223563 for WGS84.

	// In the spherical model used elsewhere in the Formulary, R1=R2=R, the earth's radius.
	// (using R=1 we get distances in radians, using R=60*180/pi distances are in nm.)

	// In the flat earth approximation, distances and bearings are given by
	// the usual plane trigonometry formulae, i.e:

	//		distance = sqrt(distance_North^2 + distance_East^2)
	//		bearing to (lat,lon) = mod(atan2(distance_East, distance_North), 2*pi)
	//							(= mod(atan2(cos(lat0)*dlon, dlat), 2*pi) in the spherical case)

	// These approximations fail in the vicinity of either pole and at large distances.
	// The fractional errors are of order (distance/R)^2. 

	double lat1, lon1, lat2, lon2, e2, R1, R2, d_N, d_E;
	USPolarVec v;

	if ((pt1.mLat == pt2.mLat) && (pt1.mLon == pt2.mLon)) {
		v.AngleInDegrees = 0.0;		/* any arbitrary angle, since they are the same point */
		v.DistanceInMeters = 0.0;
		return v;
	}

	lat1 = pt1.mLat * D2R;
	lat2 = pt2.mLat * D2R;
	lon1 = pt1.mLon * D2R;
	lon2 = pt2.mLon * D2R;
	e2 = (FLATTENING) * ( 2.0 - (FLATTENING) );
	R1 = ERADM * (1 - e2) / pow(1 - e2 * pow(sin(lat1), 2.0), 1.5);
	R2 = ERADM / sqrt(1 - e2 * pow(sin(lat1), 2.0));
	d_N = R1 * (lat2 - lat1);
	d_E = R2 * cos(lat1) * (lon2 - lon1);
	v.DistanceInMeters = sqrt(d_N * d_N + d_E * d_E);
	v.AngleInDegrees   = fmod(atan2(d_E, d_N), TWOPI) * R2D;
	return v;
}

//----------------------------------------------------------------//
double USPolar::GetAngle ( const USPolarVec v0, const USPolarVec v1 ) {

	double diff, a;
	diff = v0.AngleInDegrees - v1.AngleInDegrees;
	a = USPolar::NormalizeAngle180 ( diff );
	if ( a < EPS ) return 0.0;
	else return a;
}

//----------------------------------------------------------------//
double USPolar::GetBearing ( const USPolarCoord p0, const USPolarCoord p1 )  {
	
	double lat1 = p0.mLat * D2R;
	double lon1 = p0.mLon * D2R;
	double lat2 = p1.mLat * D2R;
	double lon2 = p1.mLon * D2R;
	
	if(lat1 == lat2 && lon1 == lon2) {
		return 0.0;
	}
	
	double dLon = ( lon2 - lon1 );
	double t1 = sin ( dLon ) * cos ( lat2 );
	double t2 = cos ( lat1 ) * sin ( lat2 ) - sin ( lat1 ) * cos ( lat2 ) * cos ( dLon );
	
	double bearing = atan2 ( t1, t2 ) * R2D;
	
	return USPolar::NormalizeAngle360 ( bearing );
}

//----------------------------------------------------------------//
double USPolar::GetDegrees ( double polar ) {

	polar = USPolar::NormalizeAngle360 ( polar );
	return floor ( polar );
}

//----------------------------------------------------------------//
double USPolar::GetDistance ( const USPolarCoord p0, const USPolarCoord p1 ) {

	return USPolar::GreatCircleDistance ( p0, p1 );
}

//----------------------------------------------------------------//
double USPolar::GetMinutes ( double polar ) {

	polar = USPolar::NormalizeAngle360 ( polar );

	return USFloat::Floor ( USFloat::Decimal ( polar ) * 60.0 );
}

//----------------------------------------------------------------//
double USPolar::GetSeconds ( double polar ) {

	polar = USPolar::NormalizeAngle360 ( polar );
	
	double minutes = USFloat::Decimal ( polar ) * 60.0;
	return USFloat::Floor ( USFloat::Decimal ( minutes ) * 60.0 );
}

//----------------------------------------------------------------//
USPolarVec USPolar::GetVector ( const USPolarCoord p0, const USPolarCoord p1 ) {

	USPolarVec v;
	v.DistanceInMeters = USPolar::GreatCircleDistance ( p0, p1 );
	v.AngleInDegrees   = USPolar::GreatCircleDirection ( p0, p1 );
	return v;
}

//----------------------------------------------------------------//
double USPolar::GreatCircleDirection ( const USPolarCoord pt1, const USPolarCoord pt2 ) {

	// Calculates the initial heading (direction, in degrees) from point one to point two along a Great Circle.
	// From Ed Williams' Aviation Formulary V1.42
	
	// For everywhere except at the poles:
	// A (in radians) = mod(atan2(sin(lon1-lon2)*cos(lat2), cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2)), 2*pi)
	double lat1, lon1, lat2, lon2, a_rad;

	if ((pt1.mLat == pt2.mLat) &&
		(pt1.mLon == pt2.mLon))
		return 0.0;									/* any arbitrary angle, since they are the same point */

	lat1 = pt1.mLat * D2R;
	if(cos(lat1) < EPS)
	{
		if(lat1 > 0) return 180.0;					/* Starting from the N pole */
		else return 0.0;							/* Starting from the S pole */
	}
	lat2 = pt2.mLat * D2R;
	if(cos(lat2) < EPS)
	{
		if(lat2 > 0) return 0.0;					/* Ending at the N pole */
		else return 180.0;							/* Ending at the S pole */
	}
	lon1 = -(pt1.mLon * D2R);						/* Formula assumes West is positive */
	lon2 = -(pt2.mLon * D2R);						/* Formula assumes West is positive */
	a_rad = atan2( sin(lon1 - lon2) * cos(lat2),
				   cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon1 - lon2));
	return USPolar::NormalizeAngle360(a_rad * R2D);
}

//----------------------------------------------------------------//
double USPolar::GreatCircleDistance ( const USPolarCoord pt1, const USPolarCoord pt2 ) {

	// Calculates the distance (in meters) along a Great Circle between two points.
	// From Ed Williams' Aviation Formulary V1.42
	
	// The great circle distance d between two points with coordinates {lat1,lon1} and {lat2,lon2} is given by:
	// D (in radians) = 2*asin(sqrt((sin((lat1-lat2)/2))^2 + cos(lat1)*cos(lat2)*(sin((lon1-lon2)/2))^2))
	
	double lat1, lon1, lat2, lon2, d_rad;

	if ((pt1.mLat == pt2.mLat) && (pt1.mLon == pt2.mLon)) return 0.0;

	lat1 = pt1.mLat * D2R;
	lat2 = pt2.mLat * D2R;
	lon1 = pt1.mLon * D2R;
	lon2 = pt2.mLon * D2R;
	d_rad = 2.0 * asin(sqrt(pow(sin((lat1 - lat2) / 2.0), 2.0) +
			cos(lat1) * cos(lat2) * pow(sin( (lon1 - lon2) / 2.0 ), 2.0)));
	return d_rad * ERADM;
}

//----------------------------------------------------------------//
USPolarCoord USPolar::GreatCircleEndPoint ( const USPolarCoord start, const USPolarVec crs ) {
	// Calculates the end point of a vector (course) along a Great Circle from the given starting point.
	// From Ed Williams' Aviation Formulary V1.42
		
	// A point {lat,lon} is a distance d out on the tc radial from point 1 if:

	//	lat=asin(sin(lat1)*cos(d)+cos(lat1)*sin(d)*cos(tc))
	//		IF (cos(lat)=0)
	//			lon=lon1      // endpoint a pole
	//		ELSE
	//			lon=mod(lon1-asin(sin(tc)*sin(d)/cos(lat))+pi,2*pi)-pi
	//		ENDIF

	// This algorithm is limited to distances such that dlon <pi/2, i.e those that
	// extend around less than one quarter of the circumference of the earth in longitude.
	// A completely general, but more complicated algorithm is necessary if greater distances are allowed:

	//	lat =asin(sin(lat1)*cos(d)+cos(lat1)*sin(d)*cos(tc))
	//	dlon=atan2(sin(tc)*sin(d)*cos(lat1),cos(d)-sin(lat1)*sin(lat))
	//	lon=mod( lon1-dlon +pi,2*pi )-pi

	double lat1, lon1, d_rad, tc_rad, lat_rad, lon_rad;
	USPolarCoord p;

	lat1 = start.mLat * D2R;
	lon1 = -(start.mLon * D2R);			/* Formula assumes West is positive */
	d_rad = crs.DistanceInMeters / ERADM;
	tc_rad = crs.AngleInDegrees * D2R;
	lat_rad = asin( sin(lat1) * cos(d_rad) + cos(lat1) * sin(d_rad) * cos(tc_rad) );
	lon_rad = 0.0;
	if(fabs(cos(lat_rad)) >= EPS)
	{	/* Endpoint is NOT a pole */
		double dlon = atan2( sin(tc_rad) * sin(d_rad) * cos(lat1),
							 cos(d_rad) - sin(lat1) * sin(lat_rad) );
		lon_rad = fmod(lon1 - dlon + PI, TWOPI) - PI;
	}
	p.mLat = lat_rad * R2D;
	p.mLon = -lon_rad * R2D;			/* Formula assumes West is positive */
	return p;
}

//----------------------------------------------------------------//
double USPolar::LerpAngle ( double d0, double d1, double t ) {

	while ( d0  < 0.0f ) d0 += 360.0f;
	while ( d0  > 360.0f ) d0 -= 360.0f;

	while ( d1  < 0.0f ) d1 += 360.0f;
	while ( d1  > 360.0f ) d1 -= 360.0f;

	double delta = d1 - d0;
	
	if ( delta > 180.0f ) delta -= 360.0f;
	if ( delta < -180.0f ) delta += 360.0f;
	
	d0 += delta * t;
	
	while ( d0  < 0.0f ) d0 += 360.0f;
	while ( d0  > 360.0f ) d0 -= 360.0f;
	
	return d0;
}

//----------------------------------------------------------------//
USPolarCoord USPolar::NearestPointOnLine ( const USPolarCoord fromPt, const USPolarCoord endPt1, const USPolarCoord endPt2 ) {
	return NearestPointOnSegment ( fromPt, endPt1, endPt2 );
}

//----------------------------------------------------------------//
USPolarCoord USPolar::NearestPointOnSegment ( const USPolarCoord fromPt, const USPolarCoord endPt1, const USPolarCoord endPt2 ) {

	// Calculates the closest point on the given line segment
	// From http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/

	// x1,y1 = endPt1; x2,y2 = endPt2; x3,y3 = fromPt
	// mag of line seg = sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
	// u = ( (x3 - x1)(x2 - x1) + (y3 - y1)(y2 - y1) ) / ( mag of line seg )^2
	// if 0.0 <= u <= 1.0 then the nearest point is on the segment:
	// x = x1 + u(x2 - x1)
	// y = y1 + u(y2 - y1)

	double x1, y1, x2, y2, x3, y3, u;
	USPolarCoord p;

	// The segment is really a single point
	if ((endPt1.mLat == endPt2.mLat) && (endPt1.mLon == endPt2.mLon)) return endPt1;	

	x1 = endPt1.mLat;
	y1 = endPt1.mLon;
	x2 = endPt2.mLat;
	y2 = endPt2.mLon;
	x3 = fromPt.mLat;
	y3 = fromPt.mLon;
	u = ( (x3-x1) * (x2-x1) + (y3-y1) * (y2-y1) ) / ( pow(x2-x1, 2.0) + pow(y2-y1, 2.0) );
	if (u < 0.0) return endPt1;				/* nearest point is an end point */
	if (u > 1.0) return endPt2;				/* nearest point is an end point */
	p.mLat  = x1 + u * (x2-x1);
	p.mLon = y1 + u * (y2-y1);
	return p;
}

//----------------------------------------------------------------//
double USPolar::NormalizeAngle180 ( const double degrees ) {

	double val = USPolar::NormalizeAngle360 ( degrees );
	if ( val > 180 ) val = 360 - val;
	return val;
}

//----------------------------------------------------------------//
double USPolar::NormalizeAngle360 ( const double degrees ) {

	double val = fmod ( degrees, 360 );
	if ( val < 0 ) val += 360;
	return val;
}

//----------------------------------------------------------------//
USPolarCoord USPolar::Translate ( const USPolarCoord p, const USPolarVec v ) {

	return USPolar::GreatCircleEndPoint ( p, v );
}

//----------------------------------------------------------------//
USPolarVec USPolar::VectorToLine ( const USPolarCoord fromPt, const USPolarCoord endPt1, const USPolarCoord endPt2 ) {
	USPolarCoord toPt = USPolar::NearestPointOnSegment ( fromPt, endPt1, endPt2 );
	return USPolar::GetVector ( fromPt, toPt );
}
