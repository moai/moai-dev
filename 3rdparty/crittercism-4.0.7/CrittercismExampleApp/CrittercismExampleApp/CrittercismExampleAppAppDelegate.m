//
//  CrittercismExampleAppAppDelegate.m
//  CrittercismExampleApp
//
//  Created by Robert Kwok on 5/2/12.
//  Copyright 2012 Crittercism. All rights reserved.
//

#import "CrittercismExampleAppAppDelegate.h"
#import "CrittercismExampleAppViewController.h"

@implementation CrittercismExampleAppAppDelegate

@synthesize window=_window;

@synthesize viewController=_viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
     
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
  /**
   * Crittercism Initialization
   *
   * Please note, this app ID is for a global demo account - you can view
   * the data for it here:
   *
   * https://www.crittercism.com/developers/demo
   *
   * If you'd like to test against your own developer account, create an app
   * and replace the app ID below with the new ID found on the app's settings
   * page.
   *
   * Also be aware that you will need to setup this example app to link
   * against the Crittercism library before it will build properly.
   *
   * Instructions can be found here:
   *
   * https://www.crittercism.com/developers/docs
   *
   */
  
#error Be sure to read above instructions before running
#error and remove these two error lines. Have fun!
  
    [Crittercism enableWithAppID:@"4ce2d43766d78766a1000013"
                     andDelegate:_viewController];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
}

- (void)dealloc
{
    [_window release];
    [_viewController release];
    [super dealloc];
}

@end
