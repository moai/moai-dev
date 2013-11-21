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

	//Action methods for toolbar buttons:	
	- ( void ) doneButtonPressed:( id )sender {
		UNUSED ( sender );
	
		if ( _mMOAIWebView ) {
			_mMOAIWebView->Hide ();
		}
	}
	
	//================================================================//
	#pragma mark -
	#pragma mark Protocol UIWebViewDelegate
	//================================================================//
	
	//----------------------------------------------------------------//
	- ( void )webView:( UIWebView* )webView webViewDidFailLoadWithError:( NSError * )error {
		UNUSED ( webView );
	
		if ( _mMOAIWebView ) {
			_mMOAIWebView->RaiseDidFailLoadWithErrorEvent ( error );
		}
	}
	
	//----------------------------------------------------------------//
	- (BOOL)webView:( UIWebView* )webView shouldStartLoadWithRequest:( NSURLRequest* )request navigationType:( UIWebViewNavigationType )navigationType {
		UNUSED ( webView );
	
		if ( _mMOAIWebView ) {
			bool result = _mMOAIWebView->RaiseShouldStartLoadWithRequestEvent ( request, navigationType );
			return result;
		}
		return true;
	}
	
	//----------------------------------------------------------------//
	- ( void ) webViewDidFinishLoad:( UIWebView * )webView {
		UNUSED ( webView );
		
		if ( _mMOAIWebView ) {
			_mMOAIWebView->RaiseWebViewDidFinishLoadEvent ();
		}
	}
	
	//----------------------------------------------------------------//
	- ( void ) webViewDidStartLoad:( UIWebView * )webView {
		UNUSED ( webView );
	
		if ( _mMOAIWebView ) {
			_mMOAIWebView->RaiseWebViewDidStartLoadEvent ();
		}
	}
	
@end
