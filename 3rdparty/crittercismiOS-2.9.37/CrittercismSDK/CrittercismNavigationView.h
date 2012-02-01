//
//  CrittercismNavigationView.h
//  CrittercismIPhone3
//
//  Created by Robert Kwok on 10/13/10.
//  Copyright 2010 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CrittercismRootViewController.h"

@interface CrittercismNavigationView : UINavigationController {

	CrittercismRootViewController *rootController;
}

-(void) setRoot:(CrittercismRootViewController *)_rootController;
-(id) initWithRoot:(CrittercismRootViewController *)_rootController;

@end
