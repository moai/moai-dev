//
//  RoomProperties.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"




/** A list of additional properties that can be used in MatchExpressions for Room objects
 
  See <MatchExpression>
 */
@interface RoomProperties : NSObject {

}
/** The Room name */
EXTERN NSString * const RoomProperties_NAME;
/** The Room group id */
EXTERN NSString * const RoomProperties_GROUP_ID;
/** The Room max users */
EXTERN NSString * const RoomProperties_MAX_USERS;
/** The Room max spectators */
EXTERN NSString * const RoomProperties_MAX_SPECTATORS;
/** The Room user count */
EXTERN NSString * const RoomProperties_USER_COUNT;
/** The Room spectator count */
EXTERN NSString * const RoomProperties_SPECTATOR_COUNT;
/** The Room isGame flag */
EXTERN NSString * const RoomProperties_IS_GAME;
/** The Room isPrivate flag */
EXTERN NSString * const RoomProperties_IS_PRIVATE;
/** Check if the Room has at least 1 free player slot */
EXTERN NSString * const RoomProperties_HAS_FREE_PLAYER_SLOTS;
/** Check if the Room is of type SFSGame */
EXTERN NSString * const RoomProperties_IS_TYPE_SFSGAME;

@end
