//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios/MOAIAppIOS.h>
#import <moai-ios/MOAIWebViewController.h>
#import <moai-ios/MOAIWebViewIOS.h>

#define TOOLBAR_HEIGHT	44
#define ANIM_DURATION	0.5
#define ANIM_DELAY		0.1

//================================================================//
// MOAIWebViewController ()
//================================================================//
@interface MOAIWebViewController () {
@private

	int					mToolbarHeight;
	BOOL				mAnimated;

	UIView*				mContainer;
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
		
		if ( mContainer ) {
			[ mContainer removeFromSuperview ];
			[ mContainer release ];
		}
	}

	//----------------------------------------------------------------//
	-( void	) doneButtonPressed {
	
		[ self hide:mAnimated ];
	}

	//----------------------------------------------------------------//
	-( void ) hide :( BOOL )animated {
	
		mAnimated = animated;
		
		if ([ mContainer isHidden ]) return;
		
		if ( mAnimated ) {
		
			CGRect frame = [ mContainer frame ];
		
			[ UIView
				animateWithDuration:ANIM_DURATION
				delay:ANIM_DELAY
				options: UIViewAnimationCurveEaseOut
				animations:^() {
					[ mContainer setFrame:CGRectMake ( frame.origin.x, frame.origin.y - frame.size.height, frame.size.width, frame.size.height )];
				}
				completion:^( BOOL finished ) {
					UNUSED ( finished );
					[ mContainer setHidden:YES ];
					
					if ( mMOAIWebView ) {
						mMOAIWebView->RaiseWebViewDidHideEvent ();
					}
					[ self release ];
				}
			];
		}
		else {
			[ mContainer setHidden:YES ];
			[ self release ];
		}
	}

	//----------------------------------------------------------------//
	-( MOAIWebViewController* ) init {
	
		self = [ super init ];
		if ( self ) {
			
			UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
			CGRect frame = window.frame;
		
			mContainer = [[ UIView alloc ] initWithFrame:frame ];
			[ mContainer setBackgroundColor:[ UIColor blueColor ]];
			[ mContainer setContentMode:UIViewContentModeScaleToFill ];
			[ mContainer setAutoresizingMask:( UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth )];
			
			UIToolbar* toolBar = [[[ UIToolbar alloc ] init ] autorelease ];
			[ toolBar setBarStyle:UIBarStyleDefault ];
			[ toolBar setFrame:CGRectMake ( 0, 0, frame.size.width, TOOLBAR_HEIGHT )];
			[ toolBar setAutoresizingMask:( UIViewAutoresizingFlexibleWidth )];
			
			UIBarButtonItem *done = [[[ UIBarButtonItem alloc ] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector ( doneButtonPressed )] autorelease ];
			[ toolBar setItems:[ NSArray arrayWithObjects: done, nil ] animated:NO ];
			
			[ mContainer addSubview:toolBar ];
			
			mWebView = [[ UIWebView alloc ] initWithFrame:CGRectMake ( 0, TOOLBAR_HEIGHT, frame.size.width, frame.size.height - TOOLBAR_HEIGHT )];
			
			[ mWebView setDelegate:self ];
			[ mWebView setScalesPageToFit:YES ];
			[ mWebView setMultipleTouchEnabled:YES ];
			[ mWebView setContentMode:UIViewContentModeScaleToFill ];
			[ mWebView setDataDetectorTypes:UIDataDetectorTypeNone ];
			[ mWebView setAutoresizingMask:( UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth )];

			[ mContainer addSubview:mWebView ];
			
			[ window addSubview:mContainer ];
			[ mContainer setHidden:YES ];
		}
		return self;
	}

	//----------------------------------------------------------------//
	-( void ) show :( BOOL )animated {
		
		mAnimated = animated;
		
		if ( ![ mContainer isHidden ]) return;
		
		[ self retain ];
		
		if ( mAnimated ) {
		
			CGRect frame = [ mContainer frame ];
			[ mContainer setFrame:CGRectMake ( frame.origin.x, frame.origin.y - frame.size.height, frame.size.width, frame.size.height )];
		
			[ UIView
				animateWithDuration:ANIM_DURATION
				delay:ANIM_DELAY
				options: UIViewAnimationCurveEaseOut
				animations:^{
					[ mContainer setFrame:frame ];
				}
				completion:NULL
			];
		}
		[ mContainer setHidden:NO ];
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
