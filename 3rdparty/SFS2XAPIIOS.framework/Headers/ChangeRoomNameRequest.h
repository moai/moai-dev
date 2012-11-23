//
//  ChangeRoomNameRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"
#import "Room.h"

EXTERN NSString * const ChangeRoomNameRequest_KEY_ROOM;
EXTERN NSString * const ChangeRoomNameRequest_KEY_NAME;

/** Changes the name of a Room at runtime.
 
 This request might fail if the Room was configured not be ranamed or if the User sending the request
 is not the owner of the Room. 
 
 Moderators and Administrator can override this last constraint.
 */ 
@interface ChangeRoomNameRequest : BaseRequest {
		
	id<Room> _room;
	NSString *_newName;
}

/**
 @param room the Room to rename
 @param newName	the new Room name (no duplicates are allowed)
 
 @see [ISFSEvents onRoomNameChange:]
 @see [ISFSEvents onRoomNameChangeError:]
 
 @see ChangeRoomCapacityRequest
 @see ChangeRoomPasswordStateRequest
 
 */
+(id)requestWithRoom:(id<Room>)room newName:(NSString *)newName;

@end
