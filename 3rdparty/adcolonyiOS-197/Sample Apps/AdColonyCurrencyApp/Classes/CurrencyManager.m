//
//  CurrencyManager.m
//  AdColonyCurrencyApp
//
//  Created by Lev Trubov on 5/4/11.
//  Copyright 2011 Jirbo, Inc. All rights reserved.
//

#import "CurrencyManager.h"


@implementation CurrencyManager

-(id)init{
    if((self = [super init])){
        allBalances = [[NSMutableDictionary alloc] init];
        delegates = [[NSMutableArray alloc] init];
        [self update];
    }
    
    return self;
}

-(void)dealloc{
    [allBalances release];
    [delegates release];
    [super dealloc];
}

#pragma mark server

-(void)update{
    [NSThread detachNewThreadSelector:@selector(_reallyUpdate) toTarget:self withObject:nil];
}

-(void)_reallyUpdate{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSString* urlString = [NSString stringWithFormat:@"%@?uid=%@", kAdColonyVirtualCurrencyTestBalanceURL, [[UIDevice currentDevice] uniqueIdentifier]];
	NSURL* url = [NSURL URLWithString:urlString];
	NSString *response = [[[NSString alloc] initWithData:[NSData dataWithContentsOfURL:url] encoding:NSUTF8StringEncoding] autorelease];
    
    NSArray *a = [response componentsSeparatedByString:@" "];
    if([a count] > 1){
        int gold = [[a objectAtIndex:0] intValue];
        int silver = [[a objectAtIndex:1] intValue];
        [allBalances setObject:[NSNumber numberWithInt:gold] forKey:kSampleVCGoldKey];
        [allBalances setObject:[NSNumber numberWithInt:silver] forKey:kSampleVCSilverKey];
        
        [self performSelectorOnMainThread:@selector(notifyDelegatesOfSuccess) withObject:nil waitUntilDone:NO];
    }

    [pool release];
}

-(void)awardFailure{
    [self performSelectorOnMainThread:@selector(notifyDelegatesOfFailure) withObject:nil waitUntilDone:NO];
}

#pragma mark delegate

-(void)addDelegate:(id<CurrencyManagerDelegate>)del{
    [delegates addObject:del];
}

-(void)removeDelegate:(id<CurrencyManagerDelegate>)del{
    [delegates removeObject:del];
}

-(void)notifyDelegatesOfSuccess{
    for(id<CurrencyManagerDelegate> del in delegates){
        [del currencyAwardSucceeded];
    }
}

-(void)notifyDelegatesOfFailure{
    for(id<CurrencyManagerDelegate> del in delegates){
        [del currencyAwardFailed];
    }
}

#pragma mark access
-(int)currencyForKey:(NSString *)key{
    id item = [allBalances objectForKey:key];
    if(!item || ![item isKindOfClass:[NSNumber class]]){
        return -1;
    }
    
    return [item intValue];
}

-(int)gold{
    return [self currencyForKey:kSampleVCGoldKey];
}

-(int)silver{
    return [self currencyForKey:kSampleVCSilverKey];
}

@end

@implementation UILabel (CurrencyLabel)

-(void)setCurrencyLabelToInt:(int)value{
    if(value >= 0){
        self.text = [NSString stringWithFormat:@"%i", value];
    }
}

@end
