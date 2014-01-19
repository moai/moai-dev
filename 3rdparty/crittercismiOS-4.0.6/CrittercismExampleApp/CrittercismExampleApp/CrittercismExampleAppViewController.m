//
//  CrittercismExampleAppViewController.m
//  CrittercismExampleApp
//
//  Created by Robert Kwok on 5/2/12.
//  Copyright 2012 Crittercism. All rights reserved.
//

#import "CrittercismExampleAppViewController.h"

@implementation CrittercismExampleAppViewController

-(IBAction) leaveBreadcrumbPressed:(id) sender {
    [Crittercism leaveBreadcrumb:@"Breadcrumb Button Pressed!"];
}

-(IBAction) attachMetadataHit:(id) sender {
//    [Crittercism setUsername:@"crittercism"];
    [Crittercism setValue:@"5" forKey:@"Game Level"];
}

-(IBAction) crashHit:(id) sender {
    
    [NSException raise:@"test version 3.3.2 crashy" format:@"awesomeness"];
    
    //    [NSException raise:@"Test Breadcrumbs" format:@"It works!"];
    //    [NSException raise:@"da da da da dada dada" format:@"can't touch this"];
    //    [self throwSignal];
    //    [self throwExceptionInThread];
    //    [self throwSignalInThread];
    //    [self throwRandomException];
    //    staticException();
    //    staticSignal();
    //    [self nestedStaticException];
    //    [self nestedStaticSignal];
}

-(IBAction) exceptionHit:(id) sender {
    @try {
        [NSException raise:@"test handled exception" format:@"excellent"];
    }
    @catch (NSException *exception) {
        // Log a handled exception to Crittercism
        [Crittercism logHandledException:exception];
    }
    @finally {
        NSLog(@"Handled Exception Thrown and Caught!  First exception is sent immediately, then a maximum of 3 are sent per minute");
    }
}

// ********
// 
// Here are all the ways you can raise exceptions and signals
// 
// *********
-(void) throwSignal {
    int i = 12345;
    NSLog(@"%@", i);
}

-(void) throwNestedSignal {
    [self throwSignal];
}

-(void) throwExceptionInThread {
    dispatch_async( dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        sleep(3);
        //        dispatch_async( dispatch_get_main_queue(), ^{
        // Add code here to update the UI/send notifications based on the
        // results of the background processing
        //        });
    });
    dispatch_async( dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        // This thread should explode
        [NSException raise:@"threaded exception!" format:@"one guy sleeps, the other messes up"];
    });
}

-(void) throwSignalInThread {
    dispatch_async( dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        sleep(3);
    });
    dispatch_async( dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        // This thread should explode
        [self throwNestedSignal];
    });
}

-(void) throwRandomException {
    NSString *alphabet  = @"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXZY0123456789";
    NSMutableString *s = [NSMutableString stringWithCapacity:20];
    for (NSUInteger i = 0U; i < 20; i++) {
        u_int32_t r = arc4random() % [alphabet length];
        unichar c = [alphabet characterAtIndex:r];
        [s appendFormat:@"%C", c];
    }
    [NSException raise:s format:@"oops!"];
}

// Implement the protocol
#pragma mark CrittercismDelegate
-(void)crittercismDidCrashOnLastLoad {
    NSLog(@"App crashed the last time it was loaded");
}

//static void staticException() {
//    // Wrong stacktrace here?
//    [NSException raise:@"static exception!" format:@"oh shit, do we get the right symbol?"];
//}
//
//static void staticSignal() {
//    int i = 12345;
//    int j = 34567;
//    NSLog( @"%@", i+j );
//}
//
//-(void) nestedStaticException {
//    staticException();
//}
//
//-(void) nestedStaticSignal {
//    staticSignal();
//}

-(IBAction) critterHit:(id) sender {
    [Crittercism leaveBreadcrumb:@"Critter is Hit!"];
    
    UIAlertView *alert = [[UIAlertView alloc] init];
    [alert setTitle:@"Hey that tickles!"];
    [alert setMessage:@"Do you love Crittercism?"];
    [alert setDelegate:self];
    [alert addButtonWithTitle:@"Yes"];
    [alert addButtonWithTitle:@"No"];
    [alert show];
    [alert release];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == 0)
	{
        [Crittercism leaveBreadcrumb:@"I am loved!"];
	}
	else if (buttonIndex == 1)
	{
        [Crittercism leaveBreadcrumb:@"True love found somewhere else :("];
	}
}

-(IBAction) viewDataHit:(id) sender {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://www.crittercism.com/developers/demo"]];
}


- (void)dealloc
{
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
