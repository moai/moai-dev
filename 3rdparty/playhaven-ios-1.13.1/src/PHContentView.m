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

 PHContentView.m (formerly PHAdUnitView.m)
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/1/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHContentView.h"
#import "PHContent.h"
#import "NSObject+QueryComponents.h"
#import "JSON.h"
#import "PHConstants.h"
#import "PHStoreProductViewControllerDelegate.h"
#import "PHConnectionManager.h"
#import "PHResourceCacher.h"

#define MAX_MARGIN 20

@interface PHContentView (Private)
+ (void)clearContentViews;
+ (NSMutableSet *)allContentViews;
- (void)sizeToFitOrientation:(BOOL)transform;
- (CGAffineTransform)transformForOrientation:(UIInterfaceOrientation)orientation;
- (void)orientationDidChange;
- (void)viewDidShow;
- (void)viewDidDismiss;
- (void)loadTemplate;
- (void)handleDismiss:(NSDictionary *)queryComponents;
- (void)handleLoadContext:(NSDictionary *)queryComponents callback:(NSString*)callback;
- (UIActivityIndicatorView *)activityView;
- (void)dismissWithError:(NSError *)error;
- (void)closeView:(BOOL)animated;
- (void)prepareForReuse;
- (void)resetRedirects;
- (void)bounceOut;
- (void)bounceIn;
//- (void)handleLaunch:(NSDictionary *)queryComponents;
//- (void)dismissView;
@end

static NSMutableSet *allContentViews = nil;

@implementation PHContentView
@synthesize content    = _content;
@synthesize delegate   = _delegate;
@synthesize targetView = _targetView;

#pragma mark - Static Methods

+ (void)initialize
{
    if (self == [PHContentView class]) {
        [[NSNotificationCenter defaultCenter] addObserver:[PHContentView class]
                                                 selector:@selector(clearContentViews)
                                                     name:UIApplicationDidReceiveMemoryWarningNotification
                                                   object:nil];
    }
}

+ (NSMutableSet *)allContentViews
{
    @synchronized (allContentViews) {
        if (allContentViews == nil) {
            allContentViews = [[NSMutableSet alloc] init];
        }
    }
    return allContentViews;
}

+ (void)clearContentViews
{
    @synchronized (allContentViews) {
        [allContentViews release], allContentViews = nil;
    }
}

- (void)contentViewsCallback:(NSNotification *)notification
{
    if ([[notification name] isEqualToString:PHCONTENTVIEW_CALLBACK_NOTIFICATION]) {
        NSDictionary *callBack = (NSDictionary *)[notification object];
        [self sendCallback:[callBack valueForKey:@"callback"]
              withResponse:[callBack valueForKey:@"response"]
                     error:[callBack valueForKey:@"error"]];
    }
}

+ (PHContentView *)dequeueContentViewInstance
{
#ifdef PH_USE_CONTENT_VIEW_RECYCLING
    PHContentView *instance = [[PHContentView allContentViews] anyObject];
    if (!!instance) {
        [instance retain];
        [[PHContentView allContentViews] removeObject:instance];
        [instance autorelease];
    }

    return instance;
#else
    return nil;
#endif
}

+ (void)enqueueContentViewInstance:(PHContentView *)contentView
{
#ifdef PH_USE_CONTENT_VIEW_RECYCLING
    [[self allContentViews] addObject:contentView];
#endif
}

