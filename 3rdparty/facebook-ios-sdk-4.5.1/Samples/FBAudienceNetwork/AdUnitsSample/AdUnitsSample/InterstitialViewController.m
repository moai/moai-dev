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

#import "InterstitialViewController.h"

@interface InterstitialViewController ()

@property (weak, nonatomic) IBOutlet UILabel *adStatusLabel;
@property (nonatomic, strong) FBInterstitialAd *interstitialAd;

@end

@implementation InterstitialViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

- (IBAction)loadInterstitalTapped:(id)sender
{
  self.adStatusLabel.text = @"Loading interstitial ad...";

  // Create the interstitial unit with a placement ID (generate your own on the Facebook app settings).
  // Use different ID for each ad placement in your app.
  self.interstitialAd = [[FBInterstitialAd alloc] initWithPlacementID:@"YOUR_PLACEMENT_ID"];

  // Set a delegate to get notified on changes or when the user interact with the ad.
  self.interstitialAd.delegate = self;

  // Initiate the request to load the ad.
  [self.interstitialAd loadAd];
}

- (IBAction)showInterstitialTapped:(id)sender
{
  if (!self.interstitialAd || !self.interstitialAd.isAdValid)
  {
    // Ad not ready to present.
    self.adStatusLabel.text = @"Ad not loaded. Click load to request an ad.";
  } else {
    self.adStatusLabel.text = nil;

    // Ad is ready, present it!
    [self.interstitialAd showAdFromRootViewController:self];
  }
}

#pragma mark - FBInterstitialAdDelegate implementation

- (void)interstitialAdDidLoad:(FBInterstitialAd *)interstitialAd
{
  NSLog(@"Interstitial ad was loaded. Can present now.");
  self.adStatusLabel.text = @"Ad loaded. Click show to present!";
}

- (void)interstitialAd:(FBInterstitialAd *)interstitialAd didFailWithError:(NSError *)error
{
  NSLog(@"Interstitial failed to load with error: %@", error.description);
  self.adStatusLabel.text = @"Interstitial ad failed to load. Check console for details.";
}

- (void)interstitialAdDidClick:(FBInterstitialAd *)interstitialAd
{
  NSLog(@"Interstitial was clicked.");
}

- (void)interstitialAdDidClose:(FBInterstitialAd *)interstitialAd
{
  NSLog(@"Interstitial closed.");

  // Optional, Cleaning up.
  self.interstitialAd = nil;
}

- (void)interstitialAdWillClose:(FBInterstitialAd *)interstitialAd
{
  NSLog(@"Interstitial will close.");
}

- (void)interstitialAdWillLogImpression:(FBInterstitialAd *)interstitialAd
{
  NSLog(@"Interstitial impression is being captured.");
}


@end
