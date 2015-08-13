// Copyright (c) 2014-present, Facebook, Inc. All rights reserved.
//
// You are hereby granted a non-exclusive, worldwide, royalty-free license to use,
// copy, modify, and distribute this software in source code or binary form for use
// in connection with the web services and APIs provided by Facebook.
//
// As with any software that integrates with the Facebook platform, your use of
// this software is subject to the Facebook Developer Principles and Policies
// [http://developers.facebook.com/policy/]. This copyright notice shall be
// included in all copies or substantial portions of the software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#import "ViewController.h"

#import "ScrollViewController.h"
#import "TableViewController.h"

@interface ViewController ()

@property (strong, nonatomic) FBNativeAd *_nativeAd;
@property (strong, nonatomic) UIImage *_emptyStar;
@property (strong, nonatomic) UIImage *_fullStar;

@end

@implementation ViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    // Used in xib must be referenced here so linker won't eliminate it.
    [FBMediaView class];

    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self._emptyStar = [UIImage imageNamed:@"empty_star"];
    self._fullStar = [UIImage imageNamed:@"full_star"];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - IB Actions

- (IBAction)loadNativeAdTapped:(id)sender
{
    self.adStatusLabel.text = @"Requesting an ad...";

    // Create a native ad request with a unique placement ID (generate your own on the Facebook app settings).
    // Use different ID for each ad placement in your app.
    FBNativeAd *nativeAd = [[FBNativeAd alloc] initWithPlacementID:@"YOUR_PLACEMENT_ID"];

    // Set a delegate to get notified when the ad was loaded.
    nativeAd.delegate = self;

    // When testing on a device, add its hashed ID to force test ads.
    // The hash ID is printed to console when running on a device.
    // [FBAdSettings addTestDevice:@"THE HASHED ID AS PRINTED TO CONSOLE"];

    // Initiate a request to load an ad.
    [nativeAd loadAd];
}

#pragma mark - FBNativeAdDelegate implementation

- (void)nativeAdDidLoad:(FBNativeAd *)nativeAd
{
    NSLog(@"Native ad was loaded, constructing native UI...");

    if (self._nativeAd) {
        [self._nativeAd unregisterView];
    }

    self._nativeAd = nativeAd;

    // Create native UI using the ad metadata.
    [self.adCoverMediaView setNativeAd:nativeAd];

    __weak typeof(self) weakSelf = self;
    [self._nativeAd.icon loadImageAsyncWithBlock:^(UIImage *image) {
        __strong typeof(self) strongSelf = weakSelf;
        strongSelf.adIconImageView.image = image;
    }];
    self.adStatusLabel.text = @"";

    // Render native ads onto UIView
    self.adTitleLabel.text = self._nativeAd.title;
    self.adBodyLabel.text = self._nativeAd.body;
    self.adSocialContextLabel.text = self._nativeAd.socialContext;
    self.sponsoredLabel.text = @"Sponsored";

    [self setCallToActionButton:self._nativeAd.callToAction];

    NSLog(@"Register UIView for impression and click...");

    // Wire up UIView with the native ad; the whole UIView will be clickable.
    [nativeAd registerViewForInteraction:self.adUIView
                      withViewController:self];

    // Or you can replace above call with following function, so you can specify the clickable areas.
    // NSArray *clickableViews = @[self.adCallToActionButton, self.adCoverMediaView];
    // [nativeAd registerViewForInteraction:self.adUIView
    //                   withViewController:self
    //                   withClickableViews:clickableViews];

}

- (void)nativeAd:(FBNativeAd *)nativeAd didFailWithError:(NSError *)error
{
    self.adStatusLabel.text = @"Ad failed to load. Check console for details.";
    NSLog(@"Native ad failed to load with error: %@", error);
}

- (void)nativeAdDidClick:(FBNativeAd *)nativeAd
{
    NSLog(@"Native ad was clicked.");
}

- (void)nativeAdDidFinishHandlingClick:(FBNativeAd *)nativeAd
{
    NSLog(@"Native ad did finish click handling.");
}

- (void)nativeAdWillLogImpression:(FBNativeAd *)nativeAd
{
    NSLog(@"Native ad impression is being captured.");
}

#pragma mark - Private Methods

- (void)setCallToActionButton:(NSString *)callToAction
{
    [self.adCallToActionButton setHidden:NO];
    [self.adCallToActionButton setTitle:callToAction
                               forState:UIControlStateNormal];
}

#pragma mark - IB Actions

- (IBAction)loadNativeAdInTableViewTapped:(id)sender
{
    TableViewController *tableViewController = [[TableViewController alloc] init];
    [self presentViewController:tableViewController animated:YES completion:nil];
}

- (IBAction)loadNativeAdInScrollViewTapped:(id)sender {
    ScrollViewController *tableViewController = [[ScrollViewController alloc] init];
    [self presentViewController:tableViewController animated:YES completion:nil];
}

#pragma mark - Orientation

- (FBInterfaceOrientationMask)supportedInterfaceOrientations
{
  return UIInterfaceOrientationMaskPortrait;
}

@end
