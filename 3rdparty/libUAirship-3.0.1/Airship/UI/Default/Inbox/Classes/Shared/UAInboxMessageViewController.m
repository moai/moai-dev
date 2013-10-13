/*
Copyright 2009-2013 Urban Airship Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binaryform must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided withthe distribution.

THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL URBAN AIRSHIP INC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#import "UAInbox.h"
#import "UAInboxMessageViewController.h"
#import "UAInboxUI.h"
#import "UAInboxMessageList.h"

#import "UIWebView+UAAdditions.h"

#import "UAUtils.h"

#define kMessageUp 0
#define kMessageDown 1

@interface UAInboxMessageViewController ()

- (void)refreshHeader;
- (void)updateMessageNavButtons;

@property (nonatomic, strong) IBOutlet UIActivityIndicatorView *activity;
@property (nonatomic, strong) IBOutlet UIView *statusBar;
@property (nonatomic, strong) IBOutlet UILabel *statusBarTitle;
@property (nonatomic, strong) UISegmentedControl *messageNav;
/**
 * The UIWebView used to display the message content.
 */
@property (nonatomic, strong) UIWebView *webView;
@end

@implementation UAInboxMessageViewController



- (void)dealloc {
    self.webView.delegate = nil;

}

- (id)initWithNibName:(NSString *)nibName bundle:(NSBundle *)nibBundle {
    if (self = [super initWithNibName:nibName bundle:nibBundle]) {
        
        self.title = UA_INBOX_TR(@"UA_Message");

        // "Segmented" up/down control to the right
        UISegmentedControl *segmentedControl = [[UISegmentedControl alloc] initWithItems:
                                                 [NSArray arrayWithObjects:
                                                  [UIImage imageNamed:@"up.png"],
                                                  [UIImage imageNamed:@"down.png"],
                                                  nil]];
        [segmentedControl addTarget:self action:@selector(segmentAction:) forControlEvents:UIControlEventValueChanged];
        segmentedControl.frame = CGRectMake(0, 0, 90, 30);
        segmentedControl.segmentedControlStyle = UISegmentedControlStyleBar;
        segmentedControl.momentary = YES;
        self.messageNav = segmentedControl;

        UIBarButtonItem *segmentBarItem = [[UIBarButtonItem alloc] initWithCustomView:segmentedControl];
        self.navigationItem.rightBarButtonItem = segmentBarItem;
        self.shouldShowAlerts = YES;

        // make our existing layout work in iOS7
        if ([self respondsToSelector:NSSelectorFromString(@"edgesForExtendedLayout")]) {
            self.edgesForExtendedLayout = UIRectEdgeNone;
        }
    }

    return self;
}

- (void)viewDidLoad {
    [self.webView setDataDetectorTypes:UIDataDetectorTypeAll];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(messageListUpdated)
                                                 name:UAInboxMessageListUpdatedNotification object:nil];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UAInboxMessageListUpdatedNotification object:nil];
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

#pragma mark -
#pragma mark UI

- (void)refreshHeader {
    NSUInteger count = [[UAInbox shared].messageList messageCount];
    NSUInteger index = [[UAInbox shared].messageList indexOfMessage:self.message];

    if (index < count) {
        self.title = [NSString stringWithFormat:UA_INBOX_TR(@"UA_Message_Fraction"), index+1, count];
    } else {
        [self.webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"about:blank"]]];
        self.statusBar.hidden = YES;
        self.title = @"";
    }

    [self updateMessageNavButtons];
}

- (void)loadMessageForID:(NSString *)mid {
    UAInboxMessage *msg = [[UAInbox shared].messageList messageForID:mid];
    if (msg == nil) {
        UALOG(@"Can not find message with ID: %@", mid);
        return;
    }

    [self loadMessageAtIndex:[[UAInbox shared].messageList indexOfMessage:msg]];
}

