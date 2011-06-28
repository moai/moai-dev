//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>
#import <moaiext-iphone/MOAIWebView.h>

@interface MoaiUiWebViewDelegate : NSObject < UIWebViewDelegate > {
@private
								MOAIWebView*			mMOAIWebView;
}	
@property ( readwrite, assign ) MOAIWebView*			mMOAIWebView;	

@end

