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

#import "UAInboxMessageListDelegate.h"

@class UAInboxMessageList;

/**
 * This protocol defines a delegate method that is called
 * when a push notification arrives with a rich push message ID
 * embedded in its payload.
 */
@protocol UAInboxPushHandlerDelegate <NSObject>

@required
/**
 * Handle an incoming rich push notification.
 * @param notification An NSDictionary with the push notification contents.
 */
- (void)richPushNotificationArrived:(NSDictionary *)notification;
/**
 * Handle a rich push notification that launched the application.
 * @param notification An NSDictionary with the push notification contents.
 */
- (void)applicationLaunchedWithRichPushNotification:(NSDictionary *)notification;

/**
 * Called when a new Rich Push message is available for viewing.
 * @param richPushMessage The Rich Push message
 */
- (void)richPushMessageAvailable:(UAInboxMessage *)richPushMessage;

/**
 * Called when a Rich Push message associated with the push that launched the application
 * is available for viewing.
 *
 * @param richPushMessage The Rich Push message
 */
- (void)launchRichPushMessageAvailable:(UAInboxMessage *)richPushMessage;

@end

/**
 * This class handles incoming rich push messages that are sent with
 * an APNS notification.
 */
@interface UAInboxPushHandler : NSObject <UAInboxMessageListDelegate>

/**
 * Handle an incoming in-app notification.  This should typically be called 
 * from the UIApplicationDelegate.
 * @param userInfo the notification as an NSDictionary
 */
+ (void)handleNotification:(NSDictionary *)userInfo;

/**
 * YES if the most recent rich push launched the app, NO otherwise.
 */
@property (nonatomic, assign) BOOL hasLaunchMessage;

/**
 * The message ID of the most recent rich push as an NSString.
 */
@property (nonatomic, strong) NSString *viewingMessageID;

/**
 * The delegate that should be notified when an incoming push is handled,
 * as an object conforming to the UAInboxPushHandlerDelegate protocol.
 * NOTE: The delegate is not retained.
 */
@property (nonatomic, weak) id <UAInboxPushHandlerDelegate> delegate;

@end