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

 PHPublisherContentRequest.m (formerly PHPublisherAdUnitRequest.m)
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/5/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHPublisherContentRequest.h"
#import "PHPublisherSubContentRequest.h"
#import "PHContent.h"
#import "PHConstants.h"
#import "PHStringUtil.h"
#import "PHReward.h"
#import "PHPurchase.h"
#import "PHPublisherIAPTrackingRequest.h"
#import "JSON.h"
#import "PHTimeInGame.h"

NSString *const PHPublisherContentRequestRewardIDKey        = @"reward";
NSString *const PHPublisherContentRequestRewardQuantityKey  = @"quantity";
NSString *const PHPublisherContentRequestRewardReceiptKey   = @"receipt";
NSString *const PHPublisherContentRequestRewardSignatureKey = @"signature";

NSString *const PHPublisherContentRequestPurchaseProductIDKey = @"product";
NSString *const PHPublisherContentRequestPurchaseNameKey      = @"name";
NSString *const PHPublisherContentRequestPurchaseQuantityKey  = @"quantity";
NSString *const PHPublisherContentRequestPurchaseReceiptKey   = @"receipt";
NSString *const PHPublisherContentRequestPurchaseSignatureKey = @"signature";
NSString *const PHPublisherContentRequestPurchaseCookieKey    = @"cookie";

PHPublisherContentDismissType * const PHPublisherContentUnitTriggeredDismiss           = @"PHPublisherContentUnitTriggeredDismiss";
PHPublisherContentDismissType * const PHPublisherNativeCloseButtonTriggeredDismiss     = @"PHPublisherNativeCloseButtonTriggeredDismiss";
PHPublisherContentDismissType * const PHPublisherApplicationBackgroundTriggeredDismiss = @"PHPublisherApplicationBackgroundTriggeredDismiss";
PHPublisherContentDismissType * const PHPublisherNoContentTriggeredDismiss             = @"PHPublisherNoContentTriggeredDismiss";

#define MAX_MARGIN 20

@interface PHAPIRequest (Private)
+ (NSMutableSet *)allRequests;
- (id)initWithApp:(NSString *)token secret:(NSString *)secret;
- (void)finish;
@end

@interface PHPublisherContentRequest (Private)
+ (PHPublisherContentRequest *)existingRequestForApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement;
- (id)initWithApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate;
- (CGAffineTransform)transformForOrientation:(UIInterfaceOrientation)orientation;
- (void)placeCloseButton;
- (void)hideCloseButton;
- (void)showCloseButtonBecauseOfTimeout;
- (void)showOverlayWindow;
- (void)hideOverlayWindow;
- (void)dismissFromButton;
- (void)dismissToBackground;
- (void)continueLoadingIfNeeded;
- (void)getContent;
- (void)showContentIfReady;
- (void)pushContent:(PHContent *)content;
- (void)removeContentView:(PHContentView *)contentView;

@property (nonatomic, readonly) UIButton *closeButton;
@property (nonatomic, readonly) PHPublisherContentRequestState state;

- (BOOL)setPublisherContentRequestState:(PHPublisherContentRequestState)state;
- (void)requestSubcontent:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source;

- (BOOL)isValidReward:(NSDictionary *)rewardData;
- (void)requestRewards:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source;

- (BOOL)isValidPurchase:(NSDictionary *)purchaseData;
- (void)requestPurchases:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source;

- (void)requestCloseButton:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source;
@end

@implementation PHPublisherContentRequest
@synthesize placement               = _placement;
@synthesize animated                = _animated;
@synthesize showsOverlayImmediately = _showsOverlayImmediately;
//@synthesize state                   = _state;
//@dynamic    state; // TODO: Figure out what's going on with this variable (state)
//                   // TODO: Lilli added @dynamic to try and quelch warnings, but still there

