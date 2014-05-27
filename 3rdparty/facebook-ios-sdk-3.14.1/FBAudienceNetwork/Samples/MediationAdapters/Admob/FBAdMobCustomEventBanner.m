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

#import "FBAdMobCustomEventBanner.h"

@interface FBAdMobCustomEventBanner ()

@property (nonatomic, retain) FBAdView *fbAdView;

@end

@implementation FBAdMobCustomEventBanner

@synthesize delegate = delegate_;

- (void)requestBannerAd:(GADAdSize)adSize
              parameter:(NSString *)serverParameter
                  label:(NSString *)serverLabel
                request:(GADCustomEventRequest *)request
{
    if ([serverParameter length] == 0) {
        [self.delegate customEventBanner:self didFailAd:nil];
        return;
    }

    self.fbAdView = [[FBAdView alloc]initWithPlacementID:serverParameter
                                                  adSize:kFBAdSize320x50
                                      rootViewController:[self.delegate viewControllerForPresentingModalView]];
    self.fbAdView.delegate = self;
    [self.fbAdView loadAd];
}

- (void) dealloc {
    _fbAdView.delegate = nil;
    [_fbAdView release];
    [super dealloc];
}

#pragma mark FBAdViewDelegate methods

- (void)adView:(FBAdView *)adView didFailWithError:(NSError *)error;
{
    [self.delegate customEventBanner:self didFailAd:error];
}

- (void)adViewDidLoad:(FBAdView *)adView;
{
    [self.delegate customEventBanner:self didReceiveAd:self.fbAdView];
}

- (void)adViewDidClick:(FBAdView *)adView
{
    [self.delegate customEventBanner:self clickDidOccurInAd:self.fbAdView];
}

@end
