// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>
#import <QuartzCore/QuartzCore.h>
#import "TJCVideoAdProtocol.h"


#define TJC_VIDEO_STATUS_TEXT_FADE_DURATION	(0.3f)

#define TJC_VIDEO_CORNER_RADIUS					(5.0f)
#define TJC_VIDEO_BORDER_WIDTH					(0.5f)
#define TJC_VIDEO_BUTTON_STROKE_COLOR			(0x494949FF)
#define TJC_VIDEO_FONT_TYPE				@"\"helvetica\""
#define TJC_VIDEO_FONT_COLOR				@"\"black\""
#define TJC_VIDEO_FONT_SIZE				@"\"3\""
#define TJC_VIDEO_CURRENCY_TEXT_COLOR	@"#004194"
#define TJC_VIDEO_AD_TEXT_FONT_SIZE		@"\"2\""
#define TJC_VIDEO_AD_TEXT_FONT_COLOR	@"#909090"

#define TJC_VIDEO_CORNER_RADIUS_IPAD		(10.0f)
#define TJC_VIDEO_BORDER_WIDTH_IPAD			(1.0f)
#define TJC_VIDEO_FONT_SIZE_IPAD				@"\"6\""
#define TJC_VIDEO_AD_TEXT_FONT_SIZE_IPAD	@"\"5\""


/*!	\interface TJCVideoLayer
 *	\brief The Tapjoy Connect Video Layer class.
 *
 */
@interface TJCVideoLayer : UIView
{
@private
	IBOutlet UILabel *statusLabel_;			/*!< Info label on the video that indicates time left. */
	IBOutlet UIView *videoCompleteView_;	/*!< The view that contains all elements in the video complete view. */
	IBOutlet UIButton *replayButton_;		/*!< Replays the video from the beginning. */
	IBOutlet UIButton *customButton1_;		/*!< Custom button 1. */
	IBOutlet UIButton *customButton2_;		/*!< Custom button 2. */
	IBOutlet UIButton *backToVideoButton_;	/*!< Goes back to the video. */
	IBOutlet UIButton *backButton_;			/*!< Allows exiting of the video view. This should not be enabled until the video ad has finished playing. */
	IBOutlet UILabel *buttonDescriptionLabel_;	/*!< The label above the buttons. */
	IBOutlet UILabel *detailsLabel_;			/*!< The label above the details view. */
	IBOutlet UIView *messageView_;			/*!< The message view contains an icon image, and a description. */
	IBOutlet UIWebView *currencyLabelWebView_;	/*!< Contains the currency message. */
	IBOutlet UIImageView *iconImageView_;	/*!< The video icon in the message view. */
	IBOutlet UINavigationBar *navBar_;		/*!< The navigation bar. */
	IBOutlet UINavigationItem *navItem_;	/*!< The navigation item within the nav bar. */
	IBOutlet UILabel *navBarLabel_;			/*!< The label on top of the nav bar. */
	IBOutlet UIButton *doneButton_;			/*!< The button on the upper right hand corner that dismmisses the complete screen. */
	IBOutlet UIImageView *tapjoyLogo_;		/*!< The logo that is displayed during video ad playback. */
	IBOutlet UIButton *closeButton_;			/*!< The close button that can cancel a video, or go back to the complete screen if the video has already been viewed once. */
	NSTimeInterval duration_;					/*!< The duration of the video currently playing. */
	id<TJCVideoAdDelegate> delegate_;		/*!< The delegate that implements the TJCVideoAdProtocol. */
	NSArray *buttons_;							/*!< Array that holds the custom buttons. Used for convenience. */
	NSMutableArray *buttonURLs_;				/*!< Array that holds the redirect URLs for the custom buttons. */
@public
	NSString *fileName_;							/*!< The name of the video file saved locally on this device. */
	NSString *linkURLString_;					/*!< The redirect URL associated with this video file. */
	MPMoviePlayerController *videoFeed_;	/*!< The video controller object for loading and managing playback of videos. */
	UIWebView *webView_;							/*!< Used for loading web content when the redirect button is pressed. */
	BOOL isVideoPlaying_;						/*!< Video ad play status. */
	NSString *offerID_;							/*!< The offer Id for the video currently being played. */
	BOOL didIconDownload_;						/*!< Flag that indicates whether the video ad icon was downloaded. */
	BOOL didLogoDownload_;						/*!< Flag that indicates whether the video ad logo was downloaded. */
	BOOL isFinishedWatching_;					/*!< Flag that indicates whether a video has finished playing once through. */
	BOOL shouldDisplayLogo_;					/*!< Set to YES if the Tapjoy logo should be displayed, NO otherwise. */
}

@property (nonatomic, retain) IBOutlet UIButton *closeButton;
@property (copy) NSString *fileName;
@property (copy) NSString *linkURLString;
@property (nonatomic, retain) MPMoviePlayerController *videoFeed;
@property (assign) BOOL isVideoPlaying;
@property (copy) NSString *offerID;
@property (assign) BOOL isFinishedWatching;
@property (assign) BOOL shouldDisplayLogo;

/*!	\fn getDescriptionWithAmountString
 *	\brief Returns an html formatted string for the video complete description.
 *
 *	\param amountString The string that displays currency amount.
 *	\param currencyName The string that displays currency name.
 *	\param adText The string that displays the ad description.
 *	\return n/a
 */