+ (PHPublisherContentRequest *)existingRequestForApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement
{
    NSEnumerator *allRequests = [[PHAPIRequest allRequests] objectEnumerator];

    PHAPIRequest *request = nil;
    while ((request = [allRequests nextObject])) {
        if ([request isKindOfClass:[PHPublisherContentRequest class]]) {
            PHPublisherContentRequest *contentRequest = (PHPublisherContentRequest*) request;
            if ([contentRequest.placement isEqualToString:placement] && [contentRequest.token isEqualToString:token] && [contentRequest.secret isEqualToString:secret]) {
                return contentRequest;
            }
        }
    }

    return nil;
}

+ (id)requestForApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate
{
    PHPublisherContentRequest *request = [PHPublisherContentRequest existingRequestForApp:token secret:secret placement:placement];
    if (!!request) {
        request.delegate = delegate;
        return request;
    } else {
        return [[[[self class] alloc] initWithApp:token secret:secret placement:placement delegate:delegate] autorelease];
    }
}

- (id)initWithApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate
{
    if ((self = [self initWithApp:token secret:secret])) {
        self.placement = placement;
        self.delegate = delegate;
    }

    return self;
}

- (id)initWithApp:(NSString *)token secret:(NSString *)secret
{
    if ((self = [super initWithApp:token secret:secret])) {
        _state    = PHPublisherContentRequestInitialized;
        _animated = YES;
    }

    return self;
}

- (PHPublisherContentRequestState)state
{
    return _state;
}

- (BOOL)setPublisherContentRequestState:(PHPublisherContentRequestState)state
{
    // State may only be set ahead!
    if (_state < state) {
        [self willChangeValueForKey:@"state"]; // TODO: Figure out what's going on with this variable (state)
        _state = state;
        [self didChangeValueForKey:@"state"];
        return YES;
    }
    return NO;
}

- (NSMutableArray *)contentViews
{
    if (_contentViews == nil) {
        _contentViews = [[NSMutableArray alloc] init];
    }

    return _contentViews;
}

- (UIView *)overlayWindow
{
    if (_overlayWindow == nil) {
        _overlayWindow = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
        _overlayWindow.backgroundColor = [UIColor colorWithWhite:0.0 alpha:0.5];
    }

    return  _overlayWindow;
}

- (UIButton *)closeButton
{
    if (_closeButton == nil) {
        _closeButton = [[UIButton buttonWithType:UIButtonTypeCustom] retain];
        _closeButton.frame  = CGRectMake(0, 0, 40, 40);
        _closeButton.hidden = YES;

        UIImage
            *closeImage       = [self contentView:nil imageForCloseButtonState:UIControlStateNormal],
            *closeActiveImage = [self contentView:nil imageForCloseButtonState:UIControlStateHighlighted];

        closeImage       = (!closeImage) ?
                                convertByteDataToUIImage((playHavenImage *)&close_image) :
                                closeImage;
        closeActiveImage = (!closeActiveImage) ?
                                convertByteDataToUIImage((playHavenImage *)&close_active_image) :
                                closeActiveImage;

        [_closeButton setImage:closeImage forState:UIControlStateNormal];
        [_closeButton setImage:closeActiveImage forState:UIControlStateHighlighted];

        [_closeButton addTarget:self action:@selector(dismissFromButton) forControlEvents:UIControlEventTouchUpInside];
    }

    return _closeButton;
}

