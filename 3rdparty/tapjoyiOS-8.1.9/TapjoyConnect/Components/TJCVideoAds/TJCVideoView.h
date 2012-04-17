// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>
#import "TJCVideoAdProtocol.h"
#import "TapjoyConnect.h"
#import "TJCVideoLayer.h"


@class TJCLoadingView;


/*!	\interface TJCVideoView
 *	\brief The Tapjoy Connect Video Ads View class.
 *
 */
@interface TJCVideoView : NSObject <UIAlertViewDelegate>
{
@private
	id<TJCVideoAdDelegate> adDelegate_;				/*!< The delegate that implements the TJCVideoAdProtocol. */
	BOOL isVideoReady_;									/*!< Set to YES only when a video is ready to be viewed. This prevents playing unloaded videos. */
	TJCLoadingView *loadingView_;						/*!< The loading view overlay. */
	BOOL isAlertShowing_;
	BOOL shouldShowStatusBar_;
@public
	IBOutlet UIView *mainView_;						/*!< The parent view for all videos. */
	IBOutlet TJCVideoLayer *mainVideoLayer_;		/*!< The main video object that contains the video to be initially loaded and viewed. */
	IBOutlet TJCVideoLayer *iPhoneLayer_;			/*!< iPhone view, set in the nib file specifically formatted for iPhone-sized screens. */
	IBOutlet TJCVideoLayer *iPadLayer_;				/*!< iPad view, set in the nib file specifically formatted for iPad-sized screens. */
}

@property (nonatomic, retain) UIView *mainView;
@property (assign) BOOL isVideoReady;


/*!	\fn bufferNextVideo
 *	\brief Takes the loaded video file and begins loading to into the MPMoviePlayerController in the active video layer.
 *
 *	\param path The path of the video to buffer.
 *	\param shouldStream Indicates whether or not a video should stream.
 *	\return n/a
 */
- (void)bufferVideoWithPath:(NSString*)path shouldStream:(BOOL)shouldStream;

/*!	\fn loadStateChanged:(NSNotification*)notification
 *	\brief Called after video begins buffering and is fired on loading status changes.
 *
 *	\param notification The MPMoviePlayerController object that just had its loading status changed.
 *	\return n/a
 */
- (void)loadStateChanged:(NSNotification*)notification;


- (void)refreshViewWithViewController:(UIViewController*)vController;

/*!	\fn refreshViewWithFrame:(CGRect)frame
 *	\brief Sets video view bounds with the give CGRect and makes sure that the video is always displayed in landscape mode.
 *
 *	\param frame The frame is used to set video ad related views and properties.
 *	\return n/a
 */
- (void)refreshViewWithFrame:(CGRect)frame;

/*!	\fn updateVideoBounds
 *	\brief Updates the video layer and other views to the window bounds.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)updateVideoBounds;

/*!	\fn refreshViewWithInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
 *	\brief Updates view elements depending on the given orientation. Makes sure that the video view is always rightside up, landscape.
 *
 *	\param interfaceOrientation The orientation.
 *	\return n/a
 */
- (void)refreshViewWithInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation;

/*!	\fn initVideoAdWithDelegate:(id<TJCVideoAdDelegate>)delegate
 *	\brief Initiates a request to start buffering a video ad.
 *
 *	\param delegate The class instance that implements the TJCVideoAdDelegate protocol.
 *	\return n/a
 */
- (id)initWithDelegate:(id<TJCVideoAdDelegate>)delegate;

/*!	\fn customButtonAction1
 *	\brief Loads a web page related to the video ad.
 *
 *	\param n/a
 *	\return n/a
 */
- (IBAction)customButtonAction1;

/*!	\fn customButtonAction1
 *	\brief Loads a web page related to the video ad.
 *
 *	\param n/a
 *	\return n/a
 */
- (IBAction)customButtonAction2;

/*!	\fn closeVideoView
 *	\brief Closes the video ad view with the set TJC transition setting
 *
 *	\param n/a
 *	\return n/a
 */
- (IBAction)closeVideoView;

/*!	\fn videoActionFromAppResume
 *	\brief This method is called when the application returns to the foreground.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)videoActionFromAppResume;

/*!	\fn cancelVideo:(id)sender
 *	\brief Cancels a video ad before it finishes playing. If canceling before first playthrough is done, no currency is awarded.
 *
 *	\param n/a
 *	\return n/a
 */
- (IBAction)cancelVideo:(id)sender;

/*!	\fn giveBackNotification
 *	\brief Removes the video view from the view stack.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)giveBackNotification;

/*!	\fn playActiveVideo
 *	\brief Plays the currently set video.
 *
 *	\param n/a
 *	\return n/a
 */
- (IBAction)playActiveVideo;

/*!	\fn playVideoWithOfferID:(NSString*)offerID
 *	\brief Plays the video with the given offer ID.
 *
 *	\param offerID The offer ID is used as the video filename. Plays the video with the given filename.
 *	\return n/a
 */
- (void)playVideoWithOfferID:(NSString*)offerID;

/*!	\fn stopActiveVideo
 *	\brief Stops playback of the currently set video.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)stopActiveVideo;

/*!	\fn videoAdCurrentlyPlaying
 *	\brief Returns play status of video ad.
 *
 *	\param n/a
 *	\return YES if a video ad is currently playing, NO otherwise.
 */
- (BOOL)videoAdCurrentlyPlaying;

/*!	\fn shouldDisplayLogo:(BOOL)display
 *	\brief Set the visibility of the Tapjoy logo during video playback.
 *
 *	\param display Boolean that indicates whether the Tapjoy logo should be displayed or not during video playback.
 *	\return n/a
 */
- (void)shouldDisplayLogo:(BOOL)display;

@end
