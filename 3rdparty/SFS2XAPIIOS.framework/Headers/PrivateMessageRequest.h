//
//  PrivateMessageRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "GenericMessageRequest.h"

/** Send a chat Private Message to another User
 
 Private Messages can be exchanged between Users in the same Rooms or in different Rooms.
 They can also be sent between Users that are not joined to any Rooms at all. 
 */
@interface PrivateMessageRequest : GenericMessageRequest {

}

-(id)initWithMessage:(NSString *)message recipientId:(NSInteger)recipientId  params:(id <ISFSObject>)params;
/**
 @param recipientId the id of the recipient User
 @param message the private chat message
 @param params custom extra parameters (optional)
 
 @see [ISFSEvents onPrivateMessage:]
 @see PublicMessageRequest
 
 */
+(id)requestWithMessage:(NSString *)message recipientId:(NSInteger)recipientId params:(id <ISFSObject>)params;
+(id)requestWithMessage:(NSString *)message recipientId:(NSInteger)recipientId;

@end