- (void)dealloc
{
    [PHPublisherContentRequest cancelPreviousPerformRequestsWithTarget:self selector:@selector(showCloseButtonBecauseOfTimeout) object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [_content release], _content = nil;
    [_placement release], _placement = nil;
    [_contentViews release], _contentViews = nil;
    [_closeButton release], _closeButton = nil;
    [_overlayWindow release], _overlayWindow = nil;

    [super dealloc];
}

#pragma mark - Internal UI management
- (void)placeCloseButton
{
    if ([_closeButton superview] == nil) {
        // TRACK_ORIENTATION see STOP_TRACK_ORIENTATION
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(placeCloseButton)
                                                     name:UIDeviceOrientationDidChangeNotification
                                                   object:nil];
    }

    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    CGFloat barHeight = ([[UIApplication sharedApplication] isStatusBarHidden]) ? 0 : 20;

    CGRect  screen = [[UIScreen mainScreen] applicationFrame];
    CGFloat width  = screen.size.width,
            height = screen.size.height;
    CGFloat X = 0.0,
            Y = 0.0;

    switch (orientation) {
        case UIInterfaceOrientationPortrait:
            X = width - MAX_MARGIN;
            Y = MAX_MARGIN + barHeight;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            X = MAX_MARGIN;
            Y = height - MAX_MARGIN;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            X = MAX_MARGIN + barHeight;
            Y = MAX_MARGIN;
            break;
        case UIInterfaceOrientationLandscapeRight:
            X = width - MAX_MARGIN;
            Y = height - MAX_MARGIN;
            break;
    }

    // Find the topmost contentView
    PHContentView *topContentView = [self.contentViews lastObject];
    if (!!topContentView) {
        CGRect contentFrame = [topContentView.content frameForOrientation:orientation];
        switch (orientation) {
            case UIInterfaceOrientationPortrait:
                X = MIN(X, CGRectGetMaxX(contentFrame));
                Y = MAX(Y, CGRectGetMinY(contentFrame) + barHeight);
                break;

            case UIInterfaceOrientationPortraitUpsideDown:
                X = MAX(X, width - CGRectGetMaxX(contentFrame));
                Y = MIN(Y, height - CGRectGetMinY(contentFrame));
                break;

            case UIInterfaceOrientationLandscapeLeft:
                X = MAX(X, CGRectGetMinY(contentFrame) + barHeight);
                Y = MAX(Y, height - CGRectGetMaxX(contentFrame));
                break;

            case UIInterfaceOrientationLandscapeRight:
                X = MIN(X, width - CGRectGetMinY(contentFrame));
                Y = MIN(Y, CGRectGetMaxX(contentFrame));
                break;
        }
    }

    self.closeButton.center = CGPointMake(X, Y);
    self.closeButton.transform = [self transformForOrientation:orientation];

    if (!!topContentView) {
        [self.overlayWindow insertSubview:self.closeButton aboveSubview:topContentView];
    } else {
        [self.overlayWindow addSubview:self.closeButton];
    }
}

- (void)showCloseButtonBecauseOfTimeout
{
    self.closeButton.hidden = NO;
}

- (void)hideCloseButton
{
    [PHPublisherContentRequest cancelPreviousPerformRequestsWithTarget:self
                                                              selector:@selector(showCloseButtonBecauseOfTimeout)
                                                                object:nil];

    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UIDeviceOrientationDidChangeNotification
                                                  object:nil];

    [_closeButton removeFromSuperview];
}

- (void)showOverlayWindow
{
    // This should keep UIKeyboard instances from blocking the content view.
    // It should be fine if this runs more than once.
    UIWindow *window = [[[UIApplication sharedApplication] windows] objectAtIndex:0];
    [window addSubview:self.overlayWindow];

    //This should keep UIKeyboard instances from blocking the content view.
    //It should be fine if this runs more than once.
    [window endEditing:YES];
}

- (void)hideOverlayWindow
{
    // Let's avoid creating an overlay instance if we don't need to.
    if (!!_overlayWindow) {
        [self.overlayWindow removeFromSuperview];
    }
}

#pragma mark - PHAPIRequest

- (NSString *)urlPath
{
    return PH_URL(/v3/publisher/content/);
}

- (NSDictionary *)additionalParameters
{
#if PH_USE_STOREKIT != 0
    return [NSDictionary dictionaryWithObjectsAndKeys:
                                 self.placement,
                                          @"placement_id",
                                 [NSNumber numberWithInt:(int)floor([[PHTimeInGame getInstance] getCurrentSessionDuration])],
                                          @"stime",
                                 [NSNumber numberWithBool:(_targetState == PHPublisherContentRequestPreloaded)],
                                          @"preload",
                                 [NSNumber numberWithBool:([SKStoreProductViewController class] != nil)],
                                          @"isa", nil];
#else
    return [NSDictionary dictionaryWithObjectsAndKeys:
                                 self.placement,
                                          @"placement_id",
                                 [NSNumber numberWithInt:(int)floor([[PHTimeInGame getInstance] getCurrentSessionDuration])],
                                          @"stime",
                                 [NSNumber numberWithBool:(_targetState == PHPublisherContentRequestPreloaded)],
                                          @"preload",
                                 [NSNumber numberWithBool:NO],
                                          @"isa", nil];
#endif
}

