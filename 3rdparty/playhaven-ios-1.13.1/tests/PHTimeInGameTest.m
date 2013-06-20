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

 PHTimeInGameTest.m
 playhaven-sdk-ios

 Created by Thomas DiZoglio on 7/5/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PHTimeInGame.h"

@interface PHTimeInGameTest : SenTestCase
@end

@implementation PHTimeInGameTest

- (void)testTimeInGame
{
    [[PHTimeInGame getInstance] gameSessionRestart];
    CFAbsoluteTime time = [[PHTimeInGame getInstance] getSumSessionDuration];
    STAssertTrue(time == 0, @"Session should be == 0 since reset. Value: %f", time);
    int lastCount = [[PHTimeInGame getInstance] getCountSessions];
    STAssertTrue(lastCount == 0, @"Session count should be 0 since reset. Value: %d", lastCount);

    [[PHTimeInGame getInstance] gameSessionStarted];
    int sessionCount = [[PHTimeInGame getInstance] getCountSessions];
    STAssertTrue(sessionCount == 1, @"Session count should be a value of 1. Value %d", sessionCount);

    CFAbsoluteTime diffTime = [[PHTimeInGame getInstance] getCurrentSessionDuration];
    STAssertTrue(diffTime > 0, @"Session MUST be greater than 0");

    [[PHTimeInGame getInstance] gameSessionStopped];
    int firstCount = [[PHTimeInGame getInstance] getCountSessions];
    STAssertTrue(firstCount == 1, @"Session count should be 1, value: %d", firstCount);

    [[PHTimeInGame getInstance] gameSessionStarted];
    sessionCount = [[PHTimeInGame getInstance] getCountSessions];
    STAssertTrue(sessionCount == 2, @"Session count should be a value of 2. value: %d", sessionCount);
}
@end
