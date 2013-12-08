//
//  BasicAppDelegate.h
//  Basic
//
//  Created by John Fernandes-Salling on 10/29/10.
//  Copyright Jirbo, INC. 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
@class BasicViewController;

#import "AdColonyPublic.h"

@interface AppDelegate : NSObject <UIApplicationDelegate, AdColonyAdministratorDelegate> {
    UIWindow *window;
    BasicViewController *viewController;
	
}

-(void)showWindow;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet BasicViewController *viewController;

@end

