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

#import <FBAudienceNetwork/FBAudienceNetwork.h>

#import "MPInterstitialCustomEvent.h"

/**
 * FBMoPubInterstitialCustomEvent is a MoPub interstitial adapter for InterstitialAd.
 * <p/>
 * Compatible with mopub-ios-sdk version 1.17.3.0
 */

@interface FBMoPubInterstitialCustomEvent : MPInterstitialCustomEvent <FBInterstitialAdDelegate>

@end