- (void)cancel
{
    if ([self.contentViews count] > 0) {
        NSArray *contentViews = [self.contentViews copy];
        for (PHContentView *contentView in contentViews) {
            contentView.delegate = nil;
            [contentView dismiss:NO];

            [self removeContentView:contentView];
        }

        [contentViews release];
    }

    [super cancel];
}

- (void)finish
{
    if ([self setPublisherContentRequestState:PHPublisherContentRequestDone]) {
        [PHAPIRequest cancelAllRequestsWithDelegate:self];


        [self hideOverlayWindow];
        [self hideCloseButton];

        [super finish];
    }
}

- (void)afterConnectionDidFinishLoading
{
    // Don't do anything
}

- (void)didSucceedWithResponse:(NSDictionary *)responseData
{
    [_content release], _content = [[PHContent contentWithDictionary:responseData] retain];
    if (!!_content) {
        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(requestDidGetContent:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(requestDidGetContent:)
                                                                       withObject:self];
        }

        if ([self setPublisherContentRequestState:PHPublisherContentRequestPreloaded]) {
            [self continueLoadingIfNeeded];
        }
    } else {
        PH_NOTE(@"The request was successful but did not contain any displayable content. This may have occurred because there are no content units assigned to this placement; all content units assigned to this placement are suppressed by frequency caps or targeting; no ad campaigns are available at this time; or an invalid placement was requested. Visit the PlayHaven Dashboard for more details. Dismissing now.");
        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:contentDidDismissWithType:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(request:contentDidDismissWithType:)
                                                                       withObject:self
                                                                       withObject:PHPublisherNoContentTriggeredDismiss];
        } else if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(requestContentDidDismiss:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(requestContentDidDismiss:)
                                                                       withObject:self];
        }

        [self finish];
    }
}

- (void)preload
{
    _targetState = PHPublisherContentRequestPreloaded;
    [self continueLoadingIfNeeded];
}

- (void)send
{
#ifdef PH_DISMISS_WHEN_BACKGROUNDED
    if (PH_MULTITASKING_SUPPORTED) {
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(dismissToBackground)
                                                     name:UIApplicationDidEnterBackgroundNotification
                                                   object:nil];
    }
#endif

    _targetState = PHPublisherContentRequestDisplayingContent;

    if (self.showsOverlayImmediately) {
        [self showOverlayWindow];
    }

    [self performSelector:@selector(showCloseButtonBecauseOfTimeout) withObject:nil afterDelay:4.0];

    [self continueLoadingIfNeeded];
}

- (void)continueLoadingIfNeeded
{
    switch (self.state) {
        case PHPublisherContentRequestInitialized:
            [self getContent];
            break;
        case PHPublisherContentRequestPreloaded:
            [self showContentIfReady];
            break;
        default:
            break;
    }
}

- (void)getContent
{
    if ([self setPublisherContentRequestState:PHPublisherContentRequestPreloading]) {
        [super send];

        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(requestWillGetContent:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(requestWillGetContent:)
                                                                       withObject:self];
        }

        [self placeCloseButton];
    }
}

- (void)showContentIfReady
{
    if (_targetState >= PHPublisherContentRequestDisplayingContent) {
        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:contentWillDisplay:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(request:contentWillDisplay:)
                                                                       withObject:self
                                                                       withObject:_content];
        }

        if ([self setPublisherContentRequestState:PHPublisherContentRequestDisplayingContent]) {
            [self showOverlayWindow];
            [self pushContent:_content];
        }
    }
}

