//
//  AppDelegate.h
//  AdColonyAdvanced
//
//  Created by John Fernandes-Salling on 9/12/12.
//

#import <UIKit/UIKit.h>
#import <AdColony/AdColony.h>

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate, AdColonyDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) ViewController *viewController;

@end
