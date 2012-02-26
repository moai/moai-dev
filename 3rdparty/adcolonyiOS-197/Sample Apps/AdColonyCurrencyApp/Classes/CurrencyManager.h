//
//  CurrencyManager.h
//  AdColonyCurrencyApp
//
//  Created by Lev Trubov on 5/4/11.
//  Copyright 2011 Jirbo, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#define kAdColonyVirtualCurrencyTestBalanceURL @"http://www.kewul.com/adcolony_test2/balance_check.php"

#define kSampleVCGoldKey    @"gold"
#define kSampleVCSilverKey  @"silver"

@protocol CurrencyManagerDelegate <NSObject>

-(void)currencyAwardSucceeded;
-(void)currencyAwardFailed;

@end

@interface CurrencyManager : NSObject {
    NSMutableDictionary *allBalances;
    NSMutableArray *delegates;
}

-(void)update;
-(void)awardFailure;

-(void)addDelegate:(id<CurrencyManagerDelegate>)del;
-(void)removeDelegate:(id<CurrencyManagerDelegate>)del;
-(void)notifyDelegatesOfSuccess;
-(void)notifyDelegatesOfFailure;
-(int)currencyForKey:(NSString *)key;

-(int)gold;
-(int)silver;

@end

@interface UILabel (CurrencyLabel)
    
//sets only if value is positive
-(void)setCurrencyLabelToInt:(int)value;

@end
