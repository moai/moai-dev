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

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#import "FBAdView.h"

@protocol FBInterstitialAdDelegate;

/* A modal view controller to represent an Facebook Interstitial ad view */
@interface FBInterstitialAd : UIViewController <FBAdViewDelegate>

@property (nonatomic, copy, readonly) NSString *placementID;
@property (nonatomic, weak) id<FBInterstitialAdDelegate> delegate;

- (instancetype)initWithPlacementID:(NSString *)placementID;
- (BOOL)isAdValid;
- (void)loadAd;
- (BOOL)showAdFromRootViewController:(UIViewController *)rootViewController;

@end

/* The methods declared by the FBInterstitialAdDelegate protocol allow the adopting delegate to respond
   to messages from the FBInterstitialAd class and thus respond to operations such
   as whether the interstitial has loaded, user has clicked or closed the interstitial. */
@protocol FBInterstitialAdDelegate <NSObject>

@optional

- (void)interstitialAdDidClick:(FBInterstitialAd *)interstitialAd;
- (void)interstitialAdDidClose:(FBInterstitialAd *)interstitialAd;
- (void)interstitialAdWillClose:(FBInterstitialAd *)interstitialAd;
- (void)interstitialAdDidLoad:(FBInterstitialAd *)interstitialAd;
- (void)interstitialAd:(FBInterstitialAd *)interstitialAd didFailWithError:(NSError *)error;

@end