#pragma mark -
- (id)initWithContent:(PHContent *)content
{
    if ((self = [super initWithFrame:[[UIScreen mainScreen] applicationFrame]])) {
        NSInvocation
            *dismissRedirect     = [NSInvocation invocationWithMethodSignature:
                                        [[PHContentView class] instanceMethodSignatureForSelector:@selector(handleDismiss:)]],
            *launchRedirect      = [NSInvocation invocationWithMethodSignature:
                                        [[PHContentView class] instanceMethodSignatureForSelector:@selector(handleLaunch:callback:)]],
            *loadContextRedirect = [NSInvocation invocationWithMethodSignature:
                                        [[PHContentView class] instanceMethodSignatureForSelector:@selector(handleLoadContext:callback:)]];

        dismissRedirect.target   = self;
        dismissRedirect.selector = @selector(handleDismiss:);

        launchRedirect.target   = self;
        launchRedirect.selector = @selector(handleLaunch:callback:);

        loadContextRedirect.target   = self;
        loadContextRedirect.selector = @selector(handleLoadContext:callback:);

        _redirects = [[NSMutableDictionary alloc] initWithObjectsAndKeys:
                                                          dismissRedirect,     @"ph://dismiss",
                                                          launchRedirect,      @"ph://launch",
                                                          loadContextRedirect, @"ph://loadContext", nil];

#ifndef PH_UNIT_TESTING
        _webView = [[UIWebView alloc] initWithFrame:CGRectZero];
        _webView.accessibilityLabel = @"content view";

#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 60000
        if ([_webView respondsToSelector:@selector(setSuppressesIncrementalRendering:)])
            [_webView setSuppressesIncrementalRendering:YES];
        if ([_webView respondsToSelector:@selector(setKeyboardDisplayRequiresUserAction:)])
            [_webView setKeyboardDisplayRequiresUserAction:NO];
#endif
#endif

        [self addSubview:_webView];
#endif

        self.content = content;
    }

    return self;
}

- (void)resetRedirects
{
#ifdef PH_USE_CONTENT_VIEW_RECYCLING
    NSEnumerator *keyEnumerator = [[_redirects allKeys] objectEnumerator];
    NSString     *key;

    while ((key = [keyEnumerator nextObject])) {
        NSInvocation *invocation = [_redirects valueForKey:key];
        if (invocation.target != self) {
            [_redirects removeObjectForKey:key];
        }
    }
#endif
}

- (void)prepareForReuse
{
    self.content  = nil;
    self.delegate = nil;

    [self resetRedirects];
    [_webView stringByEvaluatingJavaScriptFromString:@"document.open();document.close();"];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [PHURLLoader invalidateAllLoadersWithDelegate:self];
}

- (UIActivityIndicatorView *)activityView
{
    if (_activityView == nil) {
        _activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
        _activityView.hidesWhenStopped = YES;
        [_activityView startAnimating];
    }

    return _activityView;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [PHURLLoader invalidateAllLoadersWithDelegate:self];

    [_content release], _content = nil;
    [_webView release], _webView = nil;
    [_redirects release], _redirects = nil;
    [_activityView release] , _activityView = nil;

    [super dealloc];
}

- (void)orientationDidChange
{
    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if (orientation != _orientation) {
        if (CGRectIsNull([self.content frameForOrientation:orientation])) {
            [self dismissWithError:PHCreateError(PHOrientationErrorType)];
            return;
        }

        CGFloat duration = [UIApplication sharedApplication].statusBarOrientationAnimationDuration;

        if (self.content.transition == PHContentTransitionDialog) {
            CGFloat barHeight   = ([[UIApplication sharedApplication] isStatusBarHidden]) ? 0 : 20;
            CGRect contentFrame = CGRectOffset([self.content frameForOrientation:orientation], 0, barHeight);
            _webView.frame = contentFrame;
        }

        [UIView beginAnimations:nil context:nil];
        [UIView setAnimationDuration:duration];
        [self sizeToFitOrientation:YES];
        [UIView commitAnimations];
    }
}

