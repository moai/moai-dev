// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCFeaturedAppView.h"
#import "TapjoyConnect.h"
#import "TJCConstants.h"
#import "TJCLog.h"
#import "TJCOffersViewHandler.h"
#import "TJCLoadingView.h"
#import "SynthesizeSingleton.h"

static NSString *currentServiceURL = nil;

@implementation TJCFeaturedAppView

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCFeaturedAppView)

@synthesize currencyID_, publisherUserID_;


- (id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	
	if (self)
	{
		[self refreshWithFrame:frame];
		[loadingView_ fadeIn];
		
		// Initialize the back button and add it to the view.
		UIButton *backButton = [UIButton buttonWithType:UIButtonTypeCustom];
		UIImage *backBtnImg = [UIImage imageNamed:@"close_button.png"];
		UIImageView *backBtnImgView = [[UIImageView alloc] initWithImage:backBtnImg];
        
        // JC: NOTE: The coords here are not 0, 0 since that places the button in the center of the super view for some reason.
		backButton.frame = CGRectMake(-.01f, -.01f,
                                      backBtnImgView.frame.size.width, 
                                      backBtnImgView.frame.size.height);

		[backButton addTarget:self action:@selector(backtoGameAction:) forControlEvents:UIControlEventTouchUpInside];
		[backButton setBackgroundColor:[UIColor clearColor]];
		[backButton setImage:backBtnImg forState:UIControlStateNormal];
		[backButton setAutoresizingMask:
		 UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleLeftMargin |
		 UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin];
		
		[self addSubview:backButton];
		
		[backBtnImgView release];
	}
	
	return self;
}


- (void)refreshWithFrame:(CGRect)frame
{
	[self setFrame:frame];
	[loadingView_.mainView setFrame:frame];
	
	currentServiceURL = [self setUpFeaturedAdURLWithServiceURL:
                         [NSString stringWithFormat:@"%@%@?", TJC_SERVICE_URL, TJC_FEATURED_FULLSCREEN_URL_NAME]];
	
	// Touch is not yet enabled for the webview and will not be until the page has loaded.
	[cWebView_ setUserInteractionEnabled:NO];
	[cWebView_ setScalesPageToFit:YES];
	[cWebView_ setAutoresizesSubviews: YES];
	
	self.layer.borderColor = [UIColor darkGrayColor].CGColor;
	self.layer.borderWidth = FULLSCREEN_AD_BORDER_SIZE;
	
	[self setAutoresizingMask:UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth];
}


- (NSString*)setUpFeaturedAdURLWithServiceURL:(NSString*)serviceURL
{
	NSString *result = [self appendGenericParamsWithURL:serviceURL];
	
	if (publisherUserID_)
	{
		result = [NSString stringWithFormat:
					 @"%@&%@=%@",
					 result,
					 TJC_URL_PARAM_USER_ID,
					 publisherUserID_];
	}
	
	return result;
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	// The retry button action.
	if(buttonIndex == 1)
	{
		[loadingView_ fadeIn];
		[cWebView_ stopLoading];
		
		// Load the last url that was attempted.
		if (!lastURL_)
		{
			currentServiceURL = [self setUpFeaturedAdURLWithServiceURL:
                                 [NSString stringWithFormat:@"%@%@?", TJC_SERVICE_URL, TJC_FEATURED_FULLSCREEN_URL_NAME]];
		}
		else
		{
			// Check for valid url.
			if ([NSURL URLWithString:lastURL_])
			{
				// Valid URL, proceed.
				currentServiceURL = [NSString stringWithString:lastURL_];
				//NSLog(@"Retry URL: %@", lastURL_);
			}
			else
			{
				// Invalid URL, use default one.
				currentServiceURL = [self setUpFeaturedAdURLWithServiceURL:
                                     [NSString stringWithFormat:@"%@%@?", TJC_SERVICE_URL, TJC_FEATURED_FULLSCREEN_URL_NAME]];
			}
			
			[lastURL_ release];
			lastURL_ = nil;
		}
		
		[self loadViewWithURL:currentServiceURL];
	}
	// The cancel button action.
	else if(buttonIndex == 0) 
	{
		//Stop Activity Indicator
		[loadingView_ fadeOut];	
		
		// Re-enable user touch interaction so that they may click on another link to try again if so desired.
		[cWebView_ setUserInteractionEnabled:YES];
		
		//re-intitialize webview
		//[self handleWebViewUI];
	}
}


