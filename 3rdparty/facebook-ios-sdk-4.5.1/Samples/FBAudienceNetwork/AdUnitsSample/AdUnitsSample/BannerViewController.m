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

#import "BannerViewController.h"

@interface BannerViewController ()

@property (nonatomic, weak) IBOutlet UILabel *adStatusLabel;
@property (nonatomic, strong) FBAdView *adView;

@end

@implementation BannerViewController

- (void)viewDidLoad {
  [super viewDidLoad];

  // Create a banner's ad view with a unique placement ID (generate your own on the Facebook app settings).
  // Use different ID for each ad placement in your app.
  BOOL isIPAD = ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad);
  FBAdSize adSize = isIPAD ? kFBAdSizeHeight90Banner : kFBAdSizeHeight50Banner;
  self.adView = [[FBAdView alloc] initWithPlacementID:@"YOUR_PLACEMENT_ID"
                                               adSize:adSize
                                   rootViewController:self];

  // Set a delegate to get notified on changes or when the user interact with the ad.
  self.adView.delegate = self;

  // When testing on a device, add its hashed ID to force test ads.
  // The hash ID is printed to console when running on a device.
  // [FBAdSettings addTestDevice:@"THE HASHED ID AS PRINTED TO CONSOLE"];

  // Initiate a request to load an ad.
  [self.adView loadAd];

  // Reposition the adView to the bottom of the screen
  CGSize viewSize = self.view.bounds.size;
  CGSize tabBarSize = self.tabBarController.tabBar.frame.size;
  viewSize = CGSizeMake(viewSize.width, viewSize.height - tabBarSize.height);
  CGFloat bottomAlignedY = viewSize.height - adSize.size.height;
  self.adView.frame = CGRectMake(0, bottomAlignedY, viewSize.width, adSize.size.height);

  // Set autoresizingMask so the rotation is automatically handled
  self.adView.autoresizingMask =
    UIViewAutoresizingFlexibleRightMargin |
    UIViewAutoresizingFlexibleLeftMargin|
    UIViewAutoresizingFlexibleWidth |
    UIViewAutoresizingFlexibleTopMargin;

  // Add adView to the view hierarchy.
  [self.view addSubview:self.adView];

  self.adStatusLabel.text = @"Loading an ad...";
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

#pragma mark - FBAdViewDelegate implementation

// Implement this function if you want to change the viewController after the FBAdView
// is created. The viewController will be used to present the modal view (such as the
// in-app browser that can appear when an ad is clicked).
// - (UIViewController *)viewControllerForPresentingModalView
// {
//   return self;
// }

- (void)adViewDidClick:(FBAdView *)adView
{
  NSLog(@"Ad was clicked.");
}

- (void)adViewDidFinishHandlingClick:(FBAdView *)adView
{
  NSLog(@"Ad did finish click handling.");
}

- (void)adViewDidLoad:(FBAdView *)adView
{
  self.adStatusLabel.text = @"Ad loaded.";
  NSLog(@"Ad was loaded.");
  // Now that the ad was loaded, show the view in case it was hidden before.
  self.adView.hidden = NO;
}

- (void)adView:(FBAdView *)adView didFailWithError:(NSError *)error
{
  self.adStatusLabel.text = @"Ad failed to load. Check console for details.";
  NSLog(@"Ad failed to load with error: %@", error);

  // Hide the unit since no ad is shown.
  self.adView.hidden = YES;
}

- (void)adViewWillLogImpression:(FBAdView *)adView
{
  NSLog(@"Ad impression is being captured.");
}

@end
