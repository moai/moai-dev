// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCVGViewHandler.h"
#import "TJCViewCommons.h"
#import "TJCVGViewHandler.h"
#import "TJCVGView.h"
#import "TapjoyConnect.h"
#import "TJCFileDownloadHandler.h"
#import "TJCLocalPurchasedItems.h"
#import "TJCVGStoreManager.h"
#import "TJCLog.h"
#import "TJCSqliteController.h"


static NSString *globalDBSavePath = nil;

@implementation TJCVGViewHandler

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCVGViewHandler)

@synthesize fileDownloadHandler = fileDownloadHandler_, scrollToRow = scrollToRow_;


- (id)init
{
	if((self = [super init]))
	{
		fileDownloadHandler_ = [[TJCFileDownloadHandler alloc] init];
		
		[self initializeDBPathVariables];
		
		[self deleteOldDB];
	}
	return self;
}


- (UIView*)showVirtualGoodsWithFrame:(CGRect)frame
{
	// Remove any view that might possibly exist on the superview.
	[[TJCVGView sharedTJCVGView].mainView removeFromSuperview];
	
	[[TJCVGView sharedTJCVGView] refreshWithFrame:frame];
	
	[TJCViewCommons animateTJCView:[TJCVGView sharedTJCVGView].mainView 
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
	return [TJCVGView sharedTJCVGView].mainView;
}


- (UIView*)showVirtualGoods
{
	return [self showVirtualGoodsWithFrame:[[UIScreen mainScreen] bounds]];
}


- (void)showVirtualGoodsWithViewController:(UIViewController*)viewController
{
	// Remove any view that might possibly exist on the superview.
	[[TJCVGView sharedTJCVGView].mainView removeFromSuperview];
	
	[[TJCVGView sharedTJCVGView] refreshWithFrame:viewController.view.bounds];
	
	[viewController.view addSubview:[TJCVGView sharedTJCVGView].mainView];
	
	[TJCViewCommons animateTJCView:[TJCVGView sharedTJCVGView].mainView 
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
}


- (TJCVGView*)getVirtualGoodsView
{
	if([TJCVGView sharedTJCVGView] && [[TJCVGView sharedTJCVGView] isKindOfClass:[TJCVGView class]])
	{
		return [TJCVGView sharedTJCVGView];
	}
	return nil;
}


- (UIView*)getVirtualGoodsWithTab:(int)tabIndex
{
	// Remove any view that might possibly exist on the superview.
	[[TJCVGView sharedTJCVGView].mainView removeFromSuperview];
	
	[[TJCVGView sharedTJCVGView] refreshWithFrame:[[UIScreen mainScreen]bounds]];
	
	[TJCViewCommons animateTJCView:[TJCVGView sharedTJCVGView].mainView 
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
	
	return [TJCVGView sharedTJCVGView].mainView;
}


- (UIView*)getVirtualGoodsWithTab:(int)tabIndex withScrollToRow:(int)row
{
	// Remove any view that might possibly exist on the superview.
	[[TJCVGView sharedTJCVGView].mainView removeFromSuperview];
	
	[[TJCVGView sharedTJCVGView] refreshWithFrame:[[UIScreen mainScreen]bounds]];
	
	[TJCViewCommons animateTJCView:[TJCVGView sharedTJCVGView].mainView 
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
	
	return [TJCVGView sharedTJCVGView].mainView;
}


- (CGRect)getFrameFromStatusBar
{
	UIInterfaceOrientation interfaceOrientation = [[UIApplication sharedApplication] statusBarOrientation];
	
	CGRect frame = [[UIScreen mainScreen]bounds];
	
	float frameWidth = frame.size.width;
	float frameHeight = frame.size.height;
	
	float shiftVal = 20;
	
	if([[UIApplication sharedApplication] isStatusBarHidden] /*|| (parentVController_ && !parentVController_.navigationController)*/)
	{
		shiftVal = 0;
	}
	
	if(interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation ==UIInterfaceOrientationLandscapeRight)
	{
		return CGRectMake(0,0,frameHeight,frameWidth-shiftVal);		
	}
	
	return CGRectMake(0,0,frameWidth, frameHeight-shiftVal);
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == 1)
	{
		[[TJCCallsWrapper sharedTJCCallsWrapper] showVirtualGoods];
	}
}


- (void)deleteOldDB
{
	//Create a Directory Path 
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDir = [paths objectAtIndex:0];
	NSString *dbPath = [[TJCVGViewHandler getGlobalDBSavePath] stringByAppendingPathComponent:TJC_VG_DB_NAME];
	
	// Check whether the current version of the app is different than the version stored locally.
	NSString *currentBundleVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleVersionKey];
	NSString *localBundleVersionPath = [documentsDir stringByAppendingPathComponent:TJC_VG_DB_VER_FILENAME];
	
	NSError *error = nil;
	
	// Only care if a file exists. If not, we assume that this is the first time we're running the app.
	if ([[NSFileManager defaultManager] fileExistsAtPath:localBundleVersionPath])
	{
		NSData *fileData = [NSData dataWithContentsOfFile:localBundleVersionPath];
		
		if (fileData)
		{
			NSStringEncoding encoding;
			NSString *localBundleVersion = [NSString stringWithContentsOfFile:localBundleVersionPath 
																				  usedEncoding:&encoding 
																							error:&error];
			
			if (error)
			{
				[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"TJCVGViewHandler deleteOldDB Error: %@", [error localizedDescription]];
			}
			
			// If the versions do not match, removed the old DB.
			if (![localBundleVersion isEqualToString:currentBundleVersion])
			{
				[[NSFileManager defaultManager] removeItemAtPath:dbPath error:NULL];
				
				// Update the version file. Remove the old file, and replace with new one.
				[[NSFileManager defaultManager] removeItemAtPath:localBundleVersionPath error:&error];
				
				if (error)
				{
					[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"TJCVGViewHandler deleteOldDB Error: %@", [error localizedDescription]];
				}
				
				[currentBundleVersion writeToFile:localBundleVersionPath atomically:YES encoding:NSUTF8StringEncoding error:&error];
				
				if (error)
				{
					[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"TJCVGViewHandler deleteOldDB Error: %@", [error localizedDescription]];
				}
			}
		}
	}
	else
	{
		// No version file, create one.
		[currentBundleVersion writeToFile:localBundleVersionPath atomically:YES encoding:NSUTF8StringEncoding error:&error];
		
		if (error)
		{
			[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"TJCVGViewHandler deleteOldDB Error: %@", [error localizedDescription]];
		}
	}
}


- (void)initializeDBPathVariables
{
	//Create a Directory Path 
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDir = [paths objectAtIndex:0];
	
	NSString *appDirectPath = [documentsDir stringByAppendingPathComponent:[[TapjoyConnect sharedTapjoyConnect]appID]];
	
	if(![[NSFileManager defaultManager] fileExistsAtPath:appDirectPath])
	{
		[[NSFileManager defaultManager] createDirectoryAtPath:appDirectPath withIntermediateDirectories:YES attributes:nil error:nil];
	}	
	
	globalDBSavePath = [[NSString alloc] initWithString:appDirectPath];
}


- (NSMutableArray*)getAvailableVGStoreItems
{
	return [[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] allItems];
}


- (NSMutableArray*)getPurchasedVGStoreItems
{
	//return [[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] purchasedItems];
	return [[TJCLocalPurchasedItems sharedTJCLocalPurchasedItems] getPurchasedVGStoreItemsArray];
}


- (NSMutableArray*)getAllLocalVGStoreItems
{
	return [[TJCLocalPurchasedItems sharedTJCLocalPurchasedItems] getVGStoreItemsArray];
}


- (NSMutableArray*)getLocalPurchasedVGStoreItems
{
	return [[TJCLocalPurchasedItems sharedTJCLocalPurchasedItems] getPurchasedVGStoreItemsArray];
}


- (void)initiateRequestForAllStoreItems
{
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForAllVGStoreItems];
}


- (void)initiateRequestForPurchasedVGStoreItems
{
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForPurchasedVGStoreItems];
}


