//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "LocationObserver.h"

//----------------------------------------------------------------//
static double _getBearing ( double lat0, double lon0, double lat1, double lon1 )  {
	
	lat0 = lat0 * DEGREES_TO_RADIANS;
	lon0 = lat0 * DEGREES_TO_RADIANS;
	lat1 = lat1 * DEGREES_TO_RADIANS;
	lon1 = lat1 * DEGREES_TO_RADIANS;
	
	if ( lat0 == lat1 && lon0 == lon1 ) {
		return 0.0;
	}
	
	double dLon = ( lon1 - lon0 );
	double t0 = sin ( dLon ) * cos ( lat1 );
	double t1 = cos ( lat0 ) * sin ( lat1 ) - sin ( lat0 ) * cos ( lat1 ) * cos ( dLon );
	
	double bearing = atan2 ( t0, t1 ) * RADIANS_TO_DEGREES;
	bearing = fmod ( bearing, 360 );
	if ( bearing < 0 ) bearing += 360;
		
	return bearing;
}

//----------------------------------------------------------------//
static double _getSpeed ( CLLocation* location1, CLLocation* location2 ) {

	// calculate distance between locations
	double meters = [ location1 distanceFromLocation:location2 ];
	
	// calculate time between locations
	double seconds = [ location2.timestamp timeIntervalSinceDate:location1.timestamp ];
	double hours = ( seconds / SECONDS_PER_HOUR );
	
	// prevent divide by zero case
	if ( hours == 0 ) return 0;

	// return value in kilometers per hour
	double kilometers = ( meters / METERS_PER_KILOMETER );
	return ( kilometers / hours );
}

//================================================================//
// LocationObserver ()
//================================================================//
@interface LocationObserver ()

	//----------------------------------------------------------------//
	
@end

//================================================================//
// LocationObserver
//================================================================//
@implementation LocationObserver

	SYNTHESIZE	( double, altitude, Altitude );
	SYNTHESIZE	( double, course, Course );
	SYNTHESIZE	( double, hAccuracy, HAccuracy );
	SYNTHESIZE	( BOOL, hasHeading, HasHeading );
	SYNTHESIZE	( double, heading, Heading );
	SYNTHESIZE	( double, latitude, Latitude );
	SYNTHESIZE	( double, longitude, Longitude );
	SYNTHESIZE	( double, speed, Speed );
	SYNTHESIZE	( double, vAccuracy, VAccuracy );

	//----------------------------------------------------------------//
	-( double ) bearing :( double )lon :( double )lat {
		
		return _getBearing ([ self latitude ], [ self longitude ], lat, lon );
	}

	//----------------------------------------------------------------//
	-( BOOL ) headingAvailable {
	
		return [ CLLocationManager headingAvailable ];
	}

	//----------------------------------------------------------------//
	-( BOOL ) locationAvailable {
		
		return ([ CLLocationManager locationServicesEnabled ]);
	}

	//----------------------------------------------------------------//
	// designated initializer
	//----------------------------------------------------------------//
	-( id ) init {
		
		self = [ super init ];
		if ( self ) {

			// init members
			mLatitude	= 0.0;
			mLongitude	= 0.0;
			mAltitude	= 0.0;
			
			mHAccuracy	= 0.0;
			mVAccuracy	= 0.0;
			
			mHasHeading = NO;
			mHeading	= 0.0;
			mCourse		= 0.0;
			
			mSpeed		= 0.0;

			// set up location manager
			mLocMgr = [[[ CLLocationManager alloc ] init ] autorelease ];
			[ mLocMgr setDelegate:self ];
			[ mLocMgr setDesiredAccuracy:kCLLocationAccuracyBest ];
			[ mLocMgr setDistanceFilter:kCLDistanceFilterNone ];
			
			[ self start ];
		}
		
		return self;
	}

	//----------------------------------------------------------------//
	-( void ) setHeadingDelegate :( id )target :( SEL )selector {
	
		mOnHeadingDelegate = target;
		mOnHeadingSelector = selector;
	}
	
	//----------------------------------------------------------------//
	-( void ) setLocationDelegate :( id )target :( SEL )selector {
	
		mOnLocationDelegate = target;
		mOnLocationSelector = selector;
	}

	//----------------------------------------------------------------//
	-( void ) start {
		
		[ mLocMgr startUpdatingLocation ];
		
		if ([ CLLocationManager headingAvailable ]) {
			[ mLocMgr startUpdatingHeading ];
		}
	}
	
	//----------------------------------------------------------------//
	-( void ) stop {
		
		[ mLocMgr stopUpdatingLocation ];
		
		if ([ CLLocationManager headingAvailable ]) {
			[ mLocMgr stopUpdatingHeading ];
		}
	}

	//================================================================//
	#pragma mark Protocol CLLocationManagerDelegate
	//================================================================//

	//----------------------------------------------------------------//
	-( void ) locationManager :( CLLocationManager * )manager didUpdateHeading:( CLHeading* )newHeading {
		
		mHeading = newHeading.trueHeading;
		mHasHeading = true;
		
		if ([ mOnHeadingDelegate respondsToSelector:mOnHeadingSelector ]) {
			[ mOnHeadingDelegate performSelector:mOnHeadingSelector withObject:self ];
		}
	}

	//----------------------------------------------------------------//
	-( void ) locationManager :( CLLocationManager* )manager didUpdateToLocation:( CLLocation* )newLocation fromLocation:( CLLocation* )oldLocation {
		
		//NSLog ([ NSString stringWithFormat:@"LocationObserver: timestamp = %@", [ newLocation timestamp ]]);
		
		mHAccuracy = newLocation.horizontalAccuracy;
		mVAccuracy = newLocation.verticalAccuracy;
		
		if ( oldLocation ) {
			
			mSpeed = _getSpeed ( oldLocation, newLocation );
			
			if (([ CLLocationManager headingAvailable ] == false ) && ( mSpeed > 0.0f )) {
				
				mCourse = _getBearing (
					oldLocation.coordinate.latitude,
					oldLocation.coordinate.longitude,
					newLocation.coordinate.latitude,
					newLocation.coordinate.longitude
				);
				
				if ( !mHasHeading ) {
					mHeading = mCourse;
				}
			}
		}
		
		mLatitude	= [ newLocation coordinate ].latitude;
		mLongitude	= [ newLocation coordinate ].longitude;
		mAltitude	= [ newLocation altitude ];
		
		if ([ mOnLocationDelegate respondsToSelector:mOnLocationSelector ]) {
			[ mOnLocationDelegate performSelector:mOnLocationSelector withObject:self ];
		}
	}

@end
