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

 PHContentView+Automation.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/7/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHContentView+Automation.h"

@implementation DispatchLog
@synthesize dispatch   = _dispatch;
@synthesize timestamp  = _timestamp;
@synthesize callback   = _callback;
@synthesize isComplete = _isComplete;

- (void)markComplete
{
    _isComplete = YES;
}

- (void)dealloc
{
    [_dispatch release], _dispatch = nil;
    [_callback release], _callback = nil;
    [super dealloc];
}
@end

@implementation PHContentView (Automation)
+ (NSMutableArray *)_dispatchLog
{
    static NSMutableArray *_dispatchLog;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _dispatchLog = [[NSMutableArray alloc] init];
    });

    return _dispatchLog;
}

+ (DispatchLog *)firstDispatch:(NSString *)dispatch
{
    NSPredicate *searchPredicate = [NSPredicate predicateWithFormat:@"dispatch LIKE %@", dispatch];

    NSArray *results = [[PHContentView _dispatchLog] filteredArrayUsingPredicate:searchPredicate];
    results = [results sortedArrayUsingSelector:@selector(timestamp)];

    return ([results count] > 0) ? [results objectAtIndex:0] : nil;
}

+ (void)completeDispatchWithCallback:(NSString *)callback
{
    NSPredicate *searchPredicate = [NSPredicate predicateWithFormat:@"callback LIKE %@", callback];
    NSArray     *results = [[PHContentView _dispatchLog] filteredArrayUsingPredicate:searchPredicate];
    DispatchLog *result  = ([results count] > 0) ? [results objectAtIndex:0] : nil;

    [result setIsComplete:YES];
}

- (void)_logRedirectForAutomation:(NSString *)urlPath callback:(NSString *)callback
{
    DispatchLog *logItem = [DispatchLog new];
    logItem.dispatch   = urlPath;
    logItem.timestamp  = [[NSDate date] timeIntervalSinceReferenceDate];
    logItem.callback   = callback;
    logItem.isComplete = NO;
    [[PHContentView _dispatchLog] addObject:logItem];

    [logItem release];
}

- (void)_logCallbackForAutomation:(NSString *)callback
{
    [PHContentView completeDispatchWithCallback:callback];
}
@end
