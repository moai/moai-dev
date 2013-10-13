//
//  CrittercismExampleAppViewController.h
//  CrittercismExampleApp
//
//  Created by Robert Kwok on 5/2/12.
//  Copyright 2012 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Crittercism.h"

@interface CrittercismExampleAppViewController : UIViewController <CrittercismDelegate> {
    
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

-(IBAction) leaveBreadcrumbPressed:(id) sender;
-(IBAction) attachMetadataHit:(id) sender;
-(IBAction) crashHit:(id) sender;
-(IBAction) exceptionHit:(id) sender;
-(IBAction) critterHit:(id) sender;
-(IBAction) viewDataHit:(id) sender;

@end
