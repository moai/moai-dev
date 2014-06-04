/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
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
#import <Availability.h>
#import "UALocationProviderDelegate.h"
#import "UALocationEvent.h"


@class UALocationService;

@class UAStandardLocationProvider;
@class UASignificantChangeProvider;


/**
 * The best location received by the loction service if the desiredAccuracy is not available. Sent
 * on location service timeout errors.
 */
extern NSString * const UALocationServiceBestAvailableSingleLocationKey;

/**
 * The UALocationServiceDelegate receives location updates from any of the UALocationServices.
 */
@protocol UALocationServiceDelegate<NSObject>

///---------------------------------------------------------------------------------------
/// @name UALocationServiceDelegate
///---------------------------------------------------------------------------------------

@optional

/**
 * Updates the delegate when the location service generates an error.
 *
 * @warning Two error conditions will stop the location service before
 * this method is called on the delegate. kCLErrorDenied && kCLErrorNetwork. All other
 * CoreLocation errors are passed directly to the delegate without any side effects. For the case
 * when the location service times out, an NSError* UALocationServiceError will be returned with
 * the best available location in the userInfo dictionary with UALocationServiceBestAvailableSingleLocationKey
 * as the key if a location was returned. In the case of a timeout, the locationService:didUpdateToLocation:fromLocation will also
 * be called with the best available location if it is available. There is no guarantee as to the accuracy of this location.
 *
 * @param service Location service that generated the error
 * @param error Error passed from a CLLocationManager
 */
- (void)locationService:(UALocationService *)service didFailWithError:(NSError *)error;

/**
 * Updates the delegate when authorization status has changed.
 *
 * @param service Location service reporting the change
 * @param status  The updated location authorization status
 */
- (void)locationService:(UALocationService *)service didChangeAuthorizationStatus:(CLAuthorizationStatus)status;

/**
 * Updates the delegate when a new location is received. In case of a timeout, where locations have been acquired,
 * but they do not meet the accuracy requirements, the most accurate location available will be returned.
 *
 * @warning In the background, this method is called and given a limited amount of time to operate, including the time
 * necessary to update UrbanAirship. Extensive work done by the method while backgrounded could result in location data not being
 * recorded or sent.
 *
 * @param service The service reporting the location update
 * @param newLocation The updated location reported by the service
 * @param oldLocation The previously reported location. This value may be nil if there is no previous location.
 */
- (void)locationService:(UALocationService *)service didUpdateToLocation:(CLLocation*)newLocation fromLocation:(CLLocation*)oldLocation;
@end


/**
 * The UALocationService class provides an interface to both the location services on
 * device and the Urban Airship API.
 */
@interface UALocationService : NSObject<UALocationProviderDelegate>

///---------------------------------------------------------------------------------------
/// @name Standard Location Accuracy and Distance
///---------------------------------------------------------------------------------------

/**
 * The distance filter on the standard location provider.
 * @return CLLocationDistance The current distance filter on the standard location provider
 */
- (CLLocationDistance)standardLocationDistanceFilter;

/**
 * Sets the distance filter on the standard location provider.
 * @param distanceFilter The new distance filter.
 */
- (void)setStandardLocationDistanceFilter:(CLLocationDistance)distanceFilter;

/**
 * The desired accuracy on the standard location provider.
 * @return CLLocationAccuracy The current desired accuracy
 */
- (CLLocationAccuracy)standardLocationDesiredAccuracy;

/**
 * Sets the standard location desired accuracy.
 * @param desiredAccuracy The new desired accuracy
 */
- (void)setStandardLocationDesiredAccuracy:(CLLocationAccuracy)desiredAccuracy;

///---------------------------------------------------------------------------------------
/// @name Location Services Authorization
///---------------------------------------------------------------------------------------

/**
 * Current setting allowing UA location services.
 * @return YES UA location services are allowed, and will start if authorized and enabled
 * @return NO UA location services are not allowed, and will not start, even if authorized and enabled
 */
+ (BOOL)airshipLocationServiceEnabled;

/** 
 * This method allows UA Location Services to report location.
 *
 * @param airshipLocationServiceEnabled If set to YES, all UA location services will run
 * if the system reports that location services are available and authorized. This setting will not
 * override the users choice to disable location services and is safe to enable when user preferences
 * have not been established. If NO, UA Location Services are disabled. This setting is persisted in
 * NSUserDefaults.
 */
+ (void)setAirshipLocationServiceEnabled:(BOOL)airshipLocationServiceEnabled;

/**
 * Reports the current authorization status of location services as reported by the
 * system. This refers to the global location service setting switch.
 *
 * @return YES if location services are reported as enabled by the system.
 * @return NO if location services are reported as not enabled by the system.
 */
+ (BOOL)locationServicesEnabled;

