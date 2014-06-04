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

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

#import "UADisposable.h"
#import "UAInboxMessageListDelegate.h"

typedef void (^UAInboxMessageCallbackBlock)(UAInboxMessage *message);

@class UAInboxMessageList;

/**
 * This class represents a Rich Push Inbox message. It contains all
 * the available information about a message, including the URLs where
 * the message can be retrieved.
 */
@interface UAInboxMessage : NSManagedObject


/**
 * Mark the message as read.
 * @param successBlock A block to be executed if the mark-as-read operation is successful.
 * @param failureBlock A block to be executed if the mark-as-read operation fails.
 * @return A UADisposable which can be used to cancel callback execution.
 * This value will be nil if the request is not submitted due to an already scheduled update,
 * or because the message has already been marked as read.
 */
- (UADisposable *)markAsReadWithSuccessBlock:(UAInboxMessageCallbackBlock)successBlock
                  withFailureBlock:(UAInboxMessageCallbackBlock)failureBlock;

/**
 * Mark the message as read. This eventually results in a callback to
 * [UAInboxMessageListDelegate singleMessageMarkAsReadFinished:] or
 * [UAInboxMessageListDelegate singleMessageMarkAsReadFailed:].
 *
 * @param delegate An object implementing the `UAInboxMessageListDelegate` protocol.
 * @return A UADisposable which can be used to cancel callback execution.
 * This value will be nil if the request is not submitted due to an already scheduled update,
 * or because the message has already been marked as read.
 */
- (UADisposable *)markAsReadWithDelegate:(id<UAInboxMessageListDelegate>)delegate;

/**
 * Mark the message as read. This eventually results in a callback to
 * [UAInboxMessageListObserver singleMessageMarkAsReadFinished:] or
 * [UAInboxMessageListObserver singleMessageMarkAsReadFailed:].
 *
 * @return YES if the request was submitted or already complete, otherwise NO.
 *
 * @deprecated As of version 3.0. Replaced with block and delegate-based methods.
 */
- (BOOL)markAsRead __attribute__((deprecated("As of version 3.0")));

/**
 * YES if the message is expired, NO otherwise
 */
- (BOOL)isExpired;
/**
 * Invokes the UAInbox Javascript delegate from within a message's UIWebView.
 *
 * This method returns null, but a callback to the UIWebView may be made via
 * [UIWebView stringByEvaluatingJavaScriptFromString:] if the delegate returns
 * a Javascript string for evaluation.
 *
 * @param webView The UIWebView generating the request
 * @param url The URL requested by the webView
 */
+ (void)performJSDelegate:(UIWebView *)webView url:(NSURL *)url;


///---------------------------------------------------------------------------------------
/// @name Message Properties
///---------------------------------------------------------------------------------------

/**
 * The Urban Airship message ID.
 * This ID may be used to match an incoming push notification to a specific message.
 */
@property (nonatomic, strong) NSString *messageID;

/**
 * The URL for the message body itself.
 * This URL may only be accessed with Basic Auth credentials set to the user id and password.
 */
@property (nonatomic, strong) NSURL *messageBodyURL;

/** The URL for the message.
 * This URL may only be accessed with Basic Auth credentials set to the user id and password.
 */
@property (nonatomic, strong) NSURL *messageURL;

/** The MIME content type for the message (e.g., text/html) */
@property (nonatomic, copy) NSString *contentType;

/** YES if the message is unread, otherwise NO. */
@property (assign) BOOL unread;

/** The date and time the message was sent (UTC) */
@property (nonatomic, strong) NSDate *messageSent;

/**
 * The date and time the message will expire. 
 *
 * A nil value indicates it will never expire
 */
@property (nonatomic, strong) NSDate *messageExpiration;

/** The message title */
@property (nonatomic, strong) NSString *title;

/**
 * The message's extra dictionary. This dictionary can be populated
 * with arbitrary key-value data at the time the message is composed.
 */
@property (nonatomic, strong) NSDictionary *extra;

/** 
 * The raw message dictionary. This is the dictionary that
 * originally created the message.  It can contain more values
 * then the message.
 */
@property (nonatomic, strong) NSDictionary *rawMessageObject;

/**
 * The parent inbox.
 * 
 * Note that this object is not retained by the message.
 */
@property (weak) UAInboxMessageList *inbox; 

@end
