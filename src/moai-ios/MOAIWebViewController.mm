//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios/MOAIAppIOS.h>
#import <moai-ios/MOAIWebViewController.h>
#import <moai-ios/MOAIWebViewIOS.h>

//================================================================//
// MOAIWebViewController ()
//================================================================//
@interface MOAIWebViewController () {
@private

	int					mToolbarHeight;

	UIView*				mContainer;
	UIWebView*			mWebView;
	
	MOAIWebViewIOS*		mMOAIWebView;
}

	//----------------------------------------------------------------//
	-( void )						close;
	-( void	)						doneButtonPressed		:( id )sender;
	-( void )						hide;
	-( MOAIWebViewController* )		init;
	-( void )						show					:( BOOL )animated;

@end

//================================================================//
// MOAIWebViewController
//================================================================//
@implementation MOAIWebViewController

	@synthesize webView				= mWebView;
	@synthesize moaiWebView			= mMOAIWebView;

	//----------------------------------------------------------------//
	-( void ) close {
	
		if ( mWebView ) {
			mWebView.delegate = nil;
			[ mWebView stopLoading ];
			[ mWebView release ];
			mWebView = nil;
		}
		
		if ( mContainer ) {
			[ mContainer removeFromSuperview ];
			[ mContainer release ];
		}
	}

	//----------------------------------------------------------------//
	-( void	) doneButtonPressed :( id )sender {
		UNUSED ( sender );
		[ self close ];
	}

	//----------------------------------------------------------------//
	-( void ) hide {
	}

	//----------------------------------------------------------------//
	-( MOAIWebViewController* ) init :( CGRect )frame :( int )toolbarHeight {
	
		self = [ super init ];
		if ( self ) {
			
			//CGRect frame = [[ UIApplication sharedApplication ] keyWindow ].frame;
		
			mContainer = [[ UIView alloc ] initWithFrame:frame ];
			[ mContainer setBackgroundColor:[ UIColor blueColor ]];
			[ mContainer setContentMode:UIViewContentModeScaleToFill ];
			[ mContainer setAutoresizingMask:( UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth )];
			
			if ( toolbarHeight > 0.0f ) {
				
				//create toolbar using new
				UIToolbar* toolBar = [[[ UIToolbar alloc ] init ] autorelease ];
				[ toolBar setBarStyle:UIBarStyleDefault ];
				[ toolBar setFrame:CGRectMake ( 0, 0, frame.size.width, toolbarHeight )];
				[ toolBar setAutoresizingMask:( UIViewAutoresizingFlexibleWidth )];
				
				UIBarButtonItem *done = [[[ UIBarButtonItem alloc ] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector ( close )] autorelease ];
				[ toolBar setItems:[ NSArray arrayWithObjects: done, nil ] animated:NO ];
				
				[ mContainer addSubview:toolBar ];
			}
			
			mWebView = [[ UIWebView alloc ] initWithFrame:CGRectMake ( 0, toolbarHeight, frame.size.width, frame.size.height - toolbarHeight )];
			
			[ mWebView setDelegate:self ];
			[ mWebView setScalesPageToFit:YES ];
			[ mWebView setMultipleTouchEnabled:YES ];
			[ mWebView setContentMode:UIViewContentModeScaleToFill ];
			[ mWebView setDataDetectorTypes:UIDataDetectorTypeNone ];
			[ mWebView setAutoresizingMask:( UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth )];

			[ mContainer addSubview:mWebView ];
		}
		return self;
	}

	//----------------------------------------------------------------//
	-( void ) show :( BOOL )animated {
	
		UNUSED ( animated );
	
		//[ mContainer setFrame:[ self currentFrame ]];
	
		UIViewController* rootVC = [[[ UIApplication sharedApplication ] keyWindow ] rootViewController ];
		[ rootVC.view addSubview:mContainer ];
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
