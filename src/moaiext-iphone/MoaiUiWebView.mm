//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MoaiUiWebView.h>

//================================================================//
// MoaiUiWebViewDelegate
//================================================================//
@implementation MoaiUiWebViewDelegate

	@synthesize mMOAIWebView;

	//Action methods for toolbar buttons:	
	- ( void ) doneButtonPressed:( id )sender {
	
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
	
		if ( mMOAIWebView ) {
			mMOAIWebView->RaiseDidFailLoadWithErrorEvent ( error );
		}
	}
	
	//----------------------------------------------------------------//
	- (BOOL)webView:( UIWebView* )webView shouldStartLoadWithRequest:( NSURLRequest* )request navigationType:( UIWebViewNavigationType )navigationType {
	
		if ( mMOAIWebView ) {
		
			bool result = mMOAIWebView->RaiseShouldStartLoadWithRequestEvent ( request, navigationType );
			return result;
		}
		return true;
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
	
@end
