/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 EVENT SHALL URBAN AIRSHIP INC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#import <CoreLocation/CoreLocation.h>
#import "UALocationProviderDelegate.h"
#import "UALocationProviderProtocol.h"
#import "UALocationCommonValues.h"

/**
 * This is the base class for location providers. You should not
 * implement this class directly. See the 
 * [CLLocationManager documentation](https://developer.apple.com/library/ios/#documentation/CoreLocation/Reference/CLLocationManager_Class/CLLocationManager/CLLocationManager.html)
 * for details.
 */
@interface UABaseLocationProvider : NSObject<CLLocationManagerDelegate, UALocationProviderProtocol>

///---------------------------------------------------------------------------------------
/// @name NSObject 
///---------------------------------------------------------------------------------------

- (NSString *)description;

///---------------------------------------------------------------------------------------
/// @name CLLocationManager related methods
///---------------------------------------------------------------------------------------

/** 
 * Location manager used for providing location data. Setting this value has
 * the side effect of assigning this class as the delegate of the
 * new CLLocationManager
 */
@property (nonatomic, strong) CLLocationManager *locationManager;

/**
 * In certain cases, cached values are immediately returned when starting a location service.
 * Any location with a time interval older than this value will be considered invalid.
 */
@property (nonatomic, assign) NSTimeInterval maximumElapsedTimeForCachedLocation;

/**
 * The distance filter one the locationManager.
 */
- (CLLocationDistance)distanceFilter;

/**
 * Changes the distanceFilter on the locationManager.
 * @param distanceFilter The new distance filter
 */
- (void)setDistanceFilter:(CLLocationDistance)distanceFilter;

/**
 * The desired accuracy currently set on the locationManager.
 */
- (CLLocationAccuracy)desiredAccuracy;

/**
 * Changes the desiredAccuracy on the locationManager.
 * @param desiredAccuracy The new desiredAccuracy
 */
- (void)setDesiredAccuracy:(CLLocationAccuracy)desiredAccuracy;

/**
 * Current purpose attached to the CLLocationMananger locationManager.
 * @return Current purpose on the locationManager
 */
- (NSString *)purpose;

/**
 * Sets the purpose on the CLLocationManager locationManger which is displayed to the user
 * when the UIAlertView is displayed asking the user for location permission.
 * @warning This value cannot be nil.
 * @param newPurpose String to be set on the locationManager
 */
- (void)setPurpose:(NSString *)newPurpose;

/**
 * The most recently received location available from the CLLocationManager object. This may be more accurate than
 * the last reported location, and it may also be nil. See CLLocationManager documentation for more details.
 * @return The most recent location, if one is available
 * @return nil if no recent location is available
 */
- (CLLocation *)location;

///---------------------------------------------------------------------------------------
/// @name Location Service methods
///---------------------------------------------------------------------------------------


/// Delegate that receives location updates 
@property (nonatomic, weak) id<UALocationProviderDelegate> delegate;

/** 
 * Status of the location service.
 *
 * Possible UALocationProviderStatus values are:
 *
 * + kUALocationProviderUpdating The service is currently updating location
 * + kUALocationProviderNotUpdating The service is not updating location
 */
@property (nonatomic, assign) UALocationProviderStatus serviceStatus;

/**
 * Provider type must be set by subclasses
 */
@property (nonatomic, copy) UALocationServiceProviderType *provider;

///---------------------------------------------------------------------------------------
/// @name Creating a UABaseLocationProvider
///---------------------------------------------------------------------------------------

- (id)init;

/**
 * Initializes the object with a delegate
 * @param delegate Delegate object that implements the UALocationProviderDelegate protocol
 */
- (id)initWithDelegate:(id<UALocationProviderDelegate>)delegate;

///---------------------------------------------------------------------------------------
/// @name Location Accuracy
///---------------------------------------------------------------------------------------

/** 
 * Calculates location change accuracy
 * @param newLocation The updated location from the location service.
 * @param oldLocation The location that the new location is compared to.
 *
 * @return YES if the location meets accuracy requirements
 * @return NO if the location does not meet accuracy requirements
 */
- (BOOL)locationChangeMeetsAccuracyRequirements:(CLLocation *)newLocation from:(CLLocation *)oldLocation;

///---------------------------------------------------------------------------------------
/// @name Starting and Stopping Location Services
///---------------------------------------------------------------------------------------

/**
 * Abstract method for starting the location service.
 * @warning This method only controls the service status on this object. A call to
 * [super startReportingLocation] is required when overriding this method.
 */
- (void)startReportingLocation;

/**
 * Abstract method for stopping the location service.
 * @warning This method only controls the service status on this object. A call to 
 * [super stopReportingLocation] is required when overriding this method.
 */
- (void)stopReportingLocation;

@end
