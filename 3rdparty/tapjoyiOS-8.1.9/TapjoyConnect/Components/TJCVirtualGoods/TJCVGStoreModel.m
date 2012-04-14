// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCVGStoreModel.h"
#import "TJCUserAccountManager.h"
#import "TJCUserAccountModel.h"
#import "TJCLog.h"
#import "TapjoyConnect.h"

// StoreItem class constants
#define ATTRIBUTE_TYPE_PREFIX @"AttributeType"
#define ATTRIBUTE_VALUE_PREFIX @"AttributeValue"


#pragma mark -
@implementation TJCVGStoreModel

@synthesize allItems = allItems_, purchasedItems = purchasedItems_,localItems = localItems_;

-(id)init
{
	if((self = [super init]))
	{
		allItems_ = [[NSMutableArray alloc] init];
		purchasedItems_ = [[NSMutableArray alloc] init];
		localItems_ = [[NSMutableArray alloc] init];
	}
	return self;
}


-(void) addObjectForAllItems:(TJCVGStoreItem *)vgStoreItem
{
	[allItems_ addObject:vgStoreItem];
	//[vgStoreItem release];
}


-(void) addObjectForPurchasedItems:(TJCVGStoreItem *)vgStoreItem
{
	[purchasedItems_ addObject:vgStoreItem];
	//[vgStoreItem release];
}


-(void) setBuyItemResponse:(TJCVGItemPurchaseResponse *)buyitemResponse; 
{
	if(buyitemResponse_)
	{
		[buyitemResponse_ release];
		buyitemResponse_ = nil;
	}
	buyitemResponse_ = [buyitemResponse retain];
}
-(TJCVGItemPurchaseResponse *) getBuyItemResponse
{
	return buyitemResponse_;
}


- (void) releasePurchasedItems
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Releasing Purchased VG Items Array"];
	if(purchasedItems_)
	{
		[purchasedItems_ removeAllObjects];
		[purchasedItems_ release];
		purchasedItems_ = nil;
		//recievedDataForPurchasedVG = FALSE;
	}
	
}


- (void) releaseAllItems
{
	if(allItems_)
	{
		[allItems_ release];
		allItems_ = nil;
	}
}

-(void) releaseBuyItemReponse
{
	if(buyitemResponse_)
	{
		[buyitemResponse_ release];
		buyitemResponse_ = nil;
	}
}


-(void) dealloc
{
	if(allItems_)
	{	
		[allItems_ release];
		allItems_ = nil;
	}
	
	if(purchasedItems_)
	{
		[purchasedItems_ release];
		purchasedItems_ = nil;
	}
	
	if(localItems_)
	{
		[localItems_ release];
		localItems_ = nil;
	}
	
	if(buyitemResponse_)
	{
		[buyitemResponse_ release];
		buyitemResponse_ = nil;
	}
	
	[super dealloc];
	[TJCLog logWithLevel:LOG_DEBUG format:@"TJCVGStore Model Dealloc"];
}

@end




#pragma mark -
@implementation TJCVGStoreItem

@synthesize itemAttributes = itemAttributes_,
itemID = itemID_,
itemName = itemName_,
itemDescription = itemDescription_,
itemAppleProductID = itemAppleProductID_,
itemTypeName = itemTypeName_,
itemDatafileURL = itemDatafileURL_,
itemThumbImageURL = itemThumbImageURL_,
itemFullImageURL = itemFullImageURL_,
itemThumbNailImage = itemThumbNailImage_,
itemCurrencyName = itemCurrencyName_,
itemDataHash = itemDataHash_,
itemPrice = itemPrice_,
itemNumberOwned = itemNumberOwned_,
itemDataFileDirPath = itemDataFileDirPath_,
itemThumbImageFilePath = itemThumbImageFilePath_,
itemFullImageFilePath = itemFullImageFilePath_;


