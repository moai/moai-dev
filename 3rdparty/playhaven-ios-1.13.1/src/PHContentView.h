/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 PHContentView.h (formerly PHAdUnitView.h)
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/1/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <StoreKit/StoreKit.h>
#import "PHURLLoader.h"

@class PHContent;
@protocol PHContentViewDelegate;

/**
 * @internal
 *
 * @brief Displays a content unit inside of a webview and manages content-template
 * dispatches and SDK callbacks between them. Also handles show/hide
 * transitions.
 **/
@interface PHContentView : UIView <UIWebViewDelegate, PHURLLoaderDelegate> {
    PHContent                *_content;
    UIInterfaceOrientation    _orientation;
    id<PHContentViewDelegate> _delegate; // TODO: Change these to id<PHContentViewDelegate>

    UIWebView *_webView;
    BOOL       _willAnimate;

    NSMutableDictionary     *_redirects;
    UIActivityIndicatorView *_activityView;
    UIView *_targetView;
}

/**
 * To avoid the overhead of initialization, contentView instances may be
 * recycled using this status method once they have been dismissed
 *
 * @param contentView
 *   The content view
 **/
+ (void)enqueueContentViewInstance:(PHContentView *)contentView;

/**
 * Returns a PHContentView instance if one has been enqueued, otherwise returns nil
 *
 * @return
 *   A PHContentView
 **/
+ (PHContentView *)dequeueContentViewInstance;

/**
 * Initializes a new PHContentView instance for the given PHContent
 *
 * @param content
 *   The given PHContent
 *
 * @return
 *   A new PHContentView
 **/
- (id)initWithContent:(PHContent *)content;

@property(nonatomic, retain) PHContent *content;    /**< Sets the PHContent instance for this view, PHContent defines the
                                                         template, transition, frame size, and context for this content unit */
@property(nonatomic, assign) UIView    *targetView; /**< When shown, the content view will attempt to attach itself to this view. Defaults to nil */
@property(nonatomic, assign) id<PHContentViewDelegate> delegate; /**< Content view delegate */

/**
 * Show the content unit, with animation. Loads the content template as well
 *
 * @param animated
 *   BOOL value indicating if the content view should be shown with animation
 **/
- (void)show:(BOOL)animated;

/**
 * Dismiss the content unit, with animation.
 *
 * @param animated
 *   BOOL value indicating if the content view should be dismissed with animation
 **/
- (void)dismiss:(BOOL)animated;

/**
 * Special dismiss handler for native close button-initiated dismisses
 **/
- (void)dismissFromButton;

/**
 * Assigns the dispatch URL \c urlPath to an invocation of the \c action
 * selector on \c target. Replaces existing assignment if one already exists
 *
 * @param urlPath
 *   The URL path
 *
 * @param target
 *   The target
 *
 * @param action
 *   The action
 **/
- (void)redirectRequest:(NSString *)urlPath toTarget:(id)target action:(SEL)action;

/**
 * Returns response and/or error data to the content template for the callback
 * with id \c callback. Returns \c YES if the callback javascript executes without
 * throwing an exception
 *
 * @param callback
 *   The callback
 *
 * @param response
 *   The response
 *
 * @param error
 *   Error data
 *
 * @return
 *   Returns \c YES if the callback javascript executes without throwing an exception, \c NO otherwise
 **/
- (BOOL)sendCallback:(NSString *)callback withResponse:(id)response error:(id)error;
@end

/**
 * @internal
 *
 * @brief Delegates receive messages about the state of the content view
 **/
@protocol PHContentViewDelegate <NSObject>
@optional

/**
 * Content view was successfully displayed via show:
 *
 * @param contentView
 *   The contentView
 **/
- (void)contentViewDidShow:(PHContentView *)contentView;

/**
 * Content view finished loading it's contents (UIWebView loaded event)
 *
 * @param contentView
 *   The contentView
 **/
- (void)contentViewDidLoad:(PHContentView *)contentView;

/**
 * Content view dismissed successfully
 *
 * @param contentView
 *   The contentView
 **/
- (void)contentViewDidDismiss:(PHContentView *)contentView;

/**
 * Content view failed to display or load. The \c error object will be one of the PHError
 * types defined in PHConstants.h
 *
 * @param contentView
 *   The contentView
 *
 * @param error
 *   The error object
 **/
- (void)contentView:(PHContentView *)contentView didFailWithError:(NSError *)error;

/**
 * Customization delegate for changing the border color of content views with
 * a transition type of \c PH_DIALOG. Should return a UIColor instance
 *
 * @param contentView
 *   The contentView
 *
 * @return
 *   The desired border color
 **/
- (UIColor *)borderColorForContentView:(PHContentView *)contentView;
@end