- (void)sizeToFitOrientation:(BOOL)transform
{
    if (transform) {
        self.transform = CGAffineTransformIdentity;
    }

    CGRect frame = [UIScreen mainScreen].bounds;
    CGPoint center = CGPointMake(frame.origin.x + ceil(frame.size.width/2),
                                 frame.origin.y + ceil(frame.size.height/2));

    CGFloat scale_factor = 1.0f;

    CGFloat width = floor(scale_factor * frame.size.width);
    CGFloat height = floor(scale_factor * frame.size.height);

    _orientation = [UIApplication sharedApplication].statusBarOrientation;
    if (UIInterfaceOrientationIsLandscape(_orientation)) {
        self.frame = CGRectMake(0, 0, height, width);
    } else {
        self.frame = CGRectMake(0, 0, width, height);
    }

    self.center = center;

    if (transform) {
        self.transform = [self transformForOrientation:_orientation];
    }
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

- (void)show:(BOOL)animated
{
    // Reset transforms before doing anything
    _webView.transform = CGAffineTransformIdentity;
    _webView.alpha     = 1.0;

    self.transform = CGAffineTransformIdentity;
    self.alpha     = 1.0;

    // Actually start showing
    _willAnimate = animated;
    [self.targetView addSubview: self];
    [self sizeToFitOrientation:YES];

    [_webView setDelegate:self];

    [self loadTemplate];

    if (CGRectIsNull([self.content frameForOrientation:_orientation])) {
        // This is an invalid frame and we should dismiss immediately!
        [self dismissWithError:PHCreateError(PHOrientationErrorType)];
        return;
    }

    CGFloat barHeight = ([[UIApplication sharedApplication] isStatusBarHidden]) ? 0 : 20;

    if (self.content.transition == PHContentTransitionModal) { // Not really used, but not yet killed; may be in disrepair
        self.backgroundColor = [UIColor clearColor];
        self.opaque = NO;

        CGFloat width, height;
        if (UIInterfaceOrientationIsPortrait(_orientation)) {
            width  = self.frame.size.width;
            height = self.frame.size.height;
        } else {
            width  = self.frame.size.height;
            height = self.frame.size.width;
        }

        _webView.frame = CGRectMake(0, barHeight, width, height - barHeight);

        [self activityView].center = _webView.center;

        if (animated) {
            CGAffineTransform oldTransform = self.transform;
            self.transform = CGAffineTransformTranslate(oldTransform, 0, self.frame.size.height);
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
            [UIView setAnimationDuration:0.25];
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(viewDidShow)];
            self.transform = oldTransform;
            [UIView commitAnimations];
        } else {
            [self viewDidShow];
        }
    } else if (self.content.transition == PHContentTransitionDialog) {
        self.backgroundColor = [UIColor colorWithWhite:0.0 alpha:0.1];
        self.opaque = NO;

        UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
        CGRect contentFrame = CGRectOffset([self.content frameForOrientation:orientation], 0, barHeight);

        _webView.frame = contentFrame;

        _webView.layer.borderWidth = 1.0f;

        if ([self.delegate respondsToSelector:@selector(borderColorForContentView:)]) {
            _webView.layer.borderColor = [[self.delegate borderColorForContentView:self] CGColor];
        } else {
            _webView.layer.borderColor = [[UIColor blackColor] CGColor];
        }

        [self activityView].center = _webView.center;

        if (animated) {
            [self bounceIn];
        } else {
            [self viewDidShow];
        }
    }

    [self addSubview:[self activityView]];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(contentViewsCallback:)
                                                 name:PHCONTENTVIEW_CALLBACK_NOTIFICATION
                                               object:nil];

    // TRACK_ORIENTATION see STOP_TRACK_ORIENTATION
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(orientationDidChange)
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];
}

- (void)dismiss:(BOOL)animated
{
    [self closeView:animated];
}

- (void)dismissFromButton
{
    NSDictionary *queryComponents = [NSDictionary dictionaryWithObjectsAndKeys:
                                                          self.content.closeButtonURLPath, @"ping", nil];
    [self handleDismiss:queryComponents];
}

- (void)dismissWithError:(NSError *)error
{
    // This is here because get called 2x
    // first from handleLoadContext:
    // second from webView:didFailLoadWithError:
    // Only need to handle once
    if (self.delegate == nil)
        return;

    id oldDelegate = self.delegate;
    self.delegate = nil;
    [self closeView:_willAnimate];

    if ([oldDelegate respondsToSelector:(@selector(contentView:didFailWithError:))]) {
        PH_LOG(@"Error with content view: %@", [error localizedDescription]);
        [oldDelegate contentView:self didFailWithError:error];
    }
}

- (void)closeView:(BOOL)animated
{
    [_webView setDelegate:nil];
    [_webView stopLoading];

    _willAnimate = animated;
    if (self.content.transition == PHContentTransitionModal) {
        if (animated) {
            CGAffineTransform oldTransform = self.transform;
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
            [UIView setAnimationDuration:0.25];
            [UIView setAnimationDelegate:self];
            [UIView setAnimationDidStopSelector:@selector(viewDidDismiss)];
            self.transform = CGAffineTransformTranslate(oldTransform, 0, self.frame.size.height);
            [UIView commitAnimations];
        } else {
            [self viewDidDismiss];
        }
    } else if (self.content.transition == PHContentTransitionDialog) {
        if (_willAnimate) {
            [self bounceOut];
        } else {
            [self viewDidDismiss];
        }
    }

    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:PHCONTENTVIEW_CALLBACK_NOTIFICATION
                                                  object:nil];

    // STOP_TRACK_ORIENTATION see TRACK_ORIENTATION
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UIDeviceOrientationDidChangeNotification
                                                  object:nil];
}

