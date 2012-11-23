//
//  MessageRecipientMode.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "Common.h"

EXTERN NSInteger const MessageRecipientMode_TO_USER;
EXTERN NSInteger const MessageRecipientMode_TO_ROOM;
EXTERN NSInteger const MessageRecipientMode_TO_GROUP;
EXTERN NSInteger const MessageRecipientMode_TO_ZONE;


/** This class is used to specify the recipient mode for Moderator Messages and Administrator Message.
 
 There are 4 recipient modes:

 - <b>TO_USER:</b> the message will be sent to the User (passed as the <i>target</i> parameter)
 - <b>TO_ROOM:</b> the message will be sent to all Users in the Room (passed as the <i>target</i> parameter)
 - <b>TO_GROUP: </b> the message will be sent to all Users in the Room Group (passed as a String in the <i>target</i> parameter)
 - <b>TO_ZONE: </b> the message will be sent to all Users in the Zone (<i>target</i> is ignored)
 
 See <ModeratorMessageRequest>, <AdminMessageRequest>
 
 */
@interface MessageRecipientMode : NSObject {
@private
	id _target;
	NSInteger _mode;
}

@property (readonly) NSInteger mode;
@property (retain) id target;

-(id)initWithMode:(NSInteger)mode target:(id)target;
+(id)recipientModeWithMode:(NSInteger)mode target:(id)target;

@end
