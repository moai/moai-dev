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

 PHTimeInGame.m
 playhaven-sdk-ios

 Created by Thomas DiZoglio on 7/5/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHTimeInGame.h"

@implementation PHTimeInGame

static PHTimeInGame * shared = nil;

+ (PHTimeInGame *)getInstance
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (shared == nil) {
            shared = [PHTimeInGame new];
        }
    });

    return shared;
}

- (id)init
{
    self = [super init];
    if (self) {
        sessionStartTime = 0;
    }

    return self;
}

- (void)gameSessionStarted
{
    sessionStartTime = CFAbsoluteTimeGetCurrent();

    [[NSNotificationCenter defaultCenter] addObserver:[PHTimeInGame getInstance]
                                             selector:@selector(gameSessionStopped)
                                                 name:UIApplicationWillTerminateNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:[PHTimeInGame getInstance]
                                             selector:@selector(gameSessionStopped)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];

    int currentSessionCount = [[NSUserDefaults standardUserDefaults] integerForKey:@"PHSessionCount"] + 1;
    [[NSUserDefaults standardUserDefaults] setInteger:currentSessionCount forKey:@"PHSessionCount"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)gameSessionStopped
{
    if (sessionStartTime == 0)
        return;

    // Record elapsed time for this session using a background task identifier to ensure data is recorded
    // as this is normally called as an app is backgrounded/terminated.
    UIBackgroundTaskIdentifier synchronizeIdentifier = [[UIApplication sharedApplication] beginBackgroundTaskWithExpirationHandler:^{
        // no-op
    }];

    [[NSUserDefaults standardUserDefaults] setDouble:[self getSumSessionDuration] forKey:@"PHSessionDuration"];
    [[NSUserDefaults standardUserDefaults] synchronize];
    [[UIApplication sharedApplication] endBackgroundTask:synchronizeIdentifier];


    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UIApplicationWillTerminateNotification
                                                  object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UIApplicationDidEnterBackgroundNotification
                                                  object:nil];
    sessionStartTime = 0;
}

/*
 The following data is sent in the Open request to the PH server:
 ssum = It is the sum of session duration (since last successful "open") - getSumSessionDuration
 scount = A count of sessions (since last successful "open"). Used if player offline and still playing - getCountSessions

 The following data is sent in every Content request to the PH server:
 stime = That is the duration of the current session thus far - getCurrentSessionDuration
 */

- (CFAbsoluteTime)getSumSessionDuration
{
    CFAbsoluteTime totalDurationTime = [[NSUserDefaults standardUserDefaults] doubleForKey:@"PHSessionDuration"] + [self getCurrentSessionDuration];
    return totalDurationTime;
}

- (int)getCountSessions
{
    NSInteger result = [[NSUserDefaults standardUserDefaults] integerForKey:@"PHSessionCount"];
    return result;
}

/*
This method should only be invoked for testing purposes as it will destroy session data.
*/
- (void)gameSessionRestart
{
    sessionStartTime = 0;
    [[NSUserDefaults standardUserDefaults] setDouble:0.0f forKey:@"PHSessionDuration"];
    [[NSUserDefaults standardUserDefaults] setInteger:0 forKey:@"PHSessionCount"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (CFAbsoluteTime) getCurrentSessionDuration
{
    if (sessionStartTime == 0)
        return 0;

    CFAbsoluteTime currentTime = CFAbsoluteTimeGetCurrent();
    CFAbsoluteTime differenceTime = currentTime - sessionStartTime;
    return differenceTime;
}

/*
After time in game data has been reported to the API, we will purge that amount of time from the stored session duration.
*/
- (void)resetCounters
{
    // Resetting session count to 1 since this is usually called during a session
    [[NSUserDefaults standardUserDefaults] setDouble:0.0f forKey:@"PHSessionDuration"];
    [[NSUserDefaults standardUserDefaults] setInteger:1 forKey:@"PHSessionCount"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
@end
