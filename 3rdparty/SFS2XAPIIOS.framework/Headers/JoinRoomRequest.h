//
//  JoinRoomRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"

EXTERN NSString * const JoinRoomRequest_KEY_ROOM;
EXTERN NSString * const JoinRoomRequest_KEY_USER_LIST;
EXTERN NSString * const JoinRoomRequest_KEY_ROOM_NAME;
EXTERN NSString * const JoinRoomRequest_KEY_ROOM_ID;
EXTERN NSString * const JoinRoomRequest_KEY_PASS;
EXTERN NSString * const JoinRoomRequest_KEY_ROOM_TO_LEAVE;
EXTERN NSString * const JoinRoomRequest_KEY_AS_SPECTATOR;


/**
 Join the current User in a Room.
 */
@interface JoinRoomRequest : BaseRequest {
	
@private
	
	NSInteger __id;
	NSString *_name;
	NSString *_pass;
	NSNumber *_roomIdToLeave;
	BOOL _asSpectator;
	
}

/**
 @param numericIdOrName you can use the numeric ID of the Room or its name (you need to pass numeric id in a NSNumber object)
 @param pass needed only if the Room is password protected
 @param roomIdToLeave the id of a previously joined Room that you want to leave once joined in the new Room (optional). The default behavior is to leave the previous Room. You can pass a negative ID to avoid leaving any Room
 @param asSpect if true it indicates that you want to join the room as spectator (optional, for game Rooms only)
 
 @see [ISFSEvents onRoomJoin:]
 @see [ISFSEvents onRoomJoinError:]
 @see LeaveRoomRequest
 
 */
+(id)requestWithId:(id)numericIdOrName pass:(NSString *)pass roomIdToLeave:(NSNumber *)roomIdToLeave asSpect:(BOOL)asSpect;
+(id)requestWithId:(id)numericIdOrName;

@end