- (void) loadViewWithURL:(NSString*)theURL
{
	[self setBackgroundColor:[UIColor whiteColor]];
	
	//initialize WebView UI
	[self initializeWebViewUI];
	
	[self loadURLRequest:[NSURL URLWithString:theURL] withTimeOutInterval:TJC_REQUEST_TIME_OUT];
}





#pragma mark OverRidden Connection Methods

- (void) webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
	if (error.code == NSURLErrorCancelled) 
	{
		return; //error 999 fast clicked the links
	}
	
	// 102 error code for slow request completion. This happens when a NSURLConnection is made for opening links externally in Safari.
	if(error.code == 102) 
	{
		return;
	}
	
	// Try alternate URL.
	if(currentServiceURL == [NSString stringWithFormat:@"%@%@?", TJC_SERVICE_URL, TJC_FEATURED_FULLSCREEN_URL_NAME])
	{
		currentServiceURL = [self setUpFeaturedAdURLWithServiceURL:
                             [NSString stringWithFormat:@"%@%@?", TJC_SERVICE_URL_ALTERNATE, TJC_FEATURED_FULLSCREEN_URL_NAME]];
		[self loadViewWithURL:currentServiceURL];
	}
	else
	{
		UIAlertView * alertview = [[UIAlertView alloc] initWithTitle:@"" 
                                                             message:TJC_GENERIC_CONNECTION_ERROR_MESSAGE 
                                                            delegate:self 
                                                   cancelButtonTitle:@"Cancel" 
                                                   otherButtonTitles:@"Retry", nil];
		[alertview show];
		[alertview release];
		
		//Stop Activity Indicator
		[loadingView_ fadeOut];
		
		// Disable user touch interaction.
		[webView setUserInteractionEnabled:NO];
	}
}


- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
	//NSLog(@"URL host: %@", [[request URL] host]);
	
	if (!request)
	{
		return NO;
	}
	
	NSString *requestString = [[request URL] absoluteString];
	
	if (requestString == nil || (CFStringFind((CFStringRef)requestString, (CFStringRef)@"dismiss", kCFCompareCaseInsensitive).length > 0))
	{
		[self backtoGameAction:nil];
		return NO;
	}
	else if (CFStringFind((CFStringRef)requestString, (CFStringRef)@"showOffers", kCFCompareCaseInsensitive).length > 0)
	{
		// If currency ID is set, the load the offer wall with that ID.
		if (currencyID_)
		{
			[self addSubview:[TJCOffersViewHandler showOffersWithCurrencyID:currencyID_ withFrame:[self frame] withCurrencySelector:NO]];
		}
		else
		{
			[self addSubview:[TJCOffersViewHandler showOffersWithFrame:[self frame]]];
		}
		return NO;
	}
	// If we see either tapjoy or linkshare host names, we won't open it externally. All other host names will open externally from the app.
	else if ((CFStringFind((CFStringRef)[[request URL] host], (CFStringRef)TJC_TAPJOY_HOST_NAME, kCFCompareCaseInsensitive).length > 0) ||
             (CFStringFind((CFStringRef)[[request URL] host], (CFStringRef)TJC_TAPJOY_ALT_HOST_NAME, kCFCompareCaseInsensitive).length > 0) ||
             (CFStringFind((CFStringRef)[[request URL] host], (CFStringRef)TJC_LINKSHARE_HOST_NAME, kCFCompareCaseInsensitive).length > 0))
	{
		if (lastURL_)
		{
			[lastURL_ release];
			lastURL_ = nil;
		}
		
		// Save the address in case data connection craps out and we need to reload.
		lastURL_ = [[NSString alloc] initWithString:[[request URL] absoluteString]];
		
		return YES;
	}
	
	// Open the link externally.
	NSURLConnection *conn = [[NSURLConnection alloc] initWithRequest:[NSURLRequest requestWithURL:[request URL]] 
                                                            delegate:self 
                                                    startImmediately:YES];
	[conn release];
	return NO;
}


