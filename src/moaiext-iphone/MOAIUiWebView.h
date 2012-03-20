//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <UIKit/UIKit.h>
#import <moaiext-iphone/MOAIWebView.h>

//================================================================//
// MoaiUiWebViewDelegate
//================================================================//
@interface MoaiUiWebViewDelegate : NSObject < UIWebViewDelegate > {
@private
			MOAIWebView*			mMOAIWebView;
}

	-( void	)			doneButtonPressed		:( id )sender;

@property ( readwrite, assign ) MOAIWebView*	mMOAIWebView;	

@end

