/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC``AS IS'' AND ANY EXPRESS OR
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

@class UAEvent;
@class UAConfig;

/**
 * The UAAnalytics object provides an interface to the Urban Airship Analytics API.
 */
@interface UAAnalytics : NSObject

/**
 * The analytics session as an NSMutableDictionary.
 */
@property (nonatomic, strong, readonly) NSMutableDictionary *session;

/**
 * The oldest event time as an NSTimeInterval.
 */
@property (nonatomic, assign, readonly) NSTimeInterval oldestEventTime;

/**
 * Background identifier for the analytics background task.
 */
@property (nonatomic, assign, readonly) UIBackgroundTaskIdentifier sendBackgroundTask;

/**
 * The notification as an NSDictionary.
 */
@property (nonatomic, strong, readonly) NSDictionary *notificationUserInfo;



/**
 * Initializes with the specified airshipConfig file.
 * @param airshipConfig The 'AirshipConfig.plist' file
 */
- (id)initWithConfig:(UAConfig *)airshipConfig;

/**
 * Delays the next analytics send.
 * @param time The number of seconds to delay the send opertation.
 */
- (void)delayNextSend:(NSTimeInterval)time;

/**
 * Triggers an analytics event.
 * @param event The event to be triggered
 */
- (void)addEvent:(UAEvent *)event;

/**
 * Handle incoming push notifications.
 * @param userInfo The notification as an NSDictionary.
 * @param applicationState The application state at the time the notification was received.
 */
- (void)handleNotification:(NSDictionary*)userInfo inApplicationState:(UIApplicationState)applicationState;

/**
 * Date representing the last attempt to send analytics.
 */
- (NSDate*)lastSendTime;

@end
