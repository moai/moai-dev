//
//  CrittercismExampleAppDelegate.h
//  CrittercismExample
//
//  Created by Robert Kwok on 6/17/11.
//  Copyright 2011 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Crittercism.h"

@class CrittercismExampleViewController;

@interface CrittercismExampleAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet CrittercismExampleViewController *viewController;

@end
