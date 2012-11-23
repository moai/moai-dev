//
//  PlayerToSpectatorRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"
#import "Room.h"

EXTERN NSString * const PlayerToSpectatorRequest_KEY_ROOM_ID;
EXTERN NSString * const PlayerToSpectatorRequest_KEY_USER_ID;

/** Attempt to turn the current User from Player to a Spectator (in a Game Room)
 
 This could fail if no Spectator slots are available in the Room at the time of the request
 */ 
@interface PlayerToSpectatorRequest : BaseRequest {
@private
	id <Room> _room;
}

@property (retain) id <Room> room;

-(id)initWithTargetRoom:(id <Room>)room;
/**
 @param room the Room where the change has to be performed, a null value indicates the last joined Room (default)
 
 @see [ISFSEvents onPlayerToSpectator:]
 @see [ISFSEvents onPlayerToSpectatorError:]
 @see SpectatorToPlayerRequest
 
 */
+(id)requestWithTargetRoom:(id <Room>)room;
+(id)request;

@end
