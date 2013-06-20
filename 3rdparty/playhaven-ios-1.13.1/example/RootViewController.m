/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 RootViewController.m
 example

 Created by Jesus Fernandez on 4/25/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "RootViewController.h"
#import "PublisherOpenViewController.h"
#import "PublisherContentViewController.h"
#import "PublisherIAPTrackingViewController.h"
#import "PublisherCancelContentViewController.h"
#import "URLLoaderViewController.h"
#import "IAPViewController.h"
#import "IDViewController.h"
#import "SDURLCache.h"

@interface RootViewController ()
- (BOOL)isTokenAndSecretFilledIn;
- (void)loadTokenAndSecretFromDefaults;
- (void)saveTokenAndSecretToDefaults;
@property (nonatomic, retain) UIButton *clearCacheButton;
@end

@implementation RootViewController
@synthesize tokenField;
@synthesize secretField;
@synthesize optOutStatusSlider;
@synthesize serviceURLField;
@synthesize clearCacheButton;

+ (void)initialize
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString       *token    = [defaults valueForKey:@"ExampleToken"];
    NSString       *secret   = [defaults valueForKey:@"ExampleSecret"];

    if (PH_BASE_URL == nil || [PH_BASE_URL isEqualToString:@""]) {
        [defaults setValue:@"http://api2.playhaven.com" forKey:@"PHBaseUrl"];
    }

    if (token == nil || [token isEqualToString:@""]) {
        [defaults setValue:@"8ae979ddcdaf450996e897322169d26c" forKey:@"ExampleToken"];
    }

    if (secret == nil || [secret isEqualToString:@""]) {
        [defaults setValue:@"080d853e433a4468ba3315953b22615e" forKey:@"ExampleSecret"];
    }

    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)dealloc
{
    [tokenField release];
    [secretField release];
    [optOutStatusSlider release];
    [serviceURLField release];
    [clearCacheButton release];
    [super dealloc];
}

#pragma mark -
#pragma mark Private
- (BOOL)isTokenAndSecretFilledIn
{
    BOOL notNil   =  (self.tokenField.text && self.secretField.text);
    BOOL notEmpty = !([self.tokenField.text isEqualToString:@""] || [self.secretField.text isEqualToString:@""]);

    return notNil && notEmpty;
}

- (void)loadTokenAndSecretFromDefaults
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults synchronize];

    self.tokenField.text  = [defaults valueForKey:@"ExampleToken"];
    self.secretField.text = [defaults valueForKey:@"ExampleSecret"];
    self.serviceURLField.text = PH_BASE_URL;

    self.optOutStatusSlider.on = [PHAPIRequest optOutStatus];
}

- (void)saveTokenAndSecretToDefaults
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];

    [defaults setValue:self.tokenField.text forKey:@"ExampleToken"];
    [defaults setValue:self.secretField.text forKey:@"ExampleSecret"];

    [defaults synchronize];
}

#pragma mark -
#pragma mark UIViewController
- (UIView *)viewForTableFooter
{
    UIView *footerView = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 50)] autorelease];
    clearCacheButton   = [UIButton buttonWithType:UIButtonTypeRoundedRect];

    clearCacheButton.frame = CGRectMake(20, 10, self.view.frame.size.width - 40, 30);

    [clearCacheButton setTitle:@"Cache Is Empty (0 Mb)" forState:UIControlStateDisabled];

    clearCacheButton.autoresizingMask = UIViewAutoresizingNone | UIViewAutoresizingFlexibleWidth;

    footerView.backgroundColor  = [UIColor colorWithRed:0.72157 green:0.75686 blue:0.78431 alpha:1.0];
    footerView.autoresizingMask = UIViewAutoresizingNone | UIViewAutoresizingFlexibleWidth;

    [footerView addSubview:clearCacheButton];

    return footerView;
}

- (void)updateCacheButton
{
    if ([[NSURLCache sharedURLCache] isKindOfClass:[PH_SDURLCACHE_CLASS class]]) {
        NSUInteger currentCacheUsage =
                           [[NSURLCache sharedURLCache] currentDiskUsage] +
                           [[NSURLCache sharedURLCache] currentMemoryUsage];

        if (currentCacheUsage) {
            CGFloat f_size = [[NSNumber numberWithUnsignedInt:currentCacheUsage] floatValue] / 1024;

            NSString *s_size;

            if (f_size < 1024) s_size = [NSString stringWithFormat:@"(%.2f Kb)", f_size];
            else               s_size = [NSString stringWithFormat:@"(%.2f Mb)", f_size / 1024];

            [clearCacheButton setTitle:[NSString stringWithFormat:@"Clear Cache %@", s_size]
                              forState:UIControlStateNormal];
            [clearCacheButton setEnabled:YES];
        } else {
            [clearCacheButton setEnabled:NO];
        }

        [clearCacheButton addTarget:self action:@selector(clearCache:) forControlEvents:UIControlEventTouchUpInside];
    } else {
        [clearCacheButton setTitle:@"Set Cache" forState:UIControlStateNormal];
        [clearCacheButton addTarget:self action:@selector(setCache:) forControlEvents:UIControlEventTouchUpInside];
    }
}

- (void)setCache:(id)sender
{
    PH_SDURLCACHE_CLASS *urlCache = [[[PH_SDURLCACHE_CLASS alloc] initWithMemoryCapacity:PH_MAX_SIZE_MEMORY_CACHE
                                                                            diskCapacity:PH_MAX_SIZE_FILESYSTEM_CACHE
                                                                                diskPath:[PH_SDURLCACHE_CLASS defaultCachePath]] autorelease];
    [NSURLCache setSharedURLCache:urlCache];

    [self updateCacheButton];
}

