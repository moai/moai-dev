//
//  AdminMessageRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "GenericMessageRequest.h"

@class MessageRecipientMode;

/** Send a Admin Message to a specific User or groups of Users
 
 The sender must have the admin privileges to be able to send these kinds of messages.
 The recipient parameter can be used to specify if the message is sent to a User, a Room, the Room Group or the whole Zone.

 See <MessageRecipientMode>
 */

@interface AdminMessageRequest : GenericMessageRequest {

}


/** Initializes the request instance.
 
 @param message (NSString *) the admin message
 @param recipientMode (MessageRecipientMode *) the recipient mode (message can be sent to single User, a Room, the whole Room Group or the Zone)
 @param params (id <ISFSObject>)custom extra parameters (optional)
 
 @see MessageRecipientMode
 @see ModeratorMessageRequest
 */
-(id)initWithMessage:(NSString *)message recipientMode:(MessageRecipientMode *)recipientMode  params:(id <ISFSObject>)params;

/** Initializes and returns the request
 
 @param message (NSString *) the admin message
 @param recipientMode (MessageRecipientMode *) the recipient mode (message can be sent to single User, a Room, the whole Room Group or the Zone)
 @param params (id <ISFSObject>) custom extra parameters (optional)
 
 @see MessageRecipientMode
 @see ModeratorMessageRequest
 */
+(id)requestWithMessage:(NSString *)message recipientMode:(MessageRecipientMode *)recipientMode  params:(id <ISFSObject>)params;

/** Initializes and returns the request
 
 @param message (NSString *) the admin message
 @param recipientMode (MessageRecipientMode *) the recipient mode (message can be sent to single User, a Room, the whole Room Group or the Zone)
 
 @see MessageRecipientMode
 @see ModeratorMessageRequest
 */
+(id)requestWithMessage:(NSString *)message recipientMode:(MessageRecipientMode *)recipientMode;

@end
