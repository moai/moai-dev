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

#import "FBAdMobCustomEventInterstitial.h"

@interface FBAdMobCustomEventInterstitial ()

@property (nonatomic, retain) FBInterstitialAd *fbInterstitialAd;

@end

@implementation FBAdMobCustomEventInterstitial

@synthesize delegate = delegate_;

- (void)requestInterstitialAdWithParameter:(NSString *)serverParameter
                                     label:(NSString *)serverLabel
                                   request:(GADCustomEventRequest *)request{

    if ([serverParameter length] == 0) {
        [self.delegate customEventInterstitial:self didFailAd:nil];
        return;
    }

    self.fbInterstitialAd = [[FBInterstitialAd alloc ] initWithPlacementID:serverParameter];
    self.fbInterstitialAd.delegate = self;
    [self.fbInterstitialAd loadAd];
}

- (void)presentFromRootViewController:(UIViewController *)rootViewController{
    if (!self.fbInterstitialAd || !self.fbInterstitialAd.isAdValid) {
    } else {
        [self.delegate customEventInterstitialWillPresent:self];
        [self.fbInterstitialAd showAdFromRootViewController:rootViewController];
    }
}

- (void) dealloc {
    _fbInterstitialAd.delegate = nil;
    [_fbInterstitialAd release];
    [super dealloc];
}

#pragma mark FBInterstitialAdDelegate methods

- (void)interstitialAdDidLoad:(FBInterstitialAd *)interstitialAd
{
    [self.delegate customEventInterstitial:self didReceiveAd:interstitialAd];
}

- (void)interstitialAd:(FBInterstitialAd *)interstitialAd didFailWithError:(NSError *)error
{
    [self.delegate customEventInterstitial:self didFailAd:error];
}

- (void)interstitialAdDidClick:(FBInterstitialAd *)interstitialAd
{
    [self.delegate customEventInterstitialWillLeaveApplication:self];
}

- (void)interstitialAdDidClose:(FBInterstitialAd *)interstitialAd
{
    [self.delegate customEventInterstitialDidDismiss:self];
}

- (void)interstitialAdWillClose:(FBInterstitialAd *)interstitialAd
{
    [self.delegate customEventInterstitialWillDismiss:self];
}

@end
