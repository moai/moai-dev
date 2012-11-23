//
//  SpectatorToPlayerRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"
#import "Room.h"

EXTERN NSString * const SpectatorToPlayerRequest_KEY_ROOM_ID;
EXTERN NSString * const SpectatorToPlayerRequest_KEY_USER_ID;
EXTERN NSString * const SpectatorToPlayerRequest_KEY_PLAYER_ID;

/** Attempt to turn the current User from Spectator into a Player (in a Game Room)

 This could fail if no Player slots are available in the Room at the time of the request
 */ 
@interface SpectatorToPlayerRequest : BaseRequest {
	
	
	id <Room> _room;
}

@property (retain) id <Room> room;

-(id)initWithTargetRoom:(id <Room>)targetRoom;
/**
 
 @param targetRoom the Room where the change has to be performed, a null value indicates the last joined Room (default)
 
 @see [ISFSEvents onSpectatorToPlayer:]
 @see [ISFSEvents onSpectatorToPlayerError:]
 @see PlayerToSpectatorRequest
 
 */
+(id)requestWithTargetRoom:(id <Room>)targetRoom;
+(id)request;

@end
