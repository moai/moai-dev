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

#import "UAInboxAlertHandler.h"
#import "UAInbox.h"

#import "UAInboxMessageListController.h"
#import "UAInboxMessageViewController.h"

#import "UAInboxPushHandler.h"

#ifndef UA_INBOX_TR
#define UA_INBOX_TR(key) [[UAInboxNavUI shared].localizationBundle localizedStringForKey:key value:@"" table:nil]
#endif

@class UAInboxAlertProtocol;

/**
 * This class is an alternative rich push UI implementation.  When it is
 * designated as the [UAInbox uiClass], launching the inbox will cause it
 * to be displayed in either a navigation controller (in the iPhone UI idiom)
 * or a popover controller (in the iPad UI idiom).
 */
@interface UAInboxNavUI : NSObject <UAInboxUIProtocol, UAInboxPushHandlerDelegate, UIPopoverControllerDelegate>

/**
 * Set this property to YES if the class should display in-app messages
 * using UAInboxOverlayController, and NO if it should navigate to the
 * inbox and display the message as though it had been selected.
 */
@property (nonatomic, assign) BOOL useOverlay;

/**
 * The size of the popover controller's window.
 * Defaults to 320 x 1100.
 */
@property (nonatomic, assign) CGSize popoverSize;

/**
 * The button used to launch the popover display.
 */
@property (nonatomic, strong) UIBarButtonItem *popoverButton;

/**
 * The popover controller used for displaying the inbox in the iPad UI idiom.
 */
@property (nonatomic, strong) UIPopoverController *popoverController;

/**
 * The navigation controller used for displaying the inbox in the iPhone idiom.
 */
@property (nonatomic, strong) UINavigationController *navigationController;

/**
 * The parent view controller the inbox will be launched from.
 */
@property (nonatomic, strong) UIViewController *inboxParentController;

@property (nonatomic, strong) NSBundle *localizationBundle;


SINGLETON_INTERFACE(UAInboxNavUI);


///---------------------------------------------------------------------------------------
/// @name UAInboxUIProtocol Methods
///---------------------------------------------------------------------------------------
+ (void)quitInbox;
+ (void)displayInboxInViewController:(UIViewController *)parentViewController animated:(BOOL)animated;
+ (void)displayMessageWithID:(NSString *)messageID inViewController:(UIViewController *)parentViewController;

///---------------------------------------------------------------------------------------
/// @name UAInboxPushHandlerDelegate Methods
///---------------------------------------------------------------------------------------
- (void)richPushNotificationArrived:(NSDictionary *)message;
- (void)richPushMessageAvailable:(UAInboxMessage *)richPushMessage;
- (void)applicationLaunchedWithRichPushNotification:(NSDictionary *)notification;
- (void)launchRichPushMessageAvailable:(UAInboxMessage *)richPushMessage;

///---------------------------------------------------------------------------------------
/// @name Misc
///---------------------------------------------------------------------------------------

/**
 * Instructs the UI class to place the UI in an existing UINavigationController on the iPad
 * rather than using a UIPopoverController
 *
 * @param value Set to YES if the iPhone/NavigationController UI should be used even when the
 * application is using the iPad UI idiom
 */
+ (void)setRuniPhoneTargetOniPad:(BOOL)value;

+ (void)land;

@end
