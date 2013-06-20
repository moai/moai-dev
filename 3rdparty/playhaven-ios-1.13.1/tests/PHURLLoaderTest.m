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

 PHURLLoaderTest.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 3/30/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PHURLLoader.h"

#define EXPECTED_HASH @"3L0xlrDOt02UrTDwMSnye05Awwk"
//#define EXPECTED_HASH @"sbiA9ROvCFEPANNFLbq3BK6m_dU-"

@interface PHURLLoaderTest : SenTestCase
@end

@implementation PHURLLoaderTest

- (void)testLoaderParameter
{
    PHURLLoader *loader = [[PHURLLoader alloc] init];
    STAssertNoThrow(loader.opensFinalURLOnDevice = NO, @"Couldn't set opensFinalURLOnDevice property!");
}
@end
