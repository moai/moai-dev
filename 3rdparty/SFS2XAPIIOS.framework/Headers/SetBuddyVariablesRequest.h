//
//  SetBuddyVariablesRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const SetBuddyVariablesRequest_KEY_BUDDY_NAME;
EXTERN NSString * const SetBuddyVariablesRequest_KEY_BUDDY_VARS;

/** Sets one or more BuddyVariables for the current User.
 
 This will update all Users in the Zona who have this User as Buddy in their Buddy Lists.
 */
@interface SetBuddyVariablesRequest : BaseRequest {
@private
	
	NSArray *_buddyVariables;
}

@property (retain) NSArray *buddyVariables;

-(id)initWithBuddyVariables:(NSArray *)buddyVariables;
/**
 @param buddyVariables an Array of BuddyVariables
 
 @see [ISFSEvents onBuddyVariablesUpdate:]
 @see SFSBuddyVariable
 
 */
+(id)requestWithBuddyVariables:(NSArray *)buddyVariables;

@end