- (void)webViewDidStartLoad:(UIWebView *)webView
{
	[loadingView_ fadeIn];
	
	// Disable user touch interaction.
	[webView setUserInteractionEnabled:NO];
}


- (void)webViewDidFinishLoad:(UIWebView *)webView
{
	webPageRendered_ = true;
	
	[loadingView_ fadeOut];
	
	// Re-enable user touch interaction.
	[webView setUserInteractionEnabled:YES];
}


- (NSURLRequest*)connection:(NSURLConnection*)connection 
            willSendRequest:(NSURLRequest*)request 
           redirectResponse:(NSURLResponse*)response
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"OPENING EXTERNAL URL NOW ::::::%@", [request URL]];
	
	[loadingView_ fadeOut];
	
	// Before we go to itunes, remove the offers view.
	[self removeFromSuperview];
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VIEW_CLOSED_NOTIFICATION object:nil];
	
	// JC: NOTE: Deprecated since 8.1.0
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_SHOW_BOX_CLOSE_NOTIFICATION object:nil];
	
	// Open up itunes. This will effectively place this app in the background.
	[[UIApplication sharedApplication] openURL:[request URL]];
	
	// Immediately cancel redirects since we only care about the first one.
	[connection cancel];
	
	// Returning nil will also ensure that we don't follow the redirects within the webview.
	// We rely on mobile safari to do so.
	return nil;
}


- (void)connectionDidFinishLoading:(NSURLConnection*)connection
{
	
}


- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	UIAlertView * alertview = [[UIAlertView alloc] initWithTitle:@"" 
                                                         message:TJC_GENERIC_CONNECTION_ERROR_MESSAGE
                                                        delegate:self 
                                               cancelButtonTitle:@"Cancel" 
                                               otherButtonTitles:@"Retry", nil];
	[alertview show];
	[alertview release];
	
	//Stop Activity Indicator
	[loadingView_ fadeOut];
	
	// Disable user touch interaction.
	[cWebView_ setUserInteractionEnabled:NO];
}


-(void) backtoGameAction:(id) sender
{
	[TJCViewCommons animateTJCView:self 
                 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons]getReverseTransitionEffect] 
                         withDelay:[[TJCViewCommons sharedTJCViewCommons]getTransitionDelay]];
	
	[self performSelector:@selector(giveBackNotification)
               withObject:nil 
               afterDelay:[[TJCViewCommons sharedTJCViewCommons]getTransitionDelay]];
}


-(void) giveBackNotification
{	
	[self removeFromSuperview];
	
	// Remove any transforms applied to this view.
	self.transform = CGAffineTransformIdentity;
	
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VIEW_CLOSED_NOTIFICATION object:nil];
	
	// JC: NOTE: Deprecated since 8.1.0
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_SHOW_BOX_CLOSE_NOTIFICATION object:nil];
}


- (void) updateViewWithOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	[cWebView_ stringByEvaluatingJavaScriptFromString:@"window.onorientationchange();"];
}



#pragma mark UIHandling

-(void) handleWebViewUI
{
	[self initializeWebViewUI];
	[[self class]cancelPreviousPerformRequestsWithTarget:self];
}


- (void) initializeWebViewUI
{
	if(webPageRendered_) 
	{
		return; 
	}
}


- (void)dealloc 
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"TJCOffersView Dealloc"];
	[super dealloc];
}


@end
