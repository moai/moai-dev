//
//  LoggerEvent.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseEvent.h"

EXTERN NSString * const LoggerEvent_DEBUG;
EXTERN NSString * const LoggerEvent_INFO;
EXTERN NSString * const LoggerEvent_WARNING;
EXTERN NSString * const LoggerEvent_ERROR;

@interface LoggerEvent : BaseEvent {
	
}

-(id)initWithType:(NSString *)type params:(NSDictionary *)params;
-(id)clone;
@end