- (void)templateLoaded:(NSNotification *)notification
{
    NSURLRequest  *request  = [notification.userInfo objectForKey:@"request"];

    if ([request.URL.absoluteString isEqualToString:self.content.URL.absoluteString]) {

        [[NSNotificationCenter defaultCenter] removeObserver:self
                                                        name:PH_PREFETCH_CALLBACK_NOTIFICATION
                                                      object:nil];
        [self loadTemplate];

    }
}

- (void)loadTemplate
{
    [_webView stopLoading];

    NSURLRequest *request = [NSURLRequest requestWithURL:self.content.URL
                                             cachePolicy:NSURLRequestUseProtocolCachePolicy
                                         timeoutInterval:PH_REQUEST_TIMEOUT];

    if ([PHResourceCacher isRequestPending:[[request URL] absoluteString]]) {
        PH_NOTE(@"Template is already being downloaded. Will come back when complete!");

        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(templateLoaded:)
                                                     name:PH_PREFETCH_CALLBACK_NOTIFICATION
                                                   object:nil];
    } else {
        [PHResourceCacher pause];

        PH_LOG(@"Loading template from network or cahce: %@", self.content.URL);
        [_webView loadRequest:request];
    }
}

- (void)viewDidShow
{
    if ([self.delegate respondsToSelector:(@selector(contentViewDidShow:))]) {
        [self.delegate contentViewDidShow:self];
    }
}

- (void)viewDidDismiss
{
    id oldDelegate = self.delegate;
    [self prepareForReuse];

    if ([oldDelegate respondsToSelector:(@selector(contentViewDidDismiss:))]) {
        [oldDelegate contentViewDidDismiss:self];
    }
}

#pragma mark -
#pragma mark UIWebViewDelegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSURL *url = request.URL;
    NSString *urlPath;
    if ([url host] == nil) {
        // This can be nil if loading files from the local cache. The url host being nil caused the urlPath
        // not to be generated properly and the UIWebview load to fail.
        return YES;
    }
    else
        urlPath = [NSString stringWithFormat:@"%@://%@%@", [url scheme], [url host], [url path]];

    NSInvocation *redirect = [_redirects valueForKey:urlPath];

    if (redirect) {

        NSDictionary *queryComponents = [url queryComponents];
        NSString     *callback        = [queryComponents valueForKey:@"callback"];
        NSString     *contextString   = [queryComponents valueForKey:@"context"];

        // Logging for automation, this is a no-op when not automating
        // TODO: This is not the correct way of doing this.  Should fix later
        if ([self respondsToSelector:@selector(_logRedirectForAutomation:callback:)]) {
            [self performSelector:@selector(_logRedirectForAutomation:callback:) withObject:urlPath withObject:callback];
        }

        PH_SBJSONPARSER_CLASS *parser = [PH_SBJSONPARSER_CLASS new];
        id parserObject = [parser objectWithString:contextString];
        NSDictionary *context = ([parserObject isKindOfClass:[NSDictionary class]]) ? (NSDictionary*) parserObject: nil;

        [parser release];

        PH_LOG(@"Redirecting request with callback: %@ to dispatch %@", callback, urlPath);
        switch ([[redirect methodSignature] numberOfArguments]) {
            case 5:
                [redirect setArgument:&self atIndex:4];
            case 4:
                [redirect setArgument:&callback atIndex:3];
            case 3:
                [redirect setArgument:&context atIndex:2];
            default:
                break;
        }

        // NOTE: It's important to keep the invocation object around while we're invoking. This will prevent occasional EXC_BAD_ACCESS errors.
        [redirect retain];
        [redirect invoke];
        [redirect release];

        return NO;
    }

    return ![[url scheme] isEqualToString:@"ph"];
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    [self dismissWithError:error];
    [PHResourceCacher resume];
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    // This is a fix that primarily affects iOS versions older than 4.1, it should prevent http requests
    // from leaking memory from the webview. Newer iOS versions are unaffected by the bug or the fix.
    [[NSUserDefaults standardUserDefaults] setInteger:0 forKey:@"WebKitCacheModelPreferenceKey"];

    [[self activityView] stopAnimating];

    if ([self.delegate respondsToSelector:(@selector(contentViewDidLoad:))]) {
        [self.delegate contentViewDidLoad:self];
    }

    [PHResourceCacher resume];
}

