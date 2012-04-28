// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCUIWebPageView.h"
#import "TJCLog.h"
#import "TJCConstants.h"
#import "TapjoyConnect.h"
#import "TJCLoadingView.h"

@implementation TJCUIWebPageView

@synthesize lastURL = lastURL_, isViewVisible = isViewVisible_;


- (id)initWithFrame:(CGRect)frame
{
	if((self = [super initWithFrame:frame]))
	{
		cWebView_ = [[UIWebView alloc] initWithFrame:frame];
		
		[cWebView_ setAutoresizingMask:UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight];
		cWebView_.delegate = self;
		[self addSubview:cWebView_];

		// Init loading view.
		if (loadingView_)
		{
			[loadingView_ release];
		}
		loadingView_ = [[TJCLoadingView alloc] initWithFrame:frame];
		[self addSubview: loadingView_.mainView];
		// This will make the loading view not visible.
		[loadingView_ disable];
	}
	return self;
}


- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType 
{	
	// If we see either tapjoy or linkshare host names, we won't open it externally. All other host names will open externally from the app.
	if ((CFStringFind((CFStringRef)[[request URL] host], (CFStringRef)TJC_TAPJOY_HOST_NAME, kCFCompareCaseInsensitive).length > 0) ||
		 (CFStringFind((CFStringRef)[[request URL] host], (CFStringRef)TJC_TAPJOY_ALT_HOST_NAME, kCFCompareCaseInsensitive).length > 0) ||
		 (CFStringFind((CFStringRef)[[request URL] host], (CFStringRef)TJC_LINKSHARE_HOST_NAME, kCFCompareCaseInsensitive).length > 0))
	{
		return YES;
	}
	
	// Open the link externally.
	NSURLConnection *conn = [[NSURLConnection alloc] initWithRequest:[NSURLRequest requestWithURL:[request URL]] 
																			  delegate:self 
																	startImmediately:YES];
	[conn release];
	return NO;
}


- (NSString*)appendGenericParamsWithURL:(NSString*)theURL
{
	NSString *result = [NSString stringWithFormat:@"%@%@", 
							  theURL, 
							  [TapjoyConnect createQueryStringFromDict:[[TapjoyConnect sharedTapjoyConnect] genericParameters]]];
	
	return result;
}

-(id) initWithFrame:(CGRect)aFrame WithRequestURL:(NSURLRequest*) aRequest
{
	if((self = [self initWithFrame:aFrame]))
	{
		[cWebView_ loadRequest:aRequest];
		lastClickedURL_ = aRequest;
	}
	return self;
}


-(void)loadWebRequest:(NSURLRequest *)aRequest
{
	[cWebView_ loadRequest:aRequest];
	//lastClickedURL_ = [aRequest retain];
}


-(void)loadURLRequest:(NSURL*)requestURL
{
	NSURLRequest * myRequest = [NSURLRequest requestWithURL:requestURL];
	[cWebView_ loadRequest:myRequest];
	//lastClickedURL_ = [myRequest retain];
}


-(void)loadURLRequest:(NSURL*)requestURL withTimeOutInterval:(int)tInterval
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Request URL: %@",[requestURL absoluteString]];
	NSMutableURLRequest *myRequest = [NSMutableURLRequest requestWithURL:requestURL 
																				cachePolicy:NSURLRequestUseProtocolCachePolicy 
																		  timeoutInterval:tInterval];
	[cWebView_ loadRequest:myRequest];
	//lastClickedURL_ = [myRequest retain];
}


-(void) setDelegate:(id) aDelegate
{
	delegate_ = aDelegate;
}


- (void)webViewDidStartLoad:(UIWebView *)webView
{
	[loadingView_ fadeIn];
}


- (void)webViewDidFinishLoad:(UIWebView *)webView
{
	[loadingView_ fadeOut];
	
	[webView stringByEvaluatingJavaScriptFromString:@"document.body.setAttribute('orientation', 90);"];
}


- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
	if (error.code == NSURLErrorCancelled) return; //error 999 fast clicked the links
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"ERROR TEXT IS %@",[error description]];
	
	// sum bug solved in 4.0 need to confirm
	if (error.code == 102) 
		return;
	
	if (isViewVisible_)
	{
		UIAlertView * alertview = [[UIAlertView alloc] initWithTitle:@"" 
																			  message:TJC_GENERIC_CONNECTION_ERROR_MESSAGE 
																			 delegate:self 
																 cancelButtonTitle:@"Cancel" 
																 otherButtonTitles:@"Retry", nil];
		[alertview show];
		[alertview release];
	}
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"TJC CUSTOM WEB Page View Alert Click Called (Net Error Case)"];
	
	if(buttonIndex == 1)
	{
		[cWebView_ stopLoading];
		[cWebView_ loadRequest:lastClickedURL_]; 
	}
}


-(void) dealloc
{
	delegate_ = nil; // set delegate to nil; 
	//stop the web view 
	[cWebView_ release];
	[lastURL_ release];
	[loadingView_ release];
	[super dealloc];
}


@end
