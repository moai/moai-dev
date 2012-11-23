//
//  ModeratorMessageRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "GenericMessageRequest.h"

@class MessageRecipientMode;

/** Send a Moderator Message to a specific User or groups of Users
 
 The sender must have the moderator privileges to be able to send these kinds of messages.
 The <b>recipient</b> parameter can be used to specify if the message is sent to a User, a Room, the Room Group or the whole Zone.
 
 See <MessageRecipientMode>, <ISFSEvent> <onModeratorMessage>
 
 */
@interface ModeratorMessageRequest : GenericMessageRequest {

}

-(id)initWithMessage:(NSString *)message recipientMode:(MessageRecipientMode *)recipientMode params:(id <ISFSObject>)params;

/**
 @param message the moderator message
 @param recipientMode the recipient mode (message can be sent to single User, a Room, the whole Room Group or the Zone)
 @param params custom extra parameters (optional)
 
 @see [ISFSEvents onModeratorMessage:]
 @see MessageRecipientMode
 @see AdminMessageRequest
 
 */
+(id)requestWithMessage:(NSString *)message recipientMode:(MessageRecipientMode *)recipientMode params:(id <ISFSObject>)params;

@end
