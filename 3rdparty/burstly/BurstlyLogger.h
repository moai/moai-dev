//
//  AS_Logger.h
//  Burstly
//
//  Created by Nikolay Remizevich on 14.07.10.
//  Copyright 2010 App Media Group, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BurstlyLoggerConstants.h"

@interface BurstlyLogger : NSObject {

}

+ (AS_LogLevel)getLogLevel;

+ (void)setLogLevel: (AS_LogLevel)level;

+ (void)logLevel: (AS_LogLevel)level format: (NSString*)format, ...;

@end

#define LOG_DEBUG(f, ...) \
[BurstlyLogger logLevel:AS_LOG_LEVEL_DEBUG format:f, ## __VA_ARGS__]

#define LOG_DEBUG_ADMANAGER(m, f, ...) \
if (m != nil) { \
	NSString *ff = [[NSString stringWithFormat:@"m=%p, ", m] stringByAppendingString:f]; \
	[BurstlyLogger logLevel:AS_LOG_LEVEL_DEBUG format:ff, ## __VA_ARGS__]; \
} \
else { \
	[BurstlyLogger logLevel:AS_LOG_LEVEL_DEBUG format:f, ## __VA_ARGS__]; \
}

#define LOG_INFO(f, ...) \
[BurstlyLogger logLevel:AS_LOG_LEVEL_INFO format:f, ## __VA_ARGS__]

#define LOG_WARN(f, ...) \
[BurstlyLogger logLevel:AS_LOG_LEVEL_WARN format:f, ## __VA_ARGS__]

#define LOG_ERROR(f, ...) \
[BurstlyLogger logLevel:AS_LOG_LEVEL_ERROR format:f, ## __VA_ARGS__]

#define LOG_FATAL(f, ...) [BurstlyLogger logLevel:AS_LOG_LEVEL_FATAL format:f, ## __VA_ARGS__]