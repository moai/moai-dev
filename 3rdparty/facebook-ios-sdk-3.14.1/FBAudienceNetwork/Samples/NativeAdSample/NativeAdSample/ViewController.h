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

#import <UIKit/UIKit.h>

#import <FBAudienceNetwork/FBAudienceNetwork.h>

@interface ViewController : UIViewController <FBNativeAdDelegate>

@property (strong, nonatomic) IBOutlet UILabel *adStatusLabel;
@property (strong, nonatomic) IBOutlet UIImageView *adIconImageView;
@property (strong, nonatomic) IBOutlet UIImageView *adCoverImageView;
@property (strong, nonatomic) IBOutlet UILabel *adTitleLabel;
@property (strong, nonatomic) IBOutlet UILabel *adBodyLabel;
@property (strong, nonatomic) IBOutlet UIButton *adCallToActionButton;
@property (strong, nonatomic) IBOutlet UILabel *adSocialContextLabel;
@property (strong, nonatomic) IBOutlet UIView *adStarRatingView;
@property (strong, nonatomic) IBOutlet UILabel *sponsoredLabel;

- (IBAction)loadNativeAdTapped:(id)sender;
- (IBAction)callToActionTapped:(id)sender;

@end
