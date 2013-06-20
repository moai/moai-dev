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

 PHTimeInGame.h
 playhaven-sdk-ios

 Created by Thomas DiZoglio on 7/5/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
 * @internal
 *
 * @brief Singleton class that keeps track of elapsed session count and session time.
 * Stores data in NSUserDefaults until the next successful open request.
 * Information collected by this class is used for time-in-game segmentation
 * and targeting.
 **/
@interface PHTimeInGame : NSObject {
    CFAbsoluteTime sessionStartTime;
}

/**
 * Singleton accessor
 **/
+ (PHTimeInGame *)getInstance;

/**
 * Start counting up time for the current session
 **/
- (void)gameSessionStarted;

/**
 * Ends the current session and increments counters
 **/
- (void)gameSessionStopped;

/**
 * Resets time and session data. Typically used after data is sent to the API
 **/
- (void)resetCounters;

/**
 * Re-initializes session and time data. Only used for unit tests
 **/
- (void)gameSessionRestart;

/**
 * Returns total unreported session duration, in seconds
 **/
- (CFAbsoluteTime)getSumSessionDuration;

/**
 * Returns total unreported sessions
 **/
- (int)getCountSessions;

/**
 * Returns current session duration, in seconds
 **/
- (CFAbsoluteTime)getCurrentSessionDuration;
@end
