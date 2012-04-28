// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCUserAccountManager.h"
#import "SynthesizeSingleton.h"
#import "TJCUserAccountRequestHandler.h"
#import "TJCUserAccountModel.h"
#import "TapjoyConnect.h"


@implementation TJCUserAccountManager

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCUserAccountManager)

@synthesize userAccountModelObj = userAccountModelObj_;


- (id)init
{
	if ((self = [super init]))
	{
		userAccountModelObj_ = [[TJCUserAccountModel alloc] init];
		
		userAccountGetTapPointsObj_ = nil;
		userAccountSpendTapPointsObj_ = nil;
		
		waitingForResponse_ = NO;
	}
	return self;
}


- (void)updateUserAccountObjWithTBXMLElement:(TBXMLElement*)userAccElement
{
	// JC: TODO: Super hack, this prevents an issue where the earned points alert would pop up if the virtual good purchase returned incorrect Tap Points data.
	[userAccountModelObj_ updateWithTBXML:userAccElement shouldCheckEarnedPoints:NO];
}


- (void)getTapPoints
{
	if (waitingForResponse_)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"TJCUserAccountManager warning: getTapPoints cannot be called until response from the server has been received."];
		
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR 
																			 object:nil];
		
		return;
	}
	
	if (userAccountGetTapPointsObj_)
	{
		[userAccountGetTapPointsObj_ release];
		userAccountGetTapPointsObj_ = nil;
	}
	
	userAccountGetTapPointsObj_ = [[TJCUserAccountRequestHandler alloc] initRequestWithDelegate:self 
																											andRequestTag:kTJCUserAccountRequestTagGetPoints];
	
	[userAccountGetTapPointsObj_ requestTapPoints];
	
	waitingForResponse_ = YES;
}


- (void)spendTapPoints:(int)points
{
	if (waitingForResponse_)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"TJCUserAccountManager warning: spendTapPoints cannot be called until response from the server has been received."];
		
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR
																			 object:nil];
		
		return;
	}
	
	if (userAccountSpendTapPointsObj_)
	{
		[userAccountSpendTapPointsObj_ release];
		userAccountSpendTapPointsObj_ = nil;
	}
	
	userAccountSpendTapPointsObj_ = [[TJCUserAccountRequestHandler alloc] initRequestWithDelegate:self 
																											  andRequestTag:kTJCUserAccountRequestTagSpendPoints];
	
	[userAccountSpendTapPointsObj_ subtractTapPoints:points];
	
	waitingForResponse_ = YES;
}


- (void)awardTapPoints:(int)points
{
	if (waitingForResponse_)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"TJCUserAccountManager warning: awardTapPoints cannot be called until response from the server has been received."];
		
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR
																			 object:nil];
		
		return;
	}
	
	if (userAccountSpendTapPointsObj_)
	{
		[userAccountSpendTapPointsObj_ release];
		userAccountSpendTapPointsObj_ = nil;
	}
	
	userAccountSpendTapPointsObj_ = [[TJCUserAccountRequestHandler alloc] initRequestWithDelegate:self 
																											  andRequestTag:kTJCUserAccountRequestTagAwardPoints];
	
	[userAccountSpendTapPointsObj_ addTapPoints:points];
	
	waitingForResponse_ = YES;
}


// called when request succeeeds
- (void)fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag
{
	waitingForResponse_ = NO;
	
	[userAccountModelObj_ updateWithTBXML:dataObj shouldCheckEarnedPoints:YES];

	switch (aTag) 
	{
		case kTJCUserAccountRequestTagGetPoints:
		{
			// Post the notification for the callback method to indicate that the URL request has succeeded.
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_TAP_POINTS_RESPONSE_NOTIFICATION 
																				 object:[NSNumber numberWithInt:userAccountModelObj_.tapPoints]];
		}
			break;
		case kTJCUserAccountRequestTagSpendPoints:
		{
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION 
																				 object:[NSNumber numberWithInt:userAccountModelObj_.tapPoints]];
		}
		case kTJCUserAccountRequestTagAwardPoints:
		{
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION 
																				 object:[NSNumber numberWithInt:userAccountModelObj_.tapPoints]];			
		}
			break;
		default:
			break;
	}
}


// raised when error occurs
- (void)fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int) aTag
{
	switch (aTag) 
	{
		case kTJCUserAccountRequestTagGetPoints:
		{
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR 
																				 object:nil];
		}
			break;
		case kTJCUserAccountRequestTagSpendPoints:
		{
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_SPEND_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR
																				 object:nil];
		}
		case kTJCUserAccountRequestTagAwardPoints:
		{
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_AWARD_TAP_POINTS_RESPONSE_NOTIFICATION_ERROR
																				 object:nil];
		}
			break;
		default:
			break;
	}
	
	waitingForResponse_ = NO;
}


- (void)releaseUserAccount
{
	if(userAccountModelObj_)
	{
		[userAccountModelObj_ release];
		userAccountModelObj_ = nil;
	}
	
	if (userAccountGetTapPointsObj_)
	{
		[userAccountGetTapPointsObj_ release];
		userAccountGetTapPointsObj_ = nil;
	}
	
	if (userAccountSpendTapPointsObj_)
	{
		[userAccountSpendTapPointsObj_ release];
		userAccountSpendTapPointsObj_ = nil;
	}
}


- (void) dealloc
{
	[userAccountModelObj_ release];
	[userAccountGetTapPointsObj_ release];
	[userAccountSpendTapPointsObj_ release];
	[super dealloc];
}


@end



@implementation TapjoyConnect (TJCUserAccountManager)

+ (void)getTapPoints
{
	[[TJCUserAccountManager sharedTJCUserAccountManager] getTapPoints];
}


+ (void)spendTapPoints:(int)points
{
	[[TJCUserAccountManager sharedTJCUserAccountManager] spendTapPoints:points];
}


+ (void)awardTapPoints:(int)points
{
	[[TJCUserAccountManager sharedTJCUserAccountManager] awardTapPoints:points];
}


+ (void)showDefaultEarnedCurrencyAlert
{
	NSString *msg = [NSString stringWithFormat:@"You've just earned %d %@!", 
						  [[[NSUserDefaults standardUserDefaults] objectForKey:TJC_POINTS_DELTA_KEY_NAME] intValue], 
						  [[NSUserDefaults standardUserDefaults] objectForKey:TJC_CURRENCY_KEY_NAME]];
	UIAlertView *alertview = [[UIAlertView alloc] initWithTitle:@"Congratulations!"
																		 message:msg
																		delegate:self 
															cancelButtonTitle:@"OK"
															otherButtonTitles:nil];
	[alertview show];
	[alertview release];
}

@end