#pragma mark -
#pragma mark Redirects
- (void)redirectRequest:(NSString *)urlPath toTarget:(id)target action:(SEL)action
{
    if (!!target) {
        NSInvocation *redirect = [NSInvocation invocationWithMethodSignature:[[target class] instanceMethodSignatureForSelector:action]];
        redirect.target   = target;
        redirect.selector = action;

        [_redirects setValue:redirect forKey:urlPath];
    } else {
        [_redirects setValue:nil forKey:urlPath];
    }
}

- (void)handleLaunch:(NSDictionary *)queryComponents callback:(NSString *)callback
{
    NSString *urlPath = [queryComponents valueForKey:@"url"];
    if (!!urlPath && [urlPath isKindOfClass:[NSString class]]) {
        PHURLLoader *loader = [[PHURLLoader alloc] init];

        loader.targetURL = [NSURL URLWithString:urlPath];
        loader.delegate  = self;
        loader.context   = [NSDictionary dictionaryWithObjectsAndKeys:
                                                 callback,        @"callback",
                                                 queryComponents, @"queryComponents", nil];
#if PH_USE_STOREKIT != 0
        BOOL shouldUseInternal = [[queryComponents valueForKey:@"in_app_store_enabled"] boolValue] && ([SKStoreProductViewController class] != nil);
        loader.opensFinalURLOnDevice = !shouldUseInternal;
#endif

        [loader open];
        [loader release];
    }
}

- (void)handleDismiss:(NSDictionary *)queryComponents
{
    NSString *pingPath = [queryComponents valueForKey:@"ping"];
    if (!!pingPath && [pingPath isKindOfClass:[NSString class]]) {
        PHURLLoader *loader = [[PHURLLoader alloc] init];

        loader.opensFinalURLOnDevice = NO;
        loader.targetURL             = [NSURL URLWithString:pingPath];

        [loader open];
        [loader release];
    }

    [self dismiss:_willAnimate];
}

- (void)handleLoadContext:(NSDictionary *)queryComponents callback:(NSString*)callback
{
    NSString *loadCommand = [NSString stringWithFormat:@"window.PlayHavenDispatchProtocolVersion = %d", PH_DISPATCH_PROTOCOL_VERSION];
    [_webView stringByEvaluatingJavaScriptFromString:loadCommand];

    if (![self sendCallback:callback withResponse:self.content.context error:nil]) {
        [self dismissWithError:PHCreateError(PHLoadContextErrorType)];
    };
}

#pragma mark - callbacks
- (BOOL)sendCallback:(NSString *)callback withResponse:(id)response error:(id)error
{
    NSString *_callback = @"null", *_response = @"null", *_error = @"null";
    if (!!callback) {
        PH_LOG(@"Sending callback with id: %@", callback);
        _callback = callback;
    }

    PH_SBJSONWRITER_CLASS *jsonWriter = [PH_SBJSONWRITER_CLASS new];
    if (!!response) {
        _response = [jsonWriter stringWithObject:response];
    }

    if (!!error) {
        _error = [jsonWriter stringWithObject:error];
    }

    [jsonWriter release];

    NSString *callbackCommand = [NSString stringWithFormat:@"var PlayHavenAPICallback = (window[\"PlayHavenAPICallback\"])? PlayHavenAPICallback : function(c,r,e){try{PlayHaven.nativeAPI.callback(c,r,e);return \"OK\";}catch(err){ return JSON.stringify(err);}}; PlayHavenAPICallback(\"%@\",%@,%@)", _callback, _response, _error];
    NSString *callbackResponse = [_webView stringByEvaluatingJavaScriptFromString:callbackCommand];

    // Log callback for automation, this is no-op outside of automation
    if ([self respondsToSelector:@selector(_logCallbackForAutomation:)]) {
        [self performSelector:@selector(_logCallbackForAutomation:) withObject:callback];
    }

    if ([callbackResponse isEqualToString:@"OK"]) {
        return YES;
    } else {
        PH_LOG(@"content template callback failed. If this is a recurring issue, please include this console message along with the following information in your support request: %@", callbackResponse);
        return NO;
    }
}

