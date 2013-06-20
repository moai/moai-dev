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

 PHURLLoader+Automation.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/6/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHURLLoader+Automation.h"

static NSURL *LastLaunchedURL;

@implementation PHURLLoader (Automation)
+ (NSURL *)lastLaunchedURL
{
    @synchronized (self) {
        return LastLaunchedURL;
    }
}

+ (void)setLastLaunchedURL:(NSURL *)url
{
    @synchronized (self) {
        [LastLaunchedURL release], LastLaunchedURL = [url copy];
    }
}

- (void)_launchURLForAutomation:(NSURL *)targetURL
{
    // App switching interferes with automation testing
    // Instead, we pretend to launch the URL.
    NSLog(@"Pretending to launch URL: %@", targetURL);
    [PHURLLoader setLastLaunchedURL:targetURL];
}
@end
