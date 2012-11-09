//
//  CBAnalytics.h
//  Chartboost
//
//  Created by Kenneth Ballenegger on 3/15/12.
//  Copyright (c) 2012 Chartboost. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SKProduct;
@class SKPaymentTransaction;

@interface CBAnalytics : NSObject

+ (CBAnalytics *)sharedAnalytics;

- (void)recordPaymentTransaction:(SKPaymentTransaction *)transaction
                      forProduct:(SKProduct *)product
                            meta:(NSDictionary *)meta; 


- (void)trackEvent:(NSString *)eventIdentifier;
- (void)trackEvent:(NSString *)eventIdentifier withMetadata:(NSDictionary *)metadata;
- (void)trackEvent:(NSString *)eventIdentifier withValue:(NSNumber *)value;
- (void)trackEvent:(NSString *)eventIdentifier withValue:(NSNumber *)value andMetadata:(NSDictionary *)metadata;

@end
