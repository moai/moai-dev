//
//  CrittercismExampleAppAppDelegate.h
//  CrittercismExampleApp
//
//  Created by Robert Kwok on 5/2/12.
//  Copyright 2012 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Crittercism.h"

@class CrittercismExampleAppViewController;

@interface CrittercismExampleAppAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet CrittercismExampleAppViewController *viewController;

@end
