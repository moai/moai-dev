//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-iphone/MoaiUiWebView.h>

@implementation MoaiUiWebView
@synthesize mMOAIWebView, mWebView;

	//================================================================//
	#pragma mark -
	#pragma mark Protocol UIWebViewDelegate
	//================================================================//
	
	//----------------------------------------------------------------//
	- ( void )webView:( UIWebView* )webView webViewDidFailLoadWithError:( NSError * )error {
		
		printf("load fail delegate\n");
		mMOAIWebView->RaiseDidFailLoadWithErrorEvent ( error );
	}
	
	//----------------------------------------------------------------//
	- ( void ) webViewDidFinishLoad:( UIWebView * )webView {
	
	
		printf("load done delegate\n");
		mMOAIWebView->RaiseWebViewDidFinishLoadEvent ();
	}
	
	//----------------------------------------------------------------//
	- ( void ) webViewDidStartLoad:( UIWebView * )webView {
	
	
		printf("load start delegate\n");
		mMOAIWebView->RaiseWebViewDidStartLoadEvent ();
	}

	//----------------------------------------------------------------//
	- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType{
	
		printf("should start delegate\n");
		//mMOAIWebView.RaiseShouldStartLoadWithRequestEvent();
		return true;
	}
	
@end