-(id)initWithXMLElement:(TBXMLElement *)xmlElementObj
{
	if((self = [super init]))
	{
		itemID_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"VGStoreItemID" parentElement:xmlElementObj]]];
		itemAppleProductID_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"AppleProductID" parentElement:xmlElementObj]]];
		itemName_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"Name" parentElement:xmlElementObj]]];
		itemDescription_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"Description" parentElement:xmlElementObj]]];
		itemTypeName_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"VGStoreItemTypeName" parentElement:xmlElementObj]]];
		itemThumbImageURL_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"ThumbImageURL" parentElement:xmlElementObj]]];
		itemFullImageURL_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"FullImageURL" parentElement:xmlElementObj]]];
		itemDatafileURL_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"DatafileURL" parentElement:xmlElementObj]]];
		itemDataHash_ = [[NSString alloc] initWithString: 
				   [TBXML textForElement:[TBXML childElementNamed:@"DataHash" parentElement:xmlElementObj]]];
		itemPrice_ = [TBXML numberForElement:[TBXML childElementNamed:@"Price" parentElement:xmlElementObj]];
		itemNumberOwned_ = [TBXML numberForElement:[TBXML childElementNamed:@"NumbersOwned" parentElement:xmlElementObj]];
		
		itemDataFileDirPath_ = nil;
		itemThumbImageFilePath_ = nil;
		itemFullImageFilePath_ = nil;
		
		NSString *cName = [[[TJCUserAccountManager sharedTJCUserAccountManager] userAccountModelObj] currencyName];
		
		if(cName)
			[self setItemCurrencyName:cName];
		else
			[self setItemCurrencyName:@""];

		TBXMLElement * vgStoreItemAttributeArray = [TBXML childElementNamed:@"VGStoreItemAttributeValueReturnClass" 
															  parentElement:[TBXML childElementNamed:@"AttributeValues" parentElement:xmlElementObj]];

		itemAttributes_ = [[NSMutableArray alloc] init];
		
		while (vgStoreItemAttributeArray)
		{
			TJCVGStoreItemAttribute *attribObj = [[TJCVGStoreItemAttribute alloc]initWithXMLElement:vgStoreItemAttributeArray];
			[itemAttributes_ addObject:attribObj];
			[attribObj release];
			vgStoreItemAttributeArray = [TBXML nextSiblingNamed:@"VGStoreItemAttributeValueReturnClass" searchFromElement:vgStoreItemAttributeArray];
		}
		
	}
	return self;
}


-(void) dealloc
{
	[itemAttributes_ removeAllObjects];
	[itemAttributes_ release];
	
	[itemID_ release];
	[itemName_ release];
	[itemDescription_ release];
	[itemAppleProductID_ release];
	[itemTypeName_ release];
	[itemDatafileURL_ release];
	[itemThumbImageURL_ release];
	[itemFullImageURL_ release];
	[itemThumbNailImage_ release];
	[itemCurrencyName_ release];
	[itemDataHash_ release];
	
	[itemDataFileDirPath_ release];
	[itemThumbImageFilePath_ release];
	[itemFullImageFilePath_ release];
	
	[super dealloc];
	//[TJCLog logWithLevel:LOG_DEBUG format:@"TJCVGStore Item Dealloc"];
}

@end








#pragma mark -
@implementation TJCVGStoreItemAttribute

@synthesize attributeType = attributeType_,attributeValue = attributeValue_;

-(id)initWithXMLElement:(TBXMLElement *)xmlElementObj
{
	if((self = [super init]))
	{
		[self setAttributeType:[TBXML textForElement:[TBXML childElementNamed:@"AttributeType" parentElement:xmlElementObj]]];
		[self setAttributeValue:[TBXML textForElement:[TBXML childElementNamed:@"AttributeValue" parentElement:xmlElementObj]]];
	}
	return self;
}

