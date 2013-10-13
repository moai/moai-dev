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
#import "UAEvent.h"
#import "UALocationProviderProtocol.h"

/** Keys and values for location analytics */
typedef NSString UALocationEventAnalyticsKey;
extern UALocationEventAnalyticsKey * const UALocationEventForegroundKey; 
extern UALocationEventAnalyticsKey * const UALocationEventForegroundKey; 
extern UALocationEventAnalyticsKey * const UALocationEventLatitudeKey;
extern UALocationEventAnalyticsKey * const UALocationEventLongitudeKey;
extern UALocationEventAnalyticsKey * const UALocationEventDesiredAccuracyKey;
extern UALocationEventAnalyticsKey * const UALocationEventUpdateTypeKey;
extern UALocationEventAnalyticsKey * const UALocationEventProviderKey;
extern UALocationEventAnalyticsKey * const UALocationEventDistanceFilterKey;
extern UALocationEventAnalyticsKey * const UALocationEventHorizontalAccuracyKey;
extern UALocationEventAnalyticsKey * const UALocationEventVerticalAccuracyKey;
extern UALocationEventAnalyticsKey * const UALocationEventSessionIDKey;

typedef NSString UALocationEventUpdateType;
extern UALocationEventUpdateType * const UALocationEventAnalyticsType;
extern UALocationEventUpdateType * const UALocationEventUpdateTypeChange;
extern UALocationEventUpdateType * const UALocationEventUpdateTypeContinuous;
extern UALocationEventUpdateType * const UALocationEventUpdateTypeSingle;
extern UALocationEventUpdateType * const UALocationEventUpdateTypeNone;

extern NSString * const UAAnalyticsValueNone;

/** 
 * A UALocationEvent captures all the necessary information for
 * UAAnalytics.
 */
@interface UALocationEvent : UAEvent


///---------------------------------------------------------------------------------------
/// @name Object Creation
///---------------------------------------------------------------------------------------

/** 
 * Create a UALocationEvent.
 * @param context A dictionary populated with all required data
 * @return A UALocationEvent populated with appropriate values
 */
- (id)initWithLocationContext:(NSDictionary *)context;

/** 
 * Creates a UALocationEvent.
 * @param location Location going to UAAnalytics
 * @param provider Provider that produced the location
 * @param updateType One of the UALocationEvent updated types, see header for more details
 * @return UALocationEvent populated with the necessary values
 */
- (id)initWithLocation:(CLLocation *)location 
              provider:(id<UALocationProviderProtocol>)provider 
         andUpdateType:(UALocationEventUpdateType *)updateType; 

/** 
 * Creates a UALocationEvent.
 * @param location Location going to UAAnalytics
 * @param locationManager The location manager that produced the location
 * @param updateType One of the UALocationEvent updated types, see header for more details
 * @return UALocationEvent populated with the necessary values
 */
- (id)initWithLocation:(CLLocation *)location 
       locationManager:(CLLocationManager *)locationManager 
         andUpdateType:(UALocationEventUpdateType *)updateType;

/**
 * Creates a UALocationEvent.
 * @param location Location going to UAAnalytics
 * @param provider Provider that produced the location
 * @param updateType One of the UALocationEvent updated types, see header for more details
 * @return UALocationEvent populated with the necessary values
 */
+ (UALocationEvent *)locationEventWithLocation:(CLLocation *)location
                                      provider:(id<UALocationProviderProtocol>)provider 
                                 andUpdateType:(UALocationEventUpdateType *)updateType;

/**
 * Creates a UALocationEvent.
 * @param location Location going to UAAnalytics
 * @param locationManager The location manager that produced the location
 * @param updateType One of the UALocationEvent updated types, see header for more details
 * @return UALocationEvent populated with the necessary values
 */
+ (UALocationEvent *)locationEventWithLocation:(CLLocation *)location
                               locationManager:(CLLocationManager *)locationManager 
                                 andUpdateType:(UALocationEventUpdateType *)updateType;


///---------------------------------------------------------------------------------------
/// @name Support Methods
///---------------------------------------------------------------------------------------

/** 
 * Populates a dictionary with the appropriate data gathered from the CLLocation.
 * @param dictionary The dictionary to populate with values
 * @param location Location to parse values from
 */
- (void)populateDictionary:(NSMutableDictionary *)dictionary withLocationValues:(CLLocation *)location;

/** 
 * Populates a dictionary with the appropriate data gathered from the CLLocationManager.
 * @param dictionary The dictionary to populate with values
 * @param locationManager The location manager to parse values from
 */
- (void)populateDictionary:(NSMutableDictionary *)dictionary withLocationManagerValues:(CLLocationManager *)locationManager;

/** 
 * Populates a dictionary with the appropriate data gathered from the object conforming to the UALocationProviderProtocol.
 * @param dictionary The dictionary to populate with values from the location provider
 * @param locationProvider The object implementing the UALocationProviderProtocol to parse data from
 */
- (void)populateDictionary:(NSMutableDictionary *)dictionary withLocationProviderValues:(id<UALocationProviderProtocol>)locationProvider;

/** 
 * Populates a dictionary with the defaulf values for reporting a significant change event. The values currently default
 * to NONE, and are defined in the implementation file.
 * @param context The dictionary to populate with values
*/
- (void)setDefaultSignificantChangeDistanceAndAccuracyValuesInContext:(NSMutableDictionary *)context;

/**
 * Converts a double to a string keeping seven digit of precision
 * Seven digits produces sub meter accuracy at the equator.
 * http://en.wikipedia.org/wiki/Decimal_degrees
 * @param doubleValue The double to convert.
 * @return NSString representing the 7 digit value
 */
- (NSString *)stringFromDoubleToSevenDigits:(double)doubleValue;

/** 
 * Converts a double to a string and truncating it
 * to an int.
 * @param doubleValue The double to convert and truncate
 * @return NSString representing the double as an int
 */
- (NSString *)stringAsIntFromDouble:(double)doubleValue;


@end
