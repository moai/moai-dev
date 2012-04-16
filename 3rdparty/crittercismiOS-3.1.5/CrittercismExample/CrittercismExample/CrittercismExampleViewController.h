//
//  CrittercismExampleViewController.h
//  CrittercismExample
//
//  Created by Robert Kwok on 6/17/11.
//  Copyright 2011 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Crittercism.h"

@interface CrittercismExampleViewController : UIViewController <CrittercismDelegate> {

}

-(void) throwSignal;
-(void) throwNestedSignal;
-(void) throwExceptionInThread;
-(void) throwSignalInThread;
-(void) throwRandomException;

//static void staticException();
//static void staticSignal();
//
//-(void) nestedStaticException;
//-(void) nestedStaticSignal;

-(IBAction) feedbackHit:(id) sender;
-(IBAction) leaveBreadcrumbPressed:(id) sender;
-(IBAction) attachMetadataHit:(id) sender;
-(IBAction) crashHit:(id) sender;
-(IBAction) exceptionHit:(id) sender;
-(IBAction) critterHit:(id) sender;
-(IBAction) viewDataHit:(id) sender;

@end