#pragma mark -
#pragma mark Sub-content
- (void)requestSubcontent:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source
{
    if (!!queryParameters && [queryParameters valueForKey:@"url"]) {
        PHPublisherSubContentRequest *request = [PHPublisherSubContentRequest requestForApp:self.token secret:self.secret];
        request.delegate = self;

        //request.urlPath  = [queryParameters valueForKey:@"url"];

        request.additionalParameters = [queryParameters objectForKey:@"additional_parameters"];

        request.callback = callback;
        request.source   = source;

        [request send];
    } else {
        NSDictionary *errorDict = [NSDictionary dictionaryWithObjectsAndKeys:@"1", @"error", nil];
        [source sendCallback:callback withResponse:nil error:errorDict];
    }
}

- (void)request:(PHAPIRequest *)request didSucceedWithResponse:(NSDictionary *)responseData
{
    PHContent *content = [PHContent contentWithDictionary:responseData];
    PHPublisherSubContentRequest *scRequest = (PHPublisherSubContentRequest *)request;
    if (!!content) {
        [self pushContent:content];
        [scRequest.source sendCallback:scRequest.callback withResponse:responseData error:nil];
    } else {
        NSDictionary *errorDict = [NSDictionary dictionaryWithObjectsAndKeys:@"1", @"error", nil];
        [scRequest.source sendCallback:scRequest.callback withResponse:nil error:errorDict];
    }
}

- (void)request:(PHAPIRequest *)request didFailWithError:(NSError *)error
{
    PHPublisherSubContentRequest *scRequest = (PHPublisherSubContentRequest *)request;
    NSDictionary *errorDict = [NSDictionary dictionaryWithObjectsAndKeys:@"1", @"error", nil];
    [scRequest.source sendCallback:scRequest.callback withResponse:nil error:errorDict];
}

- (void)pushContent:(PHContent *)content
{
    PHContentView *contentView = [PHContentView dequeueContentViewInstance];

    if (!contentView)
        contentView = [[[PHContentView alloc] initWithContent:nil] autorelease];

    [contentView redirectRequest:@"ph://subcontent" toTarget:self action:@selector(requestSubcontent:callback:source:)];
    [contentView redirectRequest:@"ph://reward" toTarget:self action:@selector(requestRewards:callback:source:)];
    [contentView redirectRequest:@"ph://purchase" toTarget:self action:@selector(requestPurchases:callback:source:)];
    [contentView redirectRequest:@"ph://closeButton" toTarget:self action:@selector(requestCloseButton:callback:source:)];

    contentView.content = content;
    [contentView setDelegate:self];
    [contentView setTargetView:self.overlayWindow];

    [self.contentViews addObject:contentView];
    [contentView show:self.animated];

    [self placeCloseButton];
}

- (void)removeContentView:(PHContentView *)contentView
{
    [contentView retain];
    [self.contentViews removeObject:contentView];
    [PHContentView enqueueContentViewInstance:contentView];
    [contentView release];
}

- (void)dismissFromButton
{
    PH_NOTE(@"The content unit was dismissed by the user");
    if ([self.contentViews count] > 0) {
        NSArray *contentViews = [self.contentViews copy];
        for (PHContentView *contentView in contentViews) {
            contentView.delegate = nil;
            [contentView dismiss:NO];

            [self removeContentView:contentView];
        }
        [contentViews release];
    }

    if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:contentDidDismissWithType:)]) {
        [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(request:contentDidDismissWithType:)
                                                                   withObject:self
                                                                   withObject:PHPublisherNativeCloseButtonTriggeredDismiss];
    } else {
        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(requestContentDidDismiss:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(requestContentDidDismiss:)
                                                                       withObject:self];
        }
    }

    [self finish];
}

- (void)dismissToBackground
{
    PH_NOTE(@"The content unit was dismissed because the app has been backgrounded.");
    if ([self.contentViews count] > 0) {
        NSArray *contentViews = [self.contentViews copy];
        for (PHContentView *contentView in contentViews) {
            contentView.delegate = nil;
            [contentView dismiss:NO];

            [self removeContentView:contentView];
        }
        [contentViews release];
    }

    if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:contentDidDismissWithType:)]) {
        [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(request:contentDidDismissWithType:)
                                                                   withObject:self
                                                                   withObject:PHPublisherApplicationBackgroundTriggeredDismiss];
    } else {
        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(requestContentDidDismiss:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(requestContentDidDismiss:)
                                                                       withObject:self];
        }
    }

    [self finish];
}

