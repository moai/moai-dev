//
//  UserProperties.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"


EXTERN NSString * const UserProperties_NAME;
EXTERN NSString * const UserProperties_IS_PLAYER;
EXTERN NSString * const UserProperties_IS_SPECTATOR;
EXTERN NSString * const UserProperties_IS_NPC;
EXTERN NSString * const UserProperties_PRIVILEGE_ID;
EXTERN NSString * const UserProperties_IS_IN_ANY_ROOM;

/** A list of additional properties that can be used in MatchExpressions for User objects
 
  See <MatchExpression>
 */
@interface UserProperties : NSObject {
}

@end
