// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJEventDataStore.h"
#import "TJEventModel.h"
#import "TJAppModel.h"

#define TJ_APP_ARRAY_KEY	@"TJ_APP_ARRAY_KEY"

@interface TJEventDataStore ()

@property (nonatomic, strong) NSMutableArray *encodedObjects;

@end

@implementation TJEventDataStore

+(TJEventDataStore*)defaultEventDataStore
{
    static TJEventDataStore *defaultEventData = nil;
    if (!defaultEventData)
    {
        defaultEventData = [[super alloc] init];
    }
    
    return defaultEventData;
}


-(id)init
{
    self = [super init];
    
    if (self)
	{
        _events = @[[[TJEventModel alloc] initWithName:@"test_unit" value:nil],
					[[TJEventModel alloc] initWithName:@"video_unit" value:nil],
					[[TJEventModel alloc] initWithName:@"message_unit" value:nil]];
        
		_apps = [[NSMutableArray alloc] init];
		
		NSString *plistPath = [[NSBundle mainBundle] pathForResource:@"TJAppInfo" ofType:@"plist"];
		NSDictionary *appInfo = [NSDictionary dictionaryWithContentsOfFile:plistPath];
		// Load apps saved in user defaults.
		_encodedObjects = [[NSMutableArray alloc] init];
		[_encodedObjects addObjectsFromArray: [[NSUserDefaults standardUserDefaults] objectForKey:TJ_APP_ARRAY_KEY]];
		for (NSData *encodedObj in _encodedObjects)
		{
			[self saveApp:(TJAppModel*)[NSKeyedUnarchiver unarchiveObjectWithData:encodedObj]];
		}
		
		// Then load the default ones.
		for (id key in [appInfo allKeys])
		{
			NSDictionary *dict = appInfo[key];
			[self saveApp:[[TJAppModel alloc] initWithDictionary:dict]];
		}
    }
    
    return self;
}


- (void)saveApp:(TJAppModel*)app
{
	NSPredicate *predicate = [NSPredicate predicateWithFormat:@"appID == %@", app.appID];
	NSArray *foundObjects = [_apps filteredArrayUsingPredicate:predicate];
	if ([foundObjects count] <= 0)
		[_apps addObject:app];

	NSData *encodedObj = [NSKeyedArchiver archivedDataWithRootObject:app];
	if (![_encodedObjects containsObject:encodedObj])
	{
		[_encodedObjects addObject:encodedObj];
		[[NSUserDefaults standardUserDefaults] setObject:_encodedObjects forKey:TJ_APP_ARRAY_KEY];
		[[NSUserDefaults standardUserDefaults] synchronize];
	}
}

@end