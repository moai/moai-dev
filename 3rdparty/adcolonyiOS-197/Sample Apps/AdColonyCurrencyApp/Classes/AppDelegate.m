//
//  BasicAppDelegate.m
//  Basic
//
//  Created by John Fernandes-Salling on 10/29/10.
//  Copyright Jirbo, INC. 2010. All rights reserved.
//

#import "AppDelegate.h"
#import "CurrencyAppViewController.h"

@implementation AppDelegate

@synthesize window;
@synthesize navigationController;

#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	//start AdColony services
	[AdColonyAdministratorPublic initAdministratorWithDelegate:self];
    cm = [[CurrencyManager alloc] init];

	[self showWindow];
    return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return YES;
}

- (void)applicationWillEnterForeground:(UIApplication *)application{
    [cm update];
}


-(void)showWindow
{
	// Add the view controller's view to the window and display.
    [window addSubview:navigationController.view];
    [window makeKeyAndVisible];
    bvc.cm = cm;
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [bvc release];
    [cm release];
	[navigationController release];
    [window release];
    [super dealloc];
}

#pragma mark -
#pragma mark -
#pragma mark AdColonyAdministratorDelegate
#pragma mark -

//use the app id provided by adcolony.com
-(NSString*)adColonyApplicationID{
	return @"app4e541454aa92e";
}

//use the zone numbers provided by adcolony.com
//can be ordered in any way, but make sure the right
//numbers are used when requesting banner ads
//(there can only be one video zone per app)
-(NSDictionary*)adColonyAdZoneNumberAssociation{
	//*
    return [NSDictionary dictionaryWithObjectsAndKeys:
			@"z4e541640e51ce", [NSNumber numberWithInt:1],
            @"z4e5416fa24866", [NSNumber numberWithInt:2],
			nil];//*/
}

-(NSString *)adColonyLoggingStatus{
	return AdColonyLoggingOn;
}

-(NSString *)adColonySupplementalVCParametersForZone:(NSString *)zone{
    return [bvc extraParameterText];
}


#pragma mark virtual currency part of delegate

-(void)adColonyVirtualCurrencyAwardedByZone:(NSString *)zone currencyName:(NSString *)name currencyAmount:(int)amount{
    NSLog(@"v4vc award: %d of %@", amount, name);
    [cm update];
}

-(void)adColonyVirtualCurrencyNotAwardedByZone:(NSString *)zone currencyName:(NSString *)name currencyAmount:(int)amount{
    NSLog(@"v4vc no award: should have awarded %d of %@", amount, name);
}


@end
