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

#import "UAPushUI.h"
#import "UAPushNotificationHandler.h"

#import <AudioToolbox/AudioServices.h>

@implementation UAPushNotificationHandler

- (void)displayNotificationAlert:(NSString *)alertMessage {

    UA_LDEBUG(@"Received an alert in the foreground.");
    
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle: UA_PU_TR(@"UA_Notification_Title")
                                                    message: alertMessage
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
	[alert show];
}

- (void)displayLocalizedNotificationAlert:(NSDictionary *)alertDict {
	
	// The alert is a a dictionary with more details, let's just get the message without localization
	// This should be customized to fit your message details or usage scenario
	//message = [[alertDict valueForKey:@"alert"] valueForKey:@"body"];
	
    UA_LDEBUG(@"Received an alert in the foreground with a body.");
    
    NSString *body = [alertDict valueForKey:@"body"];
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: UA_PU_TR(@"UA_Notification_Title")
                                                    message: body
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
	[alert show];
}

- (void)playNotificationSound:(NSString *)sound {
    
    if (sound) {
    
        // Note: The default sound is not available in the app.
        //
        // From http://developer.apple.com/library/ios/#documentation/AudioToolbox/Reference/SystemSoundServicesReference/Reference/reference.html :
        // System-supplied alert sounds and system-supplied user-interface sound effects are not available to your iOS application.
        // For example, using the kSystemSoundID_UserPreferredAlert constant as a parameter to the AudioServicesPlayAlertSound
        // function will not play anything.

        SystemSoundID soundID;
        NSString *path = [[NSBundle mainBundle] pathForResource:[sound stringByDeletingPathExtension] 
                                                         ofType:[sound pathExtension]];
        if (path) {
            UALOG(@"Received a foreground alert with a sound: %@", sound);
            AudioServicesCreateSystemSoundID((__bridge CFURLRef)[NSURL fileURLWithPath:path], &soundID);
            AudioServicesPlayAlertSound(soundID);
        } else {
            UALOG(@"Received an alert with a sound that cannot be found the application bundle: %@", sound);
        }

    } else {
        
        // Vibrates on supported devices, on others, does nothing
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    
    }

}

- (void)handleBadgeUpdate:(NSInteger)badgeNumber {
	UA_LDEBUG(@"Received an alert in the foreground with a new badge");

    // Sets the application badge to the value in the notification
	[[UIApplication sharedApplication] setApplicationIconBadgeNumber:badgeNumber];
}

- (void)receivedForegroundNotification:(NSDictionary *)notification {
    UA_LDEBUG(@"Received a notification while the app was already in the foreground");

    // Do something with your customData JSON, then entire notification is also available
}

- (void)launchedFromNotification:(NSDictionary *)notification {
    UA_LDEBUG(@"The application was launched or resumed from a notification");

    // Do something when launched via a notification
}

- (void)receivedForegroundNotification:(NSDictionary *)notification fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler{
    UA_LDEBUG(@"Received a notification while the app was already in the foreground");

    // Do something with your customData JSON, then entire notification is also available

    // Call the completion handler
    completionHandler(UIBackgroundFetchResultNoData);
}

- (void)launchedFromNotification:(NSDictionary *)notification fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
    UA_LDEBUG(@"The application was launched or resumed from a notification");

    // Do something when launched via a notification

    // Call the completion handler
    completionHandler(UIBackgroundFetchResultNoData);
}

@end
