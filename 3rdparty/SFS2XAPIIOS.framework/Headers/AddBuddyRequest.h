//
//  AddBuddyRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const AddBuddyRequest_KEY_BUDDY_NAME;

/** Add a new Buddy to the current Buddy List
 */
@interface AddBuddyRequest : BaseRequest {
@private
	NSString *_name;
}

@property (retain) NSString *name;

-(id)initWithBuddyName:(NSString *)buddyName;

/** Sends a request to the server to add a new Buddy to the Buddy List
 
 @param buddyName the name of the buddy to add
 
 @see [ISFSEvents onBuddyAdd:]
 @see [ISFSEvents onBuddyError:]
 @see RemoveBuddyRequest

 */
+(id)requestWithBuddyName:(NSString *)buddyName;

@end
