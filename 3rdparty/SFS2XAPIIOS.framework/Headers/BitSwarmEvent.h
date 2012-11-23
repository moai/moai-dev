//
//  BitSwarmEvent.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseEvent.h"

EXTERN NSString * const BitSwarmEvent_CONNECT;
EXTERN NSString * const BitSwarmEvent_DISCONNECT;
EXTERN NSString * const BitSwarmEvent_RECONNECTION_TRY;
EXTERN NSString * const BitSwarmEvent_IO_ERROR;
EXTERN NSString * const BitSwarmEvent_SECURITY_ERROR;

@interface BitSwarmEvent : BaseEvent {
	
}

-(id)initWithType:(NSString *)type params:(NSDictionary *)params;
-(id)clone;

@end
