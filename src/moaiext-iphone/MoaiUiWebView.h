//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>
#import <moai-iphone/MOAIWebView.h>

@interface MoaiUiWebView : UIViewController < UIWebViewDelegate > {
@private
								MOAIWebView*			mMOAIWebView;
								UIWebView*              mWebView;	
}	
@property ( readwrite, assign ) MOAIWebView*			mMOAIWebView;
@property ( readwrite, assign ) UIWebView*              mWebView;		

@end
