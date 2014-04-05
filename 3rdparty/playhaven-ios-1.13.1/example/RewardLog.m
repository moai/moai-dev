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

 RewardLog.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/6/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "RewardLog.h"
#import "PHReward.h"
@implementation RewardLog
@synthesize lastRewardUnlocked = _lastRewardUnlocked;

+ (RewardLog *)sharedRewardLog
{
    static dispatch_once_t once;
    static id sharedInstance;
    dispatch_once(&once, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}



- (void)dealloc
{
    // Will never be called, but here for clarity
    [_lastRewardUnlocked release], _lastRewardUnlocked = nil;
    [super dealloc];
}
@end
