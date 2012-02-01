//
//  CrittercismViewController.h
//  CrittercismIPhone3
//
//  Created by Robert Kwok on 10/6/10.
//  Copyright 2010 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CrittercismRootViewController.h"
#import "CrittercismNavigationView.h"

@interface CrittercismViewController : UIViewController {

	NSString *appIDStr;
	NSString *keyStr;
	NSString *secretStr;
	
	float redTint;
	float greenTint;
	float blueTint;
	
	CrittercismNavigationView *navController;
	CrittercismRootViewController *crittercismRootViewController;
}

-(void) setTintRed:(float) _red green:(float)_green blue:(float)_blue;

@property(nonatomic, retain) NSString *appIDStr;
@property(nonatomic, retain) NSString *keyStr;
@property(nonatomic, retain) NSString *secretStr;

@end
