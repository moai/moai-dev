//
//  SFSEvent.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseEvent.h"

EXTERN NSString * const SFSEvent_HANDSHAKE;
EXTERN NSString * const SFSEvent_CONNECTION;
EXTERN NSString * const SFSEvent_CONNECTION_LOST;
EXTERN NSString * const SFSEvent_CONNECTION_RETRY;
EXTERN NSString * const SFSEvent_CONNECTION_RESUME;
EXTERN NSString * const SFSEvent_CONFIG_LOAD_SUCCESS;
EXTERN NSString * const SFSEvent_CONFIG_LOAD_FAILURE;
EXTERN NSString * const SFSEvent_UDP_INIT;
EXTERN NSString * const SFSEvent_LOGIN;
EXTERN NSString * const SFSEvent_LOGIN_ERROR;
EXTERN NSString * const SFSEvent_LOGOUT;
EXTERN NSString * const SFSEvent_ROOM_ADD;
EXTERN NSString * const SFSEvent_ROOM_REMOVE;
EXTERN NSString * const SFSEvent_ROOM_CREATION_ERROR;
EXTERN NSString * const SFSEvent_ROOM_JOIN;
EXTERN NSString * const SFSEvent_ROOM_JOIN_ERROR;
EXTERN NSString * const SFSEvent_USER_ENTER_ROOM;
EXTERN NSString * const SFSEvent_USER_EXIT_ROOM;
EXTERN NSString * const SFSEvent_USER_COUNT_CHANGE;
EXTERN NSString * const SFSEvent_PUBLIC_MESSAGE;
EXTERN NSString * const SFSEvent_PRIVATE_MESSAGE;
EXTERN NSString * const SFSEvent_MODERATOR_MESSAGE;
EXTERN NSString * const SFSEvent_ADMIN_MESSAGE;
EXTERN NSString * const SFSEvent_OBJECT_MESSAGE;
EXTERN NSString * const SFSEvent_EXTENSION_RESPONSE;
EXTERN NSString * const SFSEvent_ROOM_VARIABLES_UPDATE;
EXTERN NSString * const SFSEvent_USER_VARIABLES_UPDATE;
EXTERN NSString * const SFSEvent_ROOM_GROUP_SUBSCRIBE;
EXTERN NSString * const SFSEvent_ROOM_GROUP_UNSUBSCRIBE;
EXTERN NSString * const SFSEvent_ROOM_GROUP_SUBSCRIBE_ERROR;
EXTERN NSString * const SFSEvent_ROOM_GROUP_UNSUBSCRIBE_ERROR;
EXTERN NSString * const SFSEvent_SPECTATOR_TO_PLAYER;
EXTERN NSString * const SFSEvent_PLAYER_TO_SPECTATOR;
EXTERN NSString * const SFSEvent_SPECTATOR_TO_PLAYER_ERROR;
EXTERN NSString * const SFSEvent_PLAYER_TO_SPECTATOR_ERROR;
EXTERN NSString * const SFSEvent_ROOM_NAME_CHANGE;
EXTERN NSString * const SFSEvent_ROOM_NAME_CHANGE_ERROR;
EXTERN NSString * const SFSEvent_ROOM_PASSWORD_STATE_CHANGE;
EXTERN NSString * const SFSEvent_ROOM_PASSWORD_STATE_CHANGE_ERROR;
EXTERN NSString * const SFSEvent_ROOM_CAPACITY_CHANGE;
EXTERN NSString * const SFSEvent_ROOM_CAPACITY_CHANGE_ERROR;
EXTERN NSString * const SFSEvent_ROOM_FIND_RESULT;
EXTERN NSString * const SFSEvent_USER_FIND_RESULT;
EXTERN NSString * const SFSEvent_INVITATION;
EXTERN NSString * const SFSEvent_INVITATION_REPLY;
EXTERN NSString * const SFSEvent_INVITATION_REPLY_ERROR;


@interface SFSEvent : BaseEvent {
	
}

-(id)initWithType:(NSString *)type params:(NSDictionary *)params;
-(id)clone;


@end
