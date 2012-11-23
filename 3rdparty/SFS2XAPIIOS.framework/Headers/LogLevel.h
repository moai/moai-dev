//
//  LogLevel.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"

EXTERN NSInteger const LogLevel_DEBUG;
EXTERN NSInteger const LogLevel_INFO;
EXTERN NSInteger const LogLevel_WARN;
EXTERN NSInteger const LogLevel_ERROR;

@interface LogLevel : NSObject {
	
}

+(NSString *)fromString:(NSInteger)level;

@end