- (void)getPurchasedVGStoreItemsWithPrompt
{
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForPurchasedVGStoreItemsWithPrompt];	
}


- (void)updateAllLocalVGStoreItems
{
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForUpdateVGStoreItems];
}


+ (NSString*)getGlobalDBSavePath
{
	return globalDBSavePath;
}


- (void)updateViewWithOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	[[TJCVGView sharedTJCVGView] updateViewWithOrientation:fromInterfaceOrientation];
}


- (void)dealloc
{
	[fileDownloadHandler_ release];
	
	[super dealloc];
}

@end






@implementation TapjoyConnect (TJCVGViewHandler)

#pragma mark Virtual Goods

+ (UIView*) showVirtualGoodsView
{	
	return [[TJCCallsWrapper sharedTJCCallsWrapper] showVirtualGoods];
}


+ (UIView*) showVirtualGoods
{	
	return [[TJCCallsWrapper sharedTJCCallsWrapper] showVirtualGoods];
}


+ (void)showVirtualGoodsWithViewController:(UIViewController*)viewController
{
	[[TJCCallsWrapper sharedTJCCallsWrapper] showVirtualGoodsWithViewController:viewController];
}


+ (UIView*) getVirtualGoodsView
{	
	return [[TJCVGViewHandler sharedTJCVGViewHandler] showVirtualGoods];	
}