/**
 * Reports the current authorization status of location services as reported by the
 * system.
 * @warning For iOS < 4.2, this value is updated after an attempt has been made to start location
 * services, and is persisted from that point on. Prompting the user is the only way to set this value.
 *
 * @return YES if the user has authorized location services, or has yet to be asked about location services.
 * @return NO if the user has explicitly disabled location services
 */
+ (BOOL)locationServiceAuthorized;

/**
 * This method checks the underlying Core Location service to see if a user
 * will receive a prompt requesting permission for Core Location services to run.
 * 
 * @return NO If Core Location services are enabled and the user has explicitly authorized location services
 * @return YES If ANY of the following are true:
 *
 *   - Location services are not enabled (The global location service setting in Settings is disabled)
 *   - Location services are explicitly not authorized (The per app location service setting in Settings is disabled)
 *   - The user has not been asked yet to allow location services. (Location services are enabled, and CLLocationManager reports kCLAuthorizationStatusNotDetermined)
 */
+ (BOOL)coreLocationWillPromptUserForPermissionToRun;

///--------------------------------------------------------------------------------------
/// @name Recent Activity
///---------------------------------------------------------------------------------------

/**
 * The most recently received location available from the CLLocationManager object. This may be more accurate than
 * the last reported location, and it may also be nil. See CLLocationManager documentation for more details.
 * @return The most recent location, or nil if no locations are available
 */
- (CLLocation *)location;

/**
 * Last location reported to Urban Airship
 */
@property (nonatomic, strong, readonly) CLLocation *lastReportedLocation;

/**
 * Date of last location event reported
 */
@property (nonatomic, strong, readonly) NSDate *dateOfLastLocation;

/**
 * UALocationServiceDelegate for location service callbacks
 */
@property (nonatomic, weak) id <UALocationServiceDelegate> delegate;

///---------------------------------------------------------------------------------------
/// @name Automatic Location Services 
///---------------------------------------------------------------------------------------

/**
 * Starts the GPS (Standard Location) and acquires a single location on every launch.
 * Setting this property to YES has the side effect of calling reportCurrentLocation.
 */
@property (nonatomic, assign, setter=setAutomaticLocationOnForegroundEnabled:) BOOL automaticLocationOnForegroundEnabled;

/**
 * Allows location services to continue in the background.
 */
@property (nonatomic, assign) BOOL backgroundLocationServiceEnabled;

/**
 * Minimum time between automatic updates that are tied to app foreground events.
 * Default value is 120 seconds.
 */
@property (nonatomic, assign) NSTimeInterval minimumTimeBetweenForegroundUpdates;

///---------------------------------------------------------------------------------------
/// @name Status of Services
///---------------------------------------------------------------------------------------

/**
 * Status for GPS service.
 */
@property (nonatomic, assign, readonly) UALocationProviderStatus standardLocationServiceStatus;

/**
 * Status for NETWORK (cell tower) events.
 */
@property (nonatomic, assign, readonly) UALocationProviderStatus significantChangeServiceStatus;

/**
 * Status for single location service
 */
@property (nonatomic, assign, readonly) UALocationProviderStatus singleLocationServiceStatus;

///---------------------------------------------------------------------------------------
/// @name Purpose 
///---------------------------------------------------------------------------------------

/**
 * Purpose for location services shown to user
 * when prompted to allow location services to begin. The default value
 * is kUALocationServiceDefaultPurpose listed in UAirship.m. This value is set on
 * all new location services.
 * @return An NSString with the current purpose
 */
- (NSString *)purpose;

/** Purpose for location services shown to user
 * when prompted to allow location services to begin. The default value
 * is kUALocationServiceDefaultPurpose listed in UAirship.m. This value is set on
 * all new location services.
 * @param purpose The new purpose of the service
 */
- (void)setPurpose:(NSString *)purpose;

///---------------------------------------------------------------------------------------
/// @name Creating the Location Service
///---------------------------------------------------------------------------------------

/**
 * Returns a UALocationService object with the given purpose. The purpose
 * string is passed to the UALocationProviders and set on the CLLocationManager.
 * This is displayed to the user when asking for location authorization.
 * @param purpose The description that is displayed to the user when prompted for authorization.
 */
- (id)initWithPurpose:(NSString *)purpose;

/**
 * Starts the Standard Location service and
 * sends location data to Urban Airship. This service will continue updating if the location property is
 * declared in the Info.plist. Please see the Location Awareness Programming guide:
 * http://developer.apple.com/library/ios/#documentation/UserExperience/Conceptual/LocationAwarenessPG/Introduction/Introduction.html
 * for more information. If the standard location service is not setup for background
 * use, it will automatically resume once the app is brought back into the foreground.
 *
 * This will not start the location service if the app is not enabled and authorized. To force
 * location services to start, set the UALocationServicesAllowed property to YES and call this method.
 * This will prompt the user for permission if location services have not been started previously,
 * or if the user has purposely disabled location services. Given that location services were probably
 * disabled for a reason, this prompt might not be welcomed.
 *
 * @warning If the kCLErrorDenied or kCLErrorNetwork errors are returned from the Core Location
 * service, this service will be automatically shut down before the locationService:didFailWithError: delegate call
 * to conserve battery. Be aware of this, and take appropriate action to restart the service if necessary. 
 **/

