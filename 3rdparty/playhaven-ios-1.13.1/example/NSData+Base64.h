//
//  NSData+Base64.m
//  playhaven-sdk-ios
//
//  Created by lilli on 3/27/13.
//  Copyright 2013 Playhaven. All rights reserved.
//
#import <Foundation/Foundation.h>

@interface NSData (Base64)
+ (NSData *)decodeBase64WithString:(NSString *)strBase64;
@end
