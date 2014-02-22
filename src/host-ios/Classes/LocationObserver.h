//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "RefPtr.h"

#ifndef RADIANS_TO_DEGREES
	#define RADIANS_TO_DEGREES		57.2957795
#endif

#ifndef DEGREES_TO_RADIANS
	#define DEGREES_TO_RADIANS		( 1.0 / RADIANS_TO_DEGREES )
#endif

#ifndef SECONDS_PER_HOUR		
	#define SECONDS_PER_HOUR		3600.0
#endif

#ifndef METERS_PER_KILOMETER
	#define METERS_PER_KILOMETER	1000.0
#endif

//================================================================//
// LocationObserver
//================================================================//
@interface LocationObserver : NSObject < CLLocationManagerDelegate > {
@private
	
	RefPtr < CLLocationManager >		mLocMgr;
	
	double								mLongitude;
	double								mLatitude;
	double								mAltitude;
	
	double								mHAccuracy;
	double								mVAccuracy;
	
	BOOL								mHasHeading;
	double								mHeading;
	double								mCourse;
	
	double								mSpeed;
	
	id									mOnHeadingDelegate;
	SEL									mOnHeadingSelector;
	
	id									mOnLocationDelegate;
	SEL									mOnLocationSelector;
}

	PROPERTY_READONLY ( double, altitude );
	PROPERTY_READONLY ( double, course );
	PROPERTY_READONLY ( double, hAccuracy );
	PROPERTY_READONLY ( BOOL, hasHeading );
	PROPERTY_READONLY ( double, heading );
	PROPERTY_READONLY ( double, latitude )
	PROPERTY_READONLY ( double, longitude );
	PROPERTY_READONLY ( double, speed );
	PROPERTY_READONLY ( double, vAccuracy );

	//----------------------------------------------------------------//
	-( double )			bearing						:( double )lon :( double )lat;
	-( BOOL )			headingAvailable;
	-( BOOL )			locationAvailable;
	-( void )			setHeadingDelegate			:( id )target :( SEL )selector;
	-( void )			setLocationDelegate			:( id )target :( SEL )selector;
	-( void )			start;
	-( void )			stop;

@end
