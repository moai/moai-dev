//
//  MobclixFullScreenAdViewController.h
//  Mobclix iOS SDK
//
//  Copyright 2011 Mobclix. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MobclixAdView.h"

@protocol MobclixFullScreenAdDelegate;
@interface MobclixFullScreenAdViewController : UIViewController {

}

/**
 * Prefetches and loads a full screen advertisement.
 * This must be called before calling displayRequestedAdFromViewController:.
 */
- (void)requestAd;

/**
 * Displays the previously requested ad.
 * This can not be called until requestAd has been called and the ad is finished loading.
 *
 * The view controller is the view controller the ad should be displayed from. This must
 * be an already existing view controller that's visible to the user.
 * 
 * Returns a boolean indicating whether or not the ad was successfully displayed.
 * This will return false if an ad has not yet been requested or finished loading,
 * if a full screen ad is already being displayed or if an invalid view controller is passed in.
 */
- (BOOL)displayRequestedAdFromViewController:(UIViewController*)viewController;

/**
 * Calls requestAd and then automatically displays the ad once it's loaded.
 */
- (void)requestAndDisplayAdFromViewController:(UIViewController*)viewController;

/**
 * Determine whether or not an ad has been prefetched and is ready to be displayed
 *
 * You can not display an ad until hasAd is true.
 */
@property(nonatomic,readonly) BOOL hasAd;

/**
 * Receive callback information during the ad cycle
 * See documentation for MobclixFullScreenAdDelegate
 *
 * NOTE: Be sure to set to nil when releasing MobclixFullScreenAdViewController
 */
@property(nonatomic,assign) id<MobclixFullScreenAdDelegate> delegate;
@end

@protocol MobclixFullScreenAdDelegate<NSObject>
@optional

/**
 * This is called when an ad has finished loading an ad
 * Once this is called, the "hasAd" property is true an ad can be displayed
 */
- (void)fullScreenAdViewControllerDidFinishLoad:(MobclixFullScreenAdViewController*)fullScreenAdViewController;

/**
 * This is called when an ad failed to load.  The error codes will match up with the codes in MobclixAdView.h
 */
- (void)fullScreenAdViewController:(MobclixFullScreenAdViewController*)fullScreenAdViewController didFailToLoadWithError:(NSError*)error;

/**
 * This is called when an ad is about to display and take over the screen
 */
- (void)fullScreenAdViewControllerWillPresentAd:(MobclixFullScreenAdViewController*)fullScreenAdViewController;

/**
 * This is called the user dismissed an ad and it is no longer on the screen.
 */
- (void)fullScreenAdViewControllerDidDismissAd:(MobclixFullScreenAdViewController*)fullScreenAdViewController;


@end