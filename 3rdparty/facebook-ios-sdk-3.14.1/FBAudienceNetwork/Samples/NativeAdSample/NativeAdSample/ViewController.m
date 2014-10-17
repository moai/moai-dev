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

@property (strong, nonatomic) FBNativeAd *_nativeAd;
@property (strong, nonatomic) UIImage *_emptyStar;
@property (strong, nonatomic) UIImage *_fullStar;

@end

@implementation ViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self._emptyStar = [UIImage imageNamed:@"empty_star"];
    self._fullStar = [UIImage imageNamed:@"full_star"];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - IB Actions

- (IBAction)loadNativeAdTapped:(id)sender
{
    self.adStatusLabel.text = @"Requesting an ad...";
    
    // Create a native ad request with a unique placement ID (generate your own on the Facebook app settings).
    // Use different ID for each ad placement in your app.
    FBNativeAd *nativeAd = [[FBNativeAd alloc] initWithPlacementID:@"YOUR_PLACEMENT_ID"];
    
    // Set a delegate to get notified when the ad was loaded.
    nativeAd.delegate = self;
    
    // When testing on a device, add its hashed ID to force test ads.
    // The hash ID is printed to console when running on a device.
    // [FBAdSettings addTestDevice:@"THE HASHED ID AS PRINTED TO CONSOLE"];
    
    // Initiate a request to load an ad.
    [nativeAd loadAd];
}

- (IBAction)callToActionTapped:(id)sender
{
    NSLog(@"Native ad unit was clicked.");
    
    // Handle the click which will open the ad destination and report the user click.
    [self._nativeAd handleClickWithViewController:self
                                         callback:^(FBNativeAd *nativeAd) {
                                             NSLog(@"Finished handling click on native ad.");
                                         }];
}

#pragma mark - FBNativeAdDelegate implementation

- (void)nativeAdDidLoad:(FBNativeAd *)nativeAd
{
    NSLog(@"Ad was loaded, constructing native UI...");
    
    self._nativeAd = nativeAd;
    
    // Create native UI using the ad metadata.
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        NSData *iconImageData = [NSData dataWithContentsOfURL:self._nativeAd.icon.url];
        NSData *coverImageData = [NSData dataWithContentsOfURL:self._nativeAd.coverImage.url];
        
        dispatch_async(dispatch_get_main_queue(), ^{
            self.adStatusLabel.text = @"";

            self.adIconImageView.image = [UIImage imageWithData:iconImageData];
            self.adCoverImageView.image = [UIImage imageWithData:coverImageData];
            
            self.adTitleLabel.text = self._nativeAd.title;
            self.adBodyLabel.text = self._nativeAd.body;
            self.adSocialContextLabel.text = self._nativeAd.socialContext;
            self.sponsoredLabel.text = @"Sponsored";
            
            [self setCallToActionButton:self._nativeAd.callToAction];

            [self setStarRating:self._nativeAd.starRating];
            
            // Once the view is visible, log the impression.
            [self._nativeAd logImpression];
        });
    });
}

- (void)nativeAd:(FBNativeAd *)nativeAd didFailWithError:(NSError *)error
{
    self.adStatusLabel.text = @"Ad failed to load. Check console for details.";
    NSLog(@"Ad failed to load with error: %@", error);
}

#pragma mark - Private Methods

- (void)setCallToActionButton:(NSString *)callToAction
{
    [self.adCallToActionButton setHidden:NO];
    [self.adCallToActionButton setTitle:callToAction
                               forState:UIControlStateNormal];
}

- (void)setStarRating:(struct FBAdStarRating)rating
{
    [[self.adStarRatingView subviews] makeObjectsPerformSelector: @selector(removeFromSuperview)];
    
    if (rating.scale != 0) {
        int i = 0;
        for(; i < rating.value; ++i) {
            [self setStarRatingImage:self._fullStar index:i];
        }
        for (; i < rating.scale; ++i) {
            [self setStarRatingImage:self._emptyStar index:i];
        }
    }
}

- (void)setStarRatingImage:(UIImage *)starImage
                     index:(int)indexOfStar
{
    UIImageView *imageView = [[UIImageView alloc] init];
    imageView.contentMode = UIViewContentModeScaleAspectFit;
    imageView.image = starImage;
    imageView.frame = CGRectMake(indexOfStar * 12, 0, 12, 12);
    [self.adStarRatingView addSubview:imageView];
}

@end
