/*
 * Copyright 2010-present Facebook.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#import <Foundation/Foundation.h>

extern NSString *const FacebookAdsSDKErrorDomain;

@interface FBAdSettings : NSObject

/**
 * Add a test device to server ad in test mode only
 */
+ (void)addTestDevice:(NSString *)deviceHash;

/**
 * Add a collection test device to server ad in test mode only
 */
+ (void)addTestDevices:(NSArray *)devicesHash;

/**
 * Clear all the added test devices
 */
+ (void)clearTestDevices;

/**
 * Set whether the ad control is targeting a child.
 */
+ (void)setIsChildDirected:(BOOL)isChildDirected;

/**
 * Sets the url prefix to use when making requests. This method should never be used in production.
 */
+ (void)setUrlPrefix:(NSString *) urlPrefix;

@end
