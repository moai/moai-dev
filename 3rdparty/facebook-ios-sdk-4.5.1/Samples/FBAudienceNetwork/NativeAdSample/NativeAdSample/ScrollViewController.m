// Copyright (c) 2014-present, Facebook, Inc. All rights reserved.
//
// You are hereby granted a non-exclusive, worldwide, royalty-free license to use,
// copy, modify, and distribute this software in source code or binary form for use
// in connection with the web services and APIs provided by Facebook.
//
// As with any software that integrates with the Facebook platform, your use of
// this software is subject to the Facebook Developer Principles and Policies
// [http://developers.facebook.com/policy/]. This copyright notice shall be
// included in all copies or substantial portions of the software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#import "ScrollViewController.h"

@interface ScrollViewController () <FBNativeAdDelegate, FBNativeAdsManagerDelegate>

@property (nonatomic, strong) FBNativeAdsManager *manager;
@property (nonatomic, weak) FBNativeAdScrollView *scrollView;
@property (nonatomic, weak) IBOutlet UIView *containerView;

@end

@implementation ScrollViewController

- (void)viewDidLoad {
  [super viewDidLoad];

  FBNativeAdsManager *manager = [[FBNativeAdsManager alloc] initWithPlacementID:@"YOUR_PLACEMENT_ID" forNumAdsRequested:5];
  manager.delegate = self;
  [manager loadAds];
  self.manager = manager;
}

- (void)viewDidAppear:(BOOL)animated
{
  [super viewDidAppear:animated];
  self.scrollView.frame = self.containerView.bounds;
}

- (IBAction)dismissViewController:(id)sender
{
  [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)refresh:(id)sender
{
  [self.manager loadAds];
}

#pragma mark FBNativeAdDelegate

- (void)nativeAdDidClick:(FBNativeAd *)nativeAd
{
  NSLog(@"Native ad was clicked.");
}

- (void)nativeAdDidFinishHandlingClick:(FBNativeAd *)nativeAd
{
  NSLog(@"Native ad did finish click handling.");
}

- (void)nativeAdWillLogImpression:(FBNativeAd *)nativeAd
{
  NSLog(@"Native ad impression is being captured.");
}

#pragma mark FBNativeAdsManagerDelegate

- (void)nativeAdsLoaded
{
  NSLog(@"Native ads loaded, constructing native UI...");
  if (self.scrollView) {
    [self.scrollView removeFromSuperview];
    self.scrollView = nil;
  }
  FBNativeAdScrollView *scrollView = [[FBNativeAdScrollView alloc] initWithNativeAdsManager:self.manager withType:FBNativeAdViewTypeGenericHeight300];
  scrollView.delegate = self;
  scrollView.frame = self.containerView.bounds;
  [self.containerView addSubview:scrollView];
  self.scrollView = scrollView;
}

- (void)nativeAdsFailedToLoadWithError:(NSError *)error
{
  NSLog(@"Native ads failed to load with error: %@", error);
  UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Native ad failed to load"
                                                  message:@"Check console for more details"
                                                 delegate:nil
                                        cancelButtonTitle:@"OK"
                                        otherButtonTitles:nil];
  [alert show];
}

#pragma mark - Orientation

- (FBInterfaceOrientationMask)supportedInterfaceOrientations
{
  return UIInterfaceOrientationMaskPortrait;
}

@end
