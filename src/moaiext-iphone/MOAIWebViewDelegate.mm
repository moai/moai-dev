//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MOAIWebViewDelegate.h>

//================================================================//
// MOAIWebViewDelegate
//================================================================//
@implementation MOAIWebViewDelegate

	@synthesize mMOAIWebView;

	//Action methods for toolbar buttons:	
	- ( void ) doneButtonPressed:( id )sender {
		UNUSED ( sender );
	
		if ( mMOAIWebView ) {
			mMOAIWebView->Hide ();
		}
	}
	
	//================================================================//
	#pragma mark -
	#pragma mark Protocol UIWebViewDelegate
	//================================================================//
	
	//----------------------------------------------------------------//
	- ( void )webView:( UIWebView* )webView webViewDidFailLoadWithError:( NSError * )error {
		UNUSED ( webView );
	
		if ( mMOAIWebView ) {
			mMOAIWebView->RaiseDidFailLoadWithErrorEvent ( error );
		}
	}
	
	//----------------------------------------------------------------//
	- (BOOL)webView:( UIWebView* )webView shouldStartLoadWithRequest:( NSURLRequest* )request navigationType:( UIWebViewNavigationType )navigationType {
		UNUSED ( webView );
	
		if ( mMOAIWebView ) {
			bool result = mMOAIWebView->RaiseShouldStartLoadWithRequestEvent ( request, navigationType );
			return result;
		}
		return true;
	}
	
	//----------------------------------------------------------------//
	- ( void ) webViewDidFinishLoad:( UIWebView * )webView {
		UNUSED ( webView );
		
		if ( mMOAIWebView ) {
			mMOAIWebView->RaiseWebViewDidFinishLoadEvent ();
		}
	}
	
	//----------------------------------------------------------------//
	- ( void ) webViewDidStartLoad:( UIWebView * )webView {
		UNUSED ( webView );
	
		if ( mMOAIWebView ) {
			mMOAIWebView->RaiseWebViewDidStartLoadEvent ();
		}
	}
	
@end
