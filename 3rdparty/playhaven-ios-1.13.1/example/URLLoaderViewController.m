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

 URLLoaderViewController.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 7/13/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "URLLoaderViewController.h"

@interface URLLoaderViewController ()
{
    PHURLLoader *_loader;
}
@end

@implementation URLLoaderViewController
@dynamic    loader;
@synthesize URLField;
@synthesize openURLSwitch;

- (PHURLLoader *)loader
{
    if (_loader == nil) {
        _loader = [PHURLLoader new];
    }

    return _loader;
}

- (void)dealloc
{
    [_loader invalidate];

    [_loader release], _loader = nil;
    [URLField release], URLField = nil;
    [openURLSwitch release], openURLSwitch = nil;;
    [super dealloc];
}

- (void)startRequest
{
    [super startRequest];

    // Check to see if URL field has valid URL value
    NSURL *loaderURL = [NSURL URLWithString:self.URLField.text];
    if (loaderURL == nil) {
        // Finish the request and report an error!
        [self addMessage:@"[ERROR] A valid URL was not entered!"];
        [self finishRequest];
    }

    // If we have a valid loader, then start the request!
    self.loader.targetURL             = loaderURL;
    self.loader.opensFinalURLOnDevice = self.openURLSwitch.on;
    self.loader.delegate              = self;

    [self.loader open];
    self.navigationItem.rightBarButtonItem.enabled = NO;
}

- (void)finishRequest
{
    [super finishRequest];
    self.navigationItem.rightBarButtonItem.enabled = YES;
}

#pragma mark -
#pragma PHURLLoaderDelegate

- (void)loaderFinished:(PHURLLoader *)loader
{
    NSString *message = [NSString stringWithFormat:@"[SUCCESS] Loader finished with URL: %@", loader.targetURL];
    [self addMessage:message];
    [self finishRequest];
}

- (void)loaderFailed:(PHURLLoader *)loader
{
    NSString *message = [NSString stringWithFormat:@"[FAIL] Loader failed to open at URL: %@", loader.targetURL];
    [self addMessage:message];
    [self finishRequest];
}
@end