+ (void) getAvailableItems
{
	[[TJCVGViewHandler sharedTJCVGViewHandler] getAvailableVGStoreItems];
}


+ (NSMutableArray*) getPurchasedVGStoreItems
{
	return [[TJCVGViewHandler sharedTJCVGViewHandler] getPurchasedVGStoreItems];
}


+ (NSMutableArray*)getAllLocalVGStoreItems
{
	return [[TJCVGViewHandler sharedTJCVGViewHandler] getAllLocalVGStoreItems];
}


+ (void) showDownloadVGItemsPrompt
{
	[[TJCVGViewHandler sharedTJCVGViewHandler] getPurchasedVGStoreItemsWithPrompt];
}

@end







@implementation TJCCallsWrapper (TJCVGViewHandler)

- (UIView*) showVirtualGoods
{
	UIView *vgView = nil;
	
	[self moveViewToFront];
	
	[self.view setAlpha:1];
	
	vgView = [[TJCVGViewHandler sharedTJCVGViewHandler] showVirtualGoodsWithFrame:self.view.bounds];
	[self.view addSubview:vgView];
	
	return vgView;
}


- (void)showVirtualGoodsWithViewController:(UIViewController*)viewController
{
	[self.view setAlpha:0];
	
	[[TJCVGViewHandler sharedTJCVGViewHandler] showVirtualGoodsWithViewController:viewController];
}


- (void) showDownloadVGItemsPrompt
{
	[self.view setAlpha:0];
	
	@try
	{
		UIWindow *window = [[[UIApplication sharedApplication] windows] objectAtIndex:0];		
		if(!window || ![window isKindOfClass:[UIWindow class]])
		{
			[TJCLog logWithLevel:LOG_FATAL format:@"There must be a UIWindow in the project"];
			NSAssert(NO,@"No UIWindow Found, an application should have atleast one Window object");
		} 
		[window addSubview:self.view];
		
		[[TJCVGViewHandler sharedTJCVGViewHandler] getPurchasedVGStoreItemsWithPrompt];
	}
	@catch (NSException *exp) 
	{
		NSLog(@"UIWindow not found or not created yet%@",[exp description]);
	}
}

@end