-(void) dealloc
{
	[attributeType_ release];
	[attributeValue_ release];
	
	[super dealloc];
	//[TJCLog logWithLevel:LOG_DEBUG format:@"TJCVGStore ItemAttribute Dealloc"];
}


@end






#pragma mark -
@implementation TJCVGItemPurchaseResponse

@synthesize message = message_,success = success_;

- (id)initWithXMLElement:(TBXMLElement *)anInfo
{
	if((self = [self init]))
	{
		[self setMessage:[TBXML textForElement:[TBXML childElementNamed:@"Message" parentElement:anInfo]]];
		[self setSuccess:[TBXML boolForElement:[TBXML childElementNamed:@"Success" parentElement:anInfo]]];
	}
	return self;
}

-(void) dealloc
{
	[message_ release];
	[super dealloc];
	[TJCLog logWithLevel:LOG_DEBUG format:@"TJCVGPurchase Response Dealloc"];
}


@end






#pragma mark -
@implementation StoreItem


@synthesize storeItemID = storeItemID_, 
appleProductID = appleProductID_, 
storeItemType = storeItemType_, 
title = title_, 
description = description_, 
thumbImageFilePath = thumbImageFilePath_, 
fullImageFilePath = fullImageFilePath_, 
datafileLocation = datafileLocation_, 
currencyName = currencyName_,
attributeValues = attributeValues_, 
thumbImage = thumbImage_, 
fullImage = fullImage_, 
price = price_;


- (id)initWithObject:(TJCVGStoreItem*)localObj
{
	if ((self = [super init]))
	{
		storeItemID_ = [[NSString alloc] initWithString:localObj.itemID];
		appleProductID_ = [[NSString alloc] initWithString:localObj.itemAppleProductID];
		storeItemType_ = [[NSString alloc] initWithString:localObj.itemTypeName];
		price_ = localObj.itemPrice;
		currencyName_ = [[NSString alloc] initWithString:localObj.itemCurrencyName];
		title_ = [[NSString alloc] initWithString:localObj.itemName];
		description_ = [[NSString alloc] initWithString:localObj.itemDescription];
		attributeValues_ = [[NSMutableDictionary alloc] init];
		
		for (int i=0; i < [localObj.itemAttributes count]; i++) 
		{
			NSString *type = [[NSString alloc] initWithString:[[localObj.itemAttributes objectAtIndex:i] attributeType]];
			NSString *value = [[NSString alloc] initWithString:[[localObj.itemAttributes objectAtIndex:i] attributeValue]];
			NSString *index = [[NSNumber numberWithInt:(i+1)] stringValue];
			NSString *attributeI = [[NSString alloc] initWithString:[ATTRIBUTE_TYPE_PREFIX stringByAppendingString:index]];
			NSString *valueI = [[NSString alloc] initWithString:[ATTRIBUTE_VALUE_PREFIX stringByAppendingString:index]];
			[attributeValues_ setValue:value forKey:valueI];
			[attributeValues_ setValue:type forKey:attributeI];
			[type release];
			[value release];
			[attributeI release];
			[valueI release];
		}
		thumbImage_ = nil;
		fullImage_ = nil;
		thumbImageFilePath_ = [[NSString alloc] initWithString:localObj.itemThumbImageFilePath];
		fullImageFilePath_ = [[NSString alloc] initWithString:localObj.itemFullImageFilePath];
		datafileLocation_ = [[NSString alloc] initWithString:localObj.itemDataFileDirPath];
	}
	
	return self;
}

- (void)dealloc
{
	[storeItemID_ release];
	[appleProductID_ release];
	[storeItemType_ release];
	[title_ release]; // Item name
	[description_ release];
	[thumbImageFilePath_ release];
	[fullImageFilePath_ release];
	[datafileLocation_ release];
	[currencyName_ release];
	[attributeValues_ removeAllObjects];
	[attributeValues_ release];
	[thumbImage_ release];
	[fullImage_ release];
	
	[super dealloc];
}

@end
