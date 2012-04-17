// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCUserAccountModel.h"
#import "TJCConstants.h"
#import "TJCUtil.h"
#import "TJCLog.h"
#import "TapjoyConnect.h"

@implementation TJCUserAccountModel

@synthesize tapPoints = tapPoints_;
@synthesize pointsID = pointsID_;
@synthesize currencyName = currencyName_;

-(id) init
{
	if ((self = [super init]))
	{
		NSString *cachedCurrencyName=[[NSUserDefaults standardUserDefaults] objectForKey:TJC_CURRENCY_KEY_NAME];
		NSNumber *cachedPoints = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_POINTS_KEY_NAME];
		NSString *cachedPointsID = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_POINT_ID_KEY_NAME];
		
		// This will always be zero on init to prevent weird values.
		[[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithInt:0]
																forKey:TJC_POINTS_DELTA_KEY_NAME];
		
		if (!cachedCurrencyName)
		{
			cachedCurrencyName = @"Points";
			[[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%@",cachedCurrencyName] forKey:TJC_CURRENCY_KEY_NAME];
		}	
		
		if (!cachedPoints)
		{
			cachedPoints = [NSNumber numberWithInt:0];
		}
		
		if (!cachedPointsID)
		{
			cachedPointsID = [TapjoyConnect getUserID];
		}
		
		[self setCurrencyName:cachedCurrencyName];
		
		@try 
		{
			if (cachedPoints)
			{
				tapPoints_ = [cachedPoints intValue];
			}
		}
		@catch (NSException * e) 
		{
			tapPoints_ = 0;
		}
		
		[self setPointsID:cachedPointsID];
		
	}
	return self;
}


- (id)initWithTBXML:(TBXMLElement*)aXMLElement
{
	if ((self = [super init]))
	{
		if (aXMLElement)
		{
			TBXMLElement *tapPointElement = [TBXML childElementNamed:@"TapPoints" parentElement:aXMLElement];
			TBXMLElement *currencyNameElement = [TBXML childElementNamed:@"CurrencyName" parentElement:aXMLElement];
			TBXMLElement *pointsIDElement = [TBXML childElementNamed:@"PointsID" parentElement:aXMLElement];
			
			int newTapPoints = [TBXML numberForElement:tapPointElement];				
			
			tapPoints_ = newTapPoints;
			[[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%@",[NSNumber numberWithInt:newTapPoints]] forKey:TJC_POINTS_KEY_NAME];
			
			NSString *newPointsID = [TBXML textForElement:pointsIDElement];
			[self setPointsID:newPointsID];
			[[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%@",newPointsID] forKey:TJC_POINT_ID_KEY_NAME];
			
			NSString *newCurrencyName = [TBXML textForElement:currencyNameElement] ;
			//primary color which is available in cache  
			NSString * cachedCurrencyName = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_CURRENCY_KEY_NAME];
			
			//Refresh Cache
			if(newCurrencyName && ![newCurrencyName isEqualToString:@""])
			{
				//OLD CURRENCY NAME IS SAVE
				if(!cachedCurrencyName || ![cachedCurrencyName isEqualToString:newCurrencyName] )
				{
					//If it is not the same then over ride the value
					[TJCLog logWithLevel:LOG_DEBUG format:@"TJC: Updating Currency Name in Cache"];
					[[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%@",currencyName_] forKey:TJC_CURRENCY_KEY_NAME];
					cachedCurrencyName = newCurrencyName;
				}
			}	
			
			if(!cachedCurrencyName)
			{
				[self setCurrencyName:@""];
			}
			else
			{
				[self setCurrencyName:cachedCurrencyName];
			}
		}
	}
	
	return self;
}


- (void)updateWithTBXML:(TBXMLElement*)aXMLElement shouldCheckEarnedPoints:(BOOL)checkEarnedPoints
{
	if (!aXMLElement)
		return;
	
	TBXMLElement *tapPointElement = [TBXML childElementNamed:@"TapPoints" parentElement:aXMLElement];
	TBXMLElement *currencyNameElement = [TBXML childElementNamed:@"CurrencyName" parentElement:aXMLElement];
	TBXMLElement *pointsIDElement = [TBXML childElementNamed:@"PointsID" parentElement:aXMLElement];
	
	int newTapPoints = [TBXML numberForElement:tapPointElement];				

	if (checkEarnedPoints)
	{
		// Check specifically whether the user has EARNED TapPoints. If so, fire the earned-tap-points notification.
		if (tapPoints_ < newTapPoints)
		{
			// Save the delta.
			[[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithInt:(newTapPoints - tapPoints_)]
																	forKey:TJC_POINTS_DELTA_KEY_NAME];
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_TAPPOINTS_EARNED_NOTIFICATION 
																				 object:[NSNumber numberWithInt:(newTapPoints - tapPoints_)]];
		}
	}
	
	tapPoints_ = newTapPoints;
	[[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%@",[NSNumber numberWithInt:newTapPoints]] 
															forKey:TJC_POINTS_KEY_NAME];
	
	NSString *newPointsID = [TBXML textForElement:pointsIDElement];
	if (newPointsID && ![newPointsID isEqualToString:@""])
	{
		if (pointsID_)
		{
			[pointsID_ release];
			pointsID_ = nil;
		}
		[self setPointsID:newPointsID];
		[[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%@",newPointsID] 
																forKey:TJC_POINT_ID_KEY_NAME];
	}
	
	NSString *newCurrencyName = [TBXML textForElement:currencyNameElement] ;
	//primary color which is available in cache  
	NSString *cachedCurrencyName = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_CURRENCY_KEY_NAME];
	
	//Refresh Cache
	if(newCurrencyName && ![newCurrencyName isEqualToString:@""])
	{
		//OLD CURRENCY NAME IS SAVE
		if(!cachedCurrencyName || ![cachedCurrencyName isEqualToString:newCurrencyName] )
		{
			//If it is not the same then over ride the value
			[TJCLog logWithLevel:LOG_DEBUG format:@"TJC: Updating Currency Name in Cache"];
			[[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%@",currencyName_] 
																	forKey:TJC_CURRENCY_KEY_NAME];
			cachedCurrencyName = newCurrencyName;
		}
	}	
	
	if (currencyName_)
	{
		[currencyName_ release];
		currencyName_ = nil;
	}
	
	if (!cachedCurrencyName)
	{
		[self setCurrencyName:@""];
	}
	else	
	{
		[self setCurrencyName:cachedCurrencyName];
	}        
}


- (void)dealloc;
{
	[currencyName_ release];
	[pointsID_ release];
	
	[super dealloc];
}


@end
