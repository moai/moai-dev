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

 PHNetworkUtilTest.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 11/26/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PHNetworkUtil.h"
#import "PHAPIRequest.h"

@interface PHNetworkUtilTest : SenTestCase
@end

@implementation PHNetworkUtilTest

/*

 Tests the retrieval of MAC address data from the device, converting that
 data into a lowercase string representation (d_mac) and the ODIN1 string.

 Also tests to see if the MAC address is suppressed if PHOptInStatus is
 set to no.

*/
- (void)testMAC
{
    [PHAPIRequest setOptOutStatus:YES];
    CFDataRef bMACOptOut = [[PHNetworkUtil sharedInstance] newMACBytes];
    STAssertNil((NSData *)bMACOptOut, @"MAC address should not be available when opt out is active");

    [PHAPIRequest setOptOutStatus:NO];
    CFDataRef bMAC = [[PHNetworkUtil sharedInstance] newMACBytes];
    STAssertNotNil((NSData *)bMAC, @"Missing MAC address bytes!");

    uint8_t testBytes[] = { 0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f };
    CFDataRef testData  = CFDataCreate(NULL, (uint8_t *)testBytes, 6);

    NSString *macString = [[PHNetworkUtil sharedInstance] stringForMACBytes: testData];
    STAssertTrue([macString isEqualToString:@"1a2b3c4d5e6f"], @"String representation not correct!");

    NSString *odin1String = [[PHNetworkUtil sharedInstance] ODIN1ForMACBytes: testData];
    STAssertTrue([odin1String isEqualToString: @"82a53f1222f8781a5063a773231d4a7ee41bdd6f"], @"ODIN1 representation not correct!");

    CFRelease(bMAC);
}
@end
