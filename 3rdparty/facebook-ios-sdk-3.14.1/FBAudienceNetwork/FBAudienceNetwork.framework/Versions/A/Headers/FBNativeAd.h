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

#import "FBAdImage.h"
#import "FBAdView.h"

@class FBAdRequest;
@protocol FBNativeAdDelegate;

/* The class represents a native FB Ad. Raw metadata of ad is returned */
@class FBNativeAd;

typedef void (^FBNativeAdCompletionHandler)(FBNativeAd *nativeAd);

@interface FBNativeAd : NSObject

@property (nonatomic, copy, readonly) NSString *placementID;
@property (nonatomic, assign, readonly) struct FBAdStarRating starRating;
@property (nonatomic, copy, readonly) NSString *title;
@property (nonatomic, copy, readonly) NSString *socialContext;
@property (nonatomic, copy, readonly) NSString *callToAction;
@property (nonatomic, strong, readonly) FBAdImage *icon;
@property (nonatomic, strong, readonly) FBAdImage *coverImage;
@property (nonatomic, copy, readonly) NSString *body;
@property (nonatomic, weak) id<FBNativeAdDelegate> delegate;

/* initialize an instance of FBNative Ad with a placement ID and a view controller */
- (instancetype)initWithPlacementID:(NSString *)placementID;

/* Call loadAd and sign up for a delegate determine whether it succeed or fails */
- (void)loadAd;

/* call handleClick when a user click on the ad to launch the appropriate app store view or in app browser. You can supply an optional callback */
- (void)handleClickWithViewController:(UIViewController *)viewController
                             callback:(FBNativeAdCompletionHandler)completionHandler;

/* Call logImpression whenever a native ad is made visible to the user */
- (void)logImpression;

/* Call isAdValid to check whether native ad is valid & internal consistent prior rendering using its properties. If
   rendering is done as part of the loadAd callback, it is guarantee to be consistent */
- (BOOL)isAdValid;

@end

/* The methods declared by the FBNativeAdDelegate protocol allow the adopting delegate to respond
 to messages from the FBNativeAd class and thus respond to operations such
 as whether the native ad has loaded or user has clicked */
@protocol FBNativeAdDelegate <NSObject>

- (void)nativeAdDidLoad:(FBNativeAd *)nativeAd;
- (void)nativeAd:(FBNativeAd *)nativeAd didFailWithError:(NSError *)error;

@end

