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

#import "UIWebView+UAAdditions.h"
#import "UAUser.h"
#import "UAUtils.h"

@implementation UIWebView (UAAdditions)

- (void)populateJavascriptEnvironment:(UAInboxMessage *)message {

    // This will inject the current device orientation
    // Note that face up and face down orientations will be ignored as this
    // casts a device orientation to an interface orientation
    [self willRotateToInterfaceOrientation:(UIInterfaceOrientation)[[UIDevice currentDevice] orientation]];

    /*
     * Define and initialize our one global
     */
    NSString *js = @"var UAirship = {};";

    /*
     * Set the device model.
     */
    NSString *model = [UIDevice currentDevice].model;
    js = [js stringByAppendingFormat:@"UAirship.devicemodel=\"%@\";", model];

    /*
     * Set the UA user ID.
     */
    NSString *userID = [UAUser defaultUser].username;
    js = [js stringByAppendingFormat:@"UAirship.userID=\"%@\";", userID];

    /*
     * Set the current message ID.
     */
    NSString *messageID = message.messageID;
    js = [js stringByAppendingFormat:@"UAirship.messageID=\"%@\";", messageID];

    /*
     * Set the current message's sent date (GMT).
     */
    NSDate *date = message.messageSent;
    NSString *messageSentDate = [[UAUtils ISODateFormatterUTC] stringFromDate:date];
    js = [js stringByAppendingFormat:@"UAirship.messageSentDate=\"%@\";", messageSentDate];

    /*
     * Set the current message's sent date (unix epoch time in milliseconds).
     */
    NSString *messageSentDateMS = [NSString stringWithFormat:@"%.0f", [date timeIntervalSince1970] * 1000];
    js =[js stringByAppendingFormat:@"UAirship.messageSentDateMS=%@;", messageSentDateMS];

    /*
     * Set the current message's title.
     */
    NSString *messageTitle = message.title;
    js = [js stringByAppendingFormat:@"UAirship.messageTitle=\"%@\";", messageTitle];

    /*
     * Define UAirship.handleCustomURL.
     */
    js = [js stringByAppendingString:@"UAirship.invoke = function(url) { location = url; };"];

    /*
     * Execute the JS we just constructed.
     */
    [self stringByEvaluatingJavaScriptFromString:js];
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {

    switch (toInterfaceOrientation) {
        case UIDeviceOrientationPortrait:
            [self stringByEvaluatingJavaScriptFromString:@"window.__defineGetter__('orientation',function(){return 0;});window.onorientationchange();"];
            break;
        case UIDeviceOrientationLandscapeLeft:
            [self stringByEvaluatingJavaScriptFromString:@"window.__defineGetter__('orientation',function(){return 90;});window.onorientationchange();"];
            break;
        case UIDeviceOrientationLandscapeRight:
            [self stringByEvaluatingJavaScriptFromString:@"window.__defineGetter__('orientation',function(){return -90;});window.onorientationchange();"];
            break;
        case UIDeviceOrientationPortraitUpsideDown:
            [self stringByEvaluatingJavaScriptFromString:@"window.__defineGetter__('orientation',function(){return 180;});window.onorientationchange();"];
            break;
        default:
            break;
    }
}

- (void)injectViewportFix {
    NSString *js = @"var metaTag = document.createElement('meta');"
    "metaTag.name = 'viewport';"
    "metaTag.content = 'width=device-width; initial-scale=1.0; maximum-scale=1.0;';"
    "document.getElementsByTagName('head')[0].appendChild(metaTag);";

    [self stringByEvaluatingJavaScriptFromString:js];
}

@end
