//
//  LeaveRoomRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"
#import "Room.h"

EXTERN NSString * const LeaveRoomRequest_KEY_ROOM_ID;

/**
 Leaves a currently joined Room
 */
@interface LeaveRoomRequest : BaseRequest {

@private
	
	id <Room> _room;
}


/**
 @param room any joined Room that you want to leave, if nil, the last joined Room will be left
 
 @see [ISFSEvents onUserExitRoom:]
 @see JoinRoomRequest
 
 */	
+(id)requestWithRoom:(id <Room>)room;
+(id)request;

@end
