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

#import "UAInboxNavUI.h"
#import "UAInboxMessageListController.h"
#import "UAInboxMessageViewController.h"
#import "UAInboxOverlayController.h"

#import "UAInboxMessageList.h"
#import "UAInboxPushHandler.h"

@interface UAInboxNavUI ()

@property (nonatomic, strong) UIViewController *rootViewController;
@property (nonatomic, assign) BOOL isVisible;
@property (nonatomic, strong) UAInboxMessageViewController *messageViewController;
@property (nonatomic, strong) UAInboxMessageListController *messageListController;
@property (nonatomic, strong) UAInboxAlertHandler *alertHandler;

- (void)quitInbox;

@end

@implementation UAInboxNavUI

SINGLETON_IMPLEMENTATION(UAInboxNavUI)

static BOOL runiPhoneTargetOniPad = NO;

+ (void)setRuniPhoneTargetOniPad:(BOOL)value {
    runiPhoneTargetOniPad = value;
}


- (id)init {
    self = [super init];
    if (self) {
        NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"UAInboxLocalization.bundle"];
        self.localizationBundle = [NSBundle bundleWithPath:path];
		
        self.useOverlay = NO;
        self.isVisible = NO;
        
        UAInboxMessageListController *mlc = [[UAInboxMessageListController alloc] initWithNibName:@"UAInboxMessageListController" bundle:nil];
        
        mlc.navigationItem.leftBarButtonItem = 
            [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(inboxDone:)];
        
        self.messageListController = mlc;
        
        self.alertHandler = [[UAInboxAlertHandler alloc] init];
        
        self.popoverSize = CGSizeMake(320, 1100);
    }
    
    return self;
}

- (void)inboxDone:(id)sender {
    [self quitInbox];
}

+ (void)displayInboxInViewController:(UIViewController *)parentViewController animated:(BOOL)animated {

    if ([UAInboxNavUI shared].isVisible) {
        //don't display twice
        return;
    }

    if ([parentViewController isKindOfClass:[UINavigationController class]]) {
        [UAInboxNavUI shared].isVisible = YES;
        if (parentViewController) {
            [UAInboxNavUI shared].inboxParentController = parentViewController;
        }
        
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad && !runiPhoneTargetOniPad) {
            [UAInboxNavUI shared].navigationController = [[UINavigationController alloc] initWithRootViewController:[UAInboxNavUI shared].messageListController];
            [UAInboxNavUI shared].popoverController = [[UIPopoverController alloc] initWithContentViewController:[UAInboxNavUI shared].navigationController];
            
            [UAInboxNavUI shared].popoverController.popoverContentSize = [UAInboxNavUI shared].popoverSize;
            [UAInboxNavUI shared].messageListController.contentSizeForViewInPopover = [UAInboxNavUI shared].popoverSize;
            
            [UAInboxNavUI shared].popoverController.delegate = [UAInboxNavUI shared];
            
            [[UAInboxNavUI shared].popoverController 
                presentPopoverFromBarButtonItem:[UAInboxNavUI shared].popoverButton
                       permittedArrowDirections:UIPopoverArrowDirectionAny
                                       animated:animated];
        } else {
            [UAInboxNavUI shared].navigationController = (UINavigationController *)parentViewController;
            [[UAInboxNavUI shared].navigationController pushViewController:[UAInboxNavUI shared].messageListController animated:animated];
        }
    } else {
        UALOG(@"Not a navigation controller");
    }

} 

+ (void)displayMessageWithID:(NSString *)messageID inViewController:(UIViewController *)parentViewController {

    if(![UAInboxNavUI shared].isVisible) {
        
        if ([UAInboxNavUI shared].useOverlay) {
            [UAInboxOverlayController showWindowInsideViewController:[UAInboxNavUI shared].inboxParentController withMessageID:messageID];
            return;
        }

        else {
            UALOG(@"UI needs to be brought up!");
            parentViewController = parentViewController?:[UAInboxNavUI shared].inboxParentController;
            [UAInboxNavUI displayInboxInViewController:parentViewController animated:NO];
        }
    }
	
    // If the message view is already open, just load the first message.
    if ([parentViewController isKindOfClass:[UINavigationController class]]) {
		
        // For iPhone
        UINavigationController *navController = (UINavigationController *)parentViewController;
        
		if ([navController.topViewController class] == [UAInboxMessageViewController class]) {
            [[UAInboxNavUI shared].messageViewController loadMessageForID:messageID];
        } else {
			
            [UAInboxNavUI shared].messageViewController = 
                [[UAInboxMessageViewController alloc] initWithNibName:@"UAInboxMessageViewController" bundle:nil];			
            [[UAInboxNavUI shared].messageViewController loadMessageForID:messageID];
            [navController pushViewController:[UAInboxNavUI shared].messageViewController animated:YES];
        }
    }
}

+ (void)quitInbox {
    [[UAInboxNavUI shared] quitInbox];
}

- (void)quitInbox {
    self.isVisible = NO;
    [self.navigationController popToViewController:self.messageListController animated:YES];
    [self.navigationController popViewControllerAnimated:YES];
    
    if (self.popoverController) {
        [self.popoverController dismissPopoverAnimated:YES];
        self.popoverController = nil;
    }
}

+ (void)land {
    //do any necessary teardown here
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)dismissedPopoverController {
    if (self.popoverController == dismissedPopoverController) {
        [UAInbox quitInbox];
    }
}

- (void)richPushNotificationArrived:(NSDictionary *)message {
    //custom launch notification handling here
}

- (void)richPushMessageAvailable:(UAInboxMessage *)richPushMessage {
    NSString *alertText = richPushMessage.title;
    [self.alertHandler showNewMessageAlert:alertText withViewBlock:^{
        [[UAInbox shared].uiClass displayMessageWithID:richPushMessage.messageID inViewController:nil];
    }];
}

- (void)applicationLaunchedWithRichPushNotification:(NSDictionary *)notification {
    //custom launch notification handling here
}

- (void)launchRichPushMessageAvailable:(UAInboxMessage *)richPushMessage {
    [[UAInbox shared].uiClass displayMessageWithID:richPushMessage.messageID inViewController:nil];
}

@end
