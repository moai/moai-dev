//
//  QuickJoinGameRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"
#import "Room.h"

EXTERN NSString * const QuickJoinGameRequest_KEY_ROOM_LIST;
EXTERN NSString * const QuickJoinGameRequest_KEY_GROUP_LIST;
EXTERN NSString * const QuickJoinGameRequest_KEY_ROOM_TO_LEAVE;
EXTERN NSString * const QuickJoinGameRequest_KEY_MATCH_EXPRESSION;

@class MatchExpression;

/** Quick join a User in a public game.
 
 By providing a MatchExpression and a list of Rooms or Groups the system can search for matching Rooms and immediately teleport the player in the game action.
 */
@interface QuickJoinGameRequest : BaseRequest {
@private		
	NSArray *_whereToSearch;
	MatchExpression *_matchExpression;
	id <Room> _roomToLeave;
}

@property (nonatomic, retain) NSArray *whereToSearch;
@property (nonatomic, retain) MatchExpression *matchExpression;
@property (nonatomic, retain) id <Room> roomToLeave;


-(id)initWithMatchExpression:(MatchExpression *)matchExpression whereToSearch:(NSArray *)whereToSearch roomToLeave:(id <Room>)roomToLeave;
/**
 @param matchExpression a MatchExpression to filter Rooms
 @param whereToSearch this parameter accepts an array of Room objects or an array of Strings representing valid Room Group names
 @param roomToLeave an optional Room that the User will leave after joining the game
  
 
 @see [ISFSEvents onRoomJoin:]
 @see MatchExpression
 
 */
+(id)requestWithMatchExpression:(MatchExpression *)matchExpression whereToSearch:(NSArray *)whereToSearch roomToLeave:(id <Room>)roomToLeave;
+(id)requestWithMatchExpression:(MatchExpression *)matchExpression whereToSearch:(NSArray *)whereToSearch;

@end