#pragma mark -
#pragma mark PHURLLoaderDelegate
- (void)loaderFinished:(PHURLLoader *)loader
{
    NSDictionary *contextData  = (NSDictionary *)loader.context;
    NSString     *callback     = [contextData valueForKey:@"callback"];

    NSDictionary *responseDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                                       [loader.targetURL absoluteString], @"url", nil];

#if PH_USE_STOREKIT != 0
    NSDictionary *queryComponents = [contextData valueForKey:@"queryComponents"];
    BOOL shouldUseInternal = [[queryComponents valueForKey:@"in_app_store_enabled"] boolValue] && ([SKStoreProductViewController class] != nil);
    if (shouldUseInternal) {
        [[PHStoreProductViewControllerDelegate getDelegate] showProductId:[queryComponents valueForKey:@"application_id"]];
    }
#endif

    [self sendCallback:callback
          withResponse:responseDict
                 error:nil];
}

- (void)loaderFailed:(PHURLLoader *)loader
{
    NSDictionary *contextData  = (NSDictionary *)loader.context;
    NSDictionary *responseDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                                       [loader.targetURL absoluteString], @"url", nil];
    NSDictionary *errorDict    = [NSDictionary dictionaryWithObject:@"1" forKey:@"error"];

    [self sendCallback:[contextData valueForKey:@"callback"]
          withResponse:responseDict
                 error:errorDict];
}

#pragma mark - PH_DIALOG animation methods
#define ALPHA_OUT 0.0f
#define ALPHA_IN  1.0f

#define BOUNCE_OUT CGAffineTransformMakeScale(0.8, 0.8)
#define BOUNCE_MID CGAffineTransformMakeScale(1.1, 1.1)
#define BOUNCE_IN  CGAffineTransformIdentity

#define DURATION_1 0.125
#define DURATION_2 0.125

- (void)bounceIn
{
    _webView.transform = BOUNCE_OUT;
    _webView.alpha     = ALPHA_OUT;

    [UIView beginAnimations:@"bounce" context:nil];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    [UIView setAnimationDuration:DURATION_1];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(continueBounceIn)];

    _webView.transform = BOUNCE_MID;
    _webView.alpha     = ALPHA_IN;

    [UIView commitAnimations];
}

- (void)continueBounceIn
{
    _webView.transform = BOUNCE_MID;
    _webView.alpha     = ALPHA_IN;

    [UIView beginAnimations:@"bounce2" context:nil];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
    [UIView setAnimationDuration:DURATION_2];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(finishBounceIn)];

    _webView.transform = BOUNCE_IN;

    [UIView commitAnimations];
}

- (void)finishBounceIn
{
    _webView.transform = BOUNCE_IN;
    _webView.alpha     = ALPHA_IN;

    [self viewDidShow];
}

- (void)bounceOut
{
    _webView.transform = BOUNCE_IN;
    _webView.alpha     = ALPHA_IN;

    [UIView beginAnimations:@"bounce" context:nil];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
    [UIView setAnimationDuration:DURATION_1];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(continueBounceOut)];

    _webView.transform = BOUNCE_MID;

    [UIView commitAnimations];
}

- (void)continueBounceOut
{
    _webView.transform = BOUNCE_MID;
    _webView.alpha     = ALPHA_IN;

    [UIView beginAnimations:@"bounce2" context:nil];
    [UIView setAnimationCurve:UIViewAnimationCurveEaseIn];
    [UIView setAnimationDuration:DURATION_2];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationDidStopSelector:@selector(finishBounceOut)];

    _webView.transform = BOUNCE_OUT;
    _webView.alpha     = ALPHA_OUT;

    [UIView commitAnimations];
}

- (void)finishBounceOut
{
    _webView.transform = BOUNCE_OUT;
    _webView.alpha     = ALPHA_OUT;

    [self viewDidDismiss];
}
@end
