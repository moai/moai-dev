//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios/MOAIAppIOS.h>
#import <moai-ios/MOAIWebViewController.h>
#import <moai-ios/MOAIWebViewIOS.h>

#import <UIKit/UIKit.h>

#define TOOLBAR_HEIGHT	44
//#define ANIM_DURATION	0.5
//#define ANIM_DELAY		0.1

//================================================================//
// MOAIWebViewController ()
//================================================================//
@interface MOAIWebViewController () {
@private

	int					mToolbarHeight;
	BOOL				mAnimated;
	UIWebView*			mWebView;
	MOAIWebViewIOS*		mMOAIWebView;
}

	//----------------------------------------------------------------//
	-( void	)						doneButtonPressed;

@end

//================================================================//
// MOAIWebViewController
//================================================================//
@implementation MOAIWebViewController

	@synthesize animated			= mAnimated;
	@synthesize webView				= mWebView;
	@synthesize moaiWebView			= mMOAIWebView;

	//----------------------------------------------------------------//
	-( void ) dealloc {
	
		[ super dealloc ];
	
		if ( mWebView ) {
			mWebView.delegate = nil;
			[ mWebView stopLoading ];
			[ mWebView release ];
			mWebView = nil;
		}
	}

	//----------------------------------------------------------------//
	-( void	) doneButtonPressed {
	
		[ self hide:mAnimated ];
	}

	//----------------------------------------------------------------//
	-( void ) loadView {

		CGRect frame = [UIScreen mainScreen].bounds;
	
		UIView *mView = [[[ UIView alloc ] initWithFrame:frame] autorelease];
		[ mView setBackgroundColor:[ UIColor blueColor ]];
		[ mView setContentMode:UIViewContentModeScaleToFill ];
		[ mView setAutoresizingMask:( UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth )];
		
		UIToolbar* toolBar = [[[ UIToolbar alloc ] init ] autorelease ];
		[ toolBar setBarStyle:UIBarStyleDefault ];
		[ toolBar setFrame:CGRectMake ( 0, 0, frame.size.width, TOOLBAR_HEIGHT )];
		[ toolBar setAutoresizingMask:( UIViewAutoresizingFlexibleWidth )];
		
		UIBarButtonItem *done = [[[ UIBarButtonItem alloc ] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector ( doneButtonPressed )] autorelease ];
		[ toolBar setItems:[ NSArray arrayWithObjects: done, nil ] animated:NO ];
		
		[ mView addSubview:toolBar ];
		
		mWebView = [[ UIWebView alloc ] initWithFrame:CGRectMake ( 0, TOOLBAR_HEIGHT, frame.size.width, frame.size.height - TOOLBAR_HEIGHT )];

		[ mWebView setDelegate:self ];
		[ mWebView setScalesPageToFit:YES ];
		[ mWebView setMultipleTouchEnabled:YES ];
		[ mWebView setContentMode:UIViewContentModeScaleToFill ];
		[ mWebView setDataDetectorTypes:UIDataDetectorTypeNone];
		[ mWebView setAutoresizingMask:( UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth )];

		[ mView addSubview:mWebView ];
		self.view = mView;
	}

	//----------------------------------------------------------------//
	-( void ) show :( BOOL )animated {

		UIWindow *window = [[[UIApplication sharedApplication] windows] firstObject];
		[window.rootViewController presentViewController:self animated:animated completion:nil];

		mAnimated = animated;
	}

	//----------------------------------------------------------------//
	-( void ) hide :( BOOL )animated {
		
		[self dismissViewControllerAnimated:animated completion:^{
			 if ( mMOAIWebView ) {
				 mMOAIWebView->RaiseWebViewDidHideEvent ();
			 }
		}];

		mAnimated = animated;
	}

	-( BOOL ) prefersStatusBarHidden {
		return YES;
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
