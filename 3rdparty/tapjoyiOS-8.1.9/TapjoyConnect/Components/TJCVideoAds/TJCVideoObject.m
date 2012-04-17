// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCVideoObject.h"
#import "TJCLog.h"
#import "TJCVideoView.h"
#import "TJCVideoManager.h"


@implementation TJCVideoObject

@synthesize videoURL = videoURL_, 
buttons = buttons_, 
currencyAmount = currencyAmount_,
currencyName = currencyName_, 
videoAdName = videoAdName_, 
offerID = offerID_, 
clickURL = clickURL_, 
dataLocation = dataLocation_,
videoFilename = videoFilename_;


- (id)initWithTBXML:(TBXMLElement*)aXMLElement
{
	self = [super init];
	
	if (self)
	{
		if (!aXMLElement) 
		{
			return self;
		}
		
		if (buttons_)
		{
			[buttons_ release];
		}
		buttons_ = [[NSMutableArray alloc] init];
		
		// Get button data.
		TBXMLElement *buttonElements = [TBXML childElementNamed:@"Buttons" parentElement:aXMLElement];
		TBXMLElement *buttonElement = [TBXML childElementNamed:@"Button" parentElement:buttonElements];
		
		while (buttonElement)
		{
			NSString *buttonLabel = [TBXML textForElement:[TBXML childElementNamed:@"Name" parentElement:buttonElement]];
			NSString *buttonURL = [TBXML textForElement:[TBXML childElementNamed:@"URL" parentElement:buttonElement]];
			
			// TBXML fucks up encoding so & comes in as &amp;. Fix that here.
			NSString *fixedButtonURL = [buttonURL stringByReplacingOccurrencesOfString:@"&amp;" withString:@"&"];
			
			NSMutableDictionary *buttonDict = [[NSMutableDictionary alloc] init];
			[buttonDict setObject:buttonLabel forKey:TJC_VIDEO_BUTTON_LABEL_KEY];
			[buttonDict setObject:fixedButtonURL forKey:TJC_VIDEO_BUTTON_URL_KEY];
			[buttons_ addObject:buttonDict];
			[buttonDict release];
			
			buttonElement = [TBXML nextSiblingNamed:@"Button" searchFromElement:buttonElement];
		}
		
		videoURL_ = [[TBXML textForElement:[TBXML childElementNamed:@"VideoURL" parentElement:aXMLElement]] copy];
		currencyAmount_ = [[TBXML textForElement:[TBXML childElementNamed:@"Amount" parentElement:aXMLElement]] copy];
		offerID_ = [[TBXML textForElement:[TBXML childElementNamed:@"OfferID" parentElement:aXMLElement]] copy];
		clickURL_ = [[TBXML textForElement:[TBXML childElementNamed:@"ClickURL" parentElement:aXMLElement]] copy];
		videoAdName_ = [[TBXML textForElement:[TBXML childElementNamed:@"Name" parentElement:aXMLElement]] copy];
		NSArray *parts = [videoURL_ componentsSeparatedByString:@"/"];;
		videoFilename_ = [[parts lastObject] copy];
		currencyName_ = [[TBXML textForElement:[TBXML childElementNamed:@"CurrencyName" parentElement:aXMLElement]] copy];
		iconURL_ = [[TBXML textForElement:[TBXML childElementNamed:@"IconURL" parentElement:aXMLElement]] copy];
		if ([iconURL_ isEqualToString:@""] || ([iconURL_ length] <= 0))
		{
			iconURL_ = [[NSString alloc] initWithString:TJC_VIDEO_NO_ICON];
		}
		
		// Set this to nil so we can use this as a check to see if a video has finished caching.
		dataLocation_ = nil;
	}
	
	return self;
}


- (NSDictionary*)getDictionary
{
	NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
	
	[dict setObject:videoURL_ forKey:TJC_VIDEO_OBJ_VIDEO_URL];
	[dict setObject:buttons_ forKey:TJC_VIDEO_OBJ_BUTTONS];
	[dict setObject:currencyAmount_ forKey:TJC_VIDEO_OBJ_CURRENCY_AMOUNT];
	[dict setObject:currencyName_ forKey:TJC_VIDEO_OBJ_CURRENCY_NAME];
	[dict setObject:offerID_ forKey:TJC_VIDEO_OBJ_OFFER_ID];
	[dict setObject:clickURL_ forKey:TJC_VIDEO_OBJ_CLICK_URL];
	[dict setObject:videoFilename_ forKey:TJC_VIDEO_OBJ_FILENAME];
	[dict setObject:videoAdName_ forKey:TJC_VIDEO_OBJ_NAME];
	[dict setObject:iconURL_ forKey:TJC_VIDEO_OBJ_ICON_URL];
	
	// These are only set after the video is actually cached.
	if (dataLocation_)
	{
		[dict setObject:dataLocation_ forKey:TJC_VIDEO_OBJ_DATA_LOCATION];
	}

	return [dict autorelease];
}


#pragma mark -
#pragma mark Video Ad Downloading Methods


- (void)dealloc
{
	[videoURL_ release];
	[buttons_ release];
	[currencyAmount_ release];
	[currencyName_ release];
	[videoAdName_ release];
	[offerID_ release];
	[clickURL_ release];
	[dataLocation_ release];
	[videoFilename_ release];
	[iconURL_ release];
	[super dealloc];
}


@end
