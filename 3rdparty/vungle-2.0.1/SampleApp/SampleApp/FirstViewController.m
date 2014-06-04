//
//  FirstViewController.m
//  SampleApp
//
//  Created by Rolando Abarca on 10/24/13.
//  Copyright (c) 2013 Vungle. All rights reserved.
//

#import "FirstViewController.h"

@interface FirstViewController ()

@end

@implementation FirstViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)playAd:(id)sender {
	if ([VGVunglePub adIsAvailable]) {
		[VGVunglePub playModalAd:self animated:YES];
	}
}

- (void)vungleAppStoreWillAppear {
	[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
	UIViewController* fake = [[UIViewController alloc] init];
	[self presentViewController:fake animated:NO completion:nil];
	[fake dismissViewControllerAnimated:NO completion:nil];
}

- (void)vungleViewDidDisappear:(UIViewController *)viewController willShowProductView:(BOOL)willShow {
	if (willShow) {
	}
}

- (void)vungleAppStoreViewDidDisappear {
	[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
}

- (BOOL)shouldAutorotate {
	return YES;
}

- (NSUInteger)supportedInterfaceOrientations {
	return UIInterfaceOrientationMaskPortrait;
}


@end
