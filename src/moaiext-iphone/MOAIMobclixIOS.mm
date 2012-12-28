// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_MOBCLIX

#include "pch.h"

#import <UIKit/UIKit.h>
#import <moaiext-iphone/MOAIMobclixIOS.h>

#import "Mobclix.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Mobclix.
	
	@in		string appId			Available in Crittercism dashboard settings.
	@in		string appKey			Available in Crittercism dashboard settings.
	@in		string appSecret		Available in Crittercism dashboard settings.
	@out	nil
*/
int MOAIMobclixIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* appId = lua_tostring ( state, 1 );

	NSString* ID = [[ NSString alloc ] initWithUTF8String:appId ];

	NSLog(@"MOAIMobclixIOS::_init with appId %@", ID);
	
    [Mobclix startWithApplicationId:ID];
    
    // init the view
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];

	CGRect screenRect = [[ UIScreen mainScreen ] bounds ];
	CGFloat scale = [[ UIScreen mainScreen ] scale ];
	CGFloat screenWidth = screenRect.size.width;
	CGFloat screenHeight = screenRect.size.height;
	
	NSLog(@"ad at %f scale", scale);
    NSLog(@"ad at %f height", screenHeight);
    NSLog(@"ad at %f width", screenWidth);
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        
        if (screenHeight > 1024){
            MOAIMobclixIOS::Get ().mAdView = [[[MobclixAdViewiPad_728x90 alloc] initWithFrame:CGRectMake((768.0f - 728.0f)/2,screenHeight/2 - 90.0f, 728.0f, 90.0f)] autorelease];
        }
        else{
            MOAIMobclixIOS::Get ().mAdView = [[[MobclixAdViewiPad_728x90 alloc] initWithFrame:CGRectMake((768.0f - 728.0f)/2,screenHeight - 90.0f, 728.0f, 90.0f)] autorelease];
        }
        
    }
    else{
        
        if (screenHeight > 480){
            MOAIMobclixIOS::Get ().mAdView = [[[MobclixAdViewiPhone_320x50 alloc] initWithFrame:CGRectMake(0.0f, screenHeight/2 - 50.0f, 320.0f, 50.0f)] autorelease];
        }
        else {
            MOAIMobclixIOS::Get ().mAdView = [[[MobclixAdViewiPhone_320x50 alloc] initWithFrame:CGRectMake(0.0f, screenHeight - 50.0f, 320.0f, 50.0f)] autorelease];
        }
        
    }
    
	[rootVC.view addSubview:MOAIMobclixIOS::Get ().mAdView];
    
    MOAIMobclixIOS::Get ().mAdView.delegate = MOAIMobclixIOS::Get ().mMobclixDelgate;
    //[rootVC.view bringSubviewToFront:MOAIMobclixIOS::Get ().mAdView];
	//[ ID release ];
    
	return 0;
}

//----------------------------------------------------------------//
/**	@name	forceException
	@text	Forces a crash and crittercism report, can be used for internal errors or debugging crittercism
 
	@out	nil 
 */
int	MOAIMobclixIOS::_showAds ( lua_State* L ) {
	MOAILuaState state ( L );

	
	if (MOAIMobclixIOS::Get ().mAdView != nil){
		
		NSLog(@"MOAIMobclixIOS::_showAds");	
    	[MOAIMobclixIOS::Get ().mAdView resumeAdAutoRefresh];

		CGRect adBannerViewFrame = [MOAIMobclixIOS::Get ().mAdView frame];	

		CGRect screenRect = [[ UIScreen mainScreen ] bounds ];
		CGFloat scale = [[ UIScreen mainScreen ] scale ];
		CGFloat screenWidth = screenRect.size.width * scale;
		CGFloat screenHeight = screenRect.size.height * scale;

		NSLog(@"ad at %f scale", scale);
		NSLog(@"ad at %f height", screenHeight);
		NSLog(@"ad at %f width", screenWidth);
		
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
			
			if (screenHeight > 1024){
				adBannerViewFrame.origin.y = screenHeight/2 - 90.0f;
			}
			else{
				adBannerViewFrame.origin.y = screenHeight - 90.0f;
			}
		}
		else{
			
			if (screenHeight > 480){
				adBannerViewFrame.origin.y = screenHeight/2 - 50.0f;
			}
			else {
				adBannerViewFrame.origin.y = screenHeight - 50.0f;
			}
		}
		
		[MOAIMobclixIOS::Get ().mAdView setFrame:adBannerViewFrame];
		
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	leaveBreadcrumb
	@text	Leave a breadcrumb (log statement) to trace execution.
	
	@in		string breadcrumb		A string describing the code location.
	@out	nil
*/
int MOAIMobclixIOS::_hideAds ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	if (MOAIMobclixIOS::Get ().mAdView != nil){
		
		NSLog(@"MOAIMobclixIOS::_hideAds");	
			
		[MOAIMobclixIOS::Get ().mAdView pauseAdAutoRefresh];

		CGRect screenRect = [[ UIScreen mainScreen ] bounds ];
		CGFloat scale = [[ UIScreen mainScreen ] scale ];
		CGFloat screenWidth = screenRect.size.width * scale;
		CGFloat screenHeight = screenRect.size.height * scale;
		
		CGRect adBannerViewFrame = [MOAIMobclixIOS::Get ().mAdView frame];
		//adBannerViewFrame.origin.x = 0;
		adBannerViewFrame.origin.y = 100 + screenHeight;
		[MOAIMobclixIOS::Get ().mAdView setFrame:adBannerViewFrame];
	    
	}
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	dealloc
 @text	Sets the username
 
 @in	string username
 @out	nil
*/
int MOAIMobclixIOS::_dealloc ( lua_State* L ) {
	MOAILuaState state ( L );
	
	[MOAIMobclixIOS::Get ().mAdView cancelAd];
	MOAIMobclixIOS::Get ().mAdView.delegate = nil;
	MOAIMobclixIOS::Get ().mAdView = nil;
    
	return 0;
}


//================================================================//
// MOAIMobclixIOS
//================================================================//

//----------------------------------------------------------------//
MOAIMobclixIOS::MOAIMobclixIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	mMobclixDelgate = [[ MoaiMobclixDelegate alloc ] init];
	mAdView = nil;
	
}

//----------------------------------------------------------------//
MOAIMobclixIOS::~MOAIMobclixIOS () {

	[mMobclixDelgate release];
	
}

//----------------------------------------------------------------//
void MOAIMobclixIOS::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "init",				_init },
		{ "showAds",            _showAds },
		{ "hideAds",            _hideAds },
		{ "dealloc",			_dealloc },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}


#pragma mark -
#pragma mark MobclixAdViewDelegate Methods
@implementation MoaiMobclixDelegate

@synthesize mAdView;

- (void)adViewDidFinishLoad:(MobclixAdView*)adView {
	NSLog(@"Ad Loaded: %@.", NSStringFromCGSize(adView.frame.size));
}

- (void)adView:(MobclixAdView*)adView didFailLoadWithError:(NSError*)error {
	NSLog(@"Ad Failed: %@.", NSStringFromCGSize(adView.frame.size));
}

- (void)adViewWillTouchThrough:(MobclixAdView*)adView {
	NSLog(@"Ad Will Touch Through: %@.", NSStringFromCGSize(adView.frame.size));
}

- (void)adViewDidFinishTouchThrough:(MobclixAdView*)adView {
	NSLog(@"Ad Did Finish Touch Through: %@.", NSStringFromCGSize(adView.frame.size));
}

@end

#endif
