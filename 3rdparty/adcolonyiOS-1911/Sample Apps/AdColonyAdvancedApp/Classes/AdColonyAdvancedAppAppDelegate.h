//
//  AdColonyAdvancedAppAppDelegate.h
//  AdColonyAdvancedApp
//
//  Created by Lev Trubov on 2/16/10.
//  Copyright Jirbo, Inc 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AdColonyPublic.h"
#import "SoundManager.h"

@class AdColonyAdvancedAppViewController;

@interface AdColonyAdvancedAppAppDelegate : NSObject <UIApplicationDelegate, AdColonyAdministratorDelegate, AdColonyTakeoverAdDelegate> {
    UIWindow *window;
	UINavigationController *navController;
	AdColonyAdvancedAppViewController* viewController;
	BOOL mightHaveFill;
	
	UIImageView *defaultReplacement;
	BOOL shouldPlayPrestitial, isPlayingPrestitial, justEnteredForeground;
    
    SoundManager *sm;
}

+(BOOL)runningOnIPad;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navController;
@property (nonatomic, retain) IBOutlet AdColonyAdvancedAppViewController* viewController;
@property (nonatomic, readonly) SoundManager *sm;

- (void)videoin;


@end

