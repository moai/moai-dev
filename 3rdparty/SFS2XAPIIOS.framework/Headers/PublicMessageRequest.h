//
//  PublicMessageRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "GenericMessageRequest.h"

/** Sends a chat Public Message to other users in the Room
 
 Public Messages are broadcast to all users in the specified Room
 An optional custom <SFSObject> can be sent together with the message. The extra parameters can be used to transmit information 
 about the font and color of the message or any other properties that are relevant to the message.
 */
@interface PublicMessageRequest : GenericMessageRequest {

}


-(id)initWithMessage:(NSString *)message params:(id <ISFSObject>)params  targetRoom:(id <Room>)targetRoom;
/**
 
 @param message the chat message
 @param params custom extra parameters (optional)
 @param targetRoom the target room (optional). By default it is used the last joined Room
 
 @see [ISFSEvents onPublicMessage:]
 @see PrivateMessageRequest
 
 */
+(id)requestWithMessage:(NSString *)message params:(id <ISFSObject>)params  targetRoom:(id <Room>)targetRoom;
+(id)requestWithMessage:(NSString *)message;
@end
