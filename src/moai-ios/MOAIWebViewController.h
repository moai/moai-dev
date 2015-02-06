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

	@property ( readonly ) UIWebView*					webView;
	@property ( readwrite, assign ) MOAIWebViewIOS*		moaiWebView;

	//----------------------------------------------------------------//
	-( MOAIWebViewController* )		init			:( int )toolbarHeight;
	-( void )						show			:( BOOL )animated;

@end
