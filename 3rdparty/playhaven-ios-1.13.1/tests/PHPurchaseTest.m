/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 PHPurchaseTest.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 2/24/2012.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PHPurchase.h"

@interface PHPurchaseTest : SenTestCase
@end

@implementation PHPurchaseTest

- (void)testResolutionStrings
{
    STAssertTrue([[PHPurchase stringForResolution:PHPurchaseResolutionBuy] isEqualToString:@"buy"], @"Expected 'buy' got %@",[PHPurchase stringForResolution:PHPurchaseResolutionBuy]);
    STAssertTrue([[PHPurchase stringForResolution:PHPurchaseResolutionCancel] isEqualToString:@"cancel"], @"Expected 'cancel' got %@",[PHPurchase stringForResolution:PHPurchaseResolutionCancel]);
    STAssertTrue([[PHPurchase stringForResolution:PHPurchaseResolutionError] isEqualToString:@"error"], @"Expected 'error' got %@",[PHPurchase stringForResolution:PHPurchaseResolutionError]);
    STAssertTrue([[PHPurchase stringForResolution:PHPurchaseResolutionFailure] isEqualToString:@"failure"], @"Expected 'failure' got %@",[PHPurchase stringForResolution:PHPurchaseResolutionFailure]);
}
@end