- (void)clearCache:(id)sender
{
    [[NSURLCache sharedURLCache] removeAllCachedResponses];
    [self updateCacheButton];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.title = @"PlayHaven";

    UIBarButtonItem *toggleButton = [[UIBarButtonItem alloc] initWithTitle:@"Toggle"
                                                                     style:UIBarButtonItemStyleBordered
                                                                    target:self
                                                                    action:@selector(touchedToggleStatusBar:)];
    self.navigationItem.rightBarButtonItem = toggleButton;
    [toggleButton release];

    ((UITableView *)self.view).tableFooterView = [self viewForTableFooter];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self loadTokenAndSecretFromDefaults];

    [self updateCacheButton];
}

- (void)touchedToggleStatusBar:(id)sender
{
    BOOL statusBarHidden = [[UIApplication sharedApplication] isStatusBarHidden];

    if ([[UIApplication sharedApplication] respondsToSelector:@selector(setStatusBarHidden:withAnimation:)]) {
        [[UIApplication sharedApplication] setStatusBarHidden:!statusBarHidden withAnimation:UIStatusBarAnimationSlide];
    }

    [self.navigationController setNavigationBarHidden:!self.navigationController.navigationBarHidden animated:NO];
    [self.navigationController setNavigationBarHidden:!self.navigationController.navigationBarHidden animated:NO];
}

- (IBAction)touchedOptOutStatusSlider:(id)sender
{
    [PHAPIRequest setOptOutStatus:self.optOutStatusSlider.on];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 7;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";

    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }

    switch (indexPath.row) {
        case 0:
            cell.textLabel.text       = @"Open";
            cell.detailTextLabel.text = @"/publisher/open/";
            cell.accessibilityLabel   = @"open";
            break;
        case 1:
            cell.textLabel.text       = @"Content";
            cell.detailTextLabel.text = @"/publisher/content/";
            cell.accessibilityLabel   = @"content";
            break;
        case 2:
            cell.textLabel.text       = @"IAP Tracking";
            cell.detailTextLabel.text = @"";
            break;
        case 3:
            cell.textLabel.text       = @"Cancelled Content";
            cell.detailTextLabel.text = @"This content will be cancelled at an awkward time.";
            break;
        case 4:
            cell.textLabel.text       = @"URL Loader";
            cell.detailTextLabel.text = @"Test loading device URLs";
            cell.accessibilityLabel   = @"url loader";
            break;
        case 5:
            cell.textLabel.text       = @"IAP";
            cell.detailTextLabel.text = @"Test In-App Purchases";
            cell.accessibilityLabel   = @"iap";
            break;
        case 6:
            cell.textLabel.text       = @"Identifiers";
            cell.detailTextLabel.text = @"All of them";
            cell.accessibilityLabel   = @"identifiers";
        default:
            break;
    }

    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ([self isTokenAndSecretFilledIn]) {
        [self saveTokenAndSecretToDefaults];
        if (indexPath.row == 0) {
            PublisherOpenViewController *controller =
                [[PublisherOpenViewController alloc] initWithNibName:@"PublisherOpenViewController" bundle:nil];
            controller.title  = @"Open";
            controller.token  = self.tokenField.text;
            controller.secret = self.secretField.text;
            [self.navigationController pushViewController:controller animated:YES];
            [controller release];
        } else if (indexPath.row == 1) {
            PublisherContentViewController *controller =
                [[PublisherContentViewController alloc] initWithNibName:@"PublisherContentViewController" bundle:nil];
            controller.title  = @"Content";
            controller.token  = self.tokenField.text;
            controller.secret = self.secretField.text;
            [self.navigationController pushViewController:controller animated:YES];
            [controller release];
        } else if (indexPath.row == 2) {
            PublisherIAPTrackingViewController *controller =
                [[PublisherIAPTrackingViewController alloc] initWithNibName:@"PublisherIAPTrackingViewController" bundle:nil];
            controller.title  = @"IAP Tracking";
            controller.token  = self.tokenField.text;
            controller.secret = self.secretField.text;
            [self.navigationController pushViewController:controller animated:YES];
            [controller release];
        } else if (indexPath.row == 3) {
            PublisherCancelContentViewController *controller =
                [[PublisherCancelContentViewController alloc] initWithNibName:@"PublisherContentViewController" bundle:nil];
            controller.title  = @"Content";
            controller.token  = self.tokenField.text;
            controller.secret = self.secretField.text;
            [self.navigationController pushViewController:controller animated:YES];
            [controller release];
        } else if (indexPath.row == 4) {
            URLLoaderViewController *controller =
                [[URLLoaderViewController alloc] initWithNibName:@"URLLoaderViewController" bundle:nil];
            controller.title = @"URL Loader";
            [self.navigationController pushViewController:controller animated:YES];
            [controller release];
        } else if (indexPath.row == 5) {
            IAPViewController *controller =
                [[IAPViewController alloc] initWithNibName:@"IAPViewController" bundle:nil];
            controller.title = @"IAP";
            [self.navigationController pushViewController:controller animated:YES];
            [controller release];
        } else if (indexPath.row == 6) {
            IDViewController *controller =
                [[IDViewController alloc] initWithNibName:@"IDViewController" bundle:nil];
            controller.title = @"Identifiers";
            [self.navigationController pushViewController:controller animated:YES];
            [controller release];
        }
    } else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Missing Credentials"
                                                        message:@"You must supply a PlayHaven API token and secret to use this app. To get a token and secret, please visit http://playhaven.com on your computer and sign up."
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
    }
}

- (void)viewDidUnload
{
    [self setTokenField:nil];
    [self setSecretField:nil];
    [self setOptOutStatusSlider:nil];
    [self setServiceURLField:nil];
    [super viewDidUnload];
}
@end
