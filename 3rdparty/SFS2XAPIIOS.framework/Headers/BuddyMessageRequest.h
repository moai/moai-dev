//
//  BuddyMessageRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "GenericMessageRequest.h"
#import "Buddy.h"


/** BuddyMessage(s) are similar to private chat messages but are specifically designed for the Buddy List system.
 
 They don't require any Room parameter, nor do they require that Users are joined into any particular Room. Additionally the Buddy Message
 performs specific validation such as making sure that the recipient is in the User's BuddyList and verifies the Buddy block status. 
 */
@interface BuddyMessageRequest : GenericMessageRequest {

}

-(id)initWithMessage:(NSString *)message targetBuddy:(id <Buddy>)targetBuddy  params:(id <ISFSObject>)params;

/** Send a Buddy Message to the server
 
 @param message the message
 @param targetBuddy the recipient of the message
 @param params custom parameters (e.g. the textfield font and color, a sender avatar id, or any other data...)
  
 @see [ISFSEvents onBuddyMessage:]
 @see AddBuddyRequest
 @see RemoveBuddyRequest
 @see BlockBuddyRequest
  
 */
+(id)requestWithMessage:(NSString *)message targetBuddy:(id <Buddy>)targetBuddy  params:(id <ISFSObject>)params;

/** Send a Buddy Message to the server
 
 @param message the message
 @param targetBuddy the recipient of the message
 
 @see [ISFSEvents onBuddyMessage:]
 @see AddBuddyRequest
 @see RemoveBuddyRequest
 @see BlockBuddyRequest
 
 */
+(id)requestWithMessage:(NSString *)message targetBuddy:(id <Buddy>)targetBuddy;

@end
