/*
 * Copyright 2010-present Facebook.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "ViewController.h"

@interface ViewController ()

@property (nonatomic, strong, readwrite) FBAdView *adView;
@property (nonatomic, strong, readwrite) FBInterstitialAd *interstitialAd;

@end

@implementation ViewController

- (void)dealloc
{
  self.adView.delegate = nil;
}

- (void)viewDidLoad
{
  [super viewDidLoad];

  // Create a banner's ad view with a unique placement ID (generate your own on the Facebook app settings).
  // Use different ID for each ad placement in your app.
  self.adView = [[FBAdView alloc]
    initWithPlacementID:@"YOUR_PLACEMENT_ID"
    adSize:kFBAdSize320x50
    rootViewController:self];

  // Set a delegate to get notified on changes or when the user interact with the ad.
  self.adView.delegate = self;

  // When testing on a device, add its hashed ID to force test ads.
  // The hash ID is printed to console when running on a device.
  // [FBAdSettings addTestDevice:@"THE HASHED ID AS PRINTED TO CONSOLE"];

  // Initiate a request to load an ad.
  [self.adView loadAd];

  // Reposition the ad and add it to the view hierarchy.
  CGRect screenRect = [[UIScreen mainScreen] bounds];
  self.adView.frame =
  CGRectOffset(self.adView.frame,
               0,
               screenRect.size.height - kFBAdSize320x50.size.height);
  [self.view addSubview:self.adView];

}


#pragma mark - IB Actions

- (IBAction)loadInterstitalTapped:(id)sender {

  self.interstitalStatusLabel.text = @"Loading interstitial ad...";

  // Create the interstitial unit with a placement ID (generate your own on the Facebook app settings).
  // Use different ID for each ad placement in your app.
  self.interstitialAd = [[FBInterstitialAd alloc] initWithPlacementID:@"YOUR_PLACEMENT_ID"];

  // Set a delegate to get notified on changes or when the user interact with the ad.
  self.interstitialAd.delegate = self;

  // Initiate the request to load the ad.
  [self.interstitialAd loadAd];
}

- (IBAction)showInterstitialTapped:(id)sender {

  if (!self.interstitialAd || !self.interstitialAd.isAdValid)
  {
    // Ad not ready to present.
    self.interstitalStatusLabel.text = @"Ad not loaded. Click load to request an ad.";
  } else {
    self.interstitalStatusLabel.text = nil;

    // Ad is ready, present it!
    [self.interstitialAd showAdFromRootViewController:self];
  }
}

#pragma mark - FBInterstitialAdDelegate implementation

- (void)adViewDidClick:(FBAdView *)adView
{
  NSLog(@"Ad was clicked.");
}

- (void)adViewDidLoad:(FBAdView *)adView
{
  self.adViewStatusLabel.text = @"";
  NSLog(@"Ad was loaded.");

  // Now that the ad was loaded, show the view in case it was hidden before.
  adView.hidden = NO;
}

- (void)adView:(FBAdView *)adView didFailWithError:(NSError *)error
{
  self.adViewStatusLabel.text = @"Ad failed to load. Check console for details.";
  NSLog(@"Ad failed to load with error: %@", error);

  // Hide the unit since no ad is shown.
  adView.hidden = YES;
}

#pragma mark - FBInterstitialAdDelegate implementation

- (void)interstitialAdDidLoad:(FBInterstitialAd *)interstitialAd
{
  NSLog(@"Interstitial ad was laded. Can present now.");
  self.interstitalStatusLabel.text = @"Ad loaded. Click show to present!";
}

- (void)interstitialAd:(FBInterstitialAd *)interstitialAd didFailWithError:(NSError *)error
{
  NSLog(@"Interstitial failed to load with error: %@", error.description);
  self.interstitalStatusLabel.text = @"Interstitial ad failed to load. Check console for details.";
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

@end