- (CGAffineTransform)transformForOrientation:(UIInterfaceOrientation)orientation
{
    if (orientation == UIInterfaceOrientationLandscapeLeft) {
        return CGAffineTransformMakeRotation(-M_PI/2);
    } else if (orientation == UIInterfaceOrientationLandscapeRight) {
        return CGAffineTransformMakeRotation(M_PI/2);
    } else if (orientation == UIInterfaceOrientationPortraitUpsideDown) {
        return CGAffineTransformMakeRotation(-M_PI);
    } else {
        return CGAffineTransformIdentity;
    }
}

#pragma mark -
#pragma mark PHContentViewDelegate
- (void)contentViewDidLoad:(PHContentView *)contentView
{
    if ([self.contentViews count] == 1) {
        // Only pass-through the first contentView load
        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:contentDidDisplay:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(request:contentDidDisplay:)
                                                                       withObject:self
                                                                       withObject:contentView.content];
        }
    }
}

- (void)contentViewDidDismiss:(PHContentView *)contentView
{
    [self removeContentView:contentView];

    if ([self.contentViews count] == 0) {
        // Only pass-through the last contentView to dismiss
        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:contentDidDismissWithType:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(request:contentDidDismissWithType:)
                                                                       withObject:self
                                                                       withObject:PHPublisherContentUnitTriggeredDismiss];
        } else if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(requestContentDidDismiss:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(requestContentDidDismiss:)
                                                                       withObject:self];
        }

        [self finish];
    }
}

- (void)contentView:(PHContentView *)contentView didFailWithError:(NSError *)error
{
    [self removeContentView:contentView];

    if ([self.contentViews count] == 0) {
        // Only pass-through the last contentView to error
        if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:contentDidFailWithError:)]) {
            PH_NOTE(@"It seems like you're using the -request:contentDidFailWithError: delegate method. This delegate has been deprecated, please use -request:didFailWithError: instead.");
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(request:contentDidFailWithError:)
                                                                       withObject:self
                                                                       withObject:error];
        } else if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:didFailWithError:)]) {
            [(id<PHPublisherContentRequestDelegate>)self.delegate performSelector:@selector(request:didFailWithError:)
                                                                       withObject:self
                                                                       withObject:error];
        }

        [self finish];
    }
}

- (UIImage *)contentView:(PHContentView *)contentView imageForCloseButtonState:(UIControlState)state
{
    if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:closeButtonImageForControlState:content:)]) {
        return [(id<PHPublisherContentRequestDelegate>)self.delegate request:self
                                             closeButtonImageForControlState:state
                                                                     content:contentView.content];
    }

    return nil;
}

- (UIColor *)borderColorForContentView:(PHContentView *)contentView
{
    if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:borderColorForContent:)]) {
        return [(id<PHPublisherContentRequestDelegate>)self.delegate request:self
                                                       borderColorForContent:contentView.content];
    }

    return nil;
}

#pragma mark - Reward unlocking methods
- (BOOL)isValidReward:(NSDictionary *)rewardData
{
    NSString *reward    = [rewardData valueForKey:PHPublisherContentRequestRewardIDKey];
    NSNumber *quantity  = [rewardData valueForKey:PHPublisherContentRequestRewardQuantityKey];
    NSNumber *receipt   = [rewardData valueForKey:PHPublisherContentRequestRewardReceiptKey];
    NSString *signature = [rewardData valueForKey:PHPublisherContentRequestRewardSignatureKey];

    NSString *generatedSignatureString =
                     [NSString stringWithFormat:@"%@:%@:%@:%@:%@",
                                          reward,
                                          quantity,
                                          PHGID(),
                                          receipt,
                                          self.secret];

    NSString *generatedSignature = [PHStringUtil hexDigestForString:generatedSignatureString];

    return [generatedSignature isEqualToString:signature];
}

