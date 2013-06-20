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

 PublisherContentViewController.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/25/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PublisherContentViewController.h"
#import "IAPHelper.h"

@implementation PublisherContentViewController
@synthesize placementField = _placementField;
@synthesize request        = _request;
@synthesize showsOverlaySwitch;
@synthesize animateSwitch;

- (void)dealloc
{
    [PHAPIRequest cancelAllRequestsWithDelegate:self];

    [_notificationView release], _notificationView = nil;
    [_placementField release], _placementField = nil;
    [_request release], _request = nil;
    [showsOverlaySwitch release];
    [animateSwitch release];

    [super dealloc];
}

- (void)startRequest
{
    if (self.request == nil) {
        [super startRequest];

        //[self.placementField resignFirstResponder];

        NSString *placement = (![self.placementField.text isEqualToString:@""]) ?
                                    self.placementField.text :
                                    @"more_games";

        PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:self.token
                                                                               secret:self.secret
                                                                            placement:placement
                                                                             delegate:self];
        [request setShowsOverlayImmediately:[showsOverlaySwitch isOn]];
        [request setAnimated:[animateSwitch isOn]];
        [request send];

        [self setRequest:request];

        [self.navigationItem.rightBarButtonItem setTitle:@"Cancel"];

    } else {
        [self addMessage:@"Request canceled!"];

        [self.request cancel];
        self.request = nil;
        [self.navigationItem.rightBarButtonItem setTitle:@"Start"];
    }
}

- (void)finishRequest
{
    [super finishRequest];

    // Cleaning up after a completed request
    self.request = nil;
    [self.navigationItem.rightBarButtonItem setTitle:@"Start"];
}

#pragma mark - PHPublisherContentRequestDelegate
- (void)requestWillGetContent:(PHPublisherContentRequest *)request
{
    NSString *message = [NSString stringWithFormat:@"Getting content for placement: %@", request.placement];
    [self addMessage:message];
}

- (void)requestDidGetContent:(PHPublisherContentRequest *)request
{
    NSString *message = [NSString stringWithFormat:@"Got content for placement: %@", request.placement];
    [self addMessage:message];
    [self addElapsedTime];
}

- (void)request:(PHPublisherContentRequest *)request contentWillDisplay:(PHContent *)content
{
    NSString *message = [NSString stringWithFormat:@"Preparing to display content: %@", content];
    [self addMessage:message];

    [self addElapsedTime];
}

- (void)request:(PHPublisherContentRequest *)request contentDidDisplay:(PHContent *)content
{
    // This is a good place to clear any notification views attached to this request.
    [_notificationView clear];

    NSString *message = [NSString stringWithFormat:@"Displayed content: %@", content];

    [self addMessage:message];

    [self addElapsedTime];
}

- (void)request:(PHPublisherContentRequest *)request contentDidDismissWithType:(PHPublisherContentDismissType *)type
{
    NSString *message = [NSString stringWithFormat:@"[OK] User dismissed request: %@ of type %@", request, type];
    [self addMessage:message];

    [self finishRequest];
}

- (void)request:(PHAPIRequest *)request didFailWithError:(NSError *)error
{
    NSString *message = [NSString stringWithFormat:@"[ERROR] Failed with error: %@", error];
    [self addMessage:message];
    [self finishRequest];
}

- (void)request:(PHPublisherContentRequest *)request unlockedReward:(PHReward *)reward
{
    NSString *message = [NSString stringWithFormat:@"Unlocked reward: %dx %@", reward.quantity, reward.name];
    [self addMessage:message];
}

- (void)request:(PHPublisherContentRequest *)request makePurchase:(PHPurchase *)purchase
{
    NSString *message = [NSString stringWithFormat:@"Initiating purchase for: %dx %@", purchase.quantity, purchase.productIdentifier];
    [self addMessage:message];

    [[IAPHelper sharedIAPHelper] startPurchase:purchase];
}

#pragma - Notifications
/*
 * Refresh your notification view from the server each time it appears.
 * This way you can be sure the type and value of the notification is most
 * likely to match up to the content unit that will appear.
 */

- (void)viewDidLoad
{
    [super viewDidLoad];

    [self startTimers];
    [[PHPublisherContentRequest requestForApp:self.token secret:self.secret placement:@"more_games" delegate:self] preload];

    _notificationView = [[PHNotificationView alloc] initWithApp:self.token secret:self.secret placement:@"more_games"];
    _notificationView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin;
}

- (void)viewDidUnload
{
    [self setPlacementField:nil];
    [super viewDidUnload];
    [_notificationView removeFromSuperview];
    [_notificationView release], _notificationView = nil;
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self.view addSubview:_notificationView];
    [_notificationView setCenter:CGPointMake(self.view.frame.size.width - 22, 19)];
    [_notificationView refresh];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    [_notificationView removeFromSuperview];
}
@end
