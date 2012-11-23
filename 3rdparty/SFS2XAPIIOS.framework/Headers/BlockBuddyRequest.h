//
//  BlockBuddyRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const BlockBuddyRequest_KEY_BUDDY_NAME;
EXTERN NSString * const BlockBuddyRequest_KEY_BUDDY_BLOCK_STATE;

/** Block/Unblock a Buddy in the User's BuddyList
 
 Blocked buddies won't be able to see the the User online status and send messages or events
 */
@interface BlockBuddyRequest : BaseRequest {
@private
	NSString *_buddyName;
	BOOL _blocked;
}

@property (retain) NSString *buddyName;


-(id)initWithBuddyName:(NSString *)buddyName blocked:(BOOL)blocked;

/** Request to Block/Unblock a Buddy in the User's BuddyList
 
 @param buddyName the name of the buddy to add
 @param blocked the blocked status
 
 @see [ISFSEvents onBuddyBlock:]
 @see [ISFSEvents onBuddyError:]
 @see AddBuddyRequest
 @see RemoveBuddyRequest
  
 */
+(id)requestWithBuddyName:(NSString *)buddyName blocked:(BOOL)blocked;

@end
