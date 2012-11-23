//
//  GoOnlineRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const GoOnlineRequest_KEY_ONLINE;
EXTERN NSString * const GoOnlineRequest_KEY_BUDDY_NAME;
EXTERN NSString * const GoOnlineRequest_KEY_BUDDY_ID;


/** Toggles the Buddy ONLINE status of the User.
 
 All clients who have the current User as their Buddy in the BuddyList, will see his status change accordingly.
 Going online/offline as Buddy doesn't affect the User connection,the currently joined Zone/Rooms etc... 
 
 The ONLINE status is maintained by a reserved and persistent BuddyVariable.
 
 See <SFSBuddyVariable>, <ReservedBuddyVariables>
 
 */
@interface GoOnlineRequest : BaseRequest {
@private
	BOOL _online;
}

-(id)initWithOnline:(BOOL)online;
/**
 
 @param online : the Buddy Online status
 
 @see [ISFSEvents onBuddyOnlineStateUpdate:]
 @see [ISFSEvents onBuddyError:]
 
 */
+(id)requestWithOnline:(BOOL)online;

@end
