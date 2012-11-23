//
//  ClientDisconnectionReason.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "Common.h"


EXTERN NSString * const ClientDisconnectionReason_IDLE;
EXTERN NSString * const ClientDisconnectionReason_KICK;
EXTERN NSString * const ClientDisconnectionReason_BAN;
EXTERN NSString * const ClientDisconnectionReason_MANUAL;
EXTERN NSString * const ClientDisconnectionReason_UNKNOWN;

@interface ClientDisconnectionReason : NSObject {
}

+(NSString *)getReason:(NSInteger)reasonId;

@end
