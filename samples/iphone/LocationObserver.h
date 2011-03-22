#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "RefPtr.h"

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
