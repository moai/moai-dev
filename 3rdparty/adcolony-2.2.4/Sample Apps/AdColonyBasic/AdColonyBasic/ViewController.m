//
//  ViewController.m
//  AdColonyBasic
//
//  Created by John Fernandes-Salling on 8/14/12.
//

#import "ViewController.h"

#import <AdColony/AdColony.h>

@interface ViewController ()
@end

@implementation ViewController

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

#pragma mark -
#pragma mark AdColony-specific
-(IBAction)triggerVideo
{
    [AdColony playVideoAdForZone:@"vzf8fb4670a60e4a139d01b5" withDelegate:nil];
}

@end