- (NSString*)getDescriptionWithAmountString:(NSString*)amountString currencyName:(NSString*)currencyName adText:(NSString*)adText;

/*!	\fn setVideoView:(UIView*)view
 *	\brief Sets video view properties with the given UIView.
 *
 *	\param view The video frame and other view properties conform to the properties of this view.
 *	\return n/a
 */
- (void)setVideoView:(UIView*)view;

- (void)refreshViewWithBounds:(CGRect)bounds;

/*!	\fn prepareVideoWithDelegate((id<TJCVideoAdDelegate>) delegate, NSString* URLString)
 *	\brief Sets the video protocol delegate and the video URL with which to load the video file.
 *
 *	\param delegate The delegate that responds to TJCVideoAdProtocol methods.
 *	\param URLString The URL of the video file.
 *	\return n/a
 */
- (void)prepareVideoWithDelegate:(id<TJCVideoAdDelegate>)delegate videoURL:(NSString*)URLString shouldStream:(BOOL)shouldStream;

/*!	\fn transitionToWebView
 *	\brief Transitions the main view of this video layer from the video ad view to the web view.
 *
 *	\param n/a
 *	\return n/a
 */
- (IBAction)transitionToWebView;

/*!	\fn transitionToVideoView
 *	\brief Transitions the main view of this video layer from the web view to the video ad view.
 *
 *	\param n/a
 *	\return n/a
 */
- (IBAction)transitionToVideoView;

/*!	\fn enableShowBackToVideoButton:(BOOL)enable
 *	\brief Enable the back-to-video button.
 *
 *	This button swaps the video ad view back in from the web view.
 *	\param enable YES to show the button, NO to hide it.
 *	\return n/a
 */
- (void)enableShowBackToVideoButton:(BOOL)enable;

/*!	\fn enableShowVideoCompleteScreen:(BOOL)enable
 *	\brief Enables the video complete screen.
 *
 *	\param enable YES to show the video complete screen, NO to hide it.
 *	\return n/a
 */
- (void)enableShowVideoCompleteScreen:(BOOL)enable;

/*!	\fn enableShowStatusLabel:(BOOL)enable
 *	\brief Enables the status label, displaying video ad time left. Also toggles Tapjoy logo.
 *
 *	\param enable YES to show the status label, NO to hide it.
 *	\return n/a
 */
- (void)enableShowStatusLabel:(BOOL)enable;

/*!	\fn movieDurationAvailable:(id)sender
 *	\brief This method is fired when the video duration is available.
 *
 *	\param sender The object that fires this method.
 *	\return n/a
 */
- (void)movieDurationAvailable:(id)sender;

/*!	\fn movieDidFinishWatching:(id)sender
 *	\brief This method is fired when the video has finished playing.
 *
 *	\param sender The object that fires this method.
 *	\return n/a
 */
- (void)movieDidFinishWatching:(NSNotification*)notification;

/*!	\fn loadWebViewWithURL:(NSString*)URLString
 *	\brief Load the given URL in the web view.
 *
 *	\param URLString The URL to load in the web view.
 *	\return n/a
 */
- (void)loadWebViewWithURL:(NSString*)URLString;

/*!	\fn playVideo
 *	\brief Initiates playback of the video ad.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)playVideo;

/*!	\fn stopVideo
 *	\brief Stops playback of the video ad.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)stopVideo;

/*!	\fn pauseVideo
 *	\brief Pauses playback of the video ad.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)pauseVideo;

/*!	\fn resumeVideo
 *	\brief Resumes playback of the video ad from a pause state.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)resumeVideo;

/*!	\fn cancelVideo
 *	\brief Cancels playback of the video ad and shows the video complete screen.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)cancelVideo;

/*!	\fn cleanupVideo
 *	\brief Performs cleanup operations of video ad related properties.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)cleanupVideo;

/*!	\fn buttonAction1
 *	\brief Button action method for the custom button. Redirects to somewhere, set by the video offer.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)buttonAction1;

/*!	\fn buttonAction2
 *	\brief Button action method for the custom button. Redirects to somewhere, set by the video offer.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)buttonAction2;

/*!	\fn loadIconImage
 *	\brief Loads the video ad icon.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)loadIconImage;

/*!	\fn loadLogoImage
 *	\brief Loads the video ad logo.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)loadLogoImage;

/*!	\fn displayIcon:(UIImage*)iconImage
 *	\brief Sets and displays the video ad icon.
 *
 *	\param iconImage The image to display.
 *	\return n/a
 */
- (void)displayIcon:(UIImage*)iconImage;

/*!	\fn displayLogo:(UIImage*)logoImage
 *	\brief Sets and displays the Tapjoy logo during video playback.
 *
 *	\param logoImage The image to display.
 *	\return n/a
 */
- (void)displayLogo:(UIImage*)logoImage;

/*!	\fn enableLogo:(BOOL)enable
 *	\brief Set whether the Tapjoy logo should be displayed during playback.
 *
 *	\param enable Boolean indicating whether the Tapjoy logo should be displayed during playback.
 *	\return n/a
 */
- (void)enableLogo:(BOOL)enable;

@end
