//
//  RemoveBuddyRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const RemoveBuddyRequest_KEY_BUDDY_NAME;

/** Remove a new Buddy fron the current Buddy List
 */
@interface RemoveBuddyRequest : BaseRequest {
@private
	
	NSString *_name;
}

@property (retain) NSString *name;

-(id)initWithBuddyName:(NSString *)buddyName;
/**
 
 @param buddyName the name of the buddy to remove
 
 @see [ISFSEvents onBuddyRemove:]
 @see [ISFSEvents onBuddyError:]
 @see AddBuddyRequest

 */
+(id)requestWithBuddyName:(NSString *)buddyName;

@end
