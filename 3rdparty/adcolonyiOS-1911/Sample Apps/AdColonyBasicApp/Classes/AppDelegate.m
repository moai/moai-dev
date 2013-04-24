//
//  BasicAppDelegate.m
//  Basic
//
//  Created by John Fernandes-Salling on 10/29/10.
//  Copyright Jirbo, INC. 2010. All rights reserved.
//

#import "AppDelegate.h"
#import "BasicViewController.h"
#import <MediaPlayer/MediaPlayer.h>

@implementation AppDelegate

@synthesize window;
@synthesize viewController;


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	//start AdColony services
	[AdColonyAdministratorPublic initAdministratorWithDelegate:self];
	[self showWindow];
    return YES;
}

-(void)showWindow
{
	// Add the view controller's view to the window and display.
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}

#pragma mark -
#pragma mark AdColonyAdministratorDelegate

//use the app id provided by adcolony.com
-(NSString*)adColonyApplicationID{
	return @"app4e541454aa92e";
}

//use the zone numbers provided by adcolony.com
//can be ordered in any way, but make sure the right
//numbers are used when requesting banner ads
//(there can only be one video zone per app)
-(NSDictionary*)adColonyAdZoneNumberAssociation{
    return [NSDictionary dictionaryWithObjectsAndKeys:
			@"z4e54154dd2d35", [NSNumber numberWithInt:1], 
			@"z4e5415e6f0673", [NSNumber numberWithInt:2],
			nil];
}

-(NSString *)adColonyLoggingStatus{
	return AdColonyLoggingOn;
}

-(void)adColonyNoVideoFillInZone:(NSString *)zone{
    NSDictionary *d = [self adColonyAdZoneNumberAssociation];
    for(NSNumber *key in d){
        if([[d objectForKey:key] isEqualToString:zone]){
            [viewController changeUIToFailureForZoneIndex:key];
        }
    }
}

-(void)adColonyVideoAdsReadyInZone:(NSString *)zone {
    NSDictionary *d = [self adColonyAdZoneNumberAssociation];
    for(NSNumber *key in d){
        if([[d objectForKey:key] isEqualToString:zone]){
            [viewController changeUIToSuccessForZoneIndex:key];
        }
    }
}

@end
