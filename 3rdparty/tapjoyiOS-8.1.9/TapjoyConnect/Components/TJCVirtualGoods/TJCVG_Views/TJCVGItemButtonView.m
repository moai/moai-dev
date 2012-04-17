// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVGItemButtonView.h"
#import "TJCVGConstants.h"
#import "TJCVGUtil.h"
#import "TJCViewCommons.h"


@implementation TJCVGItemButtonView

@synthesize itemNameLabel, itemTypeLabel, itemCostLabel, buttonImage;


- (id)initWithFrame:(CGRect)frame 
{
	self = [super initWithFrame:frame];
	if (self) 
	{
		// Initialization code.
	}
	return self;
}


- (void)reformatViewWithFrame:(CGRect)frame
{
	// Set some button view properties to make it look better.
	//float minDim = MIN(frame.size.width, frame.size.height);
	//self.frame = CGRectMake(frame.origin.x, frame.origin.y, minDim, minDim);
	self.frame = frame;
	
	// Set rounded corner, borders, and a shadow.
	self.layer.cornerRadius = TJCVG_BUTTON_CORNER_RADIUS;
	self.layer.borderWidth = TJCVG_BUTTON_BORDER_WIDTH;
	self.layer.borderColor = [[[TJCViewCommons sharedTJCViewCommons] getUserDefinedColor] CGColor];
	//self.layer.shadowOpacity = TJCVG_BUTTON_SHADOW_OPACITY;
	
	// Remove any previous gradient to prevent artifacts.
	for (int i = 0; i < [self.layer sublayers].count; ++i)
	{
		if ([[[self.layer sublayers] objectAtIndex:i] isKindOfClass:[CAGradientLayer class]])
		{
			[[[self.layer sublayers] objectAtIndex:i] removeFromSuperlayer];
		}
	}
	
	//UIColor *lighterColor = [TJCVGUtil getUserDefinedColorWithWhiteValue:TJCVG_COLOR_LIGHTEN_VALUE];
	UIColor *lighterColor1 = [TJCVGUtil getUserDefinedColorWithWhiteValue:TJCVG_COLOR_LIGHTEN_VALUE1];
	
	// Add a gradient to the background.
	CAGradientLayer *gradient = [CAGradientLayer layer];
	gradient.frame = frame;
	gradient.cornerRadius = TJCVG_BUTTON_CORNER_RADIUS;
	gradient.colors = [NSArray arrayWithObjects:
							 (id)[lighterColor1 CGColor], 
							 (id)[[UIColor whiteColor] CGColor], 
							 nil];
	[self.layer insertSublayer:gradient atIndex:0];
	
	// Now position the item image view.
	// Resize the image to the smallest dimenstion to keep it square.
	//float buttonMinDim = MIN(buttonImage.frame.size.width, buttonImage.frame.size.height);
	//buttonImage.frame = CGRectMake(buttonImage.frame.origin.x, buttonImage.frame.origin.y, buttonMinDim, buttonMinDim);
	
	//	buttonImage.layer.cornerRadius = TJCVG_BUTTON_CORNER_RADIUS;
	//	buttonImage.layer.borderWidth = TJCVG_BUTTON_BORDER_WIDTH;
	//	buttonImage.layer.borderColor = [UIColor lightGrayColor].CGColor;
	//	buttonImage.backgroundColor = [UIColor lightGrayColor];
}


- (void)requestThumbImageWithURL:(NSString*)thumbImageURL
{
	// If the thumb image hasn't yet been downloaded, so do now.
	if (buttonImage.image)
	{
		return;
	}
	
	// Set up the request to download the button thumb image.
	NSURL *thumbURL = [NSURL URLWithString:thumbImageURL];
	
	NSURLRequest *myRequest = [NSURLRequest requestWithURL:thumbURL];
	
	if (connection)
	{
		[connection release];
		connection = nil;
	}
	
	if (data)
	{
		[data release];
		data = nil;
	}
	
	connection = [[NSURLConnection alloc] initWithRequest:myRequest delegate:self];
	data = [[NSMutableData alloc] init];
}


- (void)setImageReadyInvocation:(NSInvocation*)invocation
{
	if (invoker)
	{
		[invoker release];
		invoker = nil;
	}
	
	invoker = [invocation retain];
}


- (void)dealloc 
{
	[itemNameLabel release];
	[itemTypeLabel release];
	[itemCostLabel release];
	[buttonImage release];
	[connection release];
	[data release];
	[invoker release];
	[super dealloc];
}




#pragma mark delegate methods for asynchronous requests

- (void)connection:(NSURLConnection*)myConnection didReceiveResponse:(NSURLResponse*)myResponse;
{
	NSHTTPURLResponse *HTTPResponse = (NSHTTPURLResponse*)myResponse;
	NSInteger statusCode = [HTTPResponse statusCode];
	
	// If we don't get a success status, initiate an immediate fail.
	if (statusCode != 200)
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR
						  format:@"Virtual Good thumb image request, bad status code: %d", statusCode];
	}
}


- (void)connection:(NSURLConnection*)myConnection didReceiveData:(NSData*)myData;
{   
	// Concatenate the image data.
	[data appendData:myData];	
}


- (void)connection:(NSURLConnection*)myConnection didFailWithError:(NSError*)myError;
{
	[connection release];
	connection = nil;
}


- (void)connectionDidFinishLoading:(NSURLConnection*)myConnection;
{
	[connection release];
	connection = nil;
	
	NSData *imageData = [NSData dataWithData:data];
	// Check for valid image data.
	if(imageData)
	{
		UIImage *dataIcoImage = [UIImage imageWithData:imageData];
		UIImage *tmpResizedImg = [TJCVGUtil scaleToSize:CGSizeMake(80.0f, 80.0f) withImage:dataIcoImage];
		
		buttonImage.image = [TJCVGUtil makeRoundCornerImage:tmpResizedImg
																			:TJCVG_BUTTON_CORNER_RADIUS
																			:TJCVG_BUTTON_CORNER_RADIUS];
		
		[buttonImage setAlpha:0.0f];
		[UIView beginAnimations:nil context:nil];
		[UIView setAnimationDuration:TJCVG_LOADING_VIEW_ALPHA];
		[buttonImage setAlpha:1.0f];
		[UIView commitAnimations];
	}
	
	// If this was previously set, run the invoker now. This should fire off any method that needed to be notified that the thumb image was just downloaded.
	if (invoker)
	{
		[invoker invoke];
		
		[invoker release];
		invoker = nil;
	}
	
	[data release];
	data = nil;
}


@end