- (void)loadMessageAtIndex:(NSUInteger)index {
    [self.webView stopLoading];
    [self.webView removeFromSuperview];
    self.webView.delegate = nil;

    self.webView = [[UIWebView alloc] init];
    self.webView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
    self.webView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;

    self.webView.delegate = self;

    [self.view insertSubview:self.webView belowSubview:self.statusBar];

    self.message = [[UAInbox shared].messageList messageAtIndex:index];
    if (self.message == nil) {
        UALOG(@"Can not find message with index: %lu", (unsigned long)index);
        return;
    }

    [self refreshHeader];

    NSMutableURLRequest *requestObj = [NSMutableURLRequest requestWithURL: self.message.messageBodyURL];
    
    [requestObj setTimeoutInterval:5];
    
    NSString *auth = [UAUtils userAuthHeaderString];
    [requestObj setValue:auth forHTTPHeaderField:@"Authorization"];

    [self.webView loadRequest:requestObj];
}


#pragma mark UIWebViewDelegate

- (BOOL)webView:(UIWebView *)wv shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSURL *url = [request URL];
    
    /*
     ua://callbackArguments:withOptions:/[<arguments>][?<dictionary>]
     */

    if ([[url scheme] isEqualToString:@"ua"]) {
        if ((navigationType == UIWebViewNavigationTypeLinkClicked) || (navigationType == UIWebViewNavigationTypeOther)) {
            [UAInboxMessage performJSDelegate:wv url:url];
            return NO;
        }
    }

    // send iTunes/Phobos urls to AppStore.app
    else if ((navigationType == UIWebViewNavigationTypeLinkClicked) &&
             (([[url host] isEqualToString:@"phobos.apple.com"]) ||
              ([[url host] isEqualToString:@"itunes.apple.com"]))) {
                 
        // Set the url scheme to http, as it could be itms which will cause the store to launch twice (undesireable)
        NSString *stringURL = [NSString stringWithFormat:@"http://%@%@", url.host, url.path];
        return ![[UIApplication sharedApplication] openURL:[NSURL URLWithString:stringURL]];
    }

    // send maps.google.com url or maps: to GoogleMaps.app
    else if ((navigationType == UIWebViewNavigationTypeLinkClicked) &&
             (([[url host] isEqualToString:@"maps.google.com"]) ||
              ([[url scheme] isEqualToString:@"maps"]))) {

        /* Do any special formatting here, for example:

         NSString *title = @"title";
         float latitude = 35.4634;
         float longitude = 9.43425;
         int zoom = 13;
         NSString *stringURL = [NSString stringWithFormat:@"http://maps.google.com/maps?q=%@@%1.6f,%1.6f&z=%d", title, latitude, longitude, zoom];

         */

        return ![[UIApplication sharedApplication] openURL:url];
    }

    // send www.youtube.com url to YouTube.app
    else if ((navigationType == UIWebViewNavigationTypeLinkClicked) &&
             ([[url host] isEqualToString:@"www.youtube.com"])) {
        return ![[UIApplication sharedApplication] openURL:url];
    }

    // send mailto: to Mail.app
    else if ((navigationType == UIWebViewNavigationTypeLinkClicked) && ([[url scheme] isEqualToString:@"mailto"])) {

        /* Do any special formatting here if you like, for example:

         NSString *subject = @"Message subject";
         NSString *body = @"Message body";
         NSString *address = @"address@domain.com";
         NSString *cc = @"address@domain.com";
         NSString *path = [NSString stringWithFormat:@"mailto:%@?cc=%@&subject=%@&body=%@", address, cc, subject, body];
         NSURL *url = [NSURL URLWithString:[path stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];

         For complex body text you may want to use CFURLCreateStringByAddingPercentEscapes.

         */

        return ![[UIApplication sharedApplication] openURL:url];
    }

    // send tel: to Phone.app
    else if ((navigationType == UIWebViewNavigationTypeLinkClicked) && ([[url scheme] isEqualToString:@"tel"])) {
        NSURL *validPhoneUrl = [self createValidPhoneNumberUrlFromUrl:url];
        return ![[UIApplication sharedApplication] openURL:validPhoneUrl];
    }

    // send sms: to Messages.app
    else if ((navigationType == UIWebViewNavigationTypeLinkClicked) && ([[url scheme] isEqualToString:@"sms"])) {
        NSURL *validPhoneUrl = [self createValidPhoneNumberUrlFromUrl:url];
        return ![[UIApplication sharedApplication] openURL:validPhoneUrl];
    }

    // load local file and http/https webpages in webview
    return YES;
}