///---------------------------------------------------------------------------------------
/// @name Starting and Stopping Location Services
///---------------------------------------------------------------------------------------

/**
 * Start the standard location service.
 */
- (void)startReportingStandardLocation;

/**
 * Stops the standard location service.
 */
- (void)stopReportingStandardLocation;

/**
 * Starts the Significant Change location service
 * and sends location data to Urban Airship. This service
 * will continue in the background if stopMonitoringSignificantLocationChanges
 * is not called before the app enters the background.
 *
 * This will not start the location service if the app is not enabled and authorized. To force
 * location services to start, set the UALocationServicesAllowed property to YES and call this method.
 *
 * This will prompt the user for permission if location services have not been started previously,
 * or if the user has purposely disabled location services. Given that location services were probably
 * disabled for a reason, this prompt might not be welcomed.
 *
 * @warning If the kCLErrorDenied or kCLErrorNetwork errors are returned from the Core Location
 * service, this service will be automatically shut down before the locationService:didFailWithError: delegate call
 * to conserve battery. Be aware of this, and take appropriate action to restart the service if necessary. 
 **/
- (void)startReportingSignificantLocationChanges;

/**
 * Stops the Significant Change location service.
 */
- (void)stopReportingSignificantLocationChanges;

///---------------------------------------------------------------------------------------
/// @name Analytics
///---------------------------------------------------------------------------------------

/**
 * Creates a UALocationEvent and enqueues it with the Analytics service.
 * @param location The location to be sent to the Urban Airship analytics service
 * @param provider The provider that generated the location. Data is pulled from the provider for analytics
 * @warning This must be called on the main thread. Not doing so will result in a crash
 */
- (void)reportLocationToAnalytics:(CLLocation *)location fromProvider:(id<UALocationProviderProtocol>)provider;

///---------------------------------------------------------------------------------------
/// @name Single Location Service
///---------------------------------------------------------------------------------------

/**
 * Time interval representing the amount of time that the single location service will
 * run while waiting for a location value that meets the desired accuracy.
 * @return NSTimeInterval representing the current timeout value
 */
- (NSTimeInterval)timeoutForSingleLocationService;

/**
 * Time interval representing the length of time the single location service will
 * attempt to acquire a location that meets accuracy requirements. At the end of the time
 * interval, the most accurate location received will be sent. If no location has
 * been received, the service is shut down.
 * @param timeoutForSingleLocationService NSTimeInterval for the new timeout value
 */
- (void)setTimeoutForSingleLocationService:(NSTimeInterval)timeoutForSingleLocationService;

/**
 * Desired accuracy for single location service. Used by the reportCurrentLocation: method
 * and the automatic foreground location.
 * @return CLLocationAccuracy representing the desired accuracy for single location service.
 */
- (CLLocationAccuracy)singleLocationDesiredAccuracy;

/**
 * Sets the desiredAccuracy for the single location service. Used by the reportCurrentLocation: method
 * and the automatic foreground location.
 * @param desiredAccuracy CLLocationAccuracy The new desiredAccuracy
 * @warning Setting this value to high (such as kCLLocationAccuracyBest) will result in timeouts
 * on every location service call in areas with poor GPS. This will result in degraded device battery life.
 */
- (void)setSingleLocationDesiredAccuracy:(CLLocationAccuracy)desiredAccuracy;

/**
 * Starts the single location service (standard location) long enough to obtain a location an then uploads
 * it to Urban Airship. This method will send the first location that meets the accuracy set in
 * the singleLocationDesiredAccuracy property. If a location is not found before the timeout, the most
 * accurate location returned by CoreLocation is returned. If no location is found, the service is
 * simply terminated.
 */
- (void)reportCurrentLocation;

/**
 * Sends a location directly to Urban Airship. The required parameters are taken from the CLLocation and
 * the CLLocationManager. The UALocationEventUpdateType is helpful in providing the end developer with information
 * regarding the use of location in app. The possible values are:
 *
 * - UAUALocationEventUpdateTypeChange This is one of the periodic services, intended for the significant change or region monitoring service
 * - UAUALocationEventUpdateTypeContinuous This is meant for the standard location service.
 * - UAUALocationEventUpdateTypeSingle This is meant for a one time service, like the reportCurrentLocation method on this class
 * @param location A CLLocation
 * @param locationManager The location manager that provided the location
 * @param updateTypeOrNil The update type as described above or nil.
 * @warning This must be called from the main thread. Not doing so will result in a crash. 
 */
- (void)reportLocation:(CLLocation *)location 
        fromLocationManager:(CLLocationManager *)locationManager 
             withUpdateType:(UALocationEventUpdateType *)updateTypeOrNil;

@end
