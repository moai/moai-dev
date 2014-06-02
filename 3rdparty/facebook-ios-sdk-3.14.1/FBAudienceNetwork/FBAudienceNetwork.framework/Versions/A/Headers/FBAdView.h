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

#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>

@protocol FBAdViewDelegate;

// Ad Sizes
typedef struct FBAdSize {
  CGSize size;
} FBAdSize;

// 320x50
extern FBAdSize const kFBAdSize320x50;

// Interstital
extern FBAdSize const kFBAdSizeInterstital;

@interface FBAdView : UIView <UIWebViewDelegate>

- (instancetype)initWithPlacementID:(NSString *)placementID
                             adSize:(FBAdSize)adSize
                 rootViewController:(UIViewController *)viewController;

- (void)loadAd;

@property (nonatomic, copy, readonly) NSString *placementID;
@property (nonatomic, weak, readonly) UIViewController *rootViewController;
@property (nonatomic, weak) id<FBAdViewDelegate> delegate;

@end

@protocol FBAdViewDelegate <NSObject>

@optional

- (void)adViewDidClick:(FBAdView *)adView;
- (void)adViewDidLoad:(FBAdView *)adView;
- (void)adView:(FBAdView *)adView didFailWithError:(NSError *)error;

@end