- (NSURL *)createValidPhoneNumberUrlFromUrl:(NSURL *)url {

    NSString *decodedUrlString = [url.absoluteString stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSCharacterSet *characterSet = [[NSCharacterSet characterSetWithCharactersInString:@"+-.0123456789"] invertedSet];
    NSString *strippedNumber = [[decodedUrlString componentsSeparatedByCharactersInSet:characterSet] componentsJoinedByString:@""];

    NSString *scheme = [decodedUrlString hasPrefix:@"sms"] ? @"sms:" : @"tel:";
    
    return [NSURL URLWithString:[scheme stringByAppendingString:strippedNumber]];
}

- (void)webViewDidStartLoad:(UIWebView *)wv {
    [self.statusBar setHidden: NO];
    [self.activity startAnimating];
    self.statusBarTitle.text = self.message.title;
    
    [self.webView populateJavascriptEnvironment:self.message];
}

- (void)webViewDidFinishLoad:(UIWebView *)wv {
    [self.statusBar setHidden: YES];
    [self.activity stopAnimating];

    // Mark message as read after it has finished loading
    if(self.message.unread) {
        [self.message markAsReadWithDelegate:nil];
    }
    
    [self.webView injectViewportFix];
}

- (void)webView:(UIWebView *)wv didFailLoadWithError:(NSError *)error {
    [self.statusBar setHidden: YES];
    [self.activity stopAnimating];

    if (error.code == NSURLErrorCancelled)
        return;
    UALOG(@"Failed to load message: %@", error);
    
    if (self.shouldShowAlerts) {
        
        UIAlertView *someError = [[UIAlertView alloc] initWithTitle:UA_INBOX_TR(@"UA_Mailbox_Error_Title")
                                                            message:UA_INBOX_TR(@"UA_Error_Fetching_Message")
                                                           delegate:self
                                                  cancelButtonTitle:UA_INBOX_TR(@"UA_OK")
                                                  otherButtonTitles:nil];
        [someError show];
    }
}

#pragma mark Message Nav

- (IBAction)segmentAction:(id)sender {
    UISegmentedControl *segmentedControl = (UISegmentedControl *)sender;
    NSUInteger index = [[UAInbox shared].messageList indexOfMessage:self.message];

    if(segmentedControl.selectedSegmentIndex == kMessageUp) {
        [self loadMessageAtIndex:index-1];
    } else if(segmentedControl.selectedSegmentIndex == kMessageDown) {
        [self loadMessageAtIndex:index+1];
    }
}

- (void)updateMessageNavButtons {
    NSUInteger index = [[UAInbox shared].messageList indexOfMessage:self.message];

    if (self.message == nil || index == NSNotFound) {
        [self.messageNav setEnabled: NO forSegmentAtIndex: kMessageUp];
        [self.messageNav setEnabled: NO forSegmentAtIndex: kMessageDown];
    } else {
        if(index <= 0) {
            [self.messageNav setEnabled: NO forSegmentAtIndex: kMessageUp];
        } else {
            [self.messageNav setEnabled: YES forSegmentAtIndex: kMessageUp];
        }
        if(index >= [[UAInbox shared].messageList messageCount] - 1) {
            [self.messageNav setEnabled: NO forSegmentAtIndex: kMessageDown];
        } else {
            [self.messageNav setEnabled: YES forSegmentAtIndex: kMessageDown];
        }
    }

    UALOG(@"update nav %lu, of %lu", (unsigned long)index, (unsigned long)[[UAInbox shared].messageList messageCount]);
}

#pragma mark NSNotificationCenter callbacks

- (void)messageListUpdated {
    [self refreshHeader];
}

@end
