// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import "TBXML.h"

#define TJC_VIDEO_BUTTON_LABEL_KEY	@"TJC_VIDEO_BUTTON_LABEL_KEY"
#define TJC_VIDEO_BUTTON_URL_KEY		@"TJC_VIDEO_BUTTON_URL_KEY"


#define TJC_VIDEO_OBJ_VIDEO_URL			@"TJC_VIDEO_OBJ_VIDEO_URL"
#define TJC_VIDEO_OBJ_BUTTONS				@"TJC_VIDEO_OBJ_BUTTONS"
#define TJC_VIDEO_OBJ_CURRENCY_AMOUNT	@"TJC_VIDEO_OBJ_CURRENCY_AMOUNT"
#define TJC_VIDEO_OBJ_CURRENCY_NAME		@"TJC_VIDEO_OBJ_CURRENCY_NAME"
#define TJC_VIDEO_OBJ_OFFER_ID			@"TJC_VIDEO_OBJ_OFFER_ID"
#define TJC_VIDEO_OBJ_CLICK_URL			@"TJC_VIDEO_OBJ_CLICK_URL"
#define TJC_VIDEO_OBJ_DATA_LOCATION		@"TJC_VIDEO_OBJ_DATA_LOCATION"
#define TJC_VIDEO_OBJ_FILENAME			@"TJC_VIDEO_OBJ_FILENAME"
#define TJC_VIDEO_OBJ_NAME					@"TJC_VIDEO_OBJ_NAME"
#define TJC_VIDEO_OBJ_ICON_URL			@"TJC_VIDEO_OBJ_ICON_URL"

#define TJC_VIDEO_NO_ICON	@"NO_ICON"

@class TJCVideoView;

/*!	\interface TJCVideoObject
 *	\brief The Tapjoy Connect Video Object Class.
 *
 */
@interface TJCVideoObject : NSObject
{	
	NSString *videoURL_;				/*!< The URL to the video file. */
	NSMutableArray *buttons_;		/*!< The array of buttons for the video complete screen. */
	NSString *currencyAmount_;		/*!< The currency amount, displayed in the video complete screen. */
	NSString *currencyName_;		/*!< The currency name, displayed in the video complete screen. */
	NSString *videoAdName_;			/*!< The video Ad name, displayed in the video complete screen. */
	NSString *offerID_;				/*!< The offer ID of the video, also called video ID. */
	NSString *clickURL_;				/*!< The click URL for recording clicks for video views. */
	NSString *dataLocation_;		/*!< The file location of the video ad. */
	NSString *videoFilename_;		/*!< Used to be passed around the NSURLConnection delegate methods. */
	NSString *iconURL_;				/*!< The URL to the video ad icon. */
}


@property (copy) NSString *videoURL;
@property (nonatomic, retain) NSMutableArray *buttons;
@property (copy) NSString *currencyAmount;
@property (copy) NSString *currencyName;
@property (copy) NSString *videoAdName;
@property (copy) NSString *offerID;
@property (copy) NSString *clickURL;
@property (copy) NSString *dataLocation;
@property (copy) NSString *videoFilename;


- (id)initWithTBXML:(TBXMLElement*)aXMLElement;

- (NSDictionary*)getDictionary;

@end
