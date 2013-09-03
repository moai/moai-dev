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

 PublisherCancelContentViewController.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/20/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PublisherCancelContentViewController.h"

static NSString *PublisherCancelContentViewControllerNotification = @"PublisherCancelContentViewControllerNotification";

@implementation PublisherCancelContentViewController

#pragma mark -
- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(awkwardCancel)
                                                 name:PublisherCancelContentViewControllerNotification
                                               object:self];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:PublisherCancelContentViewControllerNotification
                                                  object:self];
}

#pragma mark -
- (void)request:(PHPublisherContentRequest *)request contentWillDisplay:(PHContent *)content
{
    if ([super respondsToSelector:@selector(request:contentWillDisplay:)]) {
        [super request:request contentWillDisplay:content];
    }

    NSNotification *cancelNotfication = [NSNotification notificationWithName:PublisherCancelContentViewControllerNotification
                                                                      object:self];
    [[NSNotificationQueue defaultQueue] enqueueNotification:cancelNotfication
                                               postingStyle:NSPostASAP];
}

- (void)awkwardCancel
{
    [self.request cancel];
    [self addMessage:@"Content Canceled!"];
    [self finishRequest];
}
@end
