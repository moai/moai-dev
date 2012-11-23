//
//  SFSBuddyEvent.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseEvent.h"


EXTERN NSString * const SFSBuddyEvent_BUDDY_ADD;
EXTERN NSString * const SFSBuddyEvent_BUDDY_BLOCK;
EXTERN NSString * const SFSBuddyEvent_BUDDY_ERROR;
EXTERN NSString * const SFSBuddyEvent_BUDDY_LIST_INIT;
EXTERN NSString * const SFSBuddyEvent_BUDDY_MESSAGE;
EXTERN NSString * const SFSBuddyEvent_BUDDY_ONLINE_STATE_UPDATE;
EXTERN NSString * const SFSBuddyEvent_BUDDY_REMOVE;
EXTERN NSString * const SFSBuddyEvent_BUDDY_VARIABLES_UPDATE;


/** This class contains all the Buddy List API related events
 
 */
@interface SFSBuddyEvent : BaseEvent {

}

-(id)initWithType:(NSString *)type params:(NSDictionary *)params;

/** Get a copy of the current instance.
 
 @return a copy of the current instance. 

 */
-(id)clone;

@end
