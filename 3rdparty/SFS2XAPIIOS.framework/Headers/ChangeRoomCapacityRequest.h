//
//  ChangeRoomCapacityRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"
#import "Room.h"

EXTERN NSString * const ChangeRoomCapacityRequest_KEY_ROOM;
EXTERN NSString * const ChangeRoomCapacityRequest_KEY_USER_SIZE;
EXTERN NSString * const ChangeRoomCapacityRequest_KEY_SPEC_SIZE;

/** Changes the capacity of a Room (maxUsers and maxSpectators) at runtime.
 
 This request might fail if the Room was configured not be re-sizable or if the User sending the request
 is not the owner of the Room. Moderators and Administrator can override this last constraint.
 
 It is possible to "shrink" the Room capacity so that maxUsers < userCount. 
 In this case nothing will happen to the "extra" users.
 As soon as clients will leave the Room the userCount will get down to the new maxUsers value.
 */ 
@interface ChangeRoomCapacityRequest : BaseRequest {
	
	id<Room> _room;
	NSInteger _newMaxUsers;
	NSInteger _newMaxSpect;

}


/**
 @param room the Room to resize
 @param newMaxUsers the new maxUsers value (use -1 if don't want to change the current value)
 @param newMaxSpect the new maxSpect value (use -1 if don't want to change the current value)
 
 
 @see [ISFSEvents onRoomCapacityChange:]
 @see [ISFSEvents onRoomCapacityChangeError:]
 @see ChangeRoomNameRequest
 @see ChangeRoomPasswordStateRequest
 
 */
+(id)requestWithRoom:(id<Room>)room newMaxUsers:(NSInteger)newMaxUsers newMaxSpect:(NSInteger)newMaxSpect;

@end
