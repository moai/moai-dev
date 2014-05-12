//
//  CrittercismDelegate.h
//  Crittercism-iOS
//
//  Created by Sean Hermany on 10/19/12.
//  Copyright (c) 2012 Crittercism. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol CrittercismDelegate <NSObject>

@optional

// Will be called if your app crashed the last time it was running.
- (void)crittercismDidCrashOnLastLoad;

@end
