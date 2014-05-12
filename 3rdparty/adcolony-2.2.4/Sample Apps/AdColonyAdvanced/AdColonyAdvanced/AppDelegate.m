//
//  AppDelegate.m
//  AdColonyAdvanced
//
//  Created by John Fernandes-Salling on 9/12/12.
//

#import "AppDelegate.h"

#import "ViewController.h"
#import "Constants.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPhone" bundle:nil];
    } else {
        self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPad" bundle:nil];
    }
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    // Configure AdColony only once, on initial launch
	[AdColony configureWithAppID:@"appbdee68ae27024084bb334a" zoneIDs:@[@"vzf8fb4670a60e4a139d01b5", @"vzf8e4e97704c4445c87504e"] delegate:self logging:YES];
    
    return YES;
}

#pragma mark -
#pragma mark AdColony V4VC

// Callback activated when a V4VC currency reward succeeds or fails
// This implementation is designed for client-side virtual currency without a server
// It uses NSUserDefaults for persistent client-side storage of the currency balance
// For applications with a server, contact the server to retrieve an updated currency balance
// On success, posts an NSNotification so the rest of the app can update the UI
// On failure, posts an NSNotification so the rest of the app can disable V4VC UI elements
- ( void ) onAdColonyV4VCReward:(BOOL)success currencyName:(NSString*)currencyName currencyAmount:(int)amount inZone:(NSString*)zoneID {
	NSLog(@"AdColony zone %@ reward %i %i %@", zoneID, success, amount, currencyName);
	
	if (success) {
		NSUserDefaults* storage = [NSUserDefaults standardUserDefaults];
		
		// Get currency balance from persistent storage and update it
		NSNumber* wrappedBalance = [storage objectForKey:kCurrencyBalance];
		NSUInteger balance = wrappedBalance && [wrappedBalance isKindOfClass:[NSNumber class]] ? [wrappedBalance unsignedIntValue] : 0;
		balance += amount;
		
		// Persist the currency balance
		[storage setValue:[NSNumber numberWithUnsignedInt:balance] forKey:kCurrencyBalance];
		[storage synchronize];
		
		// Post a notification so the rest of the app knows the balance changed
		[[NSNotificationCenter defaultCenter] postNotificationName:kCurrencyBalanceChange object:nil];
	} else {
		[[NSNotificationCenter defaultCenter] postNotificationName:kZoneOff object:nil];
	}
}

#pragma mark -
#pragma mark AdColony ad fill
- ( void ) onAdColonyAdAvailabilityChange:(BOOL)available inZone:(NSString*) zoneID {
	if(available) {
		[[NSNotificationCenter defaultCenter] postNotificationName:kZoneReady object:nil];
	} else {
		[[NSNotificationCenter defaultCenter] postNotificationName:kZoneLoading object:nil];
	}
}

@end
