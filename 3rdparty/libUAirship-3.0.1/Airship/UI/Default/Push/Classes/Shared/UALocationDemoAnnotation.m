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
#import "UALocationDemoAnnotation.h"
#import "UAGlobal.h"

@interface UALocationDemoAnnotation()

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *subtitle;

@end

@implementation UALocationDemoAnnotation


- (id)initWithLocation:(CLLocation*)location {
    self = [super init];
    if (self){
        self.coordinate = location.coordinate;
        self.title = @"Location";
        self.subtitle = [self monthDateFromDate:location.timestamp];
    }
    return self;
}

- (NSString*)monthDateFromDate:(NSDate *)date {
    NSUInteger components = NSMonthCalendarUnit | NSDayCalendarUnit;
    NSDateComponents *monthDay = [[NSCalendar currentCalendar] components:components fromDate:date];
    return [NSString stringWithFormat:@"%ld/%ld", (long)monthDay.month, (long)monthDay.day];
}

+ (UALocationDemoAnnotation*)locationAnnotationFromLocation:(CLLocation*)location {
    return [[UALocationDemoAnnotation alloc] initWithLocation:location];
}

- (NSString*)description {
    return [NSString stringWithFormat:@"%@ %@ %f %f", self.title, self.subtitle, self.coordinate.longitude, self.coordinate.latitude];
}


@end
