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

#import "FBMoPubInterstitialCustomEvent.h"

#import "MPInstanceProvider.h"
#import "MPLogging.h"

@interface MPInstanceProvider (FacebookInterstitials)

- (FBInterstitialAd *)buildFBInterstitialAdWithPlacementID:(NSString *)placementID
                                                  delegate:(id<FBInterstitialAdDelegate>)delegate;

@end

@implementation MPInstanceProvider (FacebookInterstitials)

- (FBInterstitialAd *)buildFBInterstitialAdWithPlacementID:(NSString *)placementID
                                                  delegate:(id<FBInterstitialAdDelegate>)delegate
{
    FBInterstitialAd *interstitialAd = [[[FBInterstitialAd alloc] initWithPlacementID:placementID] autorelease];
    interstitialAd.delegate = delegate;
    return interstitialAd;
}

@end

@interface FBMoPubInterstitialCustomEvent ()

@property (nonatomic, retain) FBInterstitialAd *fbInterstitialAd;

@end

@implementation FBMoPubInterstitialCustomEvent

- (void)requestInterstitialWithCustomEventInfo:(NSDictionary *)info
{
    if (![info objectForKey:@"placement_id"]) {
        MPLogError(@"Placement ID is required for Facebook interstitial ad");
        [self.delegate interstitialCustomEvent:self didFailToLoadAdWithError:nil];
        return;
    }

    MPLogInfo(@"Requesting Facebook interstitial ad");

    self.fbInterstitialAd =
        [[MPInstanceProvider sharedProvider] buildFBInterstitialAdWithPlacementID:[info objectForKey:@"placement_id"]
                                                                        delegate:self];

    [self.fbInterstitialAd loadAd];
}

- (void)showInterstitialFromRootViewController:(UIViewController *)controller {
    if (!self.fbInterstitialAd || !self.fbInterstitialAd.isAdValid) {
        MPLogError(@"Facebook interstital ad was not loaded");
    } else {
        MPLogInfo(@"Facebook interstitial ad will be presented");
        [self.delegate interstitialCustomEventWillAppear:self];
        [self.fbInterstitialAd showAdFromRootViewController:controller];
        MPLogInfo(@"Facebook interstitial ad was presented");
        [self.delegate interstitialCustomEventDidAppear:self];
    }
}

- (void)dealloc
{
    [_fbInterstitialAd release];
    [super dealloc];
}

#pragma mark FBInterstitialAdDelegate methods

- (void)interstitialAdDidLoad:(FBInterstitialAd *)interstitialAd
{
    MPLogInfo(@"Facebook intersitital ad was loaded. Can present now");
    [self.delegate interstitialCustomEvent:self didLoadAd:interstitialAd];
}

- (void)interstitialAd:(FBInterstitialAd *)interstitialAd didFailWithError:(NSError *)error
{
    MPLogInfo(@"Facebook intersitital ad failed to load with error: %@", error.description);
    [self.delegate interstitialCustomEvent:self didFailToLoadAdWithError:nil];
}

- (void)interstitialAdDidClick:(FBInterstitialAd *)interstitialAd
{
    MPLogInfo(@"Facebook interstitial ad was clicked");
    [self.delegate interstitialCustomEventDidReceiveTapEvent:self];
}

- (void)interstitialAdDidClose:(FBInterstitialAd *)interstitialAd
{
    MPLogInfo(@"Facebook interstitial ad was closed");
    [self.delegate interstitialCustomEventDidDisappear:self];
}

- (void)interstitialAdWillClose:(FBInterstitialAd *)interstitialAd
{
    MPLogInfo(@"Facebook interstitial ad will close");
    [self.delegate interstitialCustomEventWillDisappear:self];
}


@end