- (void)requestRewards:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source
{
    NSArray *rewardsArray = [queryParameters valueForKey:@"rewards"];
    for (NSDictionary *rewardData in rewardsArray) {
        if ([self isValidReward:rewardData]) {
            PHReward *reward = [[PHReward new] autorelease]; // TODO: Why is PHReward created with 'new'? Should be alloc init; fix.

            reward.name     = [rewardData valueForKey:PHPublisherContentRequestRewardIDKey];
            reward.quantity = [[rewardData valueForKey:PHPublisherContentRequestRewardQuantityKey] integerValue];
            reward.receipt  = [[rewardData valueForKey:PHPublisherContentRequestRewardReceiptKey] stringValue];

            if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:unlockedReward:)]) {
                [(id<PHPublisherContentRequestDelegate>)self.delegate request:self unlockedReward:reward];
            }
        }
    }

    [source sendCallback:callback withResponse:nil error:nil];
}

#pragma mark - Purchase unlocking methods
- (BOOL)isValidPurchase:(NSDictionary *)purchaseData
{
    NSString *productId = [purchaseData valueForKey:PHPublisherContentRequestPurchaseProductIDKey];
    NSString *name      = [purchaseData valueForKey:PHPublisherContentRequestPurchaseNameKey];
    NSNumber *quantity  = [purchaseData valueForKey:PHPublisherContentRequestPurchaseQuantityKey];
    NSNumber *receipt   = [purchaseData valueForKey:PHPublisherContentRequestPurchaseReceiptKey];
    NSString *signature = [purchaseData valueForKey:PHPublisherContentRequestPurchaseSignatureKey];

    NSString *generatedSignatureString =
                     [NSString stringWithFormat:@"%@:%@:%@:%@:%@:%@",
                                          productId,
                                          name,
                                          quantity,
                                          PHGID(),
                                          receipt,
                                          self.secret];
    NSString *generatedSignature = [PHStringUtil hexDigestForString:generatedSignatureString];

    return [generatedSignature isEqualToString:signature];
}

- (void)requestPurchases:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source
{
    NSArray *purchasesArray = [queryParameters valueForKey:@"purchases"];
    for (NSDictionary *purchaseData in purchasesArray) {
        if ([self isValidPurchase:purchaseData]) {
            PHPurchase *purchase = [PHPurchase new];

            purchase.productIdentifier = PHAgnosticStringValue([purchaseData valueForKey:PHPublisherContentRequestPurchaseProductIDKey]);
            purchase.name              = PHAgnosticStringValue([purchaseData valueForKey:PHPublisherContentRequestPurchaseNameKey]);
            purchase.quantity          = [[purchaseData valueForKey:PHPublisherContentRequestPurchaseQuantityKey] integerValue];
            purchase.receipt           = PHAgnosticStringValue([purchaseData valueForKey:PHPublisherContentRequestPurchaseReceiptKey]);
            purchase.callback          = callback;

#if PH_USE_STOREKIT != 0
            NSString *cookie = PHAgnosticStringValue([purchaseData valueForKey:PHPublisherContentRequestPurchaseCookieKey]);
            [PHPublisherIAPTrackingRequest setConversionCookie:cookie forProduct:purchase.productIdentifier];
#endif
            if ([(id<PHPublisherContentRequestDelegate>)self.delegate respondsToSelector:@selector(request:makePurchase:)]) {
                [(id<PHPublisherContentRequestDelegate>)self.delegate request:self makePurchase:purchase];
            }
            [purchase release];
        }
    }
}

#pragma mark - Close button control
- (void)requestCloseButton:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source
{
    [PHPublisherContentRequest cancelPreviousPerformRequestsWithTarget:self selector:@selector(showCloseButtonBecauseOfTimeout) object:nil];

    if ([queryParameters valueForKey:@"hidden"]) {
        self.closeButton.hidden = [[queryParameters valueForKey:@"hidden"] boolValue];
    }

    NSArray *keys = [NSArray arrayWithObjects:@"hidden",nil];
    NSDictionary *response = [self.closeButton dictionaryWithValuesForKeys:keys];
    [source sendCallback:callback withResponse:response error:nil];
}
@end
