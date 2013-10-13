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

@protocol UALocationProviderProtocol;

/**
 * Delegate methods for Location providers. All are required.
 */
@protocol UALocationProviderDelegate<NSObject>

@required
/**
 * Delegate call for authorization state changes iOS > 4.2 only.
 * @param locationProvider The location provider
 * @param locationManager The CLLocationManager object
 * @param status The new status
 */
- (void)locationProvider:(id<UALocationProviderProtocol>)locationProvider 
             withLocationManager:(CLLocationManager*)locationManager 
    didChangeAuthorizationStatus:(CLAuthorizationStatus)status;

/**
 * Delegate is called when a UALocationServices object reports an error.
 * @param locationProvider The location provider
 * @param locationManager  The CLLocationManager object
 * @param error The NSError thrown by the locationManager
 */
- (void)locationProvider:(id<UALocationProviderProtocol>)locationProvider 
       withLocationManager:(CLLocationManager *)locationManager 
          didFailWithError:(NSError *)error;

/**
 * Delegate is called when a UALocationService gets a callback
 * from a CLLocationManager with a location that meets accuracy
 * requirements.
 * @param locationProvider The location provider
 * @param locationManager The CLLocationManager object
 * @param newLocation The new location reported by the provider
 * @param oldLocation The previous location reported by the provider
 */
- (void)locationProvider:(id<UALocationProviderProtocol>)locationProvider
       withLocationManager:(CLLocationManager *)locationManager 
         didUpdateLocation:(CLLocation *)newLocation
              fromLocation:(CLLocation *)oldLocation;

@end
