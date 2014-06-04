//
//  AppDelegate.m
//  AdColonyBasic
//
//  Created by John Fernandes-Salling on 8/14/12.
//

#import "AppDelegate.h"

#import "ViewController.h"

#import <AdColony/AdColony.h>

@implementation AppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPhone" bundle:nil];
    } else {
        self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPad" bundle:nil];
    }
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    // Configure AdColony only once, on initial launch
	[AdColony configureWithAppID:@"appbdee68ae27024084bb334a" zoneIDs:@[@"vzf8fb4670a60e4a139d01b5"] delegate:nil logging:YES];
    
    return YES;
}

@end
