//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>

class MOAIWebViewIOS;

//================================================================//
// MOAIWebViewController
//================================================================//
@interface MOAIWebViewController : UIViewController < UIWebViewDelegate > {
@private
}

	@property ( readwrite, assign ) BOOL				animated;
	@property ( readonly ) UIWebView*					webView;
	@property ( readwrite, assign ) MOAIWebViewIOS*		moaiWebView;

	//----------------------------------------------------------------//
	-( void )						hide			:( BOOL )animated;
	-( void )						show			:( BOOL )animated;

@end
