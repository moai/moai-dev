//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MoaiUiWebView.h>

@implementation MoaiUiWebViewDelegate
@synthesize mMOAIWebView;


	//================================================================//
	#pragma mark -
	#pragma mark Protocol UIWebViewDelegate
	//================================================================//
	
	//----------------------------------------------------------------//
	- ( void )webView:( UIWebView* )webView webViewDidFailLoadWithError:( NSError * )error {
	
		if ( mMOAIWebView ) {
			mMOAIWebView->RaiseDidFailLoadWithErrorEvent ( error );
		}
	}
	
	//----------------------------------------------------------------//
	- ( void ) webViewDidFinishLoad:( UIWebView * )webView {
		
		if ( mMOAIWebView ) {
			mMOAIWebView->RaiseWebViewDidFinishLoadEvent ();
		}
	}
	
	//----------------------------------------------------------------//
	- ( void ) webViewDidStartLoad:( UIWebView * )webView {
	
		if ( mMOAIWebView ) {
			mMOAIWebView->RaiseWebViewDidStartLoadEvent ();
		}
	}

	//----------------------------------------------------------------//
	- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType{
	
		if ( mMOAIWebView ) {
		
			bool result = mMOAIWebView->RaiseShouldStartLoadWithRequestEvent ( request, navigationType );
			return result;
		}
		return true;
	}
	
@end
