//
//  BasicAppDelegate.h
//  Basic
//
//  Created by John Fernandes-Salling on 10/29/10.
//  Copyright Jirbo, INC. 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "AdColonyPublic.h"
#import "CurrencyManager.h"
#import "CurrencyAppViewController.h"

@interface AppDelegate : NSObject <UIApplicationDelegate, AdColonyAdministratorDelegate> {
    UIWindow *window;
	UINavigationController *navigationController;
    
    CurrencyManager *cm;
    IBOutlet CurrencyAppViewController *bvc;	
}

-(void)showWindow;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;

@end

