//
//  CreateRoomRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"
#import "Room.h"
#import "RoomSettings.h"

EXTERN NSString * const CreateRoomRequest_KEY_ROOM;
EXTERN NSString * const CreateRoomRequest_KEY_NAME;
EXTERN NSString * const CreateRoomRequest_KEY_PASSWORD;
EXTERN NSString * const CreateRoomRequest_KEY_GROUP_ID;
EXTERN NSString * const CreateRoomRequest_KEY_ISGAME;
EXTERN NSString * const CreateRoomRequest_KEY_MAXUSERS;
EXTERN NSString * const CreateRoomRequest_KEY_MAXSPECTATORS;
EXTERN NSString * const CreateRoomRequest_KEY_MAXVARS;
EXTERN NSString * const CreateRoomRequest_KEY_ROOMVARS;
EXTERN NSString * const CreateRoomRequest_KEY_PERMISSIONS;
EXTERN NSString * const CreateRoomRequest_KEY_EVENTS;
EXTERN NSString * const CreateRoomRequest_KEY_EXTID;
EXTERN NSString * const CreateRoomRequest_KEY_EXTCLASS;
EXTERN NSString * const CreateRoomRequest_KEY_EXTPROP;
EXTERN NSString * const CreateRoomRequest_KEY_AUTOJOIN;
EXTERN NSString * const CreateRoomRequest_KEY_ROOM_TO_LEAVE;


/**
 Creates a new Room in the current Zone
 */
@interface CreateRoomRequest : BaseRequest {
	
	RoomSettings *_settings;
	BOOL _autoJoin;
	id<Room> _roomToLeave;
}

-(id)initWithRoomSettings:(RoomSettings *)settings autoJoin:(BOOL)autoJoin roomToLeave:(id<Room>)roomToLeave;

/**
 @param settings the new room settings, see <RoomSettings> for all the details
 @param autoJoin if true the Room will be joined as soon as it is created
 @param roomToLeave indicates which Room should be left once the join has succeeded
 
 @see [ISFSEvent onRoomAdd:]
 @see RoomSettings
 
 */
+(id)requestWithRoomSettings:(RoomSettings *)settings autoJoin:(BOOL)autoJoin roomToLeave:(id<Room>)roomToLeave;

/**
 Creates a new room in the current zone without joining it.
 @param settings the new room settings, see RoomSettings for all the details
 
 @see [ISFSEvent onRoomAdd:]
 @see RoomSettings
 
 */
+(id)requestWithRoomSettings:(RoomSettings *)settings;

@